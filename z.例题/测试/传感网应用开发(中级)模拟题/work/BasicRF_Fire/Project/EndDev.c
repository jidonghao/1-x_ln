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
#include "TIMER.h"
 
/*  LED n 闪烁 time 毫秒 宏 */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

/******通讯协议相关*******/  
#define START_HEAD    0xCC//帧头  
#define CMD_READ      0x01//读传感器数据  
#define SENSOR_TEMP   0x01//温度  
#define SENSOR_RH     0x02//湿度  
#define SENSOR_FIRE   0x03//火焰 
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128

#define BOARD_TYPE  0x06
#define BOARD_ADDR  0x05

static basicRfCfg_t cfg;
uint8 txData[16];

void configRF(void)
{
    /*开始完善代码——basicRf配置和初始化*/

     
    cfg.channel     =   20;    //zigbee的频道设置
    cfg.panId       =   0x0321;        //zigbee的ID号设置
    cfg.myAddr      =   0x0001;       //设置本机地址
    cfg.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&cfg) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                           // 打开RF

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
        /*代码完善：完成对ADCH和ADCL两个寄存器的正确传值，将高低八位放入U16型的value中*/
    /*并对value进行电压转换计算*/
   
    value = ADCH;
    value = value<< 8;
    value |= ADCL;

    
    /*结束完善代码——采集ADC数据*/

    value = value/4096*3.3;
   // value = value >> 15;   // 除以32768
    // 返回分辨率为0.01V的电压值

    /*value传值代码结束*/
    return (uint16)value;
}


void main(void)
{
  uint16 fireValue;
    halBoardInit();
    configRF(); 
    Timer4_Init();
    Timer4_On();
    adc_Init();
    
    while(1)
    {
        if(GetSendDataFlag())
        {
      /*开始完善代码—— 采集ADC数据，将数据发送给ZigBee白板,数据格式可以自由定义*/
      /*每次发送数据点亮LED4 0.1秒*/
    
            Timer4_On();

            fireValue =  get_adc();
            txData[0] = 0x06;
            txData[1] = fireValue>>8;
            txData[2] = fireValue&0xff;
            txData[3] = 0;
            txData[4] = SENSOR_FIRE;
            txData[5] = 0;
            txData[6] = '\r';
            txData[7] = '\n';
            
            basicRfSendPacket(0x0002, txData,8);
            FlashLed(4,100);
    
    
          
          
    
    
    
    
    
      /*结束完善代码*/
        }           
    }
}