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

#define FlashLed(n,time)do{\
                        halLedSet(n);\
                        halMcuWaitMs(time);\
                        halLedClear(n);\
                        }while(0)

static basicRfCfg_t cfg;
uint8 rxData[128];
uint8 txData[128];
void configRF(void)
{
  /*开始完善代码——basicRf配置和初始化*/
   cfg.panId       =   0x0016;        //zigbee的ID号设置
    cfg.channel     =   16;    //zigbee的频道设置
    cfg.myAddr      =   0x0001;       //设置本机地址
    cfg.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&cfg) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                           // 打开RF
  
  
  
  
  
  
  /*结束完善代码——basicRf配置和初始化*/
}

void adc_Init(void)
{
    /*开始完善代码——ADC初始化*/
      APCFG  |=0x01;
    /*结束完善代码——ADC初始化*/
}

uint16 get_adc(void)
{
    uint32 value;
    
    /*开始完善代码——采集ADC数据*/
    
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
  if(GetSendDataFlag())
        {
            Timer4_On();

            lightValue =  get_adc();
            txData[0] = 0x06;
            txData[1] = lightValue>>8;
            txData[2] = lightValue&0xff;
            txData[3] = 0;
            txData[4] = 0x05;
            txData[5] = 0;
            txData[6] = '\r';
            txData[7] = '\n';
            
            basicRfSendPacket(0x0002, txData,8);
            FlashLed(2,100);
        }           
    
      /*结束完善代码——采集ADC数据，将数据发送给ZigBee白板,数据格式可以自由定义*/
  }
}