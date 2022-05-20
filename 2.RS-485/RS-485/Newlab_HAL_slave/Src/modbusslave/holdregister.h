#ifndef __HOLDREGISTER_H__

#define __HOLDREGISTER_H__

#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif

#include "..\..\drivers\485\max485.h"

#include "mcheck.h"
#include "app_slave.h"


u8 ReadHoldRegister(void);

u8 WriteHoldRegister(void);

extern u16 holdbuf[3];



#ifdef __cplusplus
}
#endif

#endif


