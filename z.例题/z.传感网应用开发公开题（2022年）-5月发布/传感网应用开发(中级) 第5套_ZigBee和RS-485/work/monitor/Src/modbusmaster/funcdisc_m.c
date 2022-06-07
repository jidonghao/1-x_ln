#include "funcdisc_m.h"
//#include "protocol.h"
#define FUNCDISC_NUM_MAX    0x07D0
#define FUNCDISC_NUM  3

#if FUNCDISC_NUM%8
u8    discbuf[FUNCDISC_NUM/8+1];
#else
u8    discbuf[FUNCDISC_NUM/8]  ;
#endif

__funcdisc_states mb_funcdisc_states;

void masterreadDiscRegister(u8 ucSndAddr, u16 usRegAddr, u16 usNRegs)
{
    u8 sendbuf[8]; 			  //���ͻ�����
    u8 send_cnt=0;

    u16 calchkval=0;			 //����õ���У��ֵ
    sendbuf[send_cnt]=ucSndAddr;
    send_cnt++;
    sendbuf[send_cnt]=0x02;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr >> 8;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr;

    send_cnt++;
    sendbuf[send_cnt]= usNRegs >> 8;
    send_cnt++;
    sendbuf[send_cnt]= usNRegs;
    send_cnt++;

    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM:						 //У���
        calchkval=mc_check_sum(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_XOR:						 //���У��
        calchkval=mc_check_xor(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC8:						 //CRC8У��
        calchkval=mc_check_crc8(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC16:						 //CRC16У��
        calchkval=mc_check_crc16(sendbuf,send_cnt);
        break;
    }

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) { //�����CRC16,����2���ֽڵ�CRC
        sendbuf[send_cnt]=(calchkval>>8)&0XFF;  //���ֽ���ǰ
        send_cnt++;
        sendbuf[send_cnt]=calchkval&0XFF;			 //���ֽ��ں�


        m_send_frame.address=sendbuf[0];
        m_send_frame.function=sendbuf[1];
        m_send_frame.reg_add=usRegAddr;
        m_send_frame.reg_cnt_value=usNRegs;
        m_send_frame.chkval=calchkval;
        DBG_B_INFO("address : %d",sendbuf[0]);
        DBG_B_INFO("func : %d",sendbuf[1]);
        DBG_B_INFO("address high: %d	 ,	 address low: %d",sendbuf[2],sendbuf[3]);
        DBG_B_INFO("number	high: %d  , number	loe:%d ",sendbuf[4],sendbuf[5]);
        DBG_B_INFO("crcvalue 0x%x	 cnt: %d	sendbuf[crch]: 0x%x  sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);

    }
    RS4851_Send_Buffer(sendbuf,send_cnt+1); //������һ֡����

}





