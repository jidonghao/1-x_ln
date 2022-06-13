#include "user_usart.h"

#ifdef BSP_USING_USART1
/* USART1������ر��� */
uint8_t usart1RxBuf = 0;
uint8_t usart1RxCounter = 0;
uint8_t usart1RxCplt_flag = 0;
uint8_t usart1_data_buf[64] = {0};

/**
  * @brief  ���usart1������ػ���
  * @param  None
  * @retval None
  */
void clear_usart1_rxbuf(void)
{
  usart1RxCplt_flag = 0;
	usart1RxCounter = 0;
  usart1RxBuf = 0;  
  memset(usart1_data_buf, 0, 64);
}
#endif


#ifdef BSP_USING_USART2
/* USART2������ر��� */
uint8_t usart2RxBuf = 0;
uint8_t usart2RxCounter = 0;
uint8_t usart2RxCplt_flag = 0;
uint8_t usart2_data_buf[64] = {0};

/**
  * @brief  ���usart2������ػ���
  * @param  None
  * @retval None
  */
void clear_usart2_rxbuf(void)
{  
  usart2RxBuf = 0;
	usart2RxCounter = 0;
	usart2RxCplt_flag = 0;  
  memset(usart2_data_buf, 0, 64);
}
#endif

#ifdef BSP_USING_UART5
/* UART5������ر��� */
uint8_t uart5RxBuf = 0;
uint8_t uart5RxCounter = 0;
uint8_t uart5RxCplt_flag = 0;
uint8_t uart5_data_buf[64] = {0};

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
#endif


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef BSP_USING_USART1
  if (huart->Instance == USART1)
  {
    usart1_data_buf[usart1RxCounter++] = usart1RxBuf;
    HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1); //���¿������ж�
  }
#endif
#ifdef BSP_USING_USART2  
  if (huart->Instance == USART2)
  {
    usart2_data_buf[usart2RxCounter++] = usart2RxBuf;
    HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1); //���¿������ж�
  }
#endif
#ifdef BSP_USING_UART5
  if (huart->Instance == UART5)
  {
    uart5_data_buf[uart5RxCounter++] = uart5RxBuf;
    HAL_UART_Receive_IT(&huart5, &uart5RxBuf, 1); //���¿������ж�
  }
#endif
}

/**
  * @brief  �û��Զ���USART����IDLE�жϻص�
  * @param  None
  * @retval None
  */
void USER_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
#ifdef BSP_USING_USART1
  /* �ж��Ƿ��Ǵ���1�����ж� */
  if (huart->Instance == USART1)
  {
    /* �ж��Ƿ��ǿ����ж� */
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {
      usart1RxCplt_flag = 1; //������ɱ�־λ��1
      printf("\r\nUART1 Idle IQR Detected\r\n");

      /* ��������жϱ�־�������һֱ���Ͻ����жϣ� */
      __HAL_UART_CLEAR_IDLEFLAG(&huart1);

      /* ���ÿ����жϴ����� */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif
#ifdef BSP_USING_USART2
  /* �ж��Ƿ��Ǵ���2�����ж� */
  if (huart->Instance == USART2)
  {
    /* �ж��Ƿ��ǿ����ж� */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
    {
      usart2RxCplt_flag = 1; //������ɱ�־λ��1
      printf("\r\nUART2 Idle IQR Detected\r\n");

      /* ��������жϱ�־�������һֱ���Ͻ����жϣ� */
      __HAL_UART_CLEAR_IDLEFLAG(&huart2);

      /* ���ÿ����жϴ����� */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif

#ifdef BSP_USING_UART5
  /* �ж��Ƿ��Ǵ���5�����ж� */
  if (huart->Instance == UART5)
  {
    /* �ж��Ƿ��ǿ����ж� */
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)
    {
      uart5RxCplt_flag = 1; //������ɱ�־λ��1
      printf("\r\nUART5 Idle IQR Detected\r\n");

      /* ��������жϱ�־�������һֱ���Ͻ����жϣ� */
      __HAL_UART_CLEAR_IDLEFLAG(&huart5);

      /* ���ÿ����жϴ����� */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif

}
