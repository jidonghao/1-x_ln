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


 /*��ʼ���ƴ��롪����Ե�ͨѶ��ַ����*/
#define RF_CHANNEL                23        // Ƶ�� 11~26  
#define PAN_ID                    0x0016    //����id  
#define MY_ADDR                   0x0002   //����ģ���ַ
#define SEND_ADDR                 0x0001   //���͵�ַ
/*�������ƴ��롪��*/

/* �Զ�����Ϣ��ʽ */
#define START_HEAD    0xCC//֡ͷ
#define CMD_READ      0x01//������������
#define SENSOR_TEMP   0x01//�¶�
#define SENSOR_RH     0x02//ʪ��
#define SENSOR_LIGHT  0x06//���մ�����  
/*  LED n ��˸ time ���� �� */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)
/*�����С*/
#define MAX_SEND_BUF_LEN  128 //������������ͳ���
#define MAX_RECV_BUF_LEN  128 //�������������ճ���
/*����*/
static basicRfCfg_t basicRfConfig; 
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
uint8   APP_SEND_DATA_FLAG;  
uint8   LED9_flag=0;                   //LED9��״̬��־��0��1��
uint16  LightAdc;
/*****************************************************************************************
*������uint8 CheckSum(uint8 *buf, uint8 len) 
*���ܣ�����У��� 
*���룺uint8 *buf-ָ�����뻺����, uint8 len���������ֽڸ��� 
*������� 
*���أ�����У��� 
*����˵������  
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
  basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������  
  basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������  
  basicRfConfig.myAddr      =   MY_ADDR;       //���ñ�����ַ  
  basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�  
  while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�  
  basicRfReceiveOn();                // ��RF  
}  

void main(void)    
{
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    Timer4_Init(); //��ʱ����ʼ��
    Timer4_On();  //�򿪶�ʱ��    
  
    while(1)
    {   APP_SEND_DATA_FLAG = GetSendDataFlag();           
        if(APP_SEND_DATA_FLAG == 1)   //��ʱʱ�䵽
        {   /*���������ɼ������� ��ʼ*/
            
            LightAdc = get_adc();    //ȡ��������
            memset(pTxData, '\0', MAX_SEND_BUF_LEN);
           pTxData[0]=START_HEAD;//֡ͷ
            pTxData[1]=CMD_READ;//����
            pTxData[2]=7;//����
            pTxData[3]=1;//1�鴫������
            pTxData[4]=SENSOR_LIGHT;//��������
            
            
    /* ��ʼ���ƴ��룬�ɼ����������ݣ����ҰѴ�����������ͨ��BasicRf������Ƶ���͸�ZigBee���ڰ壩������ʱ���ϵ�LED1��˸1��*/
            

            //uart_printf("���մ�����������ֵ��%d\r\n",LightAdc); //���ڷ���
            
             pTxData[4]=SENSOR_LIGHT;
             pTxData[5]=(uint8)((LightAdc*10)>>8);
             pTxData[6]=(uint8)(LightAdc*10);
             pTxData[7]=CheckSum((uint8 *)pTxData,pTxData[2]);
             
             //��zigbee����
             basicRfSendPacket((unsigned short)SEND_ADDR,(unsigned char *)pTxData,pTxData[2]+1);
             
             FlashLed(2,100);
             Timer4_On();
            
            
            
            
            
            
            
    /* �������ƴ���*/
            Timer4_On();  //�򿪶�ʱ
         }          
    }
}
