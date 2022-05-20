#include "InfraredSensor.h"


void InfraredSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
uint8_t Switching_Value(void)
{
    return READSTATUS;
}
