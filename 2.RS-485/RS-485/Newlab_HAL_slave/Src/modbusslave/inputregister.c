#include "inputregister.h"
//#include "protocol.h"

//inbuf[0] ����
//inbuf[1] ����
//inbuf[2] ����
//inbuf[3] ��ȼ����

u16 inbuf[4]= {0};
//u16 inbuf[4]= {111,222,333,444};

u8 ReadInputRegister(void)
{
    u16 regaddress;
    u16 regcount;
    u16 * input_value_p;

    u16 iregindex;

//m_frame_typedef rxframe;
    u8 sendbuf[20];				  //���ͻ�����
    u8 send_cnt=0;

    u16 calchkval=0;			//����õ���У��ֵ

    regaddress=(u16)(m_ctrl_dev.rxbuf[2]<<8);
    regaddress|=(u16)(m_ctrl_dev.rxbuf[3]);

    regcount =(u16)(m_ctrl_dev.rxbuf[4]<<8);
    regcount |= (u16)(m_ctrl_dev.rxbuf[5]);

    input_value_p =  inbuf;
    DBG_B_INFO("regaddress : %d   regcount  :  %d ",regaddress,regcount);
    if((1<=regcount)&&(regcount<4)) {
        if(((s32)regaddress>=0)&&(regaddress<=3)) {
            sendbuf[send_cnt]=SLAVE_ADDRESS;
            send_cnt++;
            sendbuf[send_cnt]=0x04;
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
            case M_FRAME_CHECK_SUM:							//У���
                calchkval=mc_check_sum(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_XOR:							//���У��
                calchkval=mc_check_xor(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_CRC8:						//CRC8У��
                calchkval=mc_check_crc8(sendbuf,send_cnt);
                break;
            case M_FRAME_CHECK_CRC16:						//CRC16У��
                calchkval=mc_check_crc16(sendbuf,send_cnt);
                break;
            }

            if(m_ctrl_dev.checkmode==M_FRAME_CHECK_CRC16) {	//�����CRC16,����2���ֽڵ�CRC
                sendbuf[send_cnt]=(calchkval>>8)&0XFF; 	//���ֽ���ǰ
                send_cnt++;
                sendbuf[send_cnt]=calchkval&0XFF;			//���ֽ��ں�
                DBG_B_INFO("crcvalue 0x%x   cnt: %d    sendbuf[crch]: 0x%x  sendbuf[crcl]: 0x%x ",calchkval,send_cnt,sendbuf[send_cnt-1],sendbuf[send_cnt]);
//		        send_cnt++;
            }
            RS4851_Send_Buffer(sendbuf,send_cnt+1);	//������һ֡����
        }

    } else {
        return 1;
    }
    return 0;
}






