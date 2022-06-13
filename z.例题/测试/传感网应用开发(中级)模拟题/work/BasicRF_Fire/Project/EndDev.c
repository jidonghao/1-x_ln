#include "ioCC2530.h"
#include "hal_defs.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h"
#include "UART_PRINT.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include "TIMER.h"
 
/*  LED n ��˸ time ���� �� */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

/******ͨѶЭ�����*******/  
#define START_HEAD    0xCC//֡ͷ  
#define CMD_READ      0x01//������������  
#define SENSOR_TEMP   0x01//�¶�  
#define SENSOR_RH     0x02//ʪ��  
#define SENSOR_FIRE   0x03//���� 
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128

#define BOARD_TYPE  0x06
#define BOARD_ADDR  0x05

static basicRfCfg_t cfg;
uint8 txData[16];

void configRF(void)
{
    /*��ʼ���ƴ��롪��basicRf���úͳ�ʼ��*/

     
    cfg.channel     =   20;    //zigbee��Ƶ������
    cfg.panId       =   0x0321;        //zigbee��ID������
    cfg.myAddr      =   0x0001;       //���ñ�����ַ
    cfg.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&cfg) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                           // ��RF

    /*�������ƴ��롪��basicRf���úͳ�ʼ��*/
}

void adc_Init(void)
{
    APCFG  |=0x01;
}

/************************************************************
* ����       get_adc
* ����       ��ȡADCͨ��0��ѹֵ
* ��ڲ���   ��
* ���ڲ���   16λ��ѹֵ���ֱ���Ϊ10mV
***************��ȡADCͨ��0��ѹֵ************************/

uint16 get_adc(void)
{
    uint32 value;
    ADCIF = 0;   //��ADC �жϱ�־
    //���û�׼��ѹavdd5:3.3V��ͨ��0������ADת��
    ADCCON3 = (0x80 | 0x10 | 0x00);
    while ( !ADCIF )
    {
            ;  //�ȴ�ADת������
    }
        /*�������ƣ���ɶ�ADCH��ADCL�����Ĵ�������ȷ��ֵ�����ߵͰ�λ����U16�͵�value��*/
    /*����value���е�ѹת������*/
   
    value = ADCH;
    value = value<< 8;
    value |= ADCL;

    
    /*�������ƴ��롪���ɼ�ADC����*/

    value = value/4096*3.3;
   // value = value >> 15;   // ����32768
    // ���طֱ���Ϊ0.01V�ĵ�ѹֵ

    /*value��ֵ�������*/
    return (uint16)value;
}


void main(void)
{
  uint16 fireValue;
    halBoardInit();
    configRF(); 
    Timer4_Init();
    Timer4_On();
    adc_Init();
    
    while(1)
    {
        if(GetSendDataFlag())
        {
      /*��ʼ���ƴ��롪�� �ɼ�ADC���ݣ������ݷ��͸�ZigBee�װ�,���ݸ�ʽ�������ɶ���*/
      /*ÿ�η������ݵ���LED4 0.1��*/
    
            Timer4_On();

            fireValue =  get_adc();
            txData[0] = 0x06;
            txData[1] = fireValue>>8;
            txData[2] = fireValue&0xff;
            txData[3] = 0;
            txData[4] = SENSOR_FIRE;
            txData[5] = 0;
            txData[6] = '\r';
            txData[7] = '\n';
            
            basicRfSendPacket(0x0002, txData,8);
            FlashLed(4,100);
    
    
          
          
    
    
    
    
    
      /*�������ƴ���*/
        }           
    }
}