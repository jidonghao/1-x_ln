#ifndef _USER_ADC_H_
#define _USER_ADC_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;


uint16_t Get_Voltage(void);

#endif //_USER_ADC_H_
