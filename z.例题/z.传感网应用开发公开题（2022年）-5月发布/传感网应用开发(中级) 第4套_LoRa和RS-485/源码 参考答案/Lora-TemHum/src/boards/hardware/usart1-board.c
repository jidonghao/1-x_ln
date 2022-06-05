/******************************************************************************
*
*
*
*
*
*
*
******************************************************************************/

#include "usart1-board.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];	//���ջ���,���USART1_REC_LEN���ֽ�
uint16_t USART1_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
bool F_USART1_RX_FINISH=false;//����״̬���,1:usart1������ɣ�0��usart1����δ���
bool F_USART1_RX_RECEIVING=false;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����

/******************************************************************************
*�������ƣ�void USART1_Init(uint32_t bound)
*���ܣ���ʼ������1
*���룺uint32_t bound��������
*�������
*���أ���
******************************************************************************/
void USART1_Init(uint32_t bound)
{
    //��ʼ������1
    UartInit( &Uart1, UART_1, UART_TX, UART_RX );
    UartConfig( &Uart1, RX_TX, bound, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );// RX_TX
}

/******************************************************************************
* �������ƣ�bool USART1_GetFlagStatus(uint16_t USART_FLAG)
* ���ܣ�Checks whether the specified USART flag is set or not.
* ���룺uint16_t USART_FLAG��USART_FLAG: specifies the flag to check.
*   This parameter can be one of the following values:
*     @arg USART_FLAG_LBD:  LIN Break detection flag
*     @arg USART_FLAG_TXE:  Transmit data register empty flag
*     @arg USART_FLAG_TC:   Transmission Complete flag
*     @arg USART_FLAG_RXNE: Receive data register not empty flag
*     @arg USART_FLAG_IDLE: Idle Line detection flag
*     @arg USART_FLAG_ORE:  OverRun Error flag
*     @arg USART_FLAG_NE:   Noise Error flag
*     @arg USART_FLAG_FE:   Framing Error flag
*     @arg USART_FLAG_PE:   Parity Error flag
* �������
* ���أ� The new state of USART_FLAG (SET or RESET).
******************************************************************************/
FlagStatus USART1_GetFlagStatus(uint16_t USART_FLAG)
{
    FlagStatus bitstatus = RESET;

    if ((USART1->SR & USART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/******************************************************************************
*�������ƣ�void USART1_SendData(uint16_t Data)
*���ܣ�����1����һ���ֽ�����
*���룺uint16_t Data�������͵�����
*�������
*���أ���
******************************************************************************/
void USART1_SendData(uint16_t Data)
{
    /*
    //�ȴ��������
    while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
    */
    //Transmit Data
    USART1->DR = (Data & (uint16_t)0x01FF);
}

/******************************************************************************
*�������ƣ�uint16_t USART1_ReceiveData(void)
*���ܣ�����1����һ���ֽ�����
*���룺��
*�������
*���أ����ؽ��յ��Ĵ�������
******************************************************************************/
uint16_t USART1_ReceiveData(void)
{
    /* Receive Data */
    return (uint16_t)(USART1->DR & (uint16_t)0x01FF);
}

/******************************************************************************
*�������ƣ�void USART1_SendStr(uint8_t *Data, uint8_t length)
*���ܣ�����1����length���ֽ�
*���룺uint8_t *Data-�����͵����ݣ� uint8_t length-�����͵����ݳ���
*�������
*���أ���
******************************************************************************/
void USART1_SendStr(uint8_t *Data, uint8_t length)
{
    while(length--)
    {
        //�ȴ��������
        while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
        USART1_SendData((uint16_t )(*Data));
        //�ȴ��������
        while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
        Data++;
    }
}

/******************************************************************************
*�������ƣ�void USART1_ReceiveClr(void)
*���ܣ�����1��ؼĴ����ͱ�־λ���
*���룺��
*�������
*���أ���
******************************************************************************/
void USART1_ReceiveClr(void)
{
    USART1_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
    F_USART1_RX_FINISH=false;//����״̬���,1:usart1������ɣ�0��usart1����δ���
    memset(USART1_RX_BUF, '\0', USART1_REC_LEN);
}

/******************************************************************************
*�������ƣ�void USART1_IRQHandler(void)
*���ܣ�����1�жϷ������
*���룺��
*�������
*���أ���
******************************************************************************/
void USART1_IRQHandler(void)
{
    if(USART1_GetFlagStatus((uint16_t)USART_IT_RXNE) != RESET)  //���յ�����
    {
        USART1_RX_BUF[USART1_RX_COUNT] = (uint8_t)USART1_ReceiveData();	//��ȡ���յ�������
        USART1_RX_COUNT++;
        if(USART1_RX_COUNT>USART1_REC_LEN)//���������
        {
            USART1_RX_COUNT = 0x0000;
        }
        F_USART1_RX_RECEIVING=true;//���ڽ��մ�������
        USART1_RX_TIMEOUT_COUNT=0;//��ʱ��������
    }
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif
