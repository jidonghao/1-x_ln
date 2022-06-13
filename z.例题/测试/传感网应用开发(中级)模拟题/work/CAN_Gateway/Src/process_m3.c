#include "process_m3.h"


#define START 0xDD  //起始位
#define CMD   0x01  //命令类型
#define LEN   0x09  //数据长度


#define GATEWAY_ADDR	0xaa
#define MASTER_TX_BUF_LEN 0x08
#define CMD_READ_SENSOR 0x01
#define CMD_CONTROL_RELAY 0x02

extern uint16_t vol;
extern float sensor_hum; //湿度
extern float sensor_tem; //温度
extern uint8_t switching;
extern uint8_t sensor_number;

#ifdef  ADDR_ONE
static uint8_t MASTER_TX_BUF[USART2_DATA_FIFO_BUF_SIZE]={0};
static uint8_t MASTER_DATA[USART2_DATA_FIFO_BUF_SIZE]={0};
#endif

static uint8_t Type_Sensor=0;
//static uint8_t Count_Sensor=0;
//static uint8_t Data_Sensor=0;
 

static uint8_t Relay1_Status=0;
static uint8_t Relay2_Status=0;	
static uint8_t Relay3_Status=0;


/*******************************************************************
*函数：uint8_t CHK(uint8_t *buf, uint8_t len)
*功能：计算累加和
*输入：uint8_t *buf ：需要累加的数
			 uint8_t len ：累加数的个数
*返回：累加和
*特殊说明：无
*******************************************************************/
uint8_t CHK(const uint8_t *buf, uint8_t len) //校验码
{
    uint8_t  RX_CHX=0;

    while(len--)
    {
        RX_CHX+= *buf;
        buf++;
    }

    return RX_CHX&=0xff;
}

/*******************************************************************
*函数：ValueType  ValueTypes(uint8_t sensor_type)
*功能：判断传感器值类型
*输入：uint8_t sensor_type:传感器类型
*返回：传感器值的类型
*特殊说明：无
*******************************************************************/
ValueType  ValueTypes(uint8_t sensor_type)
{
	ValueType ValueType_t;
	if(sensor_type==TemHum_Sensor)   //温湿度    心率
	{
		ValueType_t=Value_I2C;
	}
	else if((sensor_type==BodyInfrared_Sensor)||(sensor_type==Sound_Sensor)||(sensor_type==Infrared_Sensor))    //人体红外 声音  红外
	{
		ValueType_t=Value_Switch;
	}
	else
	{
		ValueType_t=Value_ADC;
	}
	return ValueType_t;
}

/*设备相关事项*/

#if defined (ADDR_TWO)|| defined (ADDR_THREE)

/*******************************************************************
*函数：Device_To_Master(uint8_t s_addr, uint8_t des_addr,uint8_t type_sensor,uint8_t count,uint8_t data_sensor)
*功能：节点向主控发送数据包
*输入：uint8_t s_addr ：源地址
			uint8_t des_addr ：目的地址
			uint8_t type_sensor ：传感器类型
			uint8_t count ：传感器个数，依据传感器种类决定，如采用温湿度传感器时，个数为2
			uint8_t data_sensor ：数据
*返回：无
*特殊说明：
*******************************************************************/
void Device_To_Master(uint8_t s_addr, uint8_t des_addr,uint8_t type_sensor,uint8_t count) //设备向主机发送协议
	
{
	uint8_t DEVICE_TX_BUF[USART2_REC_LEN]={0};
	uint8_t len=0;
	ValueType ValueType_t;
	
	DEVICE_TX_BUF[0]=START;
	DEVICE_TX_BUF[1]=s_addr;
	DEVICE_TX_BUF[2]=des_addr; 

	DEVICE_TX_BUF[4]=type_sensor;
	DEVICE_TX_BUF[5]=count;

	ValueType_t=ValueTypes(type_sensor);
	
	switch(ValueType_t)
	{
		case Value_ADC:
			sprintf((char *)(DEVICE_TX_BUF+6),"%d%c%d%c",vol/100,'.',vol%100/10,'v');			
			break;
		case Value_Switch:
			sprintf((char *)(DEVICE_TX_BUF+6),"%d",switching);
			break;
		case Value_I2C:
			sprintf((char *)(DEVICE_TX_BUF+6),"%c%d%c%c%c%d",'T',sensor_tem,',','R','H',sensor_hum);
			break;
		default:
			break;
	}
	len=strlen((const char *)(DEVICE_TX_BUF+6))+1+6;
	DEVICE_TX_BUF[3]=len;
	DEVICE_TX_BUF[len]=CHK(DEVICE_TX_BUF,len);

	
//	MAX3485_ConTx() ; 
//	USART2_SendArr(DEVICE_TX_BUF,len+1);
//	MAX3485_ConRx();
	
	USART1_SendArr(DEVICE_TX_BUF,len+1);
	
	
}

/*******************************************************************
*函数：Device_Deal_Master(void)
*功能：节点处理主控下发的数据包
*输入：无
*返回：累加和
*特殊说明：
*******************************************************************/
void Device_Deal_Master(void) //设备处理主机下发协议
{
	uint8_t DEVICE_RX_BUF[USART2_REC_LEN]={0};
	uint8_t len=0;
	uint8_t des_addr=0,s_addr=0;
	uint8_t cmd=0;
	
	memset(DEVICE_RX_BUF,0x00,USART2_REC_LEN);
	memcpy(DEVICE_RX_BUF,USART2_RX_BUF,USART2_REC_LEN);
	
	len=DEVICE_RX_BUF[3];
	des_addr=DEVICE_RX_BUF[2];
	cmd=DEVICE_RX_BUF[4];
	s_addr=DEVICE_RX_BUF[1];

	if(DEVICE_RX_BUF[0]==START)
	{
		if(DEVICE_RX_BUF[len]==CHK(DEVICE_RX_BUF,len))
		{
			if(des_addr==M3_Addr)
			{
				switch(cmd)
					{
						case CMD_READ_SENSOR:
							Device_To_Master(M3_Addr, s_addr,Sensor_Type_t,sensor_number);				  
							break;
						
						case CMD_CONTROL_RELAY:
							Relay1_Status=DEVICE_RX_BUF[5];
						  Relay2_Status=DEVICE_RX_BUF[6];
						  Relay3_Status=DEVICE_RX_BUF[7];
						
						  Relay1_Status?LED1_ON:LED1_OFF;
							Relay2_Status?LED2_ON:LED2_OFF;
						  Relay3_Status?LED3_ON:LED3_OFF;
						
							break;
						
						default:
							break;	
					}
			}

		}
	}
}

#endif

/*主机相关事项*/

#ifdef ADDR_ONE

/*******************************************************************
*函数：void Master_To_Device(uint8_t des_addr,uint8_t cmd,uint8_t relay1_status,uint8_t relay2_status,uint8_t relay3_status)
*功能：主控向节点发送数据包
*输入：uint8_t des_addr ：目的地址
			uint8_t cmd ：命令，1代表读取传感器数据；2代表控制节点继电器
			uint8_t relay1_status：继电器1状态（1代表开，0代表关）
      uint8_t relay2_status：继电器2状态（1代表开，0代表关）
			uint8_t relay3_status：继电器3状态（1代表开，0代表关）			
*返回：无
*特殊说明：
*******************************************************************/
void Master_To_Device(uint8_t des_addr,uint8_t cmd,uint8_t relay1_status,uint8_t relay2_status,uint8_t relay3_status) //主机向设备发送协议
{
	memset(MASTER_TX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
	
	MASTER_TX_BUF[0]=START;
	MASTER_TX_BUF[1]=M3_Addr_1;
	MASTER_TX_BUF[2]=des_addr;
	MASTER_TX_BUF[3]=MASTER_TX_BUF_LEN;
	MASTER_TX_BUF[4]=cmd;
	switch(cmd)
	{
		case CMD_READ_SENSOR:
			MASTER_TX_BUF[5]=0x00;
			MASTER_TX_BUF[6]=0x00;
			MASTER_TX_BUF[7]=0x00;
			break;
		
		case CMD_CONTROL_RELAY:
			MASTER_TX_BUF[5]=relay1_status;
			MASTER_TX_BUF[6]=relay2_status;
			MASTER_TX_BUF[7]=relay3_status;
		  Relay1_Status=MASTER_TX_BUF[5];
		  Relay2_Status=MASTER_TX_BUF[6];
		  Relay3_Status=MASTER_TX_BUF[7];
			break;
		
		default:
			return;		
	}
  MASTER_TX_BUF[8]=CHK(MASTER_TX_BUF,MASTER_TX_BUF_LEN);
	
//	MAX3485_ConTx() ; 
	USART2_SendArr(MASTER_TX_BUF,9);
//	MAX3485_ConRx();	
	
	memset(MASTER_TX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
}

/*******************************************************************
*函数：Master_Deal_Device(void)
*功能：主控处理节点上发的数据包
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
void Master_Deal_Device(void) //主机处理设备上发协议
{
	uint8_t MASTER_RX_BUF[USART2_DATA_FIFO_BUF_SIZE]={0};
	uint8_t USART2_RX_DATA[USART2_DATA_FIFO_BUF_SIZE] = {0};
	uint8_t len=0;
	uint8_t s_addr=0;
	
	memset(USART2_RX_DATA,0x00,USART2_DATA_FIFO_BUF_SIZE);
	int usart2_data_len = usart2_data_fifo_len();
	while(1)
	{
		if(usart2_data_fifo_is_empty()) break;
		usart2_data_fifo_read(USART2_RX_DATA, usart2_data_len);
		
	}
	
	
	memset(MASTER_RX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
	memcpy(MASTER_RX_BUF,USART2_RX_DATA, usart2_data_len);
	
	len=MASTER_RX_BUF[3];
	s_addr=MASTER_RX_BUF[2];
	
	
	if(MASTER_RX_BUF[0]==START)
	{
		if(MASTER_RX_BUF[len]==CHK(MASTER_RX_BUF,len))
		{
			if(s_addr!=GATEWAY_ADDR)
			{
				Type_Sensor=MASTER_RX_BUF[4];
//			Count_Sensor=MASTER_RX_BUF[5];
			strcpy((char *)MASTER_DATA,(const char *)(MASTER_RX_BUF+6));
			}
			else
			{
				return;
			}
			
		}
	}
	
}


/*******************************************************************
*函数：Master_Deal_Gateway(void)
*功能：主控处理网关下发的数据包
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
void Master_Deal_Gateway(void) //主机处理网关下发协议
{
	uint8_t MASTER_RX_BUF[USART2_DATA_FIFO_BUF_SIZE]={0};
	uint8_t USART2_RX_DATA[USART2_DATA_FIFO_BUF_SIZE] = {0};
	uint8_t len=0;
	uint8_t cmd=0;
	uint8_t d_addr=0;
	
	int usart2_data_len = usart2_data_fifo_len();
	memset(USART2_RX_DATA,0x00,USART2_DATA_FIFO_BUF_SIZE);
	while(1)
	{
		if(usart2_data_fifo_is_empty()) break;
		usart2_data_fifo_read(USART2_RX_DATA, usart2_data_len);
		
	}
	
	memset(MASTER_RX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
	memcpy(MASTER_RX_BUF,USART2_RX_DATA, usart2_data_len);
	
	len=MASTER_RX_BUF[3];
	d_addr=MASTER_RX_BUF[2];
	
	if(MASTER_RX_BUF[0]==START)
	{
		if(MASTER_RX_BUF[len]==CHK(MASTER_RX_BUF,len))
		{
			if(MASTER_RX_BUF[1]==GATEWAY_ADDR)
			{
				cmd=MASTER_RX_BUF[4];
				
				switch(cmd)
				{
					case CMD_READ_SENSOR:
						Master_To_Device(d_addr,CMD_READ_SENSOR,0,0,0);
						Master_Deal_Device();
//						Master_To_Gateway(d_addr);
					
						break;
					case CMD_CONTROL_RELAY:
						Relay1_Status=MASTER_RX_BUF[5];
					  Relay2_Status=MASTER_RX_BUF[6];
					  Relay3_Status=MASTER_RX_BUF[7];
						
						Master_To_Device(d_addr,CMD_CONTROL_RELAY,Relay1_Status,Relay2_Status,Relay3_Status);//主控向节点发送控制
						break;
					default:
						break;
					
				}
				
			}
		}
	}
	
}
// can 
/*******************************************************************
*函数：Master_To_Gateway(void)   
*功能：把板子自己的传感器数据上报至网关
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/

void Master_To_Gateway(uint16_t StdId,ValueType Value_Type,uint16_t vol,uint8_t  switching,uint16_t sensor_hum,uint16_t sensor_tem ) //CAN节点网关上发协议到网关 以便于在云平台展示
{

	memset(MASTER_TX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
	MASTER_TX_BUF[0]=START;
	MASTER_TX_BUF[1]=(uint8_t)StdId&0x00ff;    //取低两位做ID
	MASTER_TX_BUF[2]=StdId>>8;                 //高位在后
	MASTER_TX_BUF[3]=CMD;
	MASTER_TX_BUF[4]=LEN;
	MASTER_TX_BUF[5]=Sensor_Type_t;
	switch(Value_Type)
	{
		case Value_ADC:	
			
			break;
		case Value_Switch:
					MASTER_TX_BUF[6] = 0x00;
					MASTER_TX_BUF[7] = switching;
			break;
		case Value_I2C:
			    MASTER_TX_BUF[6]=sensor_tem;
					MASTER_TX_BUF[7]=sensor_hum;
//			sprintf((char *)(DEVICE_TX_BUF+6),"%c%d%c%c%c%d",'T',sensor_tem,',','R','H',sensor_hum);
			break;
		default:
			break;
	}
	MASTER_TX_BUF[8]=CHK(MASTER_TX_BUF,8);
	MAX3485_ConTx(); 
	USART2_SendArr(MASTER_TX_BUF,LEN);
	MAX3485_ConRx();
	printf("发送本机传感器数据至网关 MASTER_TX_BUF = %02X %02X %02X %02X %02X %02X %02X %02X %02X  \r\n",MASTER_TX_BUF[0],MASTER_TX_BUF[1],MASTER_TX_BUF[2],MASTER_TX_BUF[3],MASTER_TX_BUF[4],MASTER_TX_BUF[5],MASTER_TX_BUF[6],MASTER_TX_BUF[7],MASTER_TX_BUF[8]);
}
/*******************************************************************
*函数：Master_To_Gateway(void)   
*功能：把CAN总线上面的传感数据上报到网关
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
void CAN_Master_To_Gateway( uint8_t *can_data,uint8_t len)
{
	memset(MASTER_TX_BUF,0x00,USART2_DATA_FIFO_BUF_SIZE);
	MASTER_TX_BUF[0]=START;
	MASTER_TX_BUF[1]=can_data[3];
	MASTER_TX_BUF[2]=can_data[4];
	MASTER_TX_BUF[3]=CMD;
	MASTER_TX_BUF[4]=LEN;
	
	/*.......答题开始.......................................*/
  //1.根据协议将终端节点上的传感器类型和传感器数据正确封装到MASTER_TX_BUF[]数组中。
	
	
	
	MASTER_TX_BUF[5] = can_data[0];
	MASTER_TX_BUF[6] = can_data[1];
	MASTER_TX_BUF[7] = can_data[2];
	
	
	
	/*.......答题结束.......................................*/

//sprintf((char *)(DEVICE_TX_BUF+6),"%c%d%c%c%c%d",'T',sensor_tem,',','R','H',sensor_hum);
	MASTER_TX_BUF[8]=CHK(MASTER_TX_BUF,8);
	
	MAX3485_ConTx() ; 
	USART2_SendArr(MASTER_TX_BUF,LEN);
	MAX3485_ConRx();	
	printf("发送CAN节点数据至网关 MASTER_TX_BUF = %02X %02X %02X %02X %02X %02X %02X %02X %02X \r\n\r\n",MASTER_TX_BUF[0],MASTER_TX_BUF[1],MASTER_TX_BUF[2],MASTER_TX_BUF[3],MASTER_TX_BUF[4],MASTER_TX_BUF[5],MASTER_TX_BUF[6],MASTER_TX_BUF[7],MASTER_TX_BUF[8]);

}

#endif

