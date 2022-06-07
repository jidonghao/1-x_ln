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

#define FlashLed(n,time)do{\
                        halLedSet(n);\
                        halMcuWaitMs(time);\
                        halLedClear(n);\
                        }while(0)

static basicRfCfg_t cfg;
uint8 rxData[128];
uint8 txData[128];
void configRF(void)
{
  /*��ʼ���ƴ��롪��basicRf���úͳ�ʼ��*/
   cfg.panId       =   0x0016;        //zigbee��ID������
    cfg.channel     =   16;    //zigbee��Ƶ������
    cfg.myAddr      =   0x0001;       //���ñ�����ַ
    cfg.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&cfg) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                           // ��RF
  
  
  
  
  
  
  /*�������ƴ��롪��basicRf���úͳ�ʼ��*/
}

void adc_Init(void)
{
    /*��ʼ���ƴ��롪��ADC��ʼ��*/
      APCFG  |=0x01;
    /*�������ƴ��롪��ADC��ʼ��*/
}

uint16 get_adc(void)
{
    uint32 value;
    
    /*��ʼ���ƴ��롪���ɼ�ADC����*/
    
    ADCIF = 0;   //��ADC �жϱ�־
    //���û�׼��ѹavdd5:3.3V��ͨ��0������ADת��
    ADCCON3 = (0x80 | 0x10 | 0x00);
    while ( !ADCIF )
    {
            ;  //�ȴ�ADת������
    }
    value = ADCH;
    value = value<< 8;
    value |= ADCL;

    
    /*�������ƴ��롪���ɼ�ADC����*/
    
    // ADֵת���ɵ�ѹֵ
    // 0 ��ʾ 0V ��32768 ��ʾ 3.3V
    // ��ѹֵ = (value*3.3)/32768 ��V)
    value = (value * 330);
    value = value >> 15;   // ����32768
    // ���طֱ���Ϊ0.01V�ĵ�ѹֵ
    return (uint16)value;
}

void main(void)
{
  uint16 lightValue;
  halBoardInit();
  configRF(); 
  
  Timer4_Init();
  Timer4_On();
  adc_Init();
 
  while(1)
  {
      /*��ʼ���ƴ��롪�� �ɼ�ADC���ݣ������ݷ��͸�ZigBee�װ�,���ݸ�ʽ�������ɶ���*/
      /*ÿ3�뷢��һ�����ݣ�ÿ�η������ݵ���LED2 0.1��*/
  if(GetSendDataFlag())
        {
            Timer4_On();

            lightValue =  get_adc();
            txData[0] = 0x06;
            txData[1] = lightValue>>8;
            txData[2] = lightValue&0xff;
            txData[3] = 0;
            txData[4] = 0x05;
            txData[5] = 0;
            txData[6] = '\r';
            txData[7] = '\n';
            
            basicRfSendPacket(0x0002, txData,8);
            FlashLed(2,100);
        }           
    
      /*�������ƴ��롪���ɼ�ADC���ݣ������ݷ��͸�ZigBee�װ�,���ݸ�ʽ�������ɶ���*/
  }
}