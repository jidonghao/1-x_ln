#include "funcdisc.h"
//#include "protocol.h"
#define FUNCDISC_NUM_MAX    0x07D0
#define FUNCDISC_NUM  3

#if FUNCDISC_NUM%8
u8    discbuf[FUNCDISC_NUM/8+1];
#else
u8    discbuf[FUNCDISC_NUM/8]  ;
#endif

 __funcdisc_states mb_funcdisc_states;

u8 ReadDiscRegister(void)
{
    u16 regaddress;
    u16 regcount;
//    u8 * discbuf_p;

//    u16 iregindex,iRegBitIndex , iNReg;
   
  //m_frame_typedef rxframe;
    u8 sendbuf[20];				  //发送缓冲区
    u8 send_cnt=0;
    u8 send_byte=0;
    u16 calchkval=0;			//计算得到的校验值
//    mb_funcdisc_states.VAL=5;
    regaddress=(u16)(m_ctrl_dev.rxbuf[2]<<8);
    regaddress|=(u16)(m_ctrl_dev.rxbuf[3]);

    regcount =(u16)(m_ctrl_dev.rxbuf[4]<<8);
    regcount |= (u16)(m_ctrl_dev.rxbuf[5]);

//    discbuf_p =  discbuf;

    DBG_B_INFO("regaddress : %d   regcount  :  %d ",regaddress,regcount);
    if((1<=regcount)&&(regcount<FUNCDISC_NUM)) {
        if(regaddress<=5) {
            sendbuf[send_cnt]=SLAVE_ADDRESS;
            send_cnt++;
            sendbuf[send_cnt]=0x02;
            send_cnt++;
			if( ( regcount & 0x0007 ) != 0 )    //能不能被8整除
            {
                send_byte = ( u8 ) ( regcount / 8 + 1 );
            }
            else
            {
                send_byte = ( u8 ) ( regcount / 8 );
            }
            sendbuf[send_cnt]=send_byte;
            send_cnt++;
            DBG_B_INFO("address : %d",sendbuf[0]);
            DBG_B_INFO("func : %d",   sendbuf[1]);
            DBG_B_INFO("send_byte :%d",sendbuf[2]);
            DBG_B_INFO("regaddress : %d     regcount :%d ",regaddress,regcount);

			if(regaddress==0)
			  sendbuf[send_cnt]=(u8)mb_funcdisc_states.val.disc_BIT0;
			else if(regaddress==1)
			  sendbuf[send_cnt]=(u8)mb_funcdisc_states.val.disc_BIT1;
			else if(regaddress==2)
			  sendbuf[send_cnt]=(u8)mb_funcdisc_states.val.disc_BIT2;
			
			send_cnt++;
      //        iregindex=(u16)regaddress/8;  //此处为0 
      //        iRegBitIndex = (u16) regaddress % 8; // 1~5
	  //	      iNReg= ( u8 ) ( regcount / 8 + 1 );

      //       while(iNReg>0) {
      //           sendbuf[send_cnt]=(u8)(input_value_p[iregindex]>>8);
      //           send_cnt++;
      //           sendbuf[send_cnt]=(u8)(input_value_p[iregindex]& 0xFF);
      //           send_cnt++;
      //           iregindex++;
      //           regcount--;
      //       }


			
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


u8 GetBits( u8 * ucByteBuf, u16 usBitOffset, u8 ucNBits )
{
    u16          usWordBuf;
    u16          usMask;
    u16          usByteOffset;
    u16          usNPreBits;

    /* Calculate byte offset for first byte containing the bit values starting
     * at usBitOffset. */
    usByteOffset = ( u16 )( ( usBitOffset ) / 8U );

    /* How many bits precede our bits to set. */
    usNPreBits = ( u16 )( usBitOffset - usByteOffset * 8U );

    /* Prepare a mask for setting the new bits. */
    usMask = ( u16 )( ( 1 << ( u16 ) ucNBits ) - 1 );

    /* copy bits into temporary storage. */
    usWordBuf = ucByteBuf[usByteOffset];
    usWordBuf |= ucByteBuf[usByteOffset + 1] << 8u;

    /* throw away unneeded bits. */
    usWordBuf >>= usNPreBits;

    /* mask away bits above the requested bitfield. */
    usWordBuf &= usMask;

    return ( u8 ) usWordBuf;
}





