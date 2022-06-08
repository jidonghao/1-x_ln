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

/******��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                20         // Ƶ�� 11~26
#define PAN_ID                    0x0321     //����id
#define MY_ADDR                   0xB4F3     //����ģ���ַ
/********�������ݻ���********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
/**************************************************/
static basicRfCfg_t basicRfConfig;
uint32 rx_cnt = 0;
//ʹLED n ��˸ time ����
#define FlashLed(n,time) do{\
      halLedSet(n);\
      halMcuWaitMs(time);\
      halLedClear(n);\
      }while(0)

// ����RF��ʼ��
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������
    basicRfConfig.myAddr      =  MY_ADDR;   //���ñ�����ַ
    basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                // ��RF
}

/****************************************************************************************** 
*������void show_cnt(uint8 num) 
*���ܣ�����D3=LED1 D6=LED4 D5=LED3  3��LED�ƽ��м�����ʾ 
*���أ��� 
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
*������uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output) 
*���ܣ����������16������ʽ���ַ�����ʽ,��Ա���Կո���� 
*     ����:��{0xA1,0xB2,0xC3}����"A1 B2 C3" 
*���룺uint8 *input-ָ�����뻺����, uint8 len-�������ݵ��ֽڳ��� 
*�����uint8 *output-ָ����������� 
*���أ����������ַ����ĳ��� 
*����˵������ 
******************************************************************************************/
uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)  
{  
   char str[128];  
   memset(str, '\0', 128);  
   for(uint8 i=0; i<len; i++)  
   {  
      sprintf(str+i*3,"%02X ", *input);  //ע�⣬�ַ������Կո������"%02X "˫��������һ���ո�
      input++;  
   }  
   strcpy((char *)output, (const char *)str);  
   return strlen((const char *)str);  
}  

void main(void)  
{  
   uint8 rxData[16]; 
   uint16 rxLen = 0;
   halBoardInit();  //ģ�������Դ�ĳ�ʼ��  
   ConfigRf_Init(); //�����շ����������ó�ʼ��   
   while(1)  
   {  
         /***���յ�temprh_sensorģ�鷢�͵����ݺ󣬼���ֵ����1�������¼���ָʾ�Ƶ���ʾ************/  
       if(basicRfPacketIsReady())   //��ѯ��û�յ������ź�  
      {  
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen>=8)
            {
              
              char DebugOutput[32];  
              memset(DebugOutput, '\0', 32);  
              GetHexStr((uint8 *)rxData, rxLen, (uint8 *)DebugOutput);   
              uart_printf("���յ�ԭʼ����RF���ݣ�%s\r\n",DebugOutput);  
              //uart_printf("%s\r\n",rxData);
              
              //FlashLed(3,500);//���߽���ָʾ��LED2��100ms  
              //������������  
              show_cnt(rx_cnt++);
              //halMcuWaitMs(1000);
            }
       }  
   }  
}