#ifndef _USER_DELAY_H_
#define _USER_DELAY_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);


#endif //_USER_DELAY_H_
