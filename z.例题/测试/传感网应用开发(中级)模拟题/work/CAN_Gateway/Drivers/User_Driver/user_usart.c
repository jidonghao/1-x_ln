#include "user_usart.h"

#ifdef BSP_USING_USART1
/* USART1接收相关变量 */
uint8_t usart1RxBuf = 0;
uint8_t usart1RxCounter = 0;
uint8_t usart1RxCplt_flag = 0;
uint8_t usart1_data_buf[64] = {0};

/**
  * @brief  清除usart1接收相关缓存
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
/* USART2接收相关变量 */
uint8_t usart2RxBuf = 0;
uint8_t usart2RxCounter = 0;
uint8_t usart2RxCplt_flag = 0;
uint8_t usart2_data_buf[64] = {0};

/**
  * @brief  清除usart2接收相关缓存
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
/* UART5接收相关变量 */
uint8_t uart5RxBuf = 0;
uint8_t uart5RxCounter = 0;
uint8_t uart5RxCplt_flag = 0;
uint8_t uart5_data_buf[64] = {0};

/**
  * @brief  清除uart5接收相关缓存
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
    HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1); //重新开接收中断
  }
#endif
#ifdef BSP_USING_USART2  
  if (huart->Instance == USART2)
  {
    usart2_data_buf[usart2RxCounter++] = usart2RxBuf;
    HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1); //重新开接收中断
  }
#endif
#ifdef BSP_USING_UART5
  if (huart->Instance == UART5)
  {
    uart5_data_buf[uart5RxCounter++] = uart5RxBuf;
    HAL_UART_Receive_IT(&huart5, &uart5RxBuf, 1); //重新开接收中断
  }
#endif
}

/**
  * @brief  用户自定义USART接收IDLE中断回调
  * @param  None
  * @retval None
  */
void USER_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
#ifdef BSP_USING_USART1
  /* 判断是否是串口1空闲中断 */
  if (huart->Instance == USART1)
  {
    /* 判断是否是空闲中断 */
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {
      usart1RxCplt_flag = 1; //接收完成标志位置1
      printf("\r\nUART1 Idle IQR Detected\r\n");

      /* 清除空闲中断标志（否则会一直不断进入中断） */
      __HAL_UART_CLEAR_IDLEFLAG(&huart1);

      /* 调用空闲中断处理函数 */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif
#ifdef BSP_USING_USART2
  /* 判断是否是串口2空闲中断 */
  if (huart->Instance == USART2)
  {
    /* 判断是否是空闲中断 */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
    {
      usart2RxCplt_flag = 1; //接收完成标志位置1
      printf("\r\nUART2 Idle IQR Detected\r\n");

      /* 清除空闲中断标志（否则会一直不断进入中断） */
      __HAL_UART_CLEAR_IDLEFLAG(&huart2);

      /* 调用空闲中断处理函数 */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif

#ifdef BSP_USING_UART5
  /* 判断是否是串口5空闲中断 */
  if (huart->Instance == UART5)
  {
    /* 判断是否是空闲中断 */
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)
    {
      uart5RxCplt_flag = 1; //接收完成标志位置1
      printf("\r\nUART5 Idle IQR Detected\r\n");

      /* 清除空闲中断标志（否则会一直不断进入中断） */
      __HAL_UART_CLEAR_IDLEFLAG(&huart5);

      /* 调用空闲中断处理函数 */
      //USAR_UART_IDLECallback(huart);
    }
  }
#endif

}
