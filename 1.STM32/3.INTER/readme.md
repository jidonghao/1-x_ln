# PWM ������  
> �����ƣ��𽥱������𽥱䰵    
> ֻ��T4-CH3-PB8-LED9������������  

## CubeMX���ã�(������)
	1. LED9 ͼ2-43 ���ɱ�
	2. T4  ͼ2-44 ���ɱ�    ʱ����������1ms Ȧ3��Ȧ4���ݸ���PCLK1 PCLK2�Լ���
		 �������ã�δ��ʱ��PC13�Ӹߵ�ƽ3.3V������ʱ��PC13�ӵص͵�ƽ0V    (KEY2-PD13)
		1. ͼ2-42  ͼ2-45

## ��Ӵ���
	�����ⲿ�ж���غ�����stm32f1xx_hal_gpio.h�ļ���
	```
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // �ⲿ�жϻص�����
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//��д�ⲿ�жϻص�����
	{
		if(GPIO_Pin & GPIO_PIN_13)//����ж�����
		{
			if(keydown_flag==0)//���Ŀǰ����δ��
			{
				keydown_flag = 1;//���°���
			}
			else
			{
				keydown_flag = 0;//�ɿ�����
			}
		}
	}
```  
	TIM4��������stm32f1xx_hal_tim.h�ļ���
	HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);//��ʼ��ʱ
	����������&htim4,TIM_CHANNEL_3     ��stm32f1xx_hal_tim.h�ļ���1918��

	_ _HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)//����PWM��ѹ
	����������&htim4,TIM_CHANNEL_3,pwm_value    
		> ��stm32f1xx_hal_tim.h�ļ���1387��