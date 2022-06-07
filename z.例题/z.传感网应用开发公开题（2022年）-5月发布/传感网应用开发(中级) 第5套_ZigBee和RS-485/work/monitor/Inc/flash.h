#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"



int32_t bsp_flash_write     (uint32_t addr,uint8_t* pbuf,int32_t len);
int32_t bsp_flash_read		  (uint32_t addr,uint8_t* p_dest,int32_t len);
int32_t bsp_flash_erase		  (uint32_t address,uint32_t len);


#ifdef __cplusplus
}
#endif

#endif /*__ pinoutConfig_H */

