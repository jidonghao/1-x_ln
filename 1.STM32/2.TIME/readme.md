��Ŀ��ǿ��ʹ�ö�ʱ����Ҫ�󣬽���ʹ��HAL_Delay����
1.CubeMAX���ã�
��ʱ������2-5  T6 T7������ʱ����ֻ�ܼ�ʱ��������PWM������
                       T1 T8������APB2    ���������APB1
             T1~T5  T8  ����clock sourceΪinternal clockͼ2-44
             ��ʱʱ�����ã�ͼ2-34
                   1.�鿴ʱ����APB1 timer clocksƵ��
                   2.ͼ2-34��   Ȧ3*Ȧ4=��ʱʱ��*��1����Ƶ��
                      Ȧ3��Ȧ4����Ϊ������Ȧ4���ܳ���65535
                      ʵ����д���1
                   3.��ʱ��ʹ���ж�ģʽͼ2-35
2.������ӣ�
           ��ʱ����غ�����stm32f1xx_hal_tim.h��

           HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)    //��stm32f1xx_hal_tim.h��1877��   ��ʱ������ ����MX_TIM6_Init()����
          ����������&htim6

          void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)   //��stm32f1xx_hal_tim.h��2043��    ��д�ص���������Ҫ����  д��main��������  д��USER CODE BEGIN 4 
          void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��д�ص�����
          {
	if(TIM6 == htim->Instance)//ָ��ָ��ʵ��  ��ʱʱ�䵽
	{
	   //ʱ�䵽�����Ĺ���	
	}
         }
        ��ˮ��LED1~LED8
      num=0x100;

      num = num>>1;
      HAL_GPIO_WritePin(GPIOE, LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE, num, GPIO_PIN_RESET);
      if(num == 0)
            num=0x80;

        ��ˮ��LED8~LED1
      num=0x01;

      num = num<<1;
      HAL_GPIO_WritePin(GPIOE, LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE, num, GPIO_PIN_RESET);
      if(num == 0x100)
            num=0x01;
       