#include <string.h>
#include <stdint.h>
#include "protocol.h"
#include "..\malloc\malloc.h"

#include "debug_printf.h"
#include "app_slave.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////



m_protocol_dev_typedef m_ctrl_dev;	//定义modbus控制器

//static void mc_read_coil(u8* length ,u8* rxbuf);
//static void mc_read_digit(void);
//static void mc_read_analog(void);
//static void mc_read_write(void);



//static void mc_read_coil(u8* length ,u8* rxbuf);



void clr485rxbuf(void)
{
    mymemset(m_ctrl_dev.rxbuf, 0x00, M_MAX_FRAME_LENGTH); //清空缓存
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
}

//解析一帧数据，解析结果存储在fx里面
//注意：本函数会用到malloc给fx数据指针申请内存，后续用完fx，一定要释放内存！！
//否则会引起内存泄露！！！
//fx：帧指针
//buf：输入数据缓冲区（串口接收到的数据）
//len：输入数据长度
//返回值：解析结果，0，OK，其他，错误代码。
m_result mb_unpack_frame(m_frame_typedef *fx)
{
    u16 rxchkval=0;   	 		//接收到的校验值
    u16 calchkval=0;			  //计算得到的校验值
    u8 cmd = 0 ;            //计算功能码
    u8 datalen=0; 			  	//有效数据长度
    u8 address=0;
    u8 res;
    DBG_B_INFO("mb_unpack_frame");
//    fx->datalen=0; 				  //数据长度清零
    if(m_ctrl_dev.rxlen>M_MAX_FRAME_LENGTH||m_ctrl_dev.rxlen<M_MIN_FRAME_LENGTH) {
        m_ctrl_dev.rxlen=0;			//清除rxlen
        m_ctrl_dev.frameok=0;		//清除framok标记，以便下次可以正常接收

        return MR_FRAME_FORMAT_ERR;//帧格式错误
    }
    datalen=m_ctrl_dev.rxlen;
    DBG_B_INFO("当前数据长度 %d",m_ctrl_dev.rxlen);

    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM:							//校验和
        calchkval=mc_check_sum(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_XOR:							//异或校验
        calchkval=mc_check_xor(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8校验
        calchkval=mc_check_crc8(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16校验
        calchkval=mc_check_crc16(m_ctrl_dev.rxbuf,datalen-2);
        rxchkval=((u16)m_ctrl_dev.rxbuf[datalen-2]<<8)+m_ctrl_dev.rxbuf[datalen-1];
        break;
    }
    DBG_B_INFO("calchkval = 0x%x  、rxchkval  = 0x%x   、datalen = 0x%x",calchkval,rxchkval,datalen);
//    DBG_B_INFO("cmd = 0x%x ",cmd);

    m_ctrl_dev.rxlen=0;			//清除rxlen
    m_ctrl_dev.frameok=0;		//清除framok标记，以便下次可以正常接收

    if(calchkval==rxchkval) {	//校验正常
        address=m_ctrl_dev.rxbuf[0];
        if (address!= SLAVE_ADDRESS) {
            return MR_FRAME_SLAVE_ADDRESS;    //帧格式错误
        }

        cmd=m_ctrl_dev.rxbuf[1];

        if ((cmd > 0x06 )||(cmd < 0x01)) {
            return MR_FRANE_ILLEGAL_FUNCTION;    //命令帧错误
        }


        switch (cmd) {
           case 0x02:  res= ReadDiscRegister();
		     break;
        case 0x03:
            res=  ReadHoldRegister();
				    if(res==0)
				   DBG_B_INFO("ReadHoldRegister success");
            break;
        case 0x04:
             res=  ReadInputRegister();
			 if(res==0)
				   DBG_B_INFO("ReadInputRegister success");
            break;
		    case 0x06:
			res=  WriteHoldRegister();
			 if(res==0)
				   DBG_B_INFO("WriteHoldRegister success");
   		   	 break;
        }
//        fx->count=m_ctrl_dev.rxbuf[2];
//        fx->datalen=m_ctrl_dev.rxbuf[3];
//        if(fx->datalen) {
//            fx->data=mymalloc(SRAMIN,fx->datalen);		//申请内存
//            for(datalen=0; datalen<fx->datalen; datalen++) {
//                fx->data[datalen]=m_ctrl_dev.rxbuf[4+datalen];		//拷贝数据
//            }
//        }
//        fx->chkval=rxchkval;	//记录校验值
    } else {
        return MR_FRAME_CHECK_ERR;
    }
    return MR_OK;
}





//打包一帧数据，并发送
//fx：指向需要打包的帧
void mb_packsend_frame(m_frame_typedef *fx)
{
    u16 i;
    u16 calchkval=0;			//计算得到的校验值
    u16 framelen=0;				//打包后的帧长度
    u8 *sendbuf;				//发送缓冲区

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {
        framelen=6+fx->datalen;
    } else {
        framelen=5+fx->datalen;
    }
    sendbuf=mymalloc(SRAMIN,framelen);	//申请内存
    sendbuf[0]=fx->address;
    sendbuf[1]=fx->function;
    sendbuf[2]=fx->count;
    sendbuf[3]=fx->datalen;
    for(i=0; i<fx->datalen; i++) {
        sendbuf[4+i]=fx->data[i];
    }
    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM:							//校验和
        calchkval=mc_check_sum(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_XOR:							//异或校验
        calchkval=mc_check_xor(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8校验
        calchkval=mc_check_crc8(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16校验
        calchkval=mc_check_crc16(sendbuf,fx->datalen+4);
        break;
    }

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {	//如果是CRC16,则有2个字节的CRC
        sendbuf[4+fx->datalen]=(calchkval>>8)&0XFF; 	//高字节在前
        sendbuf[5+fx->datalen]=calchkval&0XFF;			//低字节在后
    } else {
        sendbuf[4+fx->datalen]=calchkval&0XFF;
    }
    RS4851_Send_Buffer(sendbuf,framelen);	//发送这一帧数据
    myfree(SRAMIN,sendbuf);			//释放内存
}



//初始化modbus
//checkmode：校验模式：0,校验和;1,异或;2,CRC8;3,CRC16
//返回值：0,成功;其他,错误代码
m_result mb_init(u8 checkmode)
{
    m_ctrl_dev.rxbuf=mymalloc(SRAMIN,M_MAX_FRAME_LENGTH);	//申请最大的帧接收缓存
    m_ctrl_dev.rxlen=0;
    m_ctrl_dev.frameok=0;
    m_ctrl_dev.checkmode=checkmode;
    if(m_ctrl_dev.rxbuf) {
        return MR_OK;
    } else {
        return MR_MEMORY_ERR;
    }
}



//结束MODBUS,释放内存
void mb_destroy(void)
{
    myfree(SRAMIN,m_ctrl_dev.rxbuf);	//申请最大的帧接收缓存
    m_ctrl_dev.rxlen=0;
    m_ctrl_dev.frameok=0;
}























