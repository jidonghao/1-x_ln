#include "hal_defs.h"  
#include "hal_cc8051.h"  
#include "hal_int.h"  
#include "hal_mcu.h"  
#include "hal_board.h"  
#include "hal_led.h"  
#include "hal_adc.h"  
#include "hal_rf.h"  
#include "basic_rf.h"  
#include "hal_uart.h"   
#include "TIMER.h"  
#include "get_adc.h"  
#include "sh10.h"  
#include "UART_PRINT.h"  
#include "util.h"  
#include <stdlib.h>  
#include <string.h>  
#include <stdio.h>  
#include <math.h>  


 /*开始完善代码——点对点通讯地址设置*/
#define RF_CHANNEL                23        // 频道 11~26  
#define PAN_ID                    0x0016    //网络id  
#define MY_ADDR                   0x0002   //本机模块地址
#define SEND_ADDR                 0x0001   //发送地址
/*结束完善代码——*/

/* 自定义消息格式 */
#define START_HEAD    0xCC//帧头
#define CMD_READ      0x01//读传感器数据
#define SENSOR_TEMP   0x01//温度
#define SENSOR_RH     0x02//湿度
#define SENSOR_LIGHT  0x06//光照传感器  
/*  LED n 闪烁 time 毫秒 宏 */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)
/*数组大小*/
#define MAX_SEND_BUF_LEN  128 //无线数据最大发送长度
#define MAX_RECV_BUF_LEN  128 //无线数据最大接收长度
/*变量*/
static basicRfCfg_t basicRfConfig; 
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
uint8   APP_SEND_DATA_FLAG;  
uint8   LED9_flag=0;                   //LED9的状态标志，0灭1亮
uint16  LightAdc;
/*****************************************************************************************
*函数：uint8 CheckSum(uint8 *buf, uint8 len) 
*功能：计算校验和 
*输入：uint8 *buf-指向输入缓存区, uint8 len输入数据字节个数 
*输出：无 
*返回：返回校验和 
*特殊说明：无  
*****************************************************************************************/ 
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


void ConfigRf_Init(void)  
{  
  basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置  
  basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置  
  basicRfConfig.myAddr      =   MY_ADDR;       //设置本机地址  
  basicRfConfig.ackRequest  =   TRUE;          //应答信号  
  while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功  
  basicRfReceiveOn();                // 打开RF  
}  

void main(void)    
{
    halBoardInit();  //模块相关资源的初始化
    ConfigRf_Init(); //无线收发参数的配置初始化 
    Timer4_Init(); //定时器初始化
    Timer4_On();  //打开定时器    
  
    while(1)
    {   APP_SEND_DATA_FLAG = GetSendDataFlag();           
        if(APP_SEND_DATA_FLAG == 1)   //定时时间到
        {   /*【传感器采集、处理】 开始*/
            
            LightAdc = get_adc();    //取光照数据
            memset(pTxData, '\0', MAX_SEND_BUF_LEN);
           pTxData[0]=START_HEAD;//帧头
            pTxData[1]=CMD_READ;//命令
            pTxData[2]=7;//长度
            pTxData[3]=1;//1组传感数据
            pTxData[4]=SENSOR_LIGHT;//传感类型
            
            
    /* 开始完善代码，采集传感器数据，并且把传感器的数据通过BasicRf无线射频发送给ZigBee（黑板），发送时板上的LED1闪烁1次*/
            

            //uart_printf("光照传感器，光照值：%d\r\n",LightAdc); //串口发送
            
             pTxData[4]=SENSOR_LIGHT;
             pTxData[5]=(uint8)((LightAdc*10)>>8);
             pTxData[6]=(uint8)(LightAdc*10);
             pTxData[7]=CheckSum((uint8 *)pTxData,pTxData[2]);
             
             //用zigbee发送
             basicRfSendPacket((unsigned short)SEND_ADDR,(unsigned char *)pTxData,pTxData[2]+1);
             
             FlashLed(2,100);
             Timer4_On();
            
            
            
            
            
            
            
    /* 结束完善代码*/
            Timer4_On();  //打开定时
         }          
    }
}
