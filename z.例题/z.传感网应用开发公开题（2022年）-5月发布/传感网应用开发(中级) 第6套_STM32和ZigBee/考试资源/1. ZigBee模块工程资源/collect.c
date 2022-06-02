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
#define RF_CHANNEL                16        // Ƶ�� 11~26
#define PAN_ID                    0x0000    //����id
#define MY_ADDR                   0x0000    //����ģ���ַ

/******ZigBeeͨѶЭ�����*******/
#define START_HEAD    0xCC//֡ͷ  
#define CMD_READ      0x01//������������  
#define SENSOR_TEMP   0x01//�¶�  
#define SENSOR_RH     0x02//ʪ��  
#define SENSOR_LIGHT  0x06//���մ�����  

/********�������ݻ���********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
 
/**************************************************/
static basicRfCfg_t basicRfConfig;


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
    basicRfConfig.myAddr      =   MY_ADDR;       //���ñ�����ַ
    basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                           // ��RF
}

/************************************************************************
*������uint8 CheckSum(uint8 *buf, uint8 len) 
*���ܣ�����У��� 
*���룺uint8 *buf-ָ�����뻺����, uint8 len���������ֽڸ��� 
*������� 
*���أ�����У��� 
*����˵������ 
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
*������void TransComData(uint8  *buf, uint8 len)
*���ܣ����յ�������ת��Ϊ����������ͨ��Э��
*************************************************************************/  
void TransComData(uint8  *buf, uint8 len)
{
  uint8 check = 0;
        check = CheckSum((uint8 *)pRxData, pRxData[2]);
        if((pRxData[0] == START_HEAD) && (check == pRxData[pRxData[2]]))//֡ͷ��ȷ��У��ͨ��
        {
       /*��ʼ���ƴ��롪����������������ͨ��Э�齨������֡�������pTxData������*/  
          
          
          
          
          
          
          
          
          
          
          
       /*�������ƴ��롪����������������ͨ��Э�齨������֡�������pTxData������*/  
         }           
}

/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;  
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    
    while(1)
    {
      /*��ʼ���ƴ��롪������ZIGBEE��ģ���������ݣ�ʵ��D5����˸������void TransComData(uint8  *buf, uint8 len)
      ��������������ͨ��Э�齨������֡������halUartWrite������ͨ�����ڷ���*/     
       
      
      
      
      
      
      
      
      
      
       /*�������ƴ��롪*/     
    }
}
/************************MAIN END ****************************/