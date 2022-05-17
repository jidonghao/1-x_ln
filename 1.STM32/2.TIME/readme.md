题目无强制使用定时器的要求，建议使用HAL_Delay函数
1.CubeMAX配置：
定时器：表2-5  T6 T7基本定时器，只能计时不能用于PWM呼吸灯
                       T1 T8挂载在APB2    其余挂载在APB1
             T1~T5  T8  设置clock source为internal clock图2-44
             定时时间设置：图2-34
                   1.查看时钟树APB1 timer clocks频率
                   2.图2-34中   圈3*圈4=计时时间*第1步的频率
                      圈3和圈4必须为整数，圈4不能超过65535
                      实际填写需减1
                   3.定时器使用中断模式图2-35
2.代码添加：
           定时器相关函数在stm32f1xx_hal_tim.h中

           HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)    //在stm32f1xx_hal_tim.h中1877行   定时器运行 放在MX_TIM6_Init()后面
          函数参数：&htim6

          void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)   //在stm32f1xx_hal_tim.h中2043行    重写回调函数不需要调用  写在main（）后面  写在USER CODE BEGIN 4 
          void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//重写回调函数
          {
	if(TIM6 == htim->Instance)//指针指向实例  定时时间到
	{
	   //时间到后做的工作	
	}
         }
        流水灯LED1~LED8
      num=0x100;

      num = num>>1;
      HAL_GPIO_WritePin(GPIOE, LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE, num, GPIO_PIN_RESET);
      if(num == 0)
            num=0x80;

        流水灯LED8~LED1
      num=0x01;

      num = num<<1;
      HAL_GPIO_WritePin(GPIOE, LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE, num, GPIO_PIN_RESET);
      if(num == 0x100)
            num=0x01;
       