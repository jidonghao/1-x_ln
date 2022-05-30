#include "user_delay.h"

/***********************************************************************************************
 *Function: void delay_us(uint32_t nus)
 *Describe: ΢���ʱ
 *Input   ��nus : ��ʱʱ��
 *return  : NONE
 ***********************************************************************************************/
void delay_us(uint32_t nus)
{		
	uint32_t dwCurCounter=0;                                //��ǰʱ�����ֵ
	uint32_t dwPreTickVal=SysTick->VAL;                     //��һ��SYSTICK����ֵ
	uint32_t dwCurTickVal;                                  //��һ��SYSTICK����ֵ
	nus = nus*(HAL_RCC_GetHCLKFreq()/1000000);    //����ʱʱ�䣬������ʱ�����
	for(;;){
			dwCurTickVal=SysTick->VAL;
			if(dwCurTickVal<dwPreTickVal){
					dwCurCounter=dwCurCounter+dwPreTickVal-dwCurTickVal;
			}
			else{
					dwCurCounter=dwCurCounter+dwPreTickVal+SysTick->LOAD-dwCurTickVal;
			}
			dwPreTickVal=dwCurTickVal;
			if(dwCurCounter>=nus){
					return;
			}
	}
}

/***********************************************************************************************
 *Function: void delay_ms(uint16_t nms)
 *Describe: ���뼶��ʱ
 *Input   ��nms : ��ʱʱ��
 *return  : NONE
 ***********************************************************************************************/
void delay_ms(uint32_t nms)
{	 		  	  
	uint32_t dwCurCounter=0;                                //��ǰʱ�����ֵ
	uint32_t dwPreTickVal=SysTick->VAL;                     //��һ��SYSTICK����ֵ
	uint32_t dwCurTickVal;                                  //��һ��SYSTICK����ֵ
	nms = nms*(HAL_RCC_GetHCLKFreq()/1000);    //����ʱʱ�䣬������ʱ�����
	for(;;){
			dwCurTickVal=SysTick->VAL;
			if(dwCurTickVal<dwPreTickVal){
					dwCurCounter=dwCurCounter+dwPreTickVal-dwCurTickVal;
			}
			else{
					dwCurCounter=dwCurCounter+dwPreTickVal+SysTick->LOAD-dwCurTickVal;
			}
			dwPreTickVal=dwCurTickVal;
			if(dwCurCounter>=nms){
					return;
			}
	}
} 

