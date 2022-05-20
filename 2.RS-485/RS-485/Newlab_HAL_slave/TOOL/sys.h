/******************************************************************************
* @ File name --> sys.h
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 10 - 20 - 2012
* @ Brief     --> 系统设置相关的函数
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.0.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 03 - 09 - 2014
* @ Revise    --> 增加GPIO共用初始化函数，方便在移植时直接宏定义即可
*
* @ Version   --> V1.0.2
* @ Author    --> By@ Sam Chan
* @ Date      --> 05 - 10 - 2014
* @ Revise    --> 增加对C++环境支持
*
******************************************************************************/

#ifndef _sys_h_
#define _sys_h_

/*===========================================================================*/
#ifdef __cplusplus  /* C++支持 */
	extern "C"{
#endif
/*===========================================================================*/

/******************************************************************************
                                  外部函数头文件                        
******************************************************************************/
	
#include "stm32f1xx.h"
#include <string.h>
#include <stdio.h>


/******************************************************************************
                           定义一些常用的数据类型短关键字 
******************************************************************************/
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;  

/******************************************************************************
                               JTAG模式设置定义
******************************************************************************/

#define JTAG_SWD_Enable			0x00000000	//复位值
#define JNTRST_Disable			0x00000001	//JNTRST引脚释放
#define SWD_Enable				0x00000002	//JTAG关闭，SWD开启
#define JTAG_SWD_Disable		0x00000004	//JTAG和SWD都关闭

/******************************************************************************
                              位带操作相关宏定义
                       参考《CM3权威指南》第87 ~ 92页
******************************************************************************/

#define BITBAND(addr, bitnum)			((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)					*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)			MEM_ADDR(BITBAND(addr, bitnum)) 

/******************************************************************************
                                  GPIO地址映射
                          基地址加上寄存器偏移地址组成
******************************************************************************/

#define GPIOA_ODR_Addr    (GPIOA_BASE+12)	//0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12)	//0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12)	//0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12)	//0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12)	//0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12)	//0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12)	//0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8)	//0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)	//0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)	//0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)	//0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)	//0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)	//0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)	//0x40011E08 

/******************************************************************************
                       实现单一IO操作，类似于51的IO操作
                              n值要小于IO具体数目
******************************************************************************/ 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/******************************************************************************
                               外部调用功能函数
******************************************************************************/

void STM32_Flash_Capacity(uint8_t *STMCapa);	//读取芯片闪存容量

void STM32_CPUID(uint8_t *IDbuff);	//读取CPUID



void SYS_SoftReset(void);	//系统软复位

void STM_JTAG_Set(uint32_t mode);	//JTAG模式设置


uint8_t BCD_to_HEX(uint8_t BCD_Data);	//BCD码转为HEX

uint8_t HEX_to_BCD(uint8_t HEX_Data);	//HEX码转为BCD

uint16_t DX_to_HX(uint16_t DX_Data);	//10进制码转为16进制

uint16_t HX_to_DX(uint16_t HX_Data);	//16进制码转为10进制


void HexStrToByte(unsigned char *tarBuff, const char *srcBuff, unsigned int len);
void BytetoBCD(unsigned char *tarBuff, const unsigned char *srcBuff, unsigned short len);
void ByteToHexStr(char *tarBuff, const unsigned char *srcBuff, unsigned int len);


void INTX_DISABLE(void);
void WFI_SET(void);
void INTX_ENABLE(void);
/*===========================================================================*/
#ifdef __cplusplus  /* C++支持 */
	}
#endif
/*===========================================================================*/


#endif  /* end sys.h */





