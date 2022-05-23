#ifndef _USER_CAN_H_
#define _USER_CAN_H_
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

extern CAN_HandleTypeDef hcan;

extern uint16_t vol;
extern float sensor_hum; //Êª¶È
extern float sensor_tem; //ÎÂ¶È
extern uint8_t  switching;
extern uint8_t Sensor_Type_t;	
extern uint8_t  flag_send_data;
extern uint8_t Can_data[8];

void CAN_User_Config(CAN_HandleTypeDef* hcan );
void can_start(void);
void can_stop(void);
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len);
uint8_t Can_Send_Msg_StdId(uint16_t My_StdId,uint8_t len,uint8_t Type_Sensor);


#endif //_USER_CAN_H_
