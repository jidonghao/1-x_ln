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
#define RF_CHANNEL                16         // Ƶ�� 11~26
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




// ����RF��ʼ��
void ConfigRf_Init(void)
{
  /*��ʼ���ƴ��롪��basicRf���úͳ�ʼ��*/
  
   basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������
    basicRfConfig.myAddr      =  MY_ADDR;   //���ñ�����ַ
    basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                // ��RF

  /*�������ƴ��롪��basicRf���úͳ�ʼ��*/
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
   	
       if(basicRfPacketIsReady())   //��ѯ��û�յ������ź�  
      {  
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen>=8)
            {
             /* ��ʼ���ƴ��롪�����շ��Ͷ�(ZIGBEE�װ壩�������������������ʾ */
              char DebugOutput[256];
        memset(DebugOutput,'\0',256);
        GetHexStr((uint8 *)rxData,rxLen,(uint8 *)DebugOutput);
        uart_printf("���յ���ԭʼ����Ϊ��%s\r\n",DebugOutput);
        
        uart_printf("��ǰ�¶��ǣ�%d ��\r\n��ǰʪ���ǣ�%d %%\r\n",rxData[5],rxData[7]);
             /* �������ƴ��롪�����շ��Ͷ�(ZIGBEE�װ壩�������������������ʾ */


             /* ��ʼ���ƴ��롪���յ����ݺ󣬼���ֵ����1�������¼���D3 D6 D5ָʾ�Ƶ���ʾ */
             setLedWhile(rx_cnt++);
             /* �������ƴ��롪���յ����ݺ󣬼���ֵ����1�������¼���D3 D6 D5ָʾ�Ƶ���ʾ */

            }
       }  
   }  
}