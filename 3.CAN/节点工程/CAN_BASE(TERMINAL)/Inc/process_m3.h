#ifndef	__PROCESS_M3_H
#define __PROCESS_M3_H

#include "user.h"

/*相关的模块要打开相应的宏定义*/
#define ADDR_ONE  0x01                 //代表CAN网关节点
//#define ADDR_TWO	0x02  
//#define ADDR_THREE	0x03 



enum des_addr
{
	M3_Addr_1=1,
	M3_Addr_2=2,
	M3_Addr_3=3
	
};

typedef enum sensor
{
	TemHum_Sensor=1,         //温湿度传感器
	BodyInfrared_Sensor=2,   //人体红外传感器
	Flame_Sensor=3,					 //火焰传感器
	FlammableGas_Sensor=4,	 //可燃气体传感器
	AirQuality_Sensor=5,		 //空气质量传感器
	Photosensitive_Sensor=6, //光敏传感器
	Sound_Sensor=7,					 //声音传感器
	Infrared_Sensor=8			   //红外传感器
	
}SensorType;

typedef enum ValueSensor
{
	Value_ADC,
	Value_Switch,
	Value_I2C
}ValueType;
	
/*******************************************************************
*函数：ValueType  ValueTypes(uint8_t sensor_type)
*功能：判断传感器值类型
*输入：uint8_t sensor_type:传感器类型
*返回：传感器值的类型
*特殊说明：无
*******************************************************************/
ValueType  ValueTypes(uint8_t sensor_type);

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
void Device_To_Master(uint8_t s_addr, uint8_t des_addr,uint8_t type_sensor,uint8_t count); //设备向主机发送协议

/*******************************************************************
*函数：Device_Deal_Master(void)
*功能：节点处理主控下发的数据包
*输入：无
*返回：累加和
*特殊说明：
*******************************************************************/
void Device_Deal_Master(void); //设备处理主机下发协议
#endif

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
void Master_To_Device(uint8_t des_addr,uint8_t cmd,uint8_t relay1_status,uint8_t relay2_status,uint8_t relay3_status); //主机向设备发送协议

/*******************************************************************
*函数：Master_Deal_Device(void)
*功能：主控处理节点上发的数据包
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
void Master_Deal_Device(void); //主机处理设备上发协议

/*******************************************************************
*函数：Master_To_Gateway(void)
*功能：主控上发数据包到网关
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
//void Master_To_Gateway(uint8_t s_addr); //主机向网关上发协议

void Master_To_Gateway(uint8_t StdId, ValueType Value_Type,uint16_t vol,uint8_t  switching,uint16_t sensor_hum,uint16_t sensor_tem );
void CAN_Master_To_Gateway( uint8_t *can_data,uint8_t len ); 
/*******************************************************************
*函数：Master_Deal_Gateway(void)
*功能：主控处理网关下发的数据包
*输入：无			
*返回：无
*特殊说明：
*******************************************************************/
void Master_Deal_Gateway(void); //主机处理网关下发协议

#endif 

#endif
