/******************************************************************************
******************************************************************************
* @file    app/app_test.h
* @author  zhao
* @version V1.0.0
* @date    2018.07.17
* @update  2018.07.17
* @brief   app core
******************************************************************************
******************************************************************************/

#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif

#ifndef __APP_SLAVE_H__
#define __APP_SLAVE_H__

/***********include modbus protocol**********************/

//#include "bsp_timer3.h"

#include "..\drivers\485\max485.h"
#include "kfifo.h"
#include "debug_printf.h"
#include "..\Src\modbusmaster\protocol.h"
#include "..\Src\malloc\malloc.h"


#include "..\Drivers\sensor\app_sensor.h"


#define FLASH_SAVE_ADDRESS   ((uint32_t)0X0807E800)
#define FLASH_SAVE_FIRST  ((uint32_t)0X0807F000)

extern void app_modbus_slave(void);
extern void app_modbus_init(void);

extern u8 change_address_state (u8* add_value);
extern u8  SLAVE_ADDRESS;



#endif


#ifdef __cplusplus
}
#endif

