# GPIO  
> LED������˸��ָʾ�� �������ɼ�һ����˸һ�λ��ߴ��ڷ���һ��������˸һ�Σ�����ˮ��

## CubeMx���ã�

	1. ���ԣ�ͼ2-15  
	2. ʱ������ͼ2-16 ͼ2-17��PCLK1 PCLK2������Ҫ��  
	3. GPIO��ͼ2-13 ͼ2-14��Ĭ�������ƽ Ȧ4 LED�ϵ粻������ΪHIGH��  
## ������룺  

	1. �����Ҫ����ȫ�ֱ���      
		/* USER CODE BEGIN PV */   

        2. GPIO��غ�����stm32f1xx_hal_gpio.h�ļ���  
		GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);//������״̬ ������״̬  	  
		void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);//������д״̬  ����Ϩ��ƣ��պ϶Ͽ��̵���  
		void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);//��ת����״̬   
		
		GPIO_TypeDef *GPIOx���˿���  GPIOE��LED2_GPIO_Port  
		uint16_t GPIO_Pin�����ź� GPIO_PIN_6��LED2_Pin  
		GPIO_PinState PinState��  
			�ߵ͵�ƽ GPIO_PIN_RESET��0  
			GPIO_PIN_SET��1  
											   
											   
											   
		��ʱ����HAL_Delay(1500) ms
		uint_8 num;
		num=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);
		if(num==1)
		{

		}
