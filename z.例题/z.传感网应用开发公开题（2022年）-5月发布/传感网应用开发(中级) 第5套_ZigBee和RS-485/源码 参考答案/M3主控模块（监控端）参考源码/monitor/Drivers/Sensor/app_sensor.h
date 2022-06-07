#ifndef _APP_SENSOR_H_
#define _APP_SENSOR_H_

//#include "adc.h"
//#include "InfraredSensor.h"
//#include "temHum.h"
#include "..\..\Src\modbusmaster\protocol.h"

typedef enum sensor {
    None=0,

    // 1. ������������
    BodyInfrared_Sensor=1,   //������⴫����
    Sound_Sensor=2,					 //����������
    Infrared_Sensor=3,			   //���⴫����
    // 2. ģ����
    Photosensitive_Sensor=4, //����������
    Flame_Sensor=5,					 //���洫����
    FlammableGas_Sensor=6,	 //��ȼ���崫����
    AirQuality_Sensor=7,		 //��������������
    //3. ������
    TemHum_Sensor=8,         //��ʪ�ȴ�����
} SensorType;

#define FLASH_SAVE_SENSOR_STATE   ((uint32_t)0X0807E000)


void get_sensor(SensorType    type);

void getsensor_task(void);

void sensor_init(void);



#endif



