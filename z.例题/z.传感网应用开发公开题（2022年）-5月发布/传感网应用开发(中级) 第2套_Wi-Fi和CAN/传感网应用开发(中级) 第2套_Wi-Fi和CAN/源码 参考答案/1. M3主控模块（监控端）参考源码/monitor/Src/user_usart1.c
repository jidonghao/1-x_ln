#include "user_usart1.h"



/****************************************************************
 *需要定义的全局变量
 ****************************************************************/
uint8_t usart1_data_fifo_buf[USART1_DATA_FIFO_BUF_SIZE];  

unsigned int usart1_data_fifo_buf_in = 0;
unsigned int usart1_data_fifo_buf_out = 0;
volatile uint8_t usart1_data_frame_put_index = 0;
volatile uint8_t usart1_data_frame_get_index = 0;
uint8_t usart1RxBuf;


/****************************************************************
 *Function: int fputc(int ch, FILE *f)
 *Describe: 用于实现USART1 printf()输出
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/


/****************************************************************
 *Function: void open_usart1_receive_interrupt(void)
 *Describe: 启动串口1接收中断
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/
void open_usart1_receive_interrupt(void)
{
	HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1);
}

/****************************************************************
 *Function: unsigned int usart1_data_rx_fifo_len(void)
 *Describe: 获取数据缓存区已存储的数据量
 *Input   ：NONE
 *return  ：返回数据缓存区已存储的数据量
 ****************************************************************/
unsigned int usart1_data_fifo_len(void)
{
	return (usart1_data_fifo_buf_in - usart1_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart1_data_fifo_is_empty(void)
 *Describe: 判断数据缓存区是否为空
 *Input   ：NONE
 *return  ：有数据：0   无数据：1
 ****************************************************************/
uint8_t usart1_data_fifo_is_empty(void)
{
	return (usart1_data_fifo_buf_in == usart1_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart1_data_fifo_is_full(void)
 *Describe: 判断数据缓存区是否溢出
 *Input   ：NONE
 *return  ：没溢出：0   溢出：1
 ****************************************************************/
uint8_t usart1_data_fifo_is_full(void)
{
	return (usart1_data_fifo_len() > USART1_DATA_FIFO_BUF_SIZE_MASK);
}

/****************************************************************
 *Function: void usart1_data_fifo_clean(void)
 *Describe: 清空数据缓存区
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/
void usart1_data_fifo_clean(void)
{
	if(!usart1_data_fifo_is_empty())
		usart1_data_fifo_buf_out = usart1_data_fifo_buf_in;
}

/****************************************************************
 *Function: int is_has_usart1_data_frame(void)
 *Describe: 判断数据缓存区是否有帧数据
 *Input   ：NONE
 *return  ：有：1   无：0
 ****************************************************************/
int is_has_usart1_data_frame(void)
{
	return (usart1_data_frame_put_index != usart1_data_frame_get_index);
}

/****************************************************************
 *Function: int usart1_data_fifo_put(uint8_t usart1_data)
 *Describe: 向数据缓存区写入一字节数据
 *Input   ：usart1_data(要写入的一字节数据)
 *return  ：成功：0 失败：-1
 ****************************************************************/
int usart1_data_fifo_put(uint8_t usart1_data)
{
	if(usart1_data_fifo_is_full()){
		return -1;
	}else{
		//printf("index:%d, usart1_data:%d\n", usart1_data_fifo_buf_in & USART1_DATA_FIFO_BUF_SIZE_MASK, usart1_data);
		usart1_data_fifo_buf[usart1_data_fifo_buf_in & USART1_DATA_FIFO_BUF_SIZE_MASK] = usart1_data;
		usart1_data_fifo_buf_in++;

		if(usart1_data == USART1_DATA_FRAME_BACKSPACE_CHAR){
			usart1_data_frame_put_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart1_data_fifo_get(uint8_t* usart1_data)
 *Describe: 从数据缓存区读出一字节数据
 *Input   ：usart1_data(读出的一字节数据的存储地址)
 *return  ：成功：0 失败：-1
 ****************************************************************/
int usart1_data_fifo_get(uint8_t* usart1_data)
{
	if(usart1_data_fifo_is_empty()){
		return -1;
	}else{
		*usart1_data = usart1_data_fifo_buf[usart1_data_fifo_buf_out & USART1_DATA_FIFO_BUF_SIZE_MASK];
		usart1_data_fifo_buf_out++;
		if(*usart1_data == USART1_DATA_FRAME_BACKSPACE_CHAR){
			usart1_data_frame_get_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart1_data_fifo_write(uint8_t* usart1_datas, int len)
 *Describe: 向数据缓存区写入len字节数据
 *Input   ：usart1_data(数据的首地址) len(数据的长度)
 *return  ：写入数据的长度
 ****************************************************************/
 int usart1_data_fifo_write(uint8_t* usart1_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart1_data_fifo_put(usart1_datas[i]) == -1)
			break;
	}
	return i;
}

/****************************************************************
 *Function: int usart1_data_fifo_read(uint8_t* usart1_datas, int len)
 *Describe: 从数据缓存区读出len字节数据
 *Input   ：usart1_data(数据存储的首地址) len(数据的长度)
 *return  ：读出数据的长度
 ****************************************************************/
int usart1_data_fifo_read(uint8_t* usart1_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart1_data_fifo_get(&usart1_datas[i]) == -1)
			break;
	}
	
	return i;
}


/******************************************************************************
* Function Name --> 串口1发送一个字节数据
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound：波特率	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART1_SendData(uint8_t Data)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&Data, 1, 10);
}

/******************************************************************************
* Function Name --> 串口1发送一串数据或数组
* Description   --> none
* Input         --> *Data:字符串	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART1_SendStr(uint8_t *Data)
{ 
	
	while(*Data!='\0')
	{
		USART1_SendData(*Data);
		Data++;
		
	}
}
void USART1_SendArr(uint8_t *buf, uint8_t len	)
{
	HAL_UART_Transmit(&huart1, buf, len, 100*len);	
}

