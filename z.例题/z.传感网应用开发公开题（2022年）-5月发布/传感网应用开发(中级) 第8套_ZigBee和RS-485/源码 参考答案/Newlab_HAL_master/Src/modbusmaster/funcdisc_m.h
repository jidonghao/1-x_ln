


#ifndef __FUNCDISC_H__

#define __FUNCDISC_H__


#ifdef __cplusplus  /* C++Ö§³Ö*/
extern "C" {
#endif


#include "..\..\drivers\485\max485.h"
#include "..\..\Inc\app_slave.h"

#include "mcheck.h"

typedef union {

    struct {

        unsigned disc_BIT0 : 1; //
        unsigned disc_BIT1 : 1; //
        unsigned disc_BIT2 : 1;
//        unsigned disc_BIT3 : 1;
//        unsigned disc_BIT4 : 1;
//        unsigned disc_BIT5 : 1;

    } val;
    u8 VAL;
} __funcdisc_states;

extern __funcdisc_states mb_funcdisc_states;




u8 ReadDiscRegister(void);

#ifdef __cplusplus
}
#endif

#endif


