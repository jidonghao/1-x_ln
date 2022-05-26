//这里是5.5.3的 编写的头文件的地方

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

/*点对点通信地址设置*/
#define RF_CHANNEL	16	//频道11~26
#define PAN_ID  0xD0C2  //网络id
#define MY_ADDR 0xC2BD  //本机模块地址
#define SEND_ADDR 0xB4F3  //发送地址
//自定义消息格式
#define START_HEAD  0xCC  //帧头
#define CMD_READ  0x01  //读传感器数据
#define SENSOR_TEMP 0x01  //温度
#define SENSOR_RH 0x02  //湿度
#define SENSOR_FIRE 0x03  //火焰

/*  LED n 闪烁 time 毫秒 宏 */
#define FlashLed(n,time)do{halLedSet(n);halMcuWaitMs(time);halLedClear(n);}while(0)

/*  数组大小  */
#define MAX_SEND_BUF_LEN  128 //无线数据最大发送长度
#define MAX_RECV_BUF_LEN  128 //无线数据最大接受长度

/*  定义变量和数组  */
/*  定义basicRfCfg_t变量、无线接收和发送换粗数组、定时器超时标志的变量*/
static basicRfCfg_t basicRfConfig;
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
uint8 APP_SEND_DATA_FLAG;

/*  新增计算校验和函数CheckSum()  */
/*************************************************************
* 函数：uint8 CheckSum(uint8 *buf, uint8 len)
* 功能：计算校验和
* 输入：uint8 *buf-指向输入缓冲区，uint8 len输入数据字节个数
* 返回：校验和
*************************************************************/

uint8 CheckSum(uint8 *buf, uint8 len){
  uint8 temp = 0;
  while(len--){
    temp+=*buf;
    buf++;
  }
  return (uint8)temp;
}

/*  定义初始化函数BasicRF */
void ConfigRf_Init(void){
  basicRfConfig.panId = PAN_ID; 
  basicRfConfig.channel = RF_CHANNEL;
  basicRfConfig.myAddr  = MY_ADDR;
  basicRfConfig.ackRequest  = TRUE;
  while(basicRfInit(&basicRfConfig)==FAILED);
  basicRfReceiveOn();
}

void main(void){
  halBoardInit();
  ConfigRf_Init();
  Timer4_Init();
  Timer4_On();

  //SHT_Init();
  
  while(1){
    APP_SEND_DATA_FLAG  = GetSendDataFlag();
    if(APP_SEND_DATA_FLAG == 1){
      uint16 sensor_tem,sensor_val;
      call_sht11((unsigned int *)(&sensor_tem),(unsigned int *)(&sensor_val));
      //SHT_SmpSnValue((int8*)(&sensor_tem),(uint8*)(&sensor_val));
      #ifdef CC2530_DEBUG
      uart_printf("温湿度传感器，温度：%d℃，湿度：%d%% \r\n",sensor_tem,sensor_val);
      #endif
      
      memset(pTxData,'\0',MAX_SEND_BUF_LEN);
      
      pTxData[0]  = START_HEAD;
      pTxData[1]  = CMD_READ;
      pTxData[2]  = 8;
      pTxData[3]  = 2;
      pTxData[4] = SENSOR_TEMP;
      pTxData[5]  = sensor_tem;
      pTxData[6] = SENSOR_RH;
      pTxData[7]= sensor_val;
      pTxData[8]  = CheckSum((uint8 *)pTxData,pTxData[2]);
      
      basicRfSendPacket((unsigned short)SEND_ADDR,(unsigned char *)pTxData,pTxData[2]+1);
      FlashLed(1,100);
      Timer4_On();
      
          
    
    }
  
  }
}