#include "user_delay.h"

/***********************************************************************************************
 *Function: void delay_us(uint32_t nus)
 *Describe: 微妙级延时
 *Input   ：nus : 延时时间
 *return  : NONE
 ***********************************************************************************************/
void delay_us(uint32_t nus)
{		
	uint32_t dwCurCounter=0;                                //当前时间计数值
	uint32_t dwPreTickVal=SysTick->VAL;                     //上一次SYSTICK计数值
	uint32_t dwCurTickVal;                                  //上一次SYSTICK计数值
	nus = nus*(HAL_RCC_GetHCLKFreq()/1000000);    //需延时时间，共多少时间节拍
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
 *Describe: 毫秒级延时
 *Input   ：nms : 延时时间
 *return  : NONE
 ***********************************************************************************************/
void delay_ms(uint32_t nms)
{	 		  	  
	uint32_t dwCurCounter=0;                                //当前时间计数值
	uint32_t dwPreTickVal=SysTick->VAL;                     //上一次SYSTICK计数值
	uint32_t dwCurTickVal;                                  //上一次SYSTICK计数值
	nms = nms*(HAL_RCC_GetHCLKFreq()/1000);    //需延时时间，共多少时间节拍
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

