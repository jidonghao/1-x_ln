#include "user_adc.h"
#include <math.h>

/******************************************************************************
 * FunctionName : uint16_t Get_Voltage()
 * Description  : 获取ADC1_IN0(PA0)的电压值
 * Parameters   : none
 * Returns      : voltage
*******************************************************************************/
uint16_t Get_Voltage(void)
{
	uint16_t voltage;
	uint16_t adcx=0;

	
	//启动ADC
	HAL_ADC_Start(&hadc1);  
	
	//等待采集完成
	HAL_ADC_PollForConversion(&hadc1,10); 
	
	//获取ADC采集的数据
	adcx = HAL_ADC_GetValue(&hadc1);

	//将采集到的数据转换为电压值
	voltage=(adcx*330)/4096; 
	
	//停止ADC
	HAL_ADC_Stop(&hadc1); 
	
	return voltage;
}

