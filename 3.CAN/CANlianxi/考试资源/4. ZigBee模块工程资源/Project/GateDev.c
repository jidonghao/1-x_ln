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
uint8 rxData[16];   //接收无线数据缓存区
uint16 rxLen = 0;

uint8 uTxData[16];  //发送UART数据缓存区
uint16 uTxlen = 0;

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
  
  
  
  
  
  
  
  
  
  /*结束完善代码——basicRf配置和初始化*/
}


void main(void)
{
  uint16 len = 0;   //定义串口接收到数据的长度
  halBoardInit();
  configRF();
    
  while(1)
  {    
    /*开始完善代码
      接收发送端(ZIGBEE通信模块主机）无线数据实现LED1灯闪烁，根据接收数据控制警报灯（例如电压低于2V，关闭报警器，高于2V，打开报警器）
      每次收到数据后，点亮LED2 0.1秒
    */
    
    if(basicRfPacketIsReady())
    {






    }
    /*结束完善代码*/
       
  }
  
}

