/* USER CODE BEGIN Header */
/** 
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user.h"
#include "user_time.h"  //�û����ͷ�ļ�����

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t vol = 0;
float sensor_hum = 0; //ʪ��
float sensor_tem = 0; //�¶�
uint8_t switching = 0;
uint8_t sensor_number = 0;
uint16_t send_count = 0;
uint16_t Can_STD_ID = 0;
uint8_t flag_send_data = 0;
uint8_t Can_data[8] = {0};

/* UART5������ر��� */
uint8_t uart5RxBuf = 0; //UART5���ձ���(1���ֽ�)
uint8_t uart5RxCounter = 0; //UART5���ջ���������
uint8_t uart5RxCplt_flag = 0; //UART5������ɱ�־λ
uint8_t uart5_data_buf[64] = {0}; //UART5���ջ�����

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/*************************
* ��ˮ��
**************************/
uint16_t lightFlag = 0x80;

void riverLight(void){

	if(lightFlag<=0)
		lightFlag = 0x80;
	
	HAL_GPIO_WritePin(LED1_GPIO_Port,0xff,1);
	HAL_GPIO_WritePin(LED1_GPIO_Port,lightFlag,0);
	lightFlag>>=1;
}


/* USER CODE BEGIN 0 */
/**
  * @brief  ���uart5������ػ���
  * @param  None
  * @retval None
  */
void clear_uart5_rxbuf(void)
{  
  uart5RxBuf = 0;
	uart5RxCounter = 0;
	uart5RxCplt_flag = 0;  
  memset(uart5_data_buf, 0, 64);
}

/**
  * @brief  uart5�������ݴ���
  * @param  None
  * @retval None
  */
void uart5_data_process(uint8_t *data)
{
  uint16_t adcx = 0;
  adcx = (uint16_t)data[1] << 8;
  adcx |= data[2];
  
	
  //1.���ɼ���������adcxת��Ϊ��ѹֵvol:��ʽ����ѹֵ = (adcx * 330) / 4096;
	
	vol = (adcx * 330) / 4096;
	
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	
  /* USER CODE BEGIN 1 */
  uint8_t flag = 0x00;
  etError error; // error code
  ValueType Value_Type = Value_I2C;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

	uint16_t num=0x100;
	
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  
  HAL_UART_Receive_IT(&huart5, &uart5RxBuf, 1); //�������ж�
	
	
	//1.__HAL_UART_ENABLE_IT()��������huart5�Ŀ����ж�
  __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);  //���ÿ����ж�
   
	 
  
#ifdef SHT3X
  error = SHT3X_Init(DEFAULT_SHT3X_ADDR);
  SHT3X_GetTempAndHumi(&sensor_tem, &sensor_hum, REPEATAB_HIGH, MODE_POLLING, 50);
#endif

#ifdef SHT1X
  if (error != NO_ERROR)
  {
    SHT1x_init();
    flag = 0x01;
  }
#endif
  //SHT1x_init();    //��ʪ�ȴ�������ʼ��

  if (flag != 0x01)
  {
    printf("��ʪ�ȴ������ͺţ�SHT3X\r\n");
  }
  else
  {
    //FLASH_Sensor_Type = 0x08030100;
    printf("��ʪ�ȴ������ͺţ�SHT1X\r\n");
  }
  InfraredSensor_Init();
  
  Can_STD_ID = STMFLASH_ReadHalfWord(FLASE_M3_ADDR);        //����CAN�ڵ㷢�͵ı�׼֡ID
  Sensor_Type_t = STMFLASH_ReadHalfWord(FLASH_Sensor_Type); //����M3����ģ��ɼ��Ĵ���������
  open_usart1_receive_interrupt();                          //����USART1�����ж�
 
  CAN_User_Config(&hcan); //CAN��������
  can_start();  //����CAN����
	HAL_TIM_Base_Start_IT(&htim2);  //������ʱ��2

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Value_Type = ValueTypes(Sensor_Type_t);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
    /* ���ʹ�CAN���߽��յ������ڵ����������� */
    if (flag_send_data == 1)
    {
	/*.......���⿪ʼ.......................................*/
  //LED1-LED8 �����ˮ�ƣ�ÿ�յ������л�����һ״̬

			
			
			riverLight();
			
			
			
			
	/*.......�������.......................................*/
				
      CAN_Master_To_Gateway(Can_data, 9);
      flag_send_data = 0;
			
			
    }  
    
    /* uart5������� */
    //if(uart5RxCplt_flag == 1)
    //{
    //  uart5RxCplt_flag = 0;
    //  
    //  /* �����uart5�յ������ݴ���vol����*/
    //  uart5_data_process(uart5_data_buf);
    //  /* ���ʹ�CC2530�յ��Ļ��洫�������������� */
    //  Master_To_Gateway(Can_STD_ID, Value_Type, vol, switching, sensor_hum, sensor_tem);
    //  
    //  clear_uart5_rxbuf();
    //}    
    
    //USART1 ͨ��M3����ģ�����ù������òɼ����������� �� ����CAN���ͱ�׼֡ID ��ע��׼֡���ܳ���0 to 0x7FF��
    if (!usart1_data_fifo_is_empty())
    {
      HAL_Delay(100);	//��������
      process_up();
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 40;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin, GPIO_PIN_SET);
	
	
  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = LED6_Pin|LED5_Pin|LED4_Pin|LED3_Pin 
                          |LED2_Pin|LED1_Pin|LED8_Pin|LED7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	
	
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED9_GPIO_Port, LED9_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(U5_485TX_EN_GPIO_Port, U5_485TX_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED4_Pin */
  GPIO_InitStruct.Pin = LED9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : U5_485TX_EN_Pin */
  GPIO_InitStruct.Pin = U5_485TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(U5_485TX_EN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//�����������жϺ����ջ�������
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1) //�ж����ĸ����ڲ������ж�
  {
    usart1_data_fifo_put(usart1RxBuf); //��USART1���ڻ�����д������
    HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1);
  }

  if (huart == &huart2)
  {
    usart2_data_fifo_put(usart2RxBuf); //��USART2���ڻ�����д������
    HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1);
  }
  
  if (huart->Instance == UART5)
  {
    uart5_data_buf[uart5RxCounter++] = uart5RxBuf;
    HAL_UART_Receive_IT(&huart5, &uart5RxBuf, 1); //���¿������ж�
  }
}


void USER_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
  /* �ж��Ƿ��Ǵ���5�����ж� */
  if (huart->Instance == UART5)
  {
    /* �ж��Ƿ��ǿ����ж� */
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)
    {
      uart5RxCplt_flag = 1; //������ɱ�־λ��1

      /* ��������жϱ�־�������һֱ���Ͻ����жϣ� */
      __HAL_UART_CLEAR_IDLEFLAG(&huart5);
    }
  }
}

//�����ڳ�����ת����
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  printf("UART Error:%x\r\n", huart->ErrorCode); //���������
  huart->ErrorCode = HAL_UART_ERROR_NONE;

  if (huart == &huart1)
  {
    HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1); //���´�USART1�����ж�
  }

  if (huart == &huart2)
  {
    HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1); //���´�USART2�����ж�
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
