#include "user_time.h"

/****************************************************************
 *Function: void start_TIM2(void)
 *Describe: ������ʱ��2
 *Input   ��NONE
 *return  ��NONE
 ****************************************************************/
void start_TIM2(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
}

/****************************************************************
 *Function: void start_TIM2(void)
 *Describe: ֹͣ��ʱ��2
 *Input   ��NONE
 *return  ��NONE
 ****************************************************************/
void stop_TIM2(void)
{
	HAL_TIM_Base_Stop_IT(&htim2);
}


//
/****************************************************************
 *Function: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *Describe: ��ʱ���жϻص���������ʱ�������жϺ����ջ���ת����
 *Input   ��htim
 *return  ��NONE
 ****************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		
	}
		
}

