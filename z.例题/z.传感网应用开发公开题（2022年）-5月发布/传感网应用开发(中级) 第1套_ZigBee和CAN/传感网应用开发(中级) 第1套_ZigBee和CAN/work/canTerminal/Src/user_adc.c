#include "user_adc.h"
#include <math.h>

/******************************************************************************
 * FunctionName : uint16_t Get_Voltage()
 * Description  : ��ȡADC1_IN0(PA0)�ĵ�ѹֵ
 * Parameters   : none
 * Returns      : voltage
*******************************************************************************/
uint16_t Get_Voltage(void)
{
	uint16_t voltage;
	uint16_t adcx=0;

	
	//����ADC
	HAL_ADC_Start(&hadc1);  
	
	//�ȴ��ɼ����
	HAL_ADC_PollForConversion(&hadc1,10); 
	
	//��ȡADC�ɼ�������
	adcx = HAL_ADC_GetValue(&hadc1);

	//���ɼ���������ת��Ϊ��ѹֵ
	voltage=(adcx*330)/4096; 
	
	//ֹͣADC
	HAL_ADC_Stop(&hadc1); 
	
	return voltage;
}

