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


#include "..\modbusmaster\protocol.h"
#include "..\malloc\malloc.h"

extern void RS_485test(u8 cmd)       ;

extern void modbus_master_rec(void);
extern void master_send_test(u8 cmd);




#ifdef __cplusplus
}
#endif


#endif


