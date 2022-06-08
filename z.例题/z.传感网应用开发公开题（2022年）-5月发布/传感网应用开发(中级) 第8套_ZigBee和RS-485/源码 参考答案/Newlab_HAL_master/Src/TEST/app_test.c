/******************************************************************************
******************************************************************************
* @file    app/app_test.c
* @author  zhao
* @version V1.0.0
* @date    2019.07.10
* @update  2019.09.14
* @brief   主程序
******************************************************************************
******************************************************************************/

/**************include*************************/

#include  "app_test.h"

void RS_485test(u8 cmd)
{
    /**485 模块测试代码2**/
    u8 i=0;
    uint8_t rs485buf[5];
    /**485 模块测试代码2 end**/

    /**485 模块测试代码3**/
//   for(i=0; i<5; i++) {
//     rs485buf[i]=i;//填充发送缓冲区
//   }

//    RS4851_Send_Buffer(rs485buf,5);//发送5个字节
//    RS4852_Send_Buffer(rs485buf,5);//发送5个字节

    if(cmd==1) {
        RS485_SendStr("test111111 \r\n");  // 485 1 发送测试
//        RS4851_Send_Buffer(rs485buf, 5);
	    DBG_B_INFO("测试485通道1发送");
        HAL_Delay(50);
    } else if (cmd ==2) {
        RS4852_SendStr("test222222\r\n");    // 485 2 发送测试
//        RS4852_Send_Buffer(rs485buf, 5);
		DBG_B_INFO("测试485通道2发送");
        HAL_Delay(50);
    } else if (cmd ==3) {
        RS4853_SendStr("test33333 \r\n"); // 485 3 发送测试
//        RS4853_Send_Buffer(rs485buf, 5);
		DBG_B_INFO("测试485通道3发送");
        HAL_Delay(50);
    } else if (cmd ==4) {
        kfifo_pull_out(&usart2_fifo,rs485buf,5);    // 485 1 接收测试
        DBG_B_INFO("buf[0]=0x%x buf[1]=0x%x buf[2]=0x%x buf[3]=0x%x buf[4]=0x%x ",   \
                   rs485buf[0],rs485buf[1],rs485buf[2],rs485buf[3],rs485buf[4]);
    } else if(cmd ==5) {
        kfifo_pull_out(&usart3_fifo,rs485buf,5);    // 485 2 接收测试
        DBG_B_INFO("buf[0]=0x%x buf[1]=0x%x buf[2]=0x%x buf[3]=0x%x buf[4]=0x%x ",   \
                   rs485buf[0],rs485buf[1],rs485buf[2],rs485buf[3],rs485buf[4]);
    } else if(cmd ==6) {
        kfifo_pull_out(&uart5_fifo,rs485buf,5);     // 485 3 接收测试
        DBG_B_INFO("buf[0]=0x%x buf[1]=0x%x buf[2]=0x%x buf[3]=0x%x buf[4]=0x%x ",   \
                   rs485buf[0],rs485buf[1],rs485buf[2],rs485buf[3],rs485buf[4]);
    }
    /**485 模块测试代码3 end**/

}

void modbus_show_result(m_rev_frame_typedef * fx)
{
    DBG_B_INFO("address: %02X",fx->address);
    DBG_B_INFO("function: %02X",fx->function);
  
}

void master_send_test(u8 cmd)
{

    switch (cmd) {
    case 0x01:
        masterreadDiscRegister(1,1,1);
        break;
    case 0x02:
        masterreadholdRegister(1,0,1);
        break;
    case 0x03:
        masterInputRegister(1,1,1);
        break;
    case 0x04:
        masterwriteholdRegister(1,1,1);
        break;
    default:
        break;
    }
}

void modbus_master_rec2(void)
{
    u8 res;

    // DBG_B_INFO("modbus_master_rec");
    if(m_ctrl_dev.frameok) {

        res= mb_unpack_frame(&m_send_frame,&m_rev_frame);
        if(res==MR_OK) {	//解析包成功
            DBG_B_INFO("解析包成功");

            modbus_show_result(&m_rev_frame);

//            if(rxframe.datalen) {
//                myfree(SRAMIN,rxframe.data);    //释放内存
//            }
        } else if(res==MR_FRAME_FORMAT_ERR) {
            DBG_R_E("帧格式错误");
        } else if(res==MR_FRAME_CHECK_ERR) {
            DBG_R_E("帧校验错误");
        } else if(res==MR_FRAME_SLAVE_ADDRESS) {
            DBG_R_E("从机地址错误");
        } else if(res==MR_FRANE_ILLEGAL_FUNCTION) {
            DBG_R_E("功能码错误");
        }
        clr485rxbuf();
    }

}




