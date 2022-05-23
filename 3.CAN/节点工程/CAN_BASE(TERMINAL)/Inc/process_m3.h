#ifndef	__PROCESS_M3_H
#define __PROCESS_M3_H

#include "user.h"

/*��ص�ģ��Ҫ����Ӧ�ĺ궨��*/
#define ADDR_ONE  0x01                 //����CAN���ؽڵ�
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
	TemHum_Sensor=1,         //��ʪ�ȴ�����
	BodyInfrared_Sensor=2,   //������⴫����
	Flame_Sensor=3,					 //���洫����
	FlammableGas_Sensor=4,	 //��ȼ���崫����
	AirQuality_Sensor=5,		 //��������������
	Photosensitive_Sensor=6, //����������
	Sound_Sensor=7,					 //����������
	Infrared_Sensor=8			   //���⴫����
	
}SensorType;

typedef enum ValueSensor
{
	Value_ADC,
	Value_Switch,
	Value_I2C
}ValueType;
	
/*******************************************************************
*������ValueType  ValueTypes(uint8_t sensor_type)
*���ܣ��жϴ�����ֵ����
*���룺uint8_t sensor_type:����������
*���أ�������ֵ������
*����˵������
*******************************************************************/
ValueType  ValueTypes(uint8_t sensor_type);

#if defined (ADDR_TWO)|| defined (ADDR_THREE)

/*******************************************************************
*������Device_To_Master(uint8_t s_addr, uint8_t des_addr,uint8_t type_sensor,uint8_t count,uint8_t data_sensor)
*���ܣ��ڵ������ط������ݰ�
*���룺uint8_t s_addr ��Դ��ַ
			uint8_t des_addr ��Ŀ�ĵ�ַ
			uint8_t type_sensor ������������
			uint8_t count �����������������ݴ���������������������ʪ�ȴ�����ʱ������Ϊ2
			uint8_t data_sensor ������
*���أ���
*����˵����
*******************************************************************/
void Device_To_Master(uint8_t s_addr, uint8_t des_addr,uint8_t type_sensor,uint8_t count); //�豸����������Э��

/*******************************************************************
*������Device_Deal_Master(void)
*���ܣ��ڵ㴦�������·������ݰ�
*���룺��
*���أ��ۼӺ�
*����˵����
*******************************************************************/
void Device_Deal_Master(void); //�豸���������·�Э��
#endif

#ifdef ADDR_ONE

/*******************************************************************
*������void Master_To_Device(uint8_t des_addr,uint8_t cmd,uint8_t relay1_status,uint8_t relay2_status,uint8_t relay3_status)
*���ܣ�������ڵ㷢�����ݰ�
*���룺uint8_t des_addr ��Ŀ�ĵ�ַ
			uint8_t cmd �����1�����ȡ���������ݣ�2������ƽڵ�̵���
			uint8_t relay1_status���̵���1״̬��1������0����أ�
      uint8_t relay2_status���̵���2״̬��1������0����أ�
			uint8_t relay3_status���̵���3״̬��1������0����أ�			
*���أ���
*����˵����
*******************************************************************/
void Master_To_Device(uint8_t des_addr,uint8_t cmd,uint8_t relay1_status,uint8_t relay2_status,uint8_t relay3_status); //�������豸����Э��

/*******************************************************************
*������Master_Deal_Device(void)
*���ܣ����ش���ڵ��Ϸ������ݰ�
*���룺��			
*���أ���
*����˵����
*******************************************************************/
void Master_Deal_Device(void); //���������豸�Ϸ�Э��

/*******************************************************************
*������Master_To_Gateway(void)
*���ܣ������Ϸ����ݰ�������
*���룺��			
*���أ���
*����˵����
*******************************************************************/
//void Master_To_Gateway(uint8_t s_addr); //�����������Ϸ�Э��

void Master_To_Gateway(uint8_t StdId, ValueType Value_Type,uint16_t vol,uint8_t  switching,uint16_t sensor_hum,uint16_t sensor_tem );
void CAN_Master_To_Gateway( uint8_t *can_data,uint8_t len ); 
/*******************************************************************
*������Master_Deal_Gateway(void)
*���ܣ����ش��������·������ݰ�
*���룺��			
*���أ���
*����˵����
*******************************************************************/
void Master_Deal_Gateway(void); //�������������·�Э��

#endif 

#endif
