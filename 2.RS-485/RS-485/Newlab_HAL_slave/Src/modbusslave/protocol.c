#include <string.h>
#include <stdint.h>
#include "protocol.h"
#include "..\malloc\malloc.h"

#include "debug_printf.h"
#include "app_slave.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////



m_protocol_dev_typedef m_ctrl_dev;	//����modbus������

//static void mc_read_coil(u8* length ,u8* rxbuf);
//static void mc_read_digit(void);
//static void mc_read_analog(void);
//static void mc_read_write(void);



//static void mc_read_coil(u8* length ,u8* rxbuf);



void clr485rxbuf(void)
{
    mymemset(m_ctrl_dev.rxbuf, 0x00, M_MAX_FRAME_LENGTH); //��ջ���
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
}

//����һ֡���ݣ���������洢��fx����
//ע�⣺���������õ�malloc��fx����ָ�������ڴ棬��������fx��һ��Ҫ�ͷ��ڴ棡��
//����������ڴ�й¶������
//fx��ָ֡��
//buf���������ݻ����������ڽ��յ������ݣ�
//len���������ݳ���
//����ֵ�����������0��OK��������������롣
m_result mb_unpack_frame(m_frame_typedef *fx)
{
    u16 rxchkval=0;   	 		//���յ���У��ֵ
    u16 calchkval=0;			  //����õ���У��ֵ
    u8 cmd = 0 ;            //���㹦����
    u8 datalen=0; 			  	//��Ч���ݳ���
    u8 address=0;
    u8 res;
    DBG_B_INFO("mb_unpack_frame");
//    fx->datalen=0; 				  //���ݳ�������
    if(m_ctrl_dev.rxlen>M_MAX_FRAME_LENGTH||m_ctrl_dev.rxlen<M_MIN_FRAME_LENGTH) {
        m_ctrl_dev.rxlen=0;			//���rxlen
        m_ctrl_dev.frameok=0;		//���framok��ǣ��Ա��´ο�����������

        return MR_FRAME_FORMAT_ERR;//֡��ʽ����
    }
    datalen=m_ctrl_dev.rxlen;
    DBG_B_INFO("��ǰ���ݳ��� %d",m_ctrl_dev.rxlen);

    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM:							//У���
        calchkval=mc_check_sum(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_XOR:							//���У��
        calchkval=mc_check_xor(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8У��
        calchkval=mc_check_crc8(m_ctrl_dev.rxbuf,datalen+4);
        rxchkval=m_ctrl_dev.rxbuf[datalen+4];
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16У��
        calchkval=mc_check_crc16(m_ctrl_dev.rxbuf,datalen-2);
        rxchkval=((u16)m_ctrl_dev.rxbuf[datalen-2]<<8)+m_ctrl_dev.rxbuf[datalen-1];
        break;
    }
    DBG_B_INFO("calchkval = 0x%x  ��rxchkval  = 0x%x   ��datalen = 0x%x",calchkval,rxchkval,datalen);
//    DBG_B_INFO("cmd = 0x%x ",cmd);

    m_ctrl_dev.rxlen=0;			//���rxlen
    m_ctrl_dev.frameok=0;		//���framok��ǣ��Ա��´ο�����������

    if(calchkval==rxchkval) {	//У������
        address=m_ctrl_dev.rxbuf[0];
        if (address!= SLAVE_ADDRESS) {
            return MR_FRAME_SLAVE_ADDRESS;    //֡��ʽ����
        }

        cmd=m_ctrl_dev.rxbuf[1];

        if ((cmd > 0x06 )||(cmd < 0x01)) {
            return MR_FRANE_ILLEGAL_FUNCTION;    //����֡����
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
//            fx->data=mymalloc(SRAMIN,fx->datalen);		//�����ڴ�
//            for(datalen=0; datalen<fx->datalen; datalen++) {
//                fx->data[datalen]=m_ctrl_dev.rxbuf[4+datalen];		//��������
//            }
//        }
//        fx->chkval=rxchkval;	//��¼У��ֵ
    } else {
        return MR_FRAME_CHECK_ERR;
    }
    return MR_OK;
}





//���һ֡���ݣ�������
//fx��ָ����Ҫ�����֡
void mb_packsend_frame(m_frame_typedef *fx)
{
    u16 i;
    u16 calchkval=0;			//����õ���У��ֵ
    u16 framelen=0;				//������֡����
    u8 *sendbuf;				//���ͻ�����

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {
        framelen=6+fx->datalen;
    } else {
        framelen=5+fx->datalen;
    }
    sendbuf=mymalloc(SRAMIN,framelen);	//�����ڴ�
    sendbuf[0]=fx->address;
    sendbuf[1]=fx->function;
    sendbuf[2]=fx->count;
    sendbuf[3]=fx->datalen;
    for(i=0; i<fx->datalen; i++) {
        sendbuf[4+i]=fx->data[i];
    }
    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM:							//У���
        calchkval=mc_check_sum(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_XOR:							//���У��
        calchkval=mc_check_xor(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8У��
        calchkval=mc_check_crc8(sendbuf,fx->datalen+4);
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16У��
        calchkval=mc_check_crc16(sendbuf,fx->datalen+4);
        break;
    }

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {	//�����CRC16,����2���ֽڵ�CRC
        sendbuf[4+fx->datalen]=(calchkval>>8)&0XFF; 	//���ֽ���ǰ
        sendbuf[5+fx->datalen]=calchkval&0XFF;			//���ֽ��ں�
    } else {
        sendbuf[4+fx->datalen]=calchkval&0XFF;
    }
    RS4851_Send_Buffer(sendbuf,framelen);	//������һ֡����
    myfree(SRAMIN,sendbuf);			//�ͷ��ڴ�
}



//��ʼ��modbus
//checkmode��У��ģʽ��0,У���;1,���;2,CRC8;3,CRC16
//����ֵ��0,�ɹ�;����,�������
m_result mb_init(u8 checkmode)
{
    m_ctrl_dev.rxbuf=mymalloc(SRAMIN,M_MAX_FRAME_LENGTH);	//��������֡���ջ���
    m_ctrl_dev.rxlen=0;
    m_ctrl_dev.frameok=0;
    m_ctrl_dev.checkmode=checkmode;
    if(m_ctrl_dev.rxbuf) {
        return MR_OK;
    } else {
        return MR_MEMORY_ERR;
    }
}



//����MODBUS,�ͷ��ڴ�
void mb_destroy(void)
{
    myfree(SRAMIN,m_ctrl_dev.rxbuf);	//��������֡���ջ���
    m_ctrl_dev.rxlen=0;
    m_ctrl_dev.frameok=0;
}























