


#ifndef __INPUTREGISTER_H__

#define __INPUTREGISTER_H__

#include "..\..\drivers\485\max485.h"
#include "..\..\Inc\app_slave.h"

#include "mcheck.h"
#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif


extern void masterInputRegister(u8 ucSndAddr, u16 usRegAddr, u16 usNRegs);




#endif

#ifdef __cplusplus
}
#endif
