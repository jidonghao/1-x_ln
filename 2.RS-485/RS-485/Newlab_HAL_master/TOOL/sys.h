/******************************************************************************
* @ File name --> sys.h
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 10 - 20 - 2012
* @ Brief     --> ϵͳ������صĺ���
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.0.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 03 - 09 - 2014
* @ Revise    --> ����GPIO���ó�ʼ����������������ֲʱֱ�Ӻ궨�弴��
*
* @ Version   --> V1.0.2
* @ Author    --> By@ Sam Chan
* @ Date      --> 05 - 10 - 2014
* @ Revise    --> ���Ӷ�C++����֧��
*
******************************************************************************/

#ifndef _sys_h_
#define _sys_h_

/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	extern "C"{
#endif
/*===========================================================================*/

/******************************************************************************
                                  �ⲿ����ͷ�ļ�                        
******************************************************************************/
	
#include "stm32f1xx.h"
#include <string.h>
#include <stdio.h>


/******************************************************************************
                           ����һЩ���õ��������Ͷ̹ؼ��� 
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
                               JTAGģʽ���ö���
******************************************************************************/

#define JTAG_SWD_Enable			0x00000000	//��λֵ
#define JNTRST_Disable			0x00000001	//JNTRST�����ͷ�
#define SWD_Enable				0x00000002	//JTAG�رգ�SWD����
#define JTAG_SWD_Disable		0x00000004	//JTAG��SWD���ر�

/******************************************************************************
                              λ��������غ궨��
                       �ο���CM3Ȩ��ָ�ϡ���87 ~ 92ҳ
******************************************************************************/

#define BITBAND(addr, bitnum)			((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)					*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)			MEM_ADDR(BITBAND(addr, bitnum)) 

/******************************************************************************
                                  GPIO��ַӳ��
                          ����ַ���ϼĴ���ƫ�Ƶ�ַ���
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
                       ʵ�ֵ�һIO������������51��IO����
                              nֵҪС��IO������Ŀ
******************************************************************************/ 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/******************************************************************************
                               �ⲿ���ù��ܺ���
******************************************************************************/

void STM32_Flash_Capacity(uint8_t *STMCapa);	//��ȡоƬ��������

void STM32_CPUID(uint8_t *IDbuff);	//��ȡCPUID



void SYS_SoftReset(void);	//ϵͳ��λ

void STM_JTAG_Set(uint32_t mode);	//JTAGģʽ����


uint8_t BCD_to_HEX(uint8_t BCD_Data);	//BCD��תΪHEX

uint8_t HEX_to_BCD(uint8_t HEX_Data);	//HEX��תΪBCD

uint16_t DX_to_HX(uint16_t DX_Data);	//10������תΪ16����

uint16_t HX_to_DX(uint16_t HX_Data);	//16������תΪ10����


void HexStrToByte(unsigned char *tarBuff, const char *srcBuff, unsigned int len);
void BytetoBCD(unsigned char *tarBuff, const unsigned char *srcBuff, unsigned short len);
void ByteToHexStr(char *tarBuff, const unsigned char *srcBuff, unsigned int len);


void INTX_DISABLE(void);
void WFI_SET(void);
void INTX_ENABLE(void);
/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	}
#endif
/*===========================================================================*/


#endif  /* end sys.h */





