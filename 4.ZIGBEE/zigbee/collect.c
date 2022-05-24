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

/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                26        // Ƶ�� 11~26
#define PAN_ID                    0xD0C2     //����id
#define MY_ADDR                   0xB4F3     //����ģ���ַ

/********�������ݻ���********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
//static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
 
/**************************************************/
static basicRfCfg_t basicRfConfig;

//ʹLED n ��˸ time ����
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

/**********************************************************************************************
*������uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)
*���ܣ����������16������ʽ���ַ�����ʽ,��Ա���Կո����
*     ����:��{0xA1,0xB2,0xC3}����"A1 B2 C3"
*���룺uint8 *input-ָ�����뻺����, uint8 len-�������ݵ��ֽڳ���
*�����uint8 *output-ָ�����������
*���أ����������ַ����ĳ���
*����˵������
**********************************************************************************************/
uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)
{
  char str[128];
  memset(str, '\0', 128);
  for(uint8 i=0; i<len; i++)
  {
    sprintf(str+i*3,"%02X ", *input);
    input++;
  }
  strcpy((char *)output, (const char *)str);
  return strlen((const char *)str);
}

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

/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;
    uint32 TimCnt = 0;
    int8 brightness = 0;
    uint8 flag = 0;//0-���½��䰵��1-���Ͻ�����
    
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    
    while(1)
    {
        /************************�����ƽ���***********************/
        if(TimCnt++>1024)
        {
          TimCnt = 0;
          if(flag)
          {
            brightness ++;
            if(brightness >= 90)
            {flag = 0;}//���½��䰵
          }
          else
          {
            brightness --;
            if(brightness <= 10)
            {flag = 1;}//���Ͻ�����
          }
          TIM1_PwmInit(1,brightness);
        }
      
        /*************************�������ݽ��մ������******************/
        if(basicRfPacketIsReady())   //��ѯ��û�յ������ź�
        {
            FlashLed(2,100);//���߽���ָʾ��LED2��100ms
            //������������
            len = basicRfReceive(pRxData, MAX_RECV_BUF_LEN, NULL);
#ifdef CC2530_DEBUG
            char DebugOutput[256];
            memset(DebugOutput, '\0', 256);
            GetHexStr((uint8 *)pRxData, len, (uint8 *)DebugOutput); 
            uart_printf("���յ�ԭʼ����RF���ݣ�%s\r\n",DebugOutput);
#endif /*CC2530_DEBUG*/
        }
    }
}
/************************MAIN END ****************************/