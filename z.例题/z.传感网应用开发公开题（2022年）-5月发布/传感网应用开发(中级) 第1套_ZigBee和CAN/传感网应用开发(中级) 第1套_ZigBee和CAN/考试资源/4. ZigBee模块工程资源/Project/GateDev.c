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
uint8 rxData[16];   //�����������ݻ�����
uint16 rxLen = 0;

uint8 uTxData[16];  //����UART���ݻ�����
uint16 uTxlen = 0;

void ByteCopy(uint8 *dst,uint8 *src,uint8 len)
{
    while(len--)
    {
        *dst++=*src++;
    }
}

void configRF(void)
{
  /*��ʼ���ƴ��롪��basicRf���úͳ�ʼ��*/
  
  
  
  
  
  
  
  
  
  /*�������ƴ��롪��basicRf���úͳ�ʼ��*/
}


void main(void)
{
  uint16 len = 0;   //���崮�ڽ��յ����ݵĳ���
  halBoardInit();
  configRF();
    
  while(1)
  {    
    /*��ʼ���ƴ���
      ���շ��Ͷ�(ZIGBEEͨ��ģ����������������ʵ��LED1����˸�����ݽ������ݿ��ƾ����ƣ������ѹ����2V���رձ�����������2V���򿪱�������
      ÿ���յ����ݺ󣬵���LED2 0.1��
    */
    
    if(basicRfPacketIsReady())
    {






    }
    /*�������ƴ���*/
       
  }
  
}

