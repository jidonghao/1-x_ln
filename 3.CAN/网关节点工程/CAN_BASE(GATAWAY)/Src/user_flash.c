#include "user_flash.h"



/***********************************************************************************************
 *Function: void FLASH_ErasePage(uint32_t PageAddress)
 *Describe: ����ָ����ַ������
 *Input   ��PageAddress:������ַ
 *return  ��NONE
 ***********************************************************************************************/
void FLASH_ErasePage(uint32_t PageAddress)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0;
	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = PageAddress;
	EraseInitStruct.NbPages = 1;

  HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

}

/***********************************************************************************************
 *Function: uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
 *Describe: ��ȡָ����ַ�İ���(16λ����)
 *Input   ��faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
 *return  ����Ӧ����.
 ***********************************************************************************************/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(uint16_t*)faddr; 
}


/***********************************************************************************************
 *Function: void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite) 
 *Describe: ������д��
 *Input   ��WriteAddr:��ʼ��ַ
 *          pBuffer:����ָ��
 *          NumToWrite:����(16λ)��   
*return   : NONE
 ***********************************************************************************************/
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, WriteAddr, pBuffer[i]);
	  WriteAddr+=2;//��ַ����2.
	}  
} 

/***********************************************************************************************
 *Function: void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)  
 *Describe: ��ָ����ַ��ʼ����ָ�����ȵ�����
 *Input   ��WriteAddr:��ʼ��ַ
 *          pBuffer:����ָ��
 *          NumToWrite:����(16λ)��   
*return   : NONE
 ***********************************************************************************************/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

/***********************************************************************************************
 *Function: void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
 *Describe:��ָ����ַ��ʼд��ָ�����ȵ�����
 *Input   ��WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
 *          pBuffer:����ָ��
 *          NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
*return   : NONE
 ***********************************************************************************************/
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //������ַ
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	HAL_FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();//����
}


/***********************************************************************************************
 *Function: void Flash_Write(uint32_t WriteAddr,uint16_t WriteData)  
 *Describe:��ָ����ַ��ʼд��ָ�����ȵ�����
 *Input   ��WriteAddr:��ʼ��ַ
 *         riteData:Ҫд�������
 return   : NONE
 ***********************************************************************************************/
void Flash_Write(uint32_t WriteAddr,uint16_t WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}

