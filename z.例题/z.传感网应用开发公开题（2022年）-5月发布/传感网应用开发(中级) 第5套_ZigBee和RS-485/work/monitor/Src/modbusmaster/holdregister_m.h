#ifndef __HOLDREGISTER_H__

#define __HOLDREGISTER_H__

#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif

#include "..\..\drivers\485\max485.h"

#include "mcheck.h"
#include "..\..\Inc\app_slave.h"


extern void masterreadholdRegister(u8 ucSndAddr, u16 usRegAddr, u16 usNRegs);
extern void masterwriteholdRegister(u8 ucSndAddr, u16 usRegAddr, u16 regvalue);




#ifdef __cplusplus
}
#endif

#endif


