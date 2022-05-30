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
    /*开始完善代码——ADC初始化*/
    
    
    /*结束完善代码——ADC初始化*/
}

uint16 get_adc(void)
{
    uint32 value;
    
    /*开始完善代码——采集ADC数据*/
    
    
    
    
    
    /*结束完善代码——采集ADC数据*/
    
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
      
      /*开始完善代码—— 采集ADC数据，将数据发送给ZigBee白板,数据格式可以自由定义*/
      /*每3秒发送一次数据，每次发送数据点亮LED2 0.1秒*/
    
    
    
    
    
    
    
    
    
      /*结束完善代码——采集ADC数据，将数据发送给ZigBee白板,数据格式可以自由定义*/
  }
}