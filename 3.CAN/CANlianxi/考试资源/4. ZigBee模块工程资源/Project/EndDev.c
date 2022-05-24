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
    /*��ʼ���ƴ��롪��ADC��ʼ��*/
    
    
    /*�������ƴ��롪��ADC��ʼ��*/
}

uint16 get_adc(void)
{
    uint32 value;
    
    /*��ʼ���ƴ��롪���ɼ�ADC����*/
    
    
    
    
    
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
    
    
    
    
    
    
    
    
    
      /*�������ƴ��롪���ɼ�ADC���ݣ������ݷ��͸�ZigBee�װ�,���ݸ�ʽ�������ɶ���*/
  }
}