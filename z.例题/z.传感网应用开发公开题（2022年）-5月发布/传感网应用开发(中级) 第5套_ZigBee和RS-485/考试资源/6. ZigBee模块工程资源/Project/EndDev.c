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
uint8 rxData[128];

void configRF(void)
{
  /*开始完善代码——basicRf配置和初始化*/
  
  
  
  
  
  
  
  
  /*结束完善代码——basicRf配置和初始化*/
}

void adc_Init(void)
{
    APCFG  |=0x01;
}

/************************************************************
* 名称       get_adc
* 功能       读取ADC通道0电压值
* 入口参数   无
* 出口参数   16位电压值，分辨率为10mV
***************获取ADC通道0电压值************************/

uint16 get_adc(void)
{
    uint32 value;
    ADCIF = 0;   //清ADC 中断标志
    //采用基准电压avdd5:3.3V，通道0，启动AD转化
    ADCCON3 = (0x80 | 0x10 | 0x00);
    while ( !ADCIF )
    {
            ;  //等待AD转化结束
    }
    value = ADCH;
    value = value<< 8;
    value |= ADCL;
    // AD值转化成电压值
    // 0 表示 0V ，32768 表示 3.3V
    // 电压值 = (value*3.3)/32768 （V)
    value = (value * 330);
    value = value >> 15;   // 除以32768
    // 返回分辨率为0.01V的电压值
    return (uint16)value;
}

void main(void)
{
  uint16 lightValue;
  halBoardInit();
  configRF(); 
  
  Timer4_Init();
  Timer4_On();
  adc_Init();
 
  while(1)
  {
      
      /*开始完善代码—— 将光照数据发送给ZigBee白板,数据格式可以自由定义*/
      /*每次发送数据点亮LED2 0.1秒*/
        if(GetSendDataFlag())
        {
    
    
    
    
    
    
        }
    
      /*结束完善代码*/
  }
}