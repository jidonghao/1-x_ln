#ifndef __USER_USART_H
#define __USER_USART_H
#include "main.h"
#include "stdio.h"
#include "string.h"
//#include "usart.h"

//#define BSP_USING_USART1
//#define BSP_USING_USART2
#define BSP_USING_UART5

#ifdef BSP_USING_USART1
extern UART_HandleTypeDef huart1;
/* USART1接收相关变量 */
extern uint8_t usart1RxBuf; //USART接收缓存
extern uint8_t usart1RxCounter;	//缓存下标
extern uint8_t usart1RxCplt_flag;	//接收完成标志
extern uint8_t usart1_data_buf[64];	//数据缓存数组
void clear_usart1_rxbuf(void);
#endif

#ifdef BSP_USING_USART2
extern UART_HandleTypeDef huart2;
/* USART2接收相关变量 */
extern uint8_t usart2RxBuf;
extern uint8_t usart2RxCounter;
extern uint8_t usart2RxCplt_flag;
extern uint8_t usart2_data_buf[64];
void clear_usart2_rxbuf(void);
#endif

#ifdef BSP_USING_UART5
extern UART_HandleTypeDef huart5;
/* UART5接收相关变量 */
extern uint8_t uart5RxBuf;
extern uint8_t uart5RxCounter;
extern uint8_t uart5RxCplt_flag;
extern uint8_t uart5_data_buf[64];
void clear_uart5_rxbuf(void);
#endif

void USER_IDLE_IRQHandler(UART_HandleTypeDef *huart);

#endif
