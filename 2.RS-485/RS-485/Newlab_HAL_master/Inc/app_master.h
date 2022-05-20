/******************************************************************************
* @file    app/app_test.h
* @author  zhao
* @version V1.0.0
* @date    2018.07.17
* @update  2018.07.17
* @brief   app core
******************************************************************************
******************************************************************************/
#ifndef __APP_MASTER_H__
#define __APP_MASTER_H__




#ifdef __cplusplus  /* C++支持*/
extern "C" {
#endif


#include "..\drivers\485\max485.h"
#include "kfifo.h"
#include "debug_printf.h"
#include "..\Src\modbusmaster\protocol.h"
#include "..\Src\malloc\malloc.h"
#include "..\drivers\sensor\app_sensor.h"


#define  class_num  2


__packed typedef struct {
    u8          add ;
    SensorType  senty;
    u16         value;					    //帧功能，0~255
} class_sensor;




extern class_sensor class_sen[class_num];


void master_get_slave(void);

void master_task(void);


 u8 unpack_readhold_reg(m_send_frame_typedef *tx,m_rev_frame_typedef *rx);
 u8 unpack_disc_reg(m_send_frame_typedef *tx,m_rev_frame_typedef *rx);
 u8 unpack_readinput_reg(m_send_frame_typedef *tx,m_rev_frame_typedef *rx);
 u8 unpack_writehold_reg(m_send_frame_typedef *tx,m_rev_frame_typedef *rx);




#ifdef __cplusplus
}
#endif



#endif

/***********include modbus protocol**********************/



