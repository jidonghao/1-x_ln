/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "mcheck.h"
#include "math.h"
#include "temHum.h"
#define USE_DEBUG_LOG
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rxBuf[24] = {0};
uint8_t rxPos=0;
uint8_t usart2rxBuf;
uint16_t illumination_value;//光照度
uint8_t currAddr=0x02; 			//当前节点地址

//重写fputc函数实现串口调试信息输出
int fputc(int ch, FILE *f)
{ 
#ifdef USE_DEBUG_LOG
  HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
#endif
  return(ch);
}

//LED1-LED8 组成的流水灯切换到下一状态
uint16_t num=0x100;
void Water_led_next(void)
{
	num=num>>1;
	if(num==0)
		num=0x80;
	HAL_GPIO_WritePin(GPIOE,0xff,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,num,GPIO_PIN_RESET);
}

void Refresh_Sensor()
{	
	float adcValue,voltage_val;	
	//启动ADC
	HAL_ADC_Start(&hadc1);  	
	//等待采集完成
	HAL_ADC_PollForConversion(&hadc1,40); 	
	//获取ADC采集的数据
	adcValue = HAL_ADC_GetValue(&hadc1);
	//将采集到的数据转换为电压值(单位:mv)
	voltage_val = adcValue * 3300 /(4096.0-1);
	//将电压值转换为光照强度
	illumination_value=(5/2.0)*(voltage_val/10.0);					//除以10K电阻转换成光电流	
	//停止ADC
	HAL_ADC_Stop(&hadc1); 
	
	//根据光照度进行自动开关照明灯		
	if(illumination_value<100)
		HAL_GPIO_WritePin(CTRL_GPIO_Port,CTRL_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(CTRL_GPIO_Port,CTRL_Pin,GPIO_PIN_RESET);
			
	//HAL_GPIO_TogglePin(LED9_GPIO_Port,LED9_Pin);
}



void Send_Illuminance(uint8_t idx)
{	
	uint8_t cmd[7] = {0};
	cmd[0] = idx;
	cmd[1] = 0x03;
	cmd[2] = 0x06;
	cmd[3] = 0x02;	
	cmd[4] = (illumination_value&0xff00)>>8;
	cmd[5] = illumination_value&0x00ff;
	cmd[6]=mc_check_crc8(cmd,6);
	printf("当前光照度为%d\r\n",illumination_value);	
	HAL_GPIO_WritePin(USART2_TX_EN_GPIO_Port,USART2_TX_EN_Pin,GPIO_PIN_SET);	
	HAL_UART_Transmit(&huart2,cmd,7,0xffff);	
	HAL_GPIO_WritePin(USART2_TX_EN_GPIO_Port,USART2_TX_EN_Pin,GPIO_PIN_RESET);
	
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance==TIM6)
 {
		Refresh_Sensor();
 } 
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		rxBuf[rxPos++]=usart2rxBuf;		
		HAL_UART_Receive_IT(&huart2,&usart2rxBuf,1);	
	}
	
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */	
	printf("STM32 从机节点%d程序\r\n",currAddr);	
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_UART_Receive_IT(&huart2,&usart2rxBuf,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
		
			Water_led_next();				
			HAL_Delay(2000);
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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

/* USER CODE BEGIN 4 */

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
