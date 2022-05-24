#include "user_time.h"

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


//
/****************************************************************
 *Function: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *Describe: 定时器中断回调函数，定时器产生中断后最终会跳转到此
 *Input   ：htim
 *return  ：NONE
 ****************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		
	}
		
}

