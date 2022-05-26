//������5.5.3�� ��д��ͷ�ļ��ĵط�

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

/*��Ե�ͨ�ŵ�ַ����*/
#define RF_CHANNEL	16	//Ƶ��11~26
#define PAN_ID  0xD0C2  //����id
#define MY_ADDR 0xC2BD  //����ģ���ַ
#define SEND_ADDR 0xB4F3  //���͵�ַ
//�Զ�����Ϣ��ʽ
#define START_HEAD  0xCC  //֡ͷ
#define CMD_READ  0x01  //������������
#define SENSOR_TEMP 0x01  //�¶�
#define SENSOR_RH 0x02  //ʪ��
#define SENSOR_FIRE 0x03  //����

/*  LED n ��˸ time ���� �� */
#define FlashLed(n,time)do{halLedSet(n);halMcuWaitMs(time);halLedClear(n);}while(0)

/*  �����С  */
#define MAX_SEND_BUF_LEN  128 //������������ͳ���
#define MAX_RECV_BUF_LEN  128 //�������������ܳ���

/*  �������������  */
/*  ����basicRfCfg_t���������߽��պͷ��ͻ������顢��ʱ����ʱ��־�ı���*/
static basicRfCfg_t basicRfConfig;
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
uint8 APP_SEND_DATA_FLAG;

/*  ��������У��ͺ���CheckSum()  */
/*************************************************************
* ������uint8 CheckSum(uint8 *buf, uint8 len)
* ���ܣ�����У���
* ���룺uint8 *buf-ָ�����뻺������uint8 len���������ֽڸ���
* ���أ�У���
*************************************************************/

uint8 CheckSum(uint8 *buf, uint8 len){
  uint8 temp = 0;
  while(len--){
    temp+=*buf;
    buf++;
  }
  return (uint8)temp;
}

/*  �����ʼ������BasicRF */
void ConfigRf_Init(void){
  basicRfConfig.panId = PAN_ID; 
  basicRfConfig.channel = RF_CHANNEL;
  basicRfConfig.myAddr  = MY_ADDR;
  basicRfConfig.ackRequest  = TRUE;
  while(basicRfInit(&basicRfConfig)==FAILED);
  basicRfReceiveOn();
}

void main(void){
  halBoardInit();
  ConfigRf_Init();
  Timer4_Init();
  Timer4_On();

  //SHT_Init();
  
  while(1){
    APP_SEND_DATA_FLAG  = GetSendDataFlag();
    if(APP_SEND_DATA_FLAG == 1){
      uint16 sensor_tem,sensor_val;
      call_sht11((unsigned int *)(&sensor_tem),(unsigned int *)(&sensor_val));
      //SHT_SmpSnValue((int8*)(&sensor_tem),(uint8*)(&sensor_val));
      #ifdef CC2530_DEBUG
      uart_printf("��ʪ�ȴ��������¶ȣ�%d�棬ʪ�ȣ�%d%% \r\n",sensor_tem,sensor_val);
      #endif
      
      memset(pTxData,'\0',MAX_SEND_BUF_LEN);
      
      pTxData[0]  = START_HEAD;
      pTxData[1]  = CMD_READ;
      pTxData[2]  = 8;
      pTxData[3]  = 2;
      pTxData[4] = SENSOR_TEMP;
      pTxData[5]  = sensor_tem;
      pTxData[6] = SENSOR_RH;
      pTxData[7]= sensor_val;
      pTxData[8]  = CheckSum((uint8 *)pTxData,pTxData[2]);
      
      basicRfSendPacket((unsigned short)SEND_ADDR,(unsigned char *)pTxData,pTxData[2]+1);
      FlashLed(1,100);
      Timer4_On();
      
          
    
    }
  
  }
}