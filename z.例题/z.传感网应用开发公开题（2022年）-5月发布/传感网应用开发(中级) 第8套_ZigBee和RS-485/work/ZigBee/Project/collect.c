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
#define RF_CHANNEL                16         // 频道 11~26
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


#define LED0 3
#define LED1 4
#define LED2 1

void setLedAll(uint8 a,uint8 b,uint8 c)
{
  if(a)
  {
    halLedSet(LED0);
  }
  else
  {
    halLedClear(LED0);
  }
  if (b)
  {
    halLedSet(LED1);
  }
  else
  {
    halLedClear(LED1);
  }
  if (c)
  {
    halLedSet(LED2);
  }
  else
  {
    halLedClear(LED2);
  }
}

void setLedWhile(uint32 ledFlag)
{
  switch(ledFlag%8)
  {
  case 0:
    setLedAll(0, 0, 0);
    break;
  case 1:
    setLedAll(1, 0, 0); 
    break;
  case 2:
    setLedAll(0, 1, 0); 
    break;
  case 3:
    setLedAll(1, 1, 0); 
    break;
  case 4:
    setLedAll(0, 0, 1); 
    break;
  case 5:
    setLedAll(1, 0, 1); 
    break;
  case 6:
    setLedAll(0, 1, 1); 
    break;
  case 7:
    setLedAll(1, 1, 1); 
    break;
  default:
    break;
  }
}




// 无线RF初始化
void ConfigRf_Init(void)
{
  /*开始完善代码——basicRf配置和初始化*/
  
   basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置
    basicRfConfig.myAddr      =  MY_ADDR;   //设置本机地址
    basicRfConfig.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                // 打开RF

  /*结束完善代码——basicRf配置和初始化*/
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
   	
       if(basicRfPacketIsReady())   //查询有没收到无线信号  
      {  
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen>=8)
            {
             /* 开始完善代码——接收发送端(ZIGBEE白板）无线数据输出到串口显示 */
              char DebugOutput[256];
        memset(DebugOutput,'\0',256);
        GetHexStr((uint8 *)rxData,rxLen,(uint8 *)DebugOutput);
        uart_printf("接收到的原始数据为：%s\r\n",DebugOutput);
        
        uart_printf("当前温度是：%d ℃\r\n当前湿度是：%d %%\r\n",rxData[5],rxData[7]);
             /* 结束完善代码——接收发送端(ZIGBEE白板）无线数据输出到串口显示 */


             /* 开始完善代码——收到数据后，计数值增加1，并更新计数D3 D6 D5指示灯的显示 */
             setLedWhile(rx_cnt++);
             /* 结束完善代码——收到数据后，计数值增加1，并更新计数D3 D6 D5指示灯的显示 */

            }
       }  
   }  
}