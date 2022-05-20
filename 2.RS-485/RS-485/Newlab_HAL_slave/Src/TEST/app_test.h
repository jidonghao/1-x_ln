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

#ifndef __APP_TEST_H__
#define __APP_TEST_H__

#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif



#include "..\drivers\485\max485.h"
#include "kfifo.h"
#include "debug_printf.h"


#include "..\modbusslave\protocol.h"
#include "..\malloc\malloc.h"
#include "..\..\Drivers\Sensor\app_sensor.h"
extern void RS_485test(u8 cmd)       ;
extern void modbus_slave_test(void)  ;
extern void shell_485_connect(void);
extern void shell_485_set_address(u16 address);
extern void shell_485_set_mode(u16 mode);
extern void  shell_485_set(u16 mode,u16 address);

extern void shell_485_check(void);





#ifdef __cplusplus
}
#endif


#endif


