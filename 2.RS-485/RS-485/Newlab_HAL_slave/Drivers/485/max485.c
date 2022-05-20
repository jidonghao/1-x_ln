/******************************************************************************
******************************************************************************
* @file    driver/485/max485.c
* @author  zhao
* @version V1.0.0
* @date    2019.07.30
* @update  2019.07.30
* @brief   max485 源文件
******************************************************************************
******************************************************************************/
#include "max485.h"
#include "usart.h"

uint8_t UART2_RX_TIMEOUT_COUNT=0;
uint8_t UART3_RX_TIMEOUT_COUNT=0;
uint8_t UART5_RX_TIMEOUT_COUNT=0;

/**********485 模块代码1**************/

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void RS4851_Init(u32 bound)
{

    MX_USART2_UART_Init(bound);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                            PEPin PEPin PEPin PEPin */
    GPIO_InitStruct.Pin = RS485_TX_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(RS485_TX_EN_PORT, &GPIO_InitStruct);


    HAL_GPIO_WritePin(RS485_TX_EN_PORT, RS485_TX_EN_PIN, GPIO_PIN_RESET);


}



//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS4851_Send_Buffer(u8 *buf,u8 len)
{

    HAL_GPIO_WritePin(RS485_TX_EN_PORT, RS485_TX_EN_PIN, GPIO_PIN_SET);

    HAL_UART_Transmit(&huart2,buf,len,0xFFFF);

    HAL_Delay(10);

    HAL_GPIO_WritePin(RS485_TX_EN_PORT, RS485_TX_EN_PIN, GPIO_PIN_RESET);

}


/******************************************************************************
* Function Name --> 串口4发送一个字节数据
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound：波特率
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void UART2_Send_Data(u8 Data)
{
    HAL_UART_Transmit(&huart2,&Data,1,0xFFFF);
}

/******************************************************************************
* Function Name --> USART2 485 发送字符串数据
* Description   --> none
* Input         --> *Data:字符串
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void RS485_SendStr(uint8_t *Data)
{

    HAL_GPIO_WritePin(RS485_TX_EN_PORT, RS485_TX_EN_PIN, GPIO_PIN_SET);

    while(*Data!='\0') {
        UART2_Send_Data((uint16_t )(*Data));
        Data++;
    }

    HAL_Delay(10);

    HAL_GPIO_WritePin(RS485_TX_EN_PORT, RS485_TX_EN_PIN, GPIO_PIN_RESET);

}


/*
  * @param   EN_USART2_RX 使能 USART2 接收模块
  * @param   USART2_RX_STA  接收中断标志位
  * @param   usart2_fifo 接收fifo
  * @data:   2019.7.30
  * @author: zhao(359044538@qq.com)
*/

#ifdef EN_USART2_RX   	//如果使能了接收

vu16 USART2_RX_STA;


//接收缓存区
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节

//接收fifo
kfifo_t usart2_fifo = {"485fifo1",64,RS485_RX_BUF,0,0};

//接收到的数据长度
//u8 RS485_RX_CNT=0;

void USART2_IRQHandler(void)
{

    u8 res;
    /* USER CODE BEGIN USART2_IRQn 0 */
    if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET)) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        USART2_RX_STA=1;
        UART2_RX_TIMEOUT_COUNT=0;

        HAL_UART_Receive(&huart2,&res,1,1000);
        if(m_ctrl_dev.frameok==0) {	//接收未完成
            m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
            m_ctrl_dev.rxlen++;
            if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1)) {
                m_ctrl_dev.rxlen=0;    //接收数据错误,重新开始接收
            }
        }

        kfifo_push_in(&usart2_fifo,&res,1);
    }

    /* USER CODE END USART2_IRQn 0 */

    /* USER CODE BEGIN USART2_IRQn 1 */
    HAL_UART_IRQHandler(&huart2);

    /* USER CODE END USART2_IRQn 1 */
}




#endif
/**********485 模块代码2      end**************/
vu16 USART3_RX_STA;


//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void RS4852_Init(u32 bound)
{
    /*

    */
    MX_USART3_UART_Init(bound);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                            PEPin PEPin PEPin PEPin */
    GPIO_InitStruct.Pin = RS4852_TX_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS4852_TX_EN_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(RS4852_TX_EN_PORT, RS4852_TX_EN_PIN, GPIO_PIN_RESET);

//    RS4852_TX_EN=0;			//默认为接收模式


}



//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS4852_Send_Buffer(u8 *buf,u8 len)
{


//    RS4852_TX_EN=1;			//设置为发送模式
    HAL_GPIO_WritePin(RS4852_TX_EN_PORT, RS4852_TX_EN_PIN, GPIO_PIN_SET);
    HAL_UART_Transmit(&huart3,buf,len,0xFFFF);
    HAL_Delay(10);
    HAL_GPIO_WritePin(RS4852_TX_EN_PORT, RS4852_TX_EN_PIN, GPIO_PIN_RESET);
//    RS4852_TX_EN=0;				//设置为接收模式
}


/******************************************************************************
* Function Name --> 串口4发送一个字节数据
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound：波特率
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void UART3_Send_Data(u8 Data)
{
    HAL_UART_Transmit(&huart3,&Data,1,0xFFFF);
}

/******************************************************************************
* Function Name --> USART2 485 发送字符串数据
* Description   --> none
* Input         --> *Data:字符串
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void RS4852_SendStr(uint8_t *Data)
{
//  RS4852_TX_EN=1;			//设置为发送模式
    HAL_GPIO_WritePin(RS4852_TX_EN_PORT, RS4852_TX_EN_PIN, GPIO_PIN_SET);
    while(*Data!='\0') {
        UART3_Send_Data(*Data);
        Data++;
    }
    HAL_Delay(10);
//    RS4852_TX_EN=0;				//设置为接收模式
    HAL_GPIO_WritePin(RS4852_TX_EN_PORT, RS4852_TX_EN_PIN, GPIO_PIN_RESET);

}


#ifdef EN_USART3_RX   	//如果使能了接收



/*
  * @param   EN_USART3_RX 使能 USART3 接收模块
  * @param   USART3_RX_STA  UART3接收中断标志位
  * @param   usart3_fifo  UART3接收fifo
  * @data:   2019.7.30
  * @author: zhao(359044538@qq.com)
*/

//接收缓存区
u8 RS4852_RX_BUF[128];  	//接收缓冲,最大64个字节.

//接收fifo
kfifo_t usart3_fifo = {"485fifo2",128,RS4852_RX_BUF,0,0};




//接收到的数据长度


void USART3_IRQHandler(void)
{

    u8 res;
    /* USER CODE BEGIN USART3_IRQn 0 */
    if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE)!=RESET)) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        USART3_RX_STA=1;
        UART3_RX_TIMEOUT_COUNT=0;

        HAL_UART_Receive(&huart3,&res,1,1000);
        if(m_ctrl_dev.frameok==0) {	//接收未完成
            m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
            m_ctrl_dev.rxlen++;
            if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1)) {
                m_ctrl_dev.rxlen=0;    //接收数据错误,重新开始接收
            }
        }

        kfifo_push_in(&usart3_fifo,&res,1);
    }

    /* USER CODE END USART3_IRQn 0 */

    /* USER CODE BEGIN USART3_IRQn 1 */
    HAL_UART_IRQHandler(&huart3);

    /* USER CODE END USART3_IRQn 1 */
}


#endif

vu16 UART5_RX_STA;


//初始化IO 串口5
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void RS4853_Init(u32 bound)
{
    /*

     */
    MX_UART5_Init(bound);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin
    					 PEPin PEPin PEPin PEPin */
    GPIO_InitStruct.Pin = RS4853_TX_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS4853_TX_EN_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(RS4853_TX_EN_PORT, RS4853_TX_EN_PIN, GPIO_PIN_RESET);

//    RS4853_TX_EN=0;		 //默认为接收模式
}




//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS4853_Send_Buffer(u8 *buf,u8 len)
{

//    RS4853_TX_EN=1;			//设置为发送模式
    HAL_GPIO_WritePin(RS4853_TX_EN_PORT, RS4853_TX_EN_PIN, GPIO_PIN_SET);
    HAL_UART_Transmit(&huart5,buf,len,0xFFFF);

    HAL_Delay(10);
    HAL_GPIO_WritePin(RS4853_TX_EN_PORT, RS4853_TX_EN_PIN, GPIO_PIN_RESET);
//    RS4853_TX_EN=0;				//设置为接收模式

}

/******************************************************************************
* Function Name --> 串口4发送一个字节数据
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound：波特率
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void UART5_Send_Data(u8 Data)
{
    HAL_UART_Transmit(&huart5,&Data,1,0xFFFF);
}

/******************************************************************************
* Function Name --> USART2 485 发送字符串数据
* Description   --> none
* Input         --> *Data:字符串
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void RS4853_SendStr(uint8_t *Data)
{
//    RS4853_TX_EN=1;			//设置为发送模式
    HAL_GPIO_WritePin(RS4853_TX_EN_PORT, RS4853_TX_EN_PIN, GPIO_PIN_SET);

    while(*Data!='\0') {
        UART5_Send_Data(*Data);
        Data++;
    }
    HAL_Delay(10);
    HAL_GPIO_WritePin(RS4853_TX_EN_PORT, RS4853_TX_EN_PIN, GPIO_PIN_RESET);

//    RS4853_TX_EN=0;				//设置为接收模式
}






#ifdef EN_UART5_RX   	//如果使能了接收


/*
  * @param   EN_USART3_RX 使能 USART3 接收模块
  * @param   USART3_RX_STA  UART3接收中断标志位
  * @param   usart3_fifo  UART3接收fifo
  * @data:   2019.7.30
  * @author: zhao(359044538@qq.com)
*/

//接收缓存区
u8 RS4853_RX_BUF[64];  	//接收缓冲,最大64个字节.

//接收fifo
kfifo_t uart5_fifo = {"485fifo2",64,RS4852_RX_BUF,0,0};




//接收到的数据长度


void UART5_IRQHandler(void)
{

    u8 res;
    /* USER CODE BEGIN USART3_IRQn 0 */
    if((__HAL_UART_GET_FLAG(&huart5,UART_FLAG_RXNE)!=RESET)) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        UART5_RX_STA=1;
        UART5_RX_TIMEOUT_COUNT=0;

        HAL_UART_Receive(&huart5,&res,1,1000);
        if(m_ctrl_dev.frameok==0) {	//接收未完成
            m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
            m_ctrl_dev.rxlen++;
            if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1)) {
                m_ctrl_dev.rxlen=0;    //接收数据错误,重新开始接收
            }
        }

        kfifo_push_in(&uart5_fifo,&res,1);
    }

    /* USER CODE END USART3_IRQn 0 */

    /* USER CODE BEGIN USART3_IRQn 1 */
    HAL_UART_IRQHandler(&huart5);

    /* USER CODE END USART3_IRQn 1 */
}


#endif







