#ifndef __MCHECK_H
#define __MCHECK_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////


u8 mc_check_sum(u8* buf,u16 len);  	 	//和校验 所有字节之和为0
u8 mc_check_xor(u8* buf, u16 len);		//异或校验，所有字节异或
u8 mc_check_crc8(u8 *buf,u16 len);		//CRC8校验
u16 mc_check_crc16(u8 *buf,u16 len);	//CRC16校验
#endif













