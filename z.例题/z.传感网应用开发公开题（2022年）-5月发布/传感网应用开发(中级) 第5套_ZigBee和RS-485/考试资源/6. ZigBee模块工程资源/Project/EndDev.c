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

static basicRfCfg_t cfg;
uint8 rxData[128];

void configRF(void)
{
  /*��ʼ���ƴ��롪��basicRf���úͳ�ʼ��*/
  
  
  
  
  
  
  
  
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
    value = ADCH;
    value = value<< 8;
    value |= ADCL;
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
      
      /*��ʼ���ƴ��롪�� ���������ݷ��͸�ZigBee�װ�,���ݸ�ʽ�������ɶ���*/
      /*ÿ�η������ݵ���LED2 0.1��*/
        if(GetSendDataFlag())
        {
    
    
    
    
    
    
        }
    
      /*�������ƴ���*/
  }
}