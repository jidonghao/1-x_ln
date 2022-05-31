/**********************************************************************************************************
*ʹ��˵����
*TIM2��TIM3�ӿں������û�Ҫʹ��TIM2��3�Ķ�ʱ���ж�ʱ���ȵ���Tim2McuInit()��Tim3McuInit(),�������������������
*�����ڣ���λ��MS��ȡֵ1~1023MS���û����ж�Ӧ�ó�������ӵ����ļ�ĩβ��HAL_TIM_PeriodElapsedCallback()�ĺ�����
*�״�����ʱ�����к�ÿ���ж�����ʱ�䵽�������ж��ڵ�Ӧ�ó��򣬲��Ұ��ж������ظ���
*
*
**********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "tim-board.h"

/** @addtogroup STM32L1xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
uint16_t User0Timer_MS = 0;//�û�0�ź��붨ʱ����������λ���룬ÿ��1�����Լ�1
uint8_t User0Timer_S = 0;//�û�0���붨ʱ����������λ�룬ÿ��1���Լ�1
uint8_t USART1_RX_TIMEOUT_COUNT = 0;//����1���ճ�ʱ��ʱ����λ����



/* TIM handle declaration */
TIM_HandleTypeDef    Tim2Handle;
TIM_HandleTypeDef    Tim3Handle;
TIM_HandleTypeDef    Tim4Handle;

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
*������void Tim2McuInit(uint16_t PeriodValueMs)
*���ܣ�TIM2��ʱ����ʼ���������ж�����PeriodValueMs����
*���룺uint16_t PeriodValueMs�ж����ڣ���λ:ms��ȡֵ1~1023
*�������
����˵������ʱ����16λ��ʱ�������ʱ������1024ms.ԭ��64000��Ƶ����ʱ1ms������Ϊ64-1�����Զ�ʱ
*					16λ���ֵΪ65535���������65536=64*X�����X=1024
**********************************************************************************************/
void Tim2McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM2CLK = PCLK1
        PCLK1 = HCLK
        => TIM2CLK = HCLK = SystemCoreClock
      To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
      Prescaler = (TIM2CLK / TIM2 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM2 counter clock equal to 64KHz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM2 instance */
    Tim2Handle.Instance = TIM2;

    /* Initialize TIM2 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    Tim2Handle.Init.Period            = PeriodValueMs*64 - 1;//����PeriodValueMs����
    Tim2Handle.Init.Prescaler         = uwPrescalerValue;
    Tim2Handle.Init.ClockDivision     = 0;//TIM_Counter_Mode TIM Counter Mode,TIM_COUNTERMODE_UP=((uint32_t)0x0000)
    Tim2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim2Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("��ʱ��2��ʼ��ʧ�ܣ�\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim2Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("��ʱ��2�����ж�ʧ�ܣ�\r\n");
    }
}

/**********************************************************************************************
*������void Tim3McuInit(uint16_t PeriodValueMs)
*���ܣ�TIM3��ʱ����ʼ���������ж�����PeriodValueMs����
*���룺uint16_t PeriodValueMs�ж����ڣ���λ:ms��ȡֵ1~1023
*�������
*����˵������ʱ����16λ��ʱ�������ʱ������1024ms.ԭ��64000��Ƶ����ʱ1ms������Ϊ64-1�����Զ�ʱ
*					16λ���ֵΪ65535���������65536=64*X�����X=1024
**********************************************************************************************/
void Tim3McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM3CLK = PCLK1
        PCLK1 = HCLK
        => TIM3CLK = HCLK = SystemCoreClock
      To get TIM3 counter clock at 64 KHz, the Prescaler is computed as following:
      Prescaler = (TIM3CLK / TIM3 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM3 counter clock equal to 64000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM3 instance */
    Tim3Handle.Instance = TIM3;

    /* Initialize TIM3 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    Tim3Handle.Init.Period            = (PeriodValueMs*64) - 1;//����PeriodValueMs����
    Tim3Handle.Init.Prescaler         = uwPrescalerValue;
    Tim3Handle.Init.ClockDivision     = 0;
    Tim3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim3Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("��ʱ��3��ʼ��ʧ�ܣ�\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim3Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("��ʱ��3�����ж�ʧ�ܣ�\r\n");
    }
}

/**********************************************************************************************
*������void Tim4McuInit(uint16_t PeriodValueMs)
*���ܣ�TIM4��ʱ����ʼ���������ж�����PeriodValueMs����
*���룺uint16_t PeriodValueMs�ж����ڣ���λ:ms��ȡֵ1~1023
*�������
*����˵������ʱ����16λ��ʱ�������ʱ������1024ms.ԭ��64000��Ƶ����ʱ1ms������Ϊ64-1�����Զ�ʱ
*					16λ���ֵΪ65535���������65536=64*X�����X=1024
**********************************************************************************************/
void Tim4McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM4 input clock (TIM4CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM4CLK = PCLK1
        PCLK1 = HCLK
        => TIM4CLK = HCLK = SystemCoreClock
      To get TIM4 counter clock at 64 KHz, the Prescaler is computed as following:
      Prescaler = (TIM4CLK / TIM4 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM4 counter clock equal to 64000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM4 instance */
    Tim4Handle.Instance = TIM4;

    /* Initialize TIM4 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    Tim4Handle.Init.Period            = (PeriodValueMs*64) - 1;//����PeriodValueMs����
    Tim4Handle.Init.Prescaler         = uwPrescalerValue;
    Tim4Handle.Init.ClockDivision     = 0;
    Tim4Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim4Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("��ʱ��4��ʼ��ʧ�ܣ�\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim4Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("��ʱ��4�����ж�ʧ�ܣ�\r\n");
    }
}

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM2_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if(htim->Instance == TIM3)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM3_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM3_IRQn, 6, 0);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
    else if(htim->Instance == TIM4)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM4_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM4_IRQn, 6, 0);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim2Handle);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim3Handle);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim4Handle);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        /*TIM2�û��ж�Ӧ�ó�����������·��հ״�*/
        //========================��TIM2�հ״���ʼ



        //��========================TIM2�հ״�����
    }
    else if(htim->Instance == TIM3)
    {
        /*TIM3�û��ж�Ӧ�ó�����������·��հ״�*/
        //========================��TIM3�հ״���ʼ
        User0Timer_MS++;

        /***********************����1���ճ�ʱ���̴���************************/
        if(F_USART1_RX_RECEIVING)//���ڽ��մ�������
        {
            USART1_RX_TIMEOUT_COUNT++;//���ڳ�ʱ����
            if(USART1_RX_TIMEOUT_COUNT>USART1_RX_TIMEOUT)//���ݽ��ռ������3ms
            {   //���ڽ�����ɻ����
                F_USART1_RX_RECEIVING=0;
                USART1_RX_TIMEOUT_COUNT=0;
                F_USART1_RX_FINISH=1;
            }
        }
        //��========================TIM3�հ״�����
    }
    else if(htim->Instance == TIM4)
    {
        /*TIM4�û��ж�Ӧ�ó�����������·��հ״�*/
        //========================��TIM4�հ״���ʼ
        if(User0Timer_S < 255)
        {
            User0Timer_S++;
        }
        //��========================TIM4�հ״�����
    }
}

/********************************END OF FILE********************************/
