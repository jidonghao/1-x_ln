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

/******点对点通讯地址设置******/
#define RF_CHANNEL                20         // 频道 11~26
#define PAN_ID                    0x0321     //网络id
#define MY_ADDR                   0xB4F3     //本机模块地址
/********无线数据缓存********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
/**************************************************/
static basicRfCfg_t basicRfConfig;
uint32 rx_cnt = 0;
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
    basicRfConfig.myAddr      =  MY_ADDR;   //设置本机地址
    basicRfConfig.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                // 打开RF
}

/****************************************************************************************** 
*函数：void show_cnt(uint8 num) 
*功能：根据D3=LED1 D6=LED4 D5=LED3  3个LED灯进行计数显示 
*返回：无 
******************************************************************************************/
void show_cnt(uint8 num)
{
  num=num%8;
  switch(num)
  {
  case 0:    
    halLedClear(1);
    halLedClear(4);
    halLedClear(3);
    break;
  case 1:
    halLedClear(1);
    halLedClear(4);
    halLedSet(3);
    break;
  case 2:
    halLedClear(1);
    halLedSet(4);
    halLedClear(3);
    break;
  case 3:
    halLedClear(1);
    halLedSet(4);
    halLedSet(3);
    break; 
  case 4:
    halLedSet(1);
    halLedClear(4);
    halLedClear(3);
    break;
  case 5:
    halLedSet(1);
    halLedClear(4);
    halLedSet(3);
    break;
  case 6:
    halLedSet(1);
    halLedSet(4);
    halLedClear(3);
    break;
  case 7:
    halLedSet(1);
    halLedSet(4);
    halLedSet(3);
    break;
  }
}

/****************************************************************************************** 
*函数：uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output) 
*功能：生成数组的16进制形式的字符串格式,成员间以空格隔开 
*     例如:由{0xA1,0xB2,0xC3}生成"A1 B2 C3" 
*输入：uint8 *input-指向输入缓存区, uint8 len-输入数据的字节长度 
*输出：uint8 *output-指向输出缓存区 
*返回：返回生成字符串的长度 
*特殊说明：无 
******************************************************************************************/
uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)  
{  
   char str[128];  
   memset(str, '\0', 128);  
   for(uint8 i=0; i<len; i++)  
   {  
      sprintf(str+i*3,"%02X ", *input);  //注意，字符串间以空格隔开，"%02X "双引号中有一个空格
      input++;  
   }  
   strcpy((char *)output, (const char *)str);  
   return strlen((const char *)str);  
}  

void main(void)  
{  
   uint8 rxData[16]; 
   uint16 rxLen = 0;
   halBoardInit();  //模块相关资源的初始化  
   ConfigRf_Init(); //无线收发参数的配置初始化   
   while(1)  
   {  
         /***当收到temprh_sensor模块发送的数据后，计数值增加1，并更新计数指示灯的显示************/  
       if(basicRfPacketIsReady())   //查询有没收到无线信号  
      {  
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen>=8)
            {
              
              char DebugOutput[32];  
              memset(DebugOutput, '\0', 32);  
              GetHexStr((uint8 *)rxData, rxLen, (uint8 *)DebugOutput);   
              uart_printf("接收到原始无线RF数据：%s\r\n",DebugOutput);  
              //uart_printf("%s\r\n",rxData);
              
              //FlashLed(3,500);//无线接收指示，LED2亮100ms  
              //接收无线数据  
              show_cnt(rx_cnt++);
              //halMcuWaitMs(1000);
            }
       }  
   }  
}