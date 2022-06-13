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

/*  LED n ��˸ time ���� �� */
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

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
 cfg.channel     =   20;    //zigbee��Ƶ������
    cfg.panId       =   0x0321;        //zigbee��ID������
    cfg.myAddr      =   0x0002;       //���ñ�����ַ
    cfg.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&cfg) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                           // ��RF

    /*�������ƴ���*/
}


void main(void)
{
    halBoardInit();
    configRF();
    
    while(1)
    {
       
        if(basicRfPacketIsReady())
        {
          
    /*��ʼ���ƴ��롪�����շ��Ͷ�(ZIGBEEͨ��ģ����������������ʵ��LED2����˸�����ݽ������ݿ���ָʾ��*/
          
            rxLen = basicRfReceive(rxData,8,NULL); 
            if(rxLen==8)
            {

                 sensorValue=rxData[1];
                 sensorValue<<=8;
                 sensorValue|=rxData[2];
                
                 if(sensorValue<100)
                    P1&=~0x40;
                 else
                    P1|=0x40;
                  FlashLed(2,100);
            }
          
          
          
          
          
          
    /*�������ƴ���*/
        }
    }
  
}


