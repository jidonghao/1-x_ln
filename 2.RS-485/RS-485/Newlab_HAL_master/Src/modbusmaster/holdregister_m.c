#include "holdregister_m.h"
//#include "protocol.h"




void masterreadholdRegister(u8 ucSndAddr, u16 usRegAddr, u16 usNRegs)
{
    u8 sendbuf[8]; 				 //发送缓冲区
    u8 send_cnt=0;

//    u16 * input_value_p;

    u16 calchkval=0;			//计算得到的校验值
    sendbuf[send_cnt]=ucSndAddr;
    send_cnt++;
    sendbuf[send_cnt]=0x03;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr >> 8;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr;

    send_cnt++;
    sendbuf[send_cnt]= usNRegs >> 8;
    send_cnt++;
    sendbuf[send_cnt]= usNRegs;
    send_cnt++;
    DBG_B_INFO("address : %d",sendbuf[0]);
    DBG_B_INFO("func : %d",sendbuf[1]);
    DBG_B_INFO("address high: %d    ,   address low: %d",sendbuf[2],sendbuf[3]);
    DBG_B_INFO("number  high: %d  , number  loe:%d ",sendbuf[4],sendbuf[5]);
    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM: 						//校验和
        calchkval=mc_check_sum(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_XOR: 						//异或校验
        calchkval=mc_check_xor(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8校验
        calchkval=mc_check_crc8(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16校验
        calchkval=mc_check_crc16(sendbuf,send_cnt);
        break;
    }

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) { //如果是CRC16,则有2个字节的CRC
        sendbuf[send_cnt]=(calchkval>>8)&0XFF;	//高字节在前
        send_cnt++;
        sendbuf[send_cnt]=calchkval&0XFF;			//低字节在后

        m_send_frame.address=sendbuf[0];
        m_send_frame.function=sendbuf[1];
        m_send_frame.reg_add=usRegAddr;
        m_send_frame.reg_cnt_value=usNRegs;
        m_send_frame.chkval=calchkval;
        DBG_B_INFO("address : %d",sendbuf[0]);
        DBG_B_INFO("func : %d",sendbuf[1]);
        DBG_B_INFO("address high: %d    ,   address low: %d",sendbuf[2],sendbuf[3]);
        DBG_B_INFO("number  high: %d  , number  loe:%d ",sendbuf[4],sendbuf[5]);
        DBG_B_INFO("crcvalue 0x%x	cnt: %d    sendbuf[crch]: 0x%x	sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);

    }
    RS4851_Send_Buffer(sendbuf,send_cnt+1); //发送这一帧数据

}



void masterwriteholdRegister(u8 ucSndAddr, u16 usRegAddr, u16 regvalue)
{
    u8 sendbuf[8]; 				 //发送缓冲区
    u8 send_cnt=0;

//    u16 * input_value_p;

    u16 calchkval=0;			//计算得到的校验值
    sendbuf[send_cnt]=ucSndAddr;
    send_cnt++;
    sendbuf[send_cnt]=0x06;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr >> 8;
    send_cnt++;
    sendbuf[send_cnt]= usRegAddr;

    send_cnt++;
    sendbuf[send_cnt]= regvalue >> 8;
    send_cnt++;
    sendbuf[send_cnt]= regvalue;
    send_cnt++;

    switch(m_ctrl_dev.checkmode) {
    case M_FRAME_CHECK_SUM: 						//校验和
        calchkval=mc_check_sum(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_XOR: 						//异或校验
        calchkval=mc_check_xor(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC8:						//CRC8校验
        calchkval=mc_check_crc8(sendbuf,send_cnt);
        break;
    case M_FRAME_CHECK_CRC16:						//CRC16校验
        calchkval=mc_check_crc16(sendbuf,send_cnt);
        break;
    }

    if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) { //如果是CRC16,则有2个字节的CRC
        sendbuf[send_cnt]=(calchkval>>8)&0XFF;	//高字节在前
        send_cnt++;
        sendbuf[send_cnt]=calchkval&0XFF;			//低字节在后
        m_send_frame.address=sendbuf[0];
        m_send_frame.function=sendbuf[1];
        m_send_frame.reg_add=usRegAddr;
        m_send_frame.reg_cnt_value=regvalue;
        m_send_frame.chkval=calchkval;
        DBG_B_INFO("address : %d",sendbuf[0]);
        DBG_B_INFO("func : %d",sendbuf[1]);
        DBG_B_INFO("address high: %d    ,   address low: %d",sendbuf[2],sendbuf[3]);
        DBG_B_INFO("number  high: %d  , number  loe:%d ",sendbuf[4],sendbuf[5]);
        DBG_B_INFO("crcvalue 0x%x	cnt: %d    sendbuf[crch]: 0x%x	sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);

    }
    RS4851_Send_Buffer(sendbuf,send_cnt+1); //发送这一帧数据

}






