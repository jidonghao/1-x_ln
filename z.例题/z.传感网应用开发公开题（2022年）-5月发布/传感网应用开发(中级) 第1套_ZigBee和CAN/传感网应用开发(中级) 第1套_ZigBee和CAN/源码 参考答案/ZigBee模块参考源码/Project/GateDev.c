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
uint8 rxData[16]; 
uint16 rxLen = 0;

uint8 uTxData[16]; 
uint16 uTxlen = 0;

uint16 sensorValue = 0;

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
    cfg.myAddr = 0x8016;
    cfg.panId = 0x0016;
    cfg.channel = 16;
    cfg.ackRequest = TRUE;
    
    while(basicRfInit(&cfg)==FAILED);
    basicRfReceiveOn();  
    
    /*�������ƴ��롪��basicRf���úͳ�ʼ��*/
}


void main(void)
{
    halBoardInit();
    configRF();
    
    while(1)
    {
        /*��ʼ���ƴ��롪�����շ��Ͷ�(ZIGBEEͨ��ģ����������������ʵ��LED1����˸�����ݽ������ݿ��ƾ�����*/
        if(basicRfPacketIsReady())
        {
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen==8)
            {
                halLedSet(2);
                
                 sensorValue=rxData[1];
                 sensorValue<<=8;
                 sensorValue|=rxData[2];
                
                 if(sensorValue<200)
                    P1&=~0x40;
                 else
                    P1|=0x40;
                 
                halMcuWaitMs(100);  
                halLedClear(2);  
            }
        }
    }
  
}


