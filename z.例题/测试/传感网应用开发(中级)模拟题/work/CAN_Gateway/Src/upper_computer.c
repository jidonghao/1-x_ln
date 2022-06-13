#include "upper_computer.h"

#define CONNECT_START 0x05 
#define SET_START 0x55
#define READ_START 0xaa

#define BYTE_LEN 0x08

#define SENSOR_TYPE 0x01
#define NET_TYPE 0x02


uint8_t Sensor_Type_t=0;
uint16_t M3_Addr=0;

uint8_t connect_buf[BYTE_LEN]={CONNECT_START,0x00,0x00,0x00,0x00,0x00,0x00,0x05};
/**********************************************************************************************
*函数：uint8_t *ExtractCmdframe(uint8_t *buf, uint8_t len, uint8_t CmdStart)
*功能：从一串数据中提取出命令帧出现起始地址
*输入：
*     uint8_t *buf，指向待提取的数据缓冲区地址
*     uint8_t len，缓冲区数据长度
*     uint8_t CmdStart，命令帧起始字节
*输出：无
*返回：返回首次出现命令帧的地址，若数据中无命令帧数据，则返回NULL
*特殊说明：无
**********************************************************************************************/
uint8_t *ExtractCmdframe(uint8_t *buf, uint8_t len, uint8_t CmdStart)
{
    uint8_t *point = NULL;
    uint8_t i;
    for(i=0; i<len; i++)
    {
        if(CmdStart == *buf)
        {
            point = buf;
            return point;
        }
    }
    return NULL;
}

uint8_t CHK_XOR(const uint8_t *buf, uint8_t len) //校验码
{
    uint8_t  RX_CHX=0x00;

    while(len--)
    {
        RX_CHX^= *buf;
        buf++;
    }
    return RX_CHX;
}


void process_up(void)
{
	uint8_t process_rx[USART1_DATA_FIFO_BUF_SIZE]={0};
	uint8_t USART1_RX_DATA[USART1_DATA_FIFO_BUF_SIZE] = {0};
	uint8_t reply[BYTE_LEN]={0};

	int len;
	
	memset(USART1_RX_DATA,0x00,USART1_DATA_FIFO_BUF_SIZE);
	
	len = usart1_data_fifo_len();
	while(1)
	{
		if(usart1_data_fifo_is_empty()) break;
		usart1_data_fifo_read(USART1_RX_DATA, len);
	}

	memset(process_rx,0x00,USART1_DATA_FIFO_BUF_SIZE);
	memcpy(process_rx,USART1_RX_DATA,len);
	memset(reply,0x00,BYTE_LEN);

	
	switch(process_rx[0])
	{
		case CONNECT_START:
			if(process_rx[BYTE_LEN-1]==CHK_XOR(process_rx,BYTE_LEN-1))
			{
				USART1_SendArr(connect_buf,BYTE_LEN);
			}
			break;
		case SET_START:
			if(process_rx[BYTE_LEN-1]==CHK_XOR(process_rx,BYTE_LEN-1))
			{
				Sensor_Type_t=process_rx[1];
				Can_STD_ID=process_rx[2]<<8;
				Can_STD_ID|=process_rx[3];
				reply[0]=SET_START;  //0x55
				reply[1]=Sensor_Type_t; //01
				reply[2]=Can_STD_ID>>8; //05
				reply[3]=Can_STD_ID;    //00
				reply[4]=0x00;
				reply[5]=0x00;
				reply[6]=0x00;
				reply[7]=CHK_XOR(reply,7);
				USART1_SendArr(reply,BYTE_LEN);
				Flash_Write(FLASE_M3_ADDR,Can_STD_ID);
				Flash_Write(FLASH_Sensor_Type,Sensor_Type_t);

			}
			break;
		case READ_START:
			if(process_rx[BYTE_LEN-1]==CHK_XOR(process_rx,BYTE_LEN-1))
			{				
				reply[0]=READ_START;
				reply[1]=Sensor_Type_t;
				reply[2]=Can_STD_ID>>8;
				reply[3]=Can_STD_ID;				
				reply[4]=0x00;
				reply[5]=0x00;
				reply[6]=0x00;
				reply[7]=CHK_XOR(reply,7);
				USART1_SendArr(reply,BYTE_LEN);
				
			}
			break;
		default:
			return;
	}
}
