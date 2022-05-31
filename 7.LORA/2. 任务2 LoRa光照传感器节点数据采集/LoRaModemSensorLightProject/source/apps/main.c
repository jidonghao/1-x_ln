/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "radio.h"
#include "NS_Radio.h"
#include "hal_temHum.h"

/*�궨��*/
#define START_HEAD 0x55//֡ͷ
#define CMD_READ   0x01//������
#define ACK_OK     0x00//��ӦOK
#define ACK_NONE   0x01//������
#define ACK_ERR    0x02//���ݴ���
//���������ź��豸��ַ
#define MY_NET_ID  0x016       //����ID
#define MY_ADDR    0x02         //�豸��ַ

/*ȫ�ֱ���*/
uint16_t LightLux = 200;    //���մ������ɼ����Ĺ��նȣ���λ��lux

//��������
void LoRa_Send( uint8_t *TxBuffer, uint8_t len );
void MyRadioRxDoneProcess( void );
void OLED_InitView(void);
void PlatformInit(void);

uint8_t CheckSum(uint8_t *buf, uint8_t len);
uint8_t *ExtractCmdframe(uint8_t *buf, uint8_t len, uint8_t CmdStart);
uint16_t GetHexStr(uint8_t *input, uint16_t len, uint8_t *output);
void LoRa_DataParse( uint8_t *LoRaRxBuf, uint16_t len );
void LoRa_GetSensorDataProcess(void);


/**********************************************************************************************
*������uint8 CheckSum(uint8 *buf, uint8 len)
*���ܣ�����У���
*���룺uint8 *buf-ָ�����뻺����, uint8 len���������ֽڸ���
*�������
*���أ�����У���
*����˵������
**********************************************************************************************/
uint8_t CheckSum(uint8_t *buf, uint8_t len)
{
    uint8_t temp = 0;
    while(len--)
    {
        temp += *buf;
        buf++;
    }
    return (uint8_t)temp;
}

/**********************************************************************************************
*������uint8_t *ExtractCmdframe(uint8_t *buf, uint8_t len, uint8_t CmdStart)
*���ܣ���һ����������ȡ������֡������ʼ��ַ
*���룺
*     uint8_t *buf��ָ�����ȡ�����ݻ�������ַ
*     uint8_t len�����������ݳ���
*     uint8_t CmdStart������֡��ʼ�ֽ�
*�������
*���أ������״γ�������֡�ĵ�ַ����������������֡���ݣ��򷵻�NULL
*����˵������
**********************************************************************************************/
uint8_t *ExtractCmdframe(uint8_t *buf, uint8_t len, uint8_t CmdStart)
{
    uint8_t *point = NULL;
    uint8_t i;
    for(i=0; i<len; i++)
    {
        if(CmdStart == *buf)
        {
            point = buf;
            return point;
        }
    }
    return NULL;
}

/**********************************************************************************************
*������uint16_t GetHexStr(uint8 *input, uint16_t len, uint8 *output)
*���ܣ����������16������ʽ���ַ�����ʽ,��Ա���Կո����
*     ����:��{0xA1,0xB2,0xC3}����"A1 B2 C3"
*���룺uint8 *input-ָ�����뻺����, uint16_t len-�������ݵ��ֽڳ���
*�����uint8 *output-ָ�����������
*���أ����������ַ����ĳ���
*����˵������
**********************************************************************************************/
uint16_t GetHexStr(uint8_t *input, uint16_t len, uint8_t *output)
{
    for(uint16_t i=0; i<len; i++)
    {
        sprintf((char *)(output+i*3),"%02X ", *input);
        input++;
    }
    return strlen((const char *)output);
}

/**********************************************************************************************
*������void LoRa_DataParse( uint8_t *LoRaRxBuf, uint16_t len )
*���ܣ����ݽ���
*���룺uint8_t *LoRaRxBuf-ָ����������, uint16_t len-���ݳ���
*�������
*���أ���
*����˵������
**********************************************************************************************/
void LoRa_DataParse( uint8_t *LoRaRxBuf, uint16_t len )
{
    uint8_t *DestData = NULL;
#define HEAD_DATA  *DestData     //֡ͷ
#define CMD_DATA   *(DestData+1) //����
#define NETH_DATA  *(DestData+2) //����ID���ֽ�  
#define NETL_DATA  *(DestData+3) //����ID���ֽ�  
#define ADDR_DATA  *(DestData+4) //��ַ  

#define ACK_DATA        *(DestData+5) //��Ӧ
#define LEN_DATA        *(DestData+6) //����
#define DATASTAR_DATA   *(DestData+7) //��������ʼ

    DestData = ExtractCmdframe((uint8_t *)LoRaRxBuf, len, START_HEAD);
    if(DestData != NULL)//����������֡ͷ
    {
        if((DestData - LoRaRxBuf) > (len - 6)) return;//���ݳ��Ȳ��㹹��һ֡��������
        if(CMD_DATA != CMD_READ) return;//�������
        if(CheckSum((uint8_t *)DestData, 5) != (*(DestData+5))) return;//У�鲻ͨ������������У������������У��
        if(((((uint16_t)NETH_DATA)<<8)+NETL_DATA) != MY_NET_ID) return;//����ID��һ��
        //���Ͷ���Ӧ
        if(ADDR_DATA != MY_ADDR) return;//��ַ��һ��

        uint8_t RspBuf[BUFFER_SIZE]= {0};
        memset(RspBuf, '\0', BUFFER_SIZE);

        RspBuf[0]=START_HEAD;
        RspBuf[1]=CMD_READ;
        RspBuf[2]=(uint8_t)(MY_NET_ID>>8);
        RspBuf[3]=(uint8_t)MY_NET_ID;
        RspBuf[4]=MY_ADDR;
        RspBuf[5]=ACK_OK;
        sprintf((char *)(RspBuf+7),"L(lux):%d ", LightLux);//������sprintf�У�������%����ʾ�����%��
        RspBuf[6]=strlen((const char *)(RspBuf+7))+1;//�����򳤶�
        RspBuf[6+RspBuf[6]]=CheckSum((uint8_t *)RspBuf, 6+RspBuf[6]);

        Radio.Send( RspBuf, 7+RspBuf[6]);//������Ӧ����
        GpioToggle( &Led1 );//���������л�����ָʾ
    }
}

/**********************************************************************************************
*������void LoRa_Send( uint8_t *TxBuffer, uint8_t len )
*���ܣ�����LoRa��������
*���룺
*       uint8_t *TxBuffer, ����������
*       uint8_t len, ���������ݳ���
*�������
*���أ���
*����˵������
**********************************************************************************************/
void LoRa_Send( uint8_t *TxBuffer, uint8_t len )
{
    Radio.Send( TxBuffer, len);
}

/**********************************************************************************************
*������void MyRadioRxDoneProcess( void )
*���ܣ�����ģ�����ݽ�����ɴ�����̺���
*���룺��
*�������
*���أ���
*����˵�������յ����������ݱ�����RxBuffer�У�BufferSizeΪ���յ����������ݳ���
**********************************************************************************************/
void MyRadioRxDoneProcess( void )
{
    uint16_t BufferSize = 0;
    uint8_t RxBuffer[BUFFER_SIZE];

    BufferSize = ReadRadioRxBuffer( (uint8_t *)RxBuffer );
    if(BufferSize>0)
    {
        //�û��ڴ˴���ӽ������ݴ����ܵĴ���
        GpioToggle( &Led2 );//�յ������л�����ָʾ
        LoRa_DataParse( (uint8_t *)RxBuffer, BufferSize );//���ݽ���
    }
}

/**********************************************************************************************
*������void LoRa_GetSensorDataProcess(void)
*���ܣ���ȡ���������ݽ���
*���룺��
*�������
*���أ���
*����˵������
**********************************************************************************************/
void LoRa_GetSensorDataProcess(void)
{
    const uint16_t time = 1000;
    if(User0Timer_MS > time)
    {
        User0Timer_MS = 0;
        uint16_t AdcNum,voltage;
        AdcNum = AdcReadChannel( &Adc, ADC_CHANNEL_0 );//ADC����12λ���ο���ѹ3.3V
        voltage = AdcNum*3300/(4096-1.0);//��������ѹֵ����λ��mV
        LightLux = (5/2.0)*(voltage/10.0);
        char StrBuf[64]={0};
        memset(StrBuf, '\0', 64);
        sprintf(StrBuf, " %d lux",LightLux);
        OLED_ShowString(0,4,(uint8_t *)StrBuf);
    }
}

/**********************************************************************************************
*������void OLED_InitView()
*���ܣ�LoraҺ����ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void OLED_InitView(void)
{
    OLED_Clear();
    OLED_ShowString(0,0, (uint8_t *)" Newland Edu");
    OLED_ShowString(0,2, (uint8_t *)" LoRa Light");
}

/**********************************************************************************************
*������void PlatformInit( void )
*���ܣ�ƽ̨��ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void PlatformInit(void)
{
    // ������ƽ̨��ʼ��
    BoardInitMcu();
    BoardInitPeriph();

    // �������豸��ʼ��
    OLED_Init();//Һ����ʼ��
    USART1_Init(115200);//����1��ʼ��
    OLED_Clear();
    OLED_InitView();//OLED��Ļ��ʾ��ʼ��Ϣ

    printf("�´�½���� LoRa \r\n");

    //Loraģ���ʼ��
    NS_RadioInit( (uint32_t) RF_PING_PONG_FREQUENCY, (int8_t) TX_OUTPUT_POWER, (uint32_t) TX_TIMEOUT_VALUE, (uint32_t) RX_TIMEOUT_VALUE );

    //�����·�����û���ʼ������
    printf("LoRa Light\r\n");
    AdcInit( &Adc, ADC_0);//ADC��ʼ��
    Tim3McuInit(1);//��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��
    //IWDG_PrmInit(2048);//�������Ź���ʼ������ʱ����Ϊ2048ms
}


/**
 * Main application entry point.
 */
int main( void )
{
    PlatformInit();

    while( 1 )
    {
        //IWDG_PrmRefresh( );//ι�������Ź�
        MyRadioRxDoneProcess();//LoRa������Ƶ�������ݴ������
        LoRa_GetSensorDataProcess();
    }
}
