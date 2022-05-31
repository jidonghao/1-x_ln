/*************************************************************************************************
  Filename:       hal_temhum.h
 
 *****/


#ifndef _HAL_TEMHUM_H_
#define _HAL_TEMHUM_H_

#include "board.h"

void hal_temHumInit(void);
extern void call_sht11(uint16_t *tem, uint16_t *hum); 
extern void connectionreset(void);

#endif	/* _HAL_TEMHUM_H_ */
