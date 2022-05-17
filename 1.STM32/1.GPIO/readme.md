# GPIO  
> LED亮灭、闪烁（指示灯 传感器采集一次闪烁一次或者串口发送一次数据闪烁一次）、流水灯

## CubeMx配置：

	1. 调试：图2-15  
	2. 时钟树：图2-16 图2-17（PCLK1 PCLK2看试题要求）  
	3. GPIO：图2-13 图2-14（默认输出电平 圈4 LED上电不亮设置为HIGH）  
## 补充代码：  

	1. 如果需要定义全局变量      
		/* USER CODE BEGIN PV */   

        2. GPIO相关函数在stm32f1xx_hal_gpio.h文件中  
		GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);//读引脚状态 读按键状态  	  
		void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);//给引脚写状态  点亮熄灭灯，闭合断开继电器  
		void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);//翻转引脚状态   
		
		GPIO_TypeDef *GPIOx：端口组  GPIOE或LED2_GPIO_Port  
		uint16_t GPIO_Pin：引脚号 GPIO_PIN_6或LED2_Pin  
		GPIO_PinState PinState：  
			高低电平 GPIO_PIN_RESET或0  
			GPIO_PIN_SET或1  
											   
											   
											   
		延时函数HAL_Delay(1500) ms
		uint_8 num;
		num=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);
		if(num==1)
		{

		}
