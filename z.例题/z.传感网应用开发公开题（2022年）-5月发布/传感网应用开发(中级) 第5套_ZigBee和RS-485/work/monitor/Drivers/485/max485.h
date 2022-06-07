/******************************************************************************
******************************************************************************
* @file    driver/485/max485.h
* @author  zhao
* @version V1.0.0
* @date    2019.07.30
* @update  2019.07.30
* @brief   max485
******************************************************************************
******************************************************************************/

#ifndef __MAX485_H
#define __MAX485_H

#include "debug_printf.h"
#include "kfifo.h"
#include "main.h"
#include "stdio.h"
#include "sys.h"



extern uint8_t UART2_RX_TIMEOUT_COUNT;
extern uint8_t UART3_RX_TIMEOUT_COUNT;


#define RS485_TX_EN_PORT GPIOC
#define RS485_TX_EN_PIN GPIO_PIN_9

#define RS4852_TX_EN_PORT GPIOD
#define RS4852_TX_EN_PIN GPIO_PIN_11

#define RS4853_TX_EN_PORT GPIOD
#define RS4853_TX_EN_PIN GPIO_PIN_0


/*
*
//485 模块1 初始化 参数
*
*/



extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节

//模式控制
#define  RS485_TX_EN		PCout(9)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.

extern vu16 USART2_RX_STA;   						//接收数据状态

extern kfifo_t usart2_fifo;

void RS4851_Init(u32 bound);
void RS4851_Send_Buffer(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);
void RS485_SendStr(uint8_t *Data);



/*
*
//485 模块1 初始化 参数 结束
*
*/









/*
*
//485 模块2 初始化 参数
*
*/
extern u8 RS4852_RX_BUF[128]; 		//接收缓冲,最大64个字节

extern kfifo_t usart3_fifo;

//模式控制
#define RS4852_TX_EN		PDout(11)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART3_RX 	1			//0,不接收;1,接收.


extern vu16 USART3_RX_STA;   						//接收数据状态

void RS4852_Init(u32 bound);
void RS4852_Send_Buffer(u8 *buf,u8 len);
void RS4852_Receive_Data(u8 *buf,u8 *len);
void RS4852_SendStr(uint8_t *Data);


/*
*
//485 模块2 初始化 参数 结束
*
*/



/*
*
//485 模块1 初始化 参数
*
*/



extern u8 RS4853_RX_BUF[64]; 		//接收缓冲,最大64个字节

//模式控制
#define  RS4853_TX_EN		PDout(0)	//485模式控制.0,接收;1,发送

//如果想串口中断接收，请不要注释以下宏定义

#define EN_UART5_RX 	1			//0,不接收;1,接收.

extern vu16 UART5_RX_STA;   						//接收数据状态

extern kfifo_t uart5_fifo;

void RS4853_Init(u32 bound);
void RS4853_Send_Buffer(u8 *buf,u8 len);
void RS4853_Receive_Data(u8 *buf,u8 *len);
void RS4853_SendStr(uint8_t *Data);




#endif

