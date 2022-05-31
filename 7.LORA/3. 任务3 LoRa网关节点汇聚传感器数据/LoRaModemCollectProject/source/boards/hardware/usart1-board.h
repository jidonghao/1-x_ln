/******************************************************************************
*
*
*
*
*
*
*
******************************************************************************/

#ifndef _USART1_BOARD_H_
#define _USART1_BOARD_H_

/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	extern "C"{
#endif
/*===========================================================================*/

/******************************************************************************
                               �ⲿ����ͷ�ļ�                        
******************************************************************************/

#include "board.h" 


/*
#define USART_FLAG_CTS                       ((uint16_t)0x0200)
#define USART_FLAG_LBD                       ((uint16_t)0x0100)
#define USART_FLAG_TXE                       ((uint16_t)0x0080)
#define USART_FLAG_TC                        ((uint16_t)0x0040)
#define USART_FLAG_RXNE                      ((uint16_t)0x0020)
#define USART_FLAG_IDLE                      ((uint16_t)0x0010)
#define USART_FLAG_ORE                       ((uint16_t)0x0008)
#define USART_FLAG_NE                        ((uint16_t)0x0004)
#define USART_FLAG_FE                        ((uint16_t)0x0002)
#define USART_FLAG_PE                        ((uint16_t)0x0001)
*/

/******************************************************************************
                               �������Ӧ����
******************************************************************************/

#define USART1_REC_LEN					255	//�����������ֽ���
#define USART1_RX_TIMEOUT				3	//������ճ�ʱʱ��,��λ��ms

/******************************************************************************
                              �ⲿ���ù��ܺ���
******************************************************************************/

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];	//���ջ��壬���USART1_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з� 
extern uint16_t USART1_RX_COUNT;					//usart1���յ����ݸ���
extern bool F_USART1_RX_FINISH;//����״̬���,1:usart1������ɣ�0��usart1����δ���
extern bool F_USART1_RX_RECEIVING;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����

/******************************************************************************
*�������ƣ�void USART1_Init(uint32_t bound)
*���ܣ���ʼ������1
*���룺uint32_t bound��������
*�������
*���أ���
******************************************************************************/
void USART1_Init(uint32_t bound);

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
FlagStatus USART1_GetFlagStatus(uint16_t USART_FLAG);

/******************************************************************************
*�������ƣ�void USART1_SendData(uint16_t Data)
*���ܣ�����1����һ���ֽ�����
*���룺uint16_t Data�������͵�����
*�������
*���أ���
******************************************************************************/
void USART1_SendData(uint16_t Data);

/******************************************************************************
*�������ƣ�uint16_t USART1_ReceiveData(void)
*���ܣ�����1����һ���ֽ�����
*���룺��
*�������
*���أ����ؽ��յ��Ĵ�������
******************************************************************************/
uint16_t USART1_ReceiveData(void);

/******************************************************************************
*�������ƣ�void USART1_SendStr(uint8_t *Data, uint8_t length)
*���ܣ�����1����length���ֽ�
*���룺uint8_t *Data-�����͵����ݣ� uint8_t length-�����͵����ݳ���
*�������
*���أ���
******************************************************************************/
void USART1_SendStr(uint8_t *Data, uint8_t length);

/******************************************************************************
*�������ƣ�void USART1_ReceiveClr(void)
*���ܣ�����1��ؼĴ����ͱ�־λ���
*���룺��
*�������
*���أ���
******************************************************************************/
void USART1_ReceiveClr(void);

/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	}
#endif
/*===========================================================================*/


#endif  /* _USART2_BOARD_H_ */





