/******************************************************************************
*
*
*
*
*
*
*
******************************************************************************/

#include "usart2-board.h"

uint8_t USART2_RX_BUF[USART2_REC_LEN];	//���ջ���,���USART2_REC_LEN���ֽ�
uint16_t USART2_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
bool F_USART2_RX_FINISH=false;//����״̬���,1:usart1������ɣ�0��usart1����δ���
bool F_USART2_RX_RECEIVING=false;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����

/******************************************************************************
*�������ƣ�void USART2_Init(uint32_t bound)
*���ܣ���ʼ������2
*���룺uint32_t bound��������
*�������
*���أ���
******************************************************************************/
void USART2_Init(uint32_t bound)
{
	//��ʼ������2
	UartInit( &Uart2, UART_2, UART2_TX, UART2_RX );	
	UartConfig( &Uart2, RX_TX, bound, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );// RX_TX
}

/******************************************************************************
* �������ƣ�bool USART_GetFlagStatus(uint16_t USART_FLAG)
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
FlagStatus USART2_GetFlagStatus(uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  if ((USART2->SR & USART_FLAG) != (uint16_t)RESET)
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
*�������ƣ�void USART2_SendData(uint16_t Data)
*���ܣ�����2����һ���ֽ�����
*���룺uint16_t Data�������͵�����
*�������
*���أ���
******************************************************************************/
void USART2_SendData(uint16_t Data)
{
  /*
  //�ȴ��������
  while(USART2_GetFlagStatus(USART_FLAG_TC) != SET);
  */
  //Transmit Data
  USART2->DR = (Data & (uint16_t)0x01FF);
}

/******************************************************************************
*�������ƣ�uint16_t USART2_ReceiveData(void)
*���ܣ�����2����һ���ֽ�����
*���룺��
*�������
*���أ����ؽ��յ��Ĵ�������
******************************************************************************/
uint16_t USART2_ReceiveData(void)
{
  /* Receive Data */
  return (uint16_t)(USART2->DR & (uint16_t)0x01FF);
}

/******************************************************************************
*�������ƣ�void USART2_SendStr(uint8_t *Data, uint8_t length)
*���ܣ�����2����length���ֽ�
*���룺uint8_t *Data-�����͵����ݣ� uint8_t length-�����͵����ݳ���
*�������
*���أ���
******************************************************************************/
void USART2_SendStr(uint8_t *Data, uint8_t length)
{
	while(length--)
	{
		//�ȴ��������
		while(USART2_GetFlagStatus(USART_FLAG_TC) != SET);
		USART2_SendData((uint16_t )(*Data));
		Data++;
	}
}

/******************************************************************************
*�������ƣ�void USART2_ReceiveClr(void)
*���ܣ�����2��ؼĴ����ͱ�־λ���
*���룺��
*�������
*���أ���
******************************************************************************/
void USART2_ReceiveClr(void)
{
	USART2_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
	F_USART2_RX_FINISH=false;//����״̬���,1:usart1������ɣ�0��usart1����δ���
	memset(USART2_RX_BUF, '\0', USART2_REC_LEN);
}

/******************************************************************************
*�������ƣ�void USART2_IRQHandler(void)
*���ܣ�����2�жϷ������
*���룺��
*�������
*���أ���
******************************************************************************/
void USART2_IRQHandler(void)
{
	if(USART2_GetFlagStatus((uint16_t)USART_IT_RXNE) != RESET)  //���յ�����
	{
		USART2_RX_BUF[USART2_RX_COUNT] = (uint8_t)USART2_ReceiveData();	//��ȡ���յ�������
		USART2_RX_COUNT++;
		if(USART2_RX_COUNT>USART2_REC_LEN)//���������
		{USART2_RX_COUNT = 0x0000;}
		F_USART2_RX_RECEIVING=true;//���ڽ��մ�������
		USART2_RX_TIMEOUT_COUNT=0;//��ʱ��������
	}
}
