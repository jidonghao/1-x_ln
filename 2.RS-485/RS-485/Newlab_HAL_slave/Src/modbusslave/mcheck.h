#ifndef __MCHECK_H
#define __MCHECK_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////


u8 mc_check_sum(u8* buf,u16 len);  	 	//��У�� �����ֽ�֮��Ϊ0
u8 mc_check_xor(u8* buf, u16 len);		//���У�飬�����ֽ����
u8 mc_check_crc8(u8 *buf,u16 len);		//CRC8У��
u16 mc_check_crc16(u8 *buf,u16 len);	//CRC16У��
#endif













