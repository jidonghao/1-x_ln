#include "ioCC2530.h"
#include "hal_defs.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h"
#include "UART_PRINT.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
 
static basicRfCfg_t cfg; 
uint8 rxData[16]; 
uint16 rxLen = 0;

uint8 uTxData[16]; 
uint16 uTxlen = 0;

uint16 sensorValue = 0;

/*  LED n 闪烁 time 毫秒 宏 */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

void ByteCopy(uint8 *dst,uint8 *src,uint8 len)
{
    while(len--)
    {
        *dst++=*src++;
    }
}

void configRF(void)
{
    /*开始完善代码——basicRf配置和初始化*/
 cfg.channel     =   20;    //zigbee的频道设置
    cfg.panId       =   0x0321;        //zigbee的ID号设置
    cfg.myAddr      =   0x0002;       //设置本机地址
    cfg.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&cfg) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                           // 打开RF

    /*结束完善代码*/
}


void main(void)
{
    halBoardInit();
    configRF();
    
    while(1)
    {
       
        if(basicRfPacketIsReady())
        {
          
    /*开始完善代码——接收发送端(ZIGBEE通信模块主机）无线数据实现LED2灯闪烁，根据接收数据控制指示灯*/
          
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen==8)
            {

                 sensorValue=rxData[1];
                 sensorValue<<=8;
                 sensorValue|=rxData[2];
                
                 if(sensorValue<100)
                    P1&=~0x40;
                 else
                    P1|=0x40;
                  FlashLed(2,100);
            }
          
          
          
          
          
          
    /*结束完善代码*/
        }
    }
  
}


