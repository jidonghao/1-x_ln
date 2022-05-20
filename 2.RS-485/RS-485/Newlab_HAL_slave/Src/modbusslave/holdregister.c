#include "holdregister.h"
//#include "protocol.h"


u16 holdbuf[3]= {0xBBBB,0xBBBB,0xBBBB};
//u16 inbuf[4]= {111,222,333,444};

u8 ReadHoldRegister(void)
{
    u16 regaddress;
    u16 regcount;
    u16 * input_value_p;

    u16 iregindex;

    //m_frame_typedef rxframe;
    u8 sendbuf[20];				  //发送缓冲区
    u8 send_cnt=0;

    u16 calchkval=0;			//计算得到的校验值

    regaddress=(u16)(m_ctrl_dev.rxbuf[2]<<8);
    regaddress|=(u16)(m_ctrl_dev.rxbuf[3]);

    regcount =(u16)(m_ctrl_dev.rxbuf[4]<<8);
    regcount |= (u16)(m_ctrl_dev.rxbuf[5]);

    input_value_p =  holdbuf;
    DBG_B_INFO("regaddress : %d   regcount  :  %d ",regaddress,regcount);
    if((1<=regcount)&&(regcount<4)) {
        if(regaddress<=3) {
            sendbuf[send_cnt]=SLAVE_ADDRESS;
            send_cnt++;
            sendbuf[send_cnt]=0x03;
            send_cnt++;
            sendbuf[send_cnt]=regcount*2;
            send_cnt++;
            DBG_B_INFO("address : %d",sendbuf[0]);
            DBG_B_INFO("func : %d",sendbuf[1]);
            DBG_B_INFO("read cnt : %d",sendbuf[2]);
            DBG_B_INFO("regaddress : %d     regcount :%d ",regaddress,regcount);
            iregindex=regaddress-0;

            while(regcount>0) {
                sendbuf[send_cnt]=(u8)(input_value_p[iregindex]>>8);
                send_cnt++;
                sendbuf[send_cnt]=(u8)(input_value_p[iregindex]& 0xFF);
                send_cnt++;
                iregindex++;
                regcount--;
            }
            switch(m_ctrl_dev.checkmode) {
            case M_FRAME_CHECK_SUM:							//校验和
                calchkval=mc_check_sum(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_XOR:							//异或校验
                calchkval=mc_check_xor(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_CRC8:						//CRC8校验
                calchkval=mc_check_crc8(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_CRC16:						//CRC16校验
                calchkval=mc_check_crc16(sendbuf,send_cnt);
                break;
            }
            if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {	//如果是CRC16,则有2个字节的CRC
                sendbuf[send_cnt]=(calchkval>>8)&0XFF; 	//高字节在前
                send_cnt++;
                sendbuf[send_cnt]=calchkval&0XFF;			//低字节在后
                DBG_B_INFO("crcvalue 0x%x   cnt: %d    sendbuf[crch]: 0x%x  sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);
//		        send_cnt++;
            }
            RS4851_Send_Buffer(sendbuf,send_cnt+1);	//发送这一帧数据
        }

    } else {
        return 1;
    }
    return 0;
}

u8 WriteHoldRegister(void)
{
    u16 regaddress;
    u16 regvalue;
    u16 *input_value_p;

    u16 iregindex;

    //m_frame_typedef rxframe;
    u8 sendbuf[20];				  //发送缓冲区
    u8 send_cnt=0;

    u16 calchkval=0;			//计算得到的校验值

    regaddress=(u16)(m_ctrl_dev.rxbuf[2]<<8);
    regaddress|=(u16)(m_ctrl_dev.rxbuf[3]);

    regvalue  =(u16)(m_ctrl_dev.rxbuf[4]<<8);
    regvalue |= (u16)(m_ctrl_dev.rxbuf[5]);

    input_value_p =  holdbuf;
    DBG_B_INFO("regaddress : %d   value  :  %d ",regaddress,regvalue);
    if(regvalue<=4096) {
        if(regaddress<=2){
            sendbuf[send_cnt]=SLAVE_ADDRESS;
            send_cnt++;
            sendbuf[send_cnt]=0x06;
            send_cnt++;
            sendbuf[send_cnt]=(regaddress>>8)&0xFF;
            send_cnt++;
            sendbuf[send_cnt]=regaddress&0xFF;
            send_cnt++;

            DBG_B_INFO("address : %d",sendbuf[0]);
            DBG_B_INFO("func : %d",sendbuf[1]);
            DBG_B_INFO("regaddress high: %d",sendbuf[2]);
            DBG_B_INFO("regaddress low : %d",sendbuf[3]);

            iregindex=regaddress-0;
            input_value_p[iregindex]=regvalue;
			
            sendbuf[send_cnt]=(u8)(regvalue>>8)&0xFF;
            send_cnt++;
			sendbuf[send_cnt]=(u8)(regvalue)&0xFF;
			send_cnt++;

        }
        switch(m_ctrl_dev.checkmode) {
        case M_FRAME_CHECK_SUM:							//校验和
            calchkval=mc_check_sum(sendbuf,send_cnt);
            break;
        case M_FRAME_CHECK_XOR:							//异或校验
            calchkval=mc_check_xor(sendbuf,send_cnt);
            break;
        case M_FRAME_CHECK_CRC8:						//CRC8校验
            calchkval=mc_check_crc8(sendbuf,send_cnt);
            break;
        case M_FRAME_CHECK_CRC16:						//CRC16校验
            calchkval=mc_check_crc16(sendbuf,send_cnt);
            break;
        }
        if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {	//如果是CRC16,则有2个字节的CRC
            sendbuf[send_cnt]=(calchkval>>8)&0XFF; 	//高字节在前
            send_cnt++;
            sendbuf[send_cnt]=calchkval&0XFF;			//低字节在后
            DBG_B_INFO("crcvalue 0x%x   cnt: %d    sendbuf[crch]: 0x%x  sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);

        }
        RS4851_Send_Buffer(sendbuf,send_cnt+1);	//发送这一帧数据
    }
else
{
    return 1;
}
    return 0;
}





