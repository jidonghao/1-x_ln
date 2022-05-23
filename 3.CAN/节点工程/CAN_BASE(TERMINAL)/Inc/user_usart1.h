#ifndef _USER_USART1_H_
#define _USER_USART1_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define SET_USART1_DBG //#define SET_USART2_DBG  //打开该宏定义重定向printf为USART1，

#define USART1_DATA_FRAME_NEXTLINE_CHAR	'\n'
#define USART1_DATA_FRAME_BACKSPACE_CHAR 0x0D //\'\r'

#define USART1_DATA_FIFO_BUF_SIZE		256 //该数必须为2的N次方，否者会出错 
#define USART1_DATA_FIFO_BUF_SIZE_MASK		(USART1_DATA_FIFO_BUF_SIZE-1)

extern UART_HandleTypeDef huart1;
extern uint8_t usart1RxBuf;

void open_usart1_receive_interrupt(void);

uint32_t usart1_data_fifo_len(void);
uint8_t usart1_data_fifo_is_empty(void);
uint8_t usart1_data_fifo_is_full(void);
void usart1_data_fifo_clean(void);
int is_has_usart1_data_frame(void);
int usart1_data_fifo_put(uint8_t usart1_data);
int usart1_data_fifo_get(uint8_t* usart1_data);
int usart1_data_fifo_write(uint8_t* usart1_datas, int len);
int usart1_data_fifo_read(uint8_t* usart1_datas, int len);

void USART1_SendData(uint8_t Data);
void USART1_SendStr(uint8_t *Data);
void USART1_SendArr(uint8_t *buf, uint8_t len	);


#endif  //_USER_USART1_H
