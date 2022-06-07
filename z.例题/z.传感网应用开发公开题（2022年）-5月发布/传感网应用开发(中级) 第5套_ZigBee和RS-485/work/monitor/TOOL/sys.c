/******************************************************************************
* @ File name --> sys.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 10 - 20 - 2012
* @ Brief     --> 系统设置相关的函数
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************

******************************************************************************/

#include "sys.h"


/******************************************************************************
* Function Name --> 读取芯片闪存容量
* Description   --> none
* Input         --> none
* Output        --> *STMCapa：容量字符显示缓存
* Reaturn       --> none 
******************************************************************************/
void STM32_Flash_Capacity(uint8_t *STMCapa)
{
	uint16_t capa;
	
	capa = *((uint16_t*)0x1ffff7e0);	//读取闪存容量寄存器，低16位有效
	
	capa = ((capa >> 12) * 4096 + ((capa >> 8) & 0x0f) * 256 + ((capa >> 4) & 0x0f) * 16 + (capa & 0x0f));	//转换成十进制
	
	STMCapa[0] = 0x4d;	//M
	STMCapa[1] = 0x43;	//C
	STMCapa[2] = 0x55;	//U
	STMCapa[3] = 0x43;	//C
	STMCapa[4] = 0x61;	//a
	STMCapa[5] = 0x70;	//p
	STMCapa[6] = 0x3a;	//:
	
	if((capa / 1000) != 0)	STMCapa[7] = capa / 1000 + 48;	//千位不为0时显示
	else	STMCapa[7] = 0x20;
	
	STMCapa[8] = capa % 1000 / 100 + 48;	//百位
	STMCapa[9] = capa % 100 /10 + 48;		//十位
	STMCapa[10] = capa % 10 + 48;			//个位
	STMCapa[11] = 0x4b;	//K
	STMCapa[12] = 0x62;	//b
}
/******************************************************************************
* Function Name --> 读取CPUID
* Description   --> none
* Input         --> none
* Output        --> 存储CPUID缓存（字符串输出）
* Reaturn       --> none 
******************************************************************************/
void STM32_CPUID(uint8_t *IDbuff)
{
	uint32_t CPUID;
	CPUID = *((uint32_t*)0xe000ed00);
	sprintf((char*)IDbuff, "CPU ID:%08X", CPUID);
}



/******************************************************************************
* Function Name --> THUMB指令不支持汇编内联
* Description   --> 采用如下方法实现执行汇编指令WFI
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/ 
void WFI_SET(void)
{
	__ASM volatile("wfi");		 
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}


/******************************************************************************
* Function Name --> 系统软复位
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void SYS_SoftReset(void)
{
	SCB->AIRCR = 0x05fa0000 | 0x00000004;  //具体请参考《Cortex-M3权威指南(中文).pdf》第285页内容
}
/******************************************************************************
* Function Name --> JTAG模式设置
* Description   --> none
* Input         --> mode：模式参数（详细宏定义见sys.h中定义）
*                   000：JTAG-DP + SW-DP（复位状态）
*                   001：JTAG-DP + SW-DP（除了JNTRST引脚，释放JRST引脚）
*                   010：JTAG-DP接口禁止，SW-DP接口允许
*                   100：JTAG-DP接口和SW-DP接口都禁止
*                   xxx：其他值，禁止
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void STM_JTAG_Set(uint32_t mode)
{
	RCC->APB2ENR |= 1 << 0;		//使能辅助时钟
	AFIO->MAPR &= 0xf8ffffff;	//清除SWJ_CFG[2:0]位，即【26:24】
	AFIO->MAPR |= (mode << 24);	//设置JTAG模式
}

/******************************************************************************
* Function Name --> BCD码转为HEX
* Description   --> none
* Input         --> BCD_Data：要转换的BCD数据
* Output        --> none
* Reaturn       --> HEX码
******************************************************************************/
uint8_t BCD_to_HEX(uint8_t BCD_Data)
{
	return((BCD_Data / 10) << 4 | (BCD_Data % 10));
}
/******************************************************************************
* Function Name --> HEX码转为BCD
* Description   --> none
* Input         --> HEX_Data：要转换的BCD数据
* Output        --> none
* Reaturn       --> BCD码
******************************************************************************/
uint8_t HEX_to_BCD(uint8_t HEX_Data)
{
	return((HEX_Data >> 4) * 10 + (HEX_Data & 0x0f));
}
/**function: CharToHex()
*** ACSII change to 16 hex
*** input:ACSII
***Return :Hex
**/
unsigned char CharToHex(unsigned char bHex)
{
	
	if(bHex<=9)
	{
		bHex += 0x30;
	}
	else if((bHex>=10)&&(bHex<=15))//Capital
	{
		bHex += 0x37;
	}
	else 
	{
		bHex = 0xff;
	}
	return bHex;
}

unsigned char HexToChar(unsigned char bChar)
{
	if((bChar>=0x30)&&(bChar<=0x39))
	{
		bChar -= 0x30;
	}
	else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
	{
		bChar -= 0x37;
	}
	else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
	{
		bChar -= 0x57;
	}
	else 
	{
		bChar = 0xff;
	}
	return bChar;
}

/*
将srcBuff指向的len个长度的字符串数据，按十六进制字符编码转换为字符，并存于tarBuff中
*/
void HexStrToByte(unsigned char *tarBuff, const char *srcBuff, unsigned int len)
{
	char byte;
	
	if (len % 2 == 1)
		--len;
	
	while (len) {
		byte = *srcBuff>'9' ? (*srcBuff-'A'+10) : (*srcBuff-'0');
		++srcBuff;
		byte <<= 4;
		
		byte |= *srcBuff>'9' ? (*srcBuff-'A'+10) : (*srcBuff-'0');
		++srcBuff;
		
		*tarBuff++ = byte;
		len -= 2;
	}
}


/*********************************
HEX/BCD/BYTE/之间的转换
**********************************/
/*
将srcBuff指向的len个字节的数据，按十六进制字符编码转换，并存于tarBuff中
*/
void ByteToHexStr(char *tarBuff, const unsigned char *srcBuff, unsigned int len)
{
	char byte;
	
	while (len--) 
	{
		byte = *srcBuff >> 4;
		*tarBuff++ = (byte>9) ? (byte+'A'-10) : (byte+'0');
		
		byte = *srcBuff & 0x0F;
		*tarBuff++ = (byte>9) ? (byte+'A'-10) : (byte+'0');
		
		++srcBuff;
	}
	*tarBuff = 0;//空字符
}
 
/*
将无符号字符数据，转换为十六进制数据，并存于tarBuff中
*/
void BytetoBCD(unsigned char *tarBuff, const unsigned char *srcBuff, unsigned short len)
{
	while (len--) {
		*tarBuff++ = (((*srcBuff / 10) % 10) << 4) | (*srcBuff % 10);
		++srcBuff;
	}
}

/******************************************************************************
* Function Name --> 10进制码转为16进制
* Description   --> none
* Input         --> DX_Data：要转换的10进制数据
* Output        --> none
* Reaturn       --> 16进制
******************************************************************************/
uint16_t DX_to_HX(uint16_t DX_Data)
{
	return(((DX_Data/1000)<<12) | ((DX_Data%1000/100)<<8) | ((DX_Data%100/10)<<4) | (DX_Data%10));
}
/******************************************************************************
* Function Name --> 16进制码转为10进制
* Description   --> none
* Input         --> HX_Data：要转换的16进制数据
* Output        --> none
* Reaturn       --> 10进制
******************************************************************************/
uint16_t HX_to_DX(uint16_t HX_Data)
{
	return((HX_Data>>12)*1000+((HX_Data&0x0f00)>>8)*100+((HX_Data&0x00f0)>>4)*10+(HX_Data&0x000f));
}	









