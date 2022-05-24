#ifndef __MAX3485_H
#define __MAX3485_H
#include "stm32f1xx_hal.h"

/*����485���ͺͽ��գ����������Ӧ�ĺ궨��*/
#define MAX3485_ConRx() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)
#define MAX3485_ConTx() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)

/*******************************************************************
*������void MAX3485_INIT(void)
*���ܣ�MAX3485��ʼ��
*���룺��
*���أ���
*����˵����
*******************************************************************/
void MAX3485_INIT(void);

#endif
