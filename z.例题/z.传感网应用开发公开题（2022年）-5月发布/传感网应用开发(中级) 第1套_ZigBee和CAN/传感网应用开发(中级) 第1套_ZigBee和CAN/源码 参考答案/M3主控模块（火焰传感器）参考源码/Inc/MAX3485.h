#ifndef __MAX3485_H
#define __MAX3485_H
#include "stm32f1xx_hal.h"

/*利用485发送和接收，都需调用相应的宏定义*/
#define MAX3485_ConRx() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)
#define MAX3485_ConTx() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)

/*******************************************************************
*函数：void MAX3485_INIT(void)
*功能：MAX3485初始化
*输入：无
*返回：无
*特殊说明：
*******************************************************************/
void MAX3485_INIT(void);

#endif
