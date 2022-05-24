#ifndef	__UPPER_COMPUTER_H
#define __UPPER_COMPUTER_H
#include "user.h"

#define FLASE_M3_ADDR 0x08030000
#define FLASH_Sensor_Type 0x08030100

extern uint8_t Sensor_Type_t;
extern uint16_t M3_Addr;
extern uint16_t Can_STD_ID;
void process_up(void);
#endif
