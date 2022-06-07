#ifndef _APP_SENSOR_H_
#define _APP_SENSOR_H_

//#include "adc.h"
//#include "InfraredSensor.h"
//#include "temHum.h"
#include "..\..\Src\modbusmaster\protocol.h"

typedef enum sensor {
    None=0,

    // 1. 开关量传感器
    BodyInfrared_Sensor=1,   //人体红外传感器
    Sound_Sensor=2,					 //声音传感器
    Infrared_Sensor=3,			   //红外传感器
    // 2. 模拟量
    Photosensitive_Sensor=4, //光敏传感器
    Flame_Sensor=5,					 //火焰传感器
    FlammableGas_Sensor=6,	 //可燃气体传感器
    AirQuality_Sensor=7,		 //空气质量传感器
    //3. 数字量
    TemHum_Sensor=8,         //温湿度传感器
} SensorType;

#define FLASH_SAVE_SENSOR_STATE   ((uint32_t)0X0807E000)


void get_sensor(SensorType    type);

void getsensor_task(void);

void sensor_init(void);



#endif



