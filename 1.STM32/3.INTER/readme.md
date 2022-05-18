# PWM 呼吸灯  
> 呼吸灯：逐渐变亮，逐渐变暗    
> 只有T4-CH3-PB8-LED9可以做呼吸灯  

## CubeMX配置：(呼吸灯)
	1. LED9 图2-43 不可变
	2. T4  图2-44 不可变    时间间隔不超过1ms 圈3和圈4根据给的PCLK1 PCLK2自己算
		 按键配置：未按时，PC13接高电平3.3V；按下时，PC13接地低电平0V    (KEY2-PD13)
		1. 图2-42  图2-45

## 添加代码
	按键外部中断相关函数在stm32f1xx_hal_gpio.h文件中
	```
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // 外部中断回调函数
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//重写外部中断回调函数
	{
		if(GPIO_Pin & GPIO_PIN_13)//检测中断请求
		{
			if(keydown_flag==0)//如果目前按键未按
			{
				keydown_flag = 1;//按下按键
			}
			else
			{
				keydown_flag = 0;//松开按键
			}
		}
	}
```  
	TIM4函数：在stm32f1xx_hal_tim.h文件中
	HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);//开始计时
	函数参数：&htim4,TIM_CHANNEL_3     在stm32f1xx_hal_tim.h文件中1918行

	_ _HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)//生成PWM电压
	函数参数：&htim4,TIM_CHANNEL_3,pwm_value    
		> 在stm32f1xx_hal_tim.h文件中1387行