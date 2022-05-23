#ifndef _USER_TIME_H_
#define _USER_TIME_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


extern TIM_HandleTypeDef htim2;


void start_TIM2(void);
void stop_TIM2(void);


#endif //_USER_TIME_H_
