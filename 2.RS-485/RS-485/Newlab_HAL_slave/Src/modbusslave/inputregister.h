


#ifndef __INPUTREGISTER_H__

#define __INPUTREGISTER_H__

#include "..\..\drivers\485\max485.h"
#include "app_slave.h"

#include "mcheck.h"
#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif


u8 ReadInputRegister(void);


extern u16 inbuf[4];



#endif

#ifdef __cplusplus
}
#endif
