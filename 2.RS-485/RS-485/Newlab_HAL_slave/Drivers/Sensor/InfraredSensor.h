#ifndef __INFRAREDSENSOR_H
#define __INFRAREDSENSOR_H
#include "stm32f1xx_hal.h"

#define READSTATUS  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)

void InfraredSensor_Init(void);
uint8_t Switching_Value(void);

#endif
