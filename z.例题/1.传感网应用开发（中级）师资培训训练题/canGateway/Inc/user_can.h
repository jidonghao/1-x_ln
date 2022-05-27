#ifndef _USER_CAN_H_
#define _USER_CAN_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/***Filter ID号的高低位**/
#define FilterIDH(x)   FilterID(1,x)
#define FilterIDL(x)   FilterID(0,x)
extern CAN_HandleTypeDef hcan;

extern uint16_t vol;
extern float sensor_hum; //湿度
extern float sensor_tem; //温度
extern uint8_t  switching;
extern uint8_t Sensor_Type_t;	
extern uint8_t  flag_send_data;
extern uint8_t Can_data[8];
unsigned short FilterID(uint8_t gaodiwei,uint32_t FID);
void CAN_User_Config(CAN_HandleTypeDef* hcan );
void can_start(void);
void can_stop(void);
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len);
uint8_t Can_Send_Msg_ID(uint8_t* msg,uint8_t stdid,uint8_t len);
uint8_t Can_Send_Msg_StdId(uint16_t My_StdId,uint8_t len,uint8_t Type_Sensor);


void CAN_User_Init(CAN_HandleTypeDef* hcan )  ;
#endif //_USER_CAN_H_
