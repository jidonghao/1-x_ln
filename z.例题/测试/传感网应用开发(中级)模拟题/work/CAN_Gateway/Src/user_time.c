#include "user_time.h"
#include "main.h"

/****************************************************************
 *Function: void start_TIM2(void)
 *Describe: 启动定时器2
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/
void start_TIM2(void)
{
  HAL_TIM_Base_Start_IT(&htim2);
}

/****************************************************************
 *Function: void start_TIM2(void)
 *Describe: 停止定时器2
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/
void stop_TIM2(void)
{
  HAL_TIM_Base_Stop_IT(&htim2);
}

/****************************************************************
 *Function: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *Describe: 定时器中断回调函数，定时器产生中断后最终会跳转到此
 *Input   ：htim
 *return  ：NONE
 ****************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t count = 0;  //TIM2计数值
  if (htim == &htim2)
  {
    if(count++ >= 1000)
    {
      HAL_GPIO_TogglePin(LED9_GPIO_Port, LED9_Pin);
      count = 0;
    }
  }
}
