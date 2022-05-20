/******************************************************************************
* @ File name --> sys.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 10 - 20 - 2012
* @ Brief     --> ϵͳ������صĺ���
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************

******************************************************************************/

#include "sys.h"


/******************************************************************************
* Function Name --> ��ȡоƬ��������
* Description   --> none
* Input         --> none
* Output        --> *STMCapa�������ַ���ʾ����
* Reaturn       --> none 
******************************************************************************/
void STM32_Flash_Capacity(uint8_t *STMCapa)
{
	uint16_t capa;
	
	capa = *((uint16_t*)0x1ffff7e0);	//��ȡ���������Ĵ�������16λ��Ч
	
	capa = ((capa >> 12) * 4096 + ((capa >> 8) & 0x0f) * 256 + ((capa >> 4) & 0x0f) * 16 + (capa & 0x0f));	//ת����ʮ����
	
	STMCapa[0] = 0x4d;	//M
	STMCapa[1] = 0x43;	//C
	STMCapa[2] = 0x55;	//U
	STMCapa[3] = 0x43;	//C
	STMCapa[4] = 0x61;	//a
	STMCapa[5] = 0x70;	//p
	STMCapa[6] = 0x3a;	//:
	
	if((capa / 1000) != 0)	STMCapa[7] = capa / 1000 + 48;	//ǧλ��Ϊ0ʱ��ʾ
	else	STMCapa[7] = 0x20;
	
	STMCapa[8] = capa % 1000 / 100 + 48;	//��λ
	STMCapa[9] = capa % 100 /10 + 48;		//ʮλ
	STMCapa[10] = capa % 10 + 48;			//��λ
	STMCapa[11] = 0x4b;	//K
	STMCapa[12] = 0x62;	//b
}
/******************************************************************************
* Function Name --> ��ȡCPUID
* Description   --> none
* Input         --> none
* Output        --> �洢CPUID���棨�ַ��������
* Reaturn       --> none 
******************************************************************************/
void STM32_CPUID(uint8_t *IDbuff)
{
	uint32_t CPUID;
	CPUID = *((uint32_t*)0xe000ed00);
	sprintf((char*)IDbuff, "CPU ID:%08X", CPUID);
}



/******************************************************************************
* Function Name --> THUMBָ�֧�ֻ������
* Description   --> �������·���ʵ��ִ�л��ָ��WFI
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/ 
void WFI_SET(void)
{
	__ASM volatile("wfi");		 
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}


/******************************************************************************
* Function Name --> ϵͳ��λ
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void SYS_SoftReset(void)
{
	SCB->AIRCR = 0x05fa0000 | 0x00000004;  //������ο���Cortex-M3Ȩ��ָ��(����).pdf����285ҳ����
}
/******************************************************************************
* Function Name --> JTAGģʽ����
* Description   --> none
* Input         --> mode��ģʽ��������ϸ�궨���sys.h�ж��壩
*                   000��JTAG-DP + SW-DP����λ״̬��
*                   001��JTAG-DP + SW-DP������JNTRST���ţ��ͷ�JRST���ţ�
*                   010��JTAG-DP�ӿڽ�ֹ��SW-DP�ӿ�����
*                   100��JTAG-DP�ӿں�SW-DP�ӿڶ���ֹ
*                   xxx������ֵ����ֹ
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void STM_JTAG_Set(uint32_t mode)
{
	RCC->APB2ENR |= 1 << 0;		//ʹ�ܸ���ʱ��
	AFIO->MAPR &= 0xf8ffffff;	//���SWJ_CFG[2:0]λ������26:24��
	AFIO->MAPR |= (mode << 24);	//����JTAGģʽ
}

/******************************************************************************
* Function Name --> BCD��תΪHEX
* Description   --> none
* Input         --> BCD_Data��Ҫת����BCD����
* Output        --> none
* Reaturn       --> HEX��
******************************************************************************/
uint8_t BCD_to_HEX(uint8_t BCD_Data)
{
	return((BCD_Data / 10) << 4 | (BCD_Data % 10));
}
/******************************************************************************
* Function Name --> HEX��תΪBCD
* Description   --> none
* Input         --> HEX_Data��Ҫת����BCD����
* Output        --> none
* Reaturn       --> BCD��
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
��srcBuffָ���len�����ȵ��ַ������ݣ���ʮ�������ַ�����ת��Ϊ�ַ���������tarBuff��
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
HEX/BCD/BYTE/֮���ת��
**********************************/
/*
��srcBuffָ���len���ֽڵ����ݣ���ʮ�������ַ�����ת����������tarBuff��
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
	*tarBuff = 0;//���ַ�
}
 
/*
���޷����ַ����ݣ�ת��Ϊʮ���������ݣ�������tarBuff��
*/
void BytetoBCD(unsigned char *tarBuff, const unsigned char *srcBuff, unsigned short len)
{
	while (len--) {
		*tarBuff++ = (((*srcBuff / 10) % 10) << 4) | (*srcBuff % 10);
		++srcBuff;
	}
}

/******************************************************************************
* Function Name --> 10������תΪ16����
* Description   --> none
* Input         --> DX_Data��Ҫת����10��������
* Output        --> none
* Reaturn       --> 16����
******************************************************************************/
uint16_t DX_to_HX(uint16_t DX_Data)
{
	return(((DX_Data/1000)<<12) | ((DX_Data%1000/100)<<8) | ((DX_Data%100/10)<<4) | (DX_Data%10));
}
/******************************************************************************
* Function Name --> 16������תΪ10����
* Description   --> none
* Input         --> HX_Data��Ҫת����16��������
* Output        --> none
* Reaturn       --> 10����
******************************************************************************/
uint16_t HX_to_DX(uint16_t HX_Data)
{
	return((HX_Data>>12)*1000+((HX_Data&0x0f00)>>8)*100+((HX_Data&0x00f0)>>4)*10+(HX_Data&0x000f));
}	









