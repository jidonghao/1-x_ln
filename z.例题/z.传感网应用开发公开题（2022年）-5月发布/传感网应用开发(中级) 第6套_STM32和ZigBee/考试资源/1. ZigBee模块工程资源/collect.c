#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "hal_pwm.h" 
#include "UART_PRINT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*****点对点通讯地址设置******/
#define RF_CHANNEL                16        // 频道 11~26
#define PAN_ID                    0x0000    //网络id
#define MY_ADDR                   0x0000    //本机模块地址

/******ZigBee通讯协议相关*******/
#define START_HEAD    0xCC//帧头  
#define CMD_READ      0x01//读传感器数据  
#define SENSOR_TEMP   0x01//温度  
#define SENSOR_RH     0x02//湿度  
#define SENSOR_LIGHT  0x06//光照传感器  

/********无线数据缓存********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
 
/**************************************************/
static basicRfCfg_t basicRfConfig;


//使LED n 闪烁 time 毫秒
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置
    basicRfConfig.myAddr      =   MY_ADDR;       //设置本机地址
    basicRfConfig.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                           // 打开RF
}

/************************************************************************
*函数：uint8 CheckSum(uint8 *buf, uint8 len) 
*功能：计算校验和 
*输入：uint8 *buf-指向输入缓存区, uint8 len输入数据字节个数 
*输出：无 
*返回：返回校验和 
*特殊说明：无 
*************************************************************************/  
uint8 CheckSum(uint8 *buf, uint8 len)  
{  
  uint8 temp = 0;  
  while(len--)  
  {  
    temp += *buf;  
    buf++;  
  }  
  return (uint8)temp;  
}  

/************************************************************************
*函数：void TransComData(uint8  *buf, uint8 len)
*功能：把收到的数据转换为物联网网关通信协议
*************************************************************************/  
void TransComData(uint8  *buf, uint8 len)
{
  uint8 check = 0;
        check = CheckSum((uint8 *)pRxData, pRxData[2]);
        if((pRxData[0] == START_HEAD) && (check == pRxData[pRxData[2]]))//帧头正确且校验通过
        {
       /*开始完善代码——根据物联网网关通信协议建立数据帧，存放在pTxData缓冲区*/  
          
          
          
          
          
          
          
          
          
          
          
       /*结束完善代码——根据物联网网关通信协议建立数据帧，存放在pTxData缓冲区*/  
         }           
}

/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;  
    halBoardInit();  //模块相关资源的初始化
    ConfigRf_Init(); //无线收发参数的配置初始化 
    
    while(1)
    {
      /*开始完善代码——接收ZIGBEE从模块无线数据，实现D5灯闪烁，调用void TransComData(uint8  *buf, uint8 len)
      根据物联网网关通信协议建立数据帧，调用halUartWrite把数据通过串口发送*/     
       
      
      
      
      
      
      
      
      
      
       /*结束完善代码—*/     
    }
}
/************************MAIN END ****************************/