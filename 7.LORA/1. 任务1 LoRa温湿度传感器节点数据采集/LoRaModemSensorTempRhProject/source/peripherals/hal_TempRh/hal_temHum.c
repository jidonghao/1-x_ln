/**************************************************************************************************
  Filename:      hal_temhum.c

  Description:    ��ʪ�ȴ����� M9 ����
                  
**************************************************************************************************/

/******************************************************************************
 * INCLUDES
 */

#include "hal_temHum.h"

//-----------------�¶�ʪ��ģ��IO�˿ڶ���----------------  
#define HAL_TEMHUM_SCK 	PB_6
#define HAL_TEMHUM_SDA 	PB_7

Gpio_t HAL_TEMHUM_SCK_PIN;
Gpio_t HAL_TEMHUM_SDA_PIN;

//stm32 IO����
#define HAL_SHT_SCK_DIR_OUT()          GpioInit( &HAL_TEMHUM_SCK_PIN, HAL_TEMHUM_SCK, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );//��ʼ��IO�ڣ������1
#define HAL_SHT_SCK_SET()              GpioWrite( &HAL_TEMHUM_SCK_PIN, 1 )
#define HAL_SHT_SCK_CLR()              GpioWrite( &HAL_TEMHUM_SCK_PIN, 0 )

#define HAL_SHT_SDA_SET()              GpioWrite( &HAL_TEMHUM_SDA_PIN, 1 )
#define HAL_SHT_SDA_CLR()              GpioWrite( &HAL_TEMHUM_SDA_PIN, 0 )
#define HAL_SHT_SDA_DIR_OUT()          GpioInit( &HAL_TEMHUM_SDA_PIN, HAL_TEMHUM_SDA, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );//��ʼ��IO�ڣ������1
#define HAL_SHT_SDA_DIR_IN()           GpioInit( &HAL_TEMHUM_SDA_PIN, HAL_TEMHUM_SDA, PIN_INPUT, PIN_OPEN_DRAIN, PIN_PULL_UP, 1 );//��ʼ��IO�ڣ������1
#define HAL_SHT_SDA_VAL()              GpioRead( &HAL_TEMHUM_SDA_PIN ) 

#define MCU_SHT_SDA_OUTPUT(n)          GpioInit( &HAL_TEMHUM_SDA_PIN, HAL_TEMHUM_SDA, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, n );//��ʼ��IO�ڣ������1


#define ACK   1
#define noACK 0
#define measure_temp 0x03 //�����¶�����
#define measure_humi 0x05 //����ʪ������
#define RESET        0x1e //������

//------------------------���ݽṹ�嶨��---------------------------
typedef union                      //��������õ��¶ȣ�ʪ��ֵ
{ 
	uint16_t  i;
	float f;
} value;
//WENSHIDU S;






/******************************************************************************
 * LOCAL FUNCTIONS
 */

void SHT11_DELAY(uint16_t usec);
char write_byte(uint8_t value); 	  
char read_byte(uint8_t ack);		
void transstart(void);
char s_measure(uint8_t *p_value, uint8_t *p_checksum, uint8_t mode);
void calc_sth11(float *p_humidity ,float *p_temperature);


#define SENSOR_SHT10





#if defined SENSOR_SHT10

void SHT11_DELAY(uint16_t usec)
{
	uint16_t i=4;
	
	usec >>= 1;
	
	while (usec--)
	{
		while(i--)
		{
			;
		}
	}	
}

//��ʼ����ʪ��ģ��
void hal_temHumInit(void)
{ 
	uint16_t Tem,RH;
	
	//����HAL_TEMHUM_SCK��
	GpioInit( &HAL_TEMHUM_SCK_PIN, HAL_TEMHUM_SCK, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );//��ʼ��IO�ڣ������1
	//����HAL_TEMHUM_SDA��
	GpioInit( &HAL_TEMHUM_SDA_PIN, HAL_TEMHUM_SDA, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );//��ʼ��IO�ڣ������1
	//��һ�ζ���ʪ��
	call_sht11(&Tem, &RH);
}  


char write_byte(uint8_t value)
{ 
  	uint8_t  i,error=0;

	for (i=0x80;i>0;i/=2)           
	{ 
		if (i & value) 
			HAL_SHT_SDA_SET();           
		else 
			HAL_SHT_SDA_CLR();

		HAL_SHT_SCK_SET();               
		SHT11_DELAY(1);                    
		HAL_SHT_SCK_CLR();
	}
	
	HAL_SHT_SDA_SET();                 
	HAL_SHT_SDA_DIR_IN();
	HAL_SHT_SCK_SET();               
	error=HAL_SHT_SDA_VAL();         
	HAL_SHT_SCK_CLR();

	return error;                   
}

char read_byte(uint8_t ack)
{ 
	uint8_t i,val=0;

	HAL_SHT_SDA_SET();                
	HAL_SHT_SDA_DIR_IN();
	for (i=0x80;i>0;i/=2)            
	{ 
		HAL_SHT_SCK_SET();               
		if (HAL_SHT_SDA_VAL()) 
			val=(val | i);               
		HAL_SHT_SCK_CLR();
	}
	MCU_SHT_SDA_OUTPUT(!ack);
	HAL_SHT_SCK_SET();                            
	SHT11_DELAY(1);                             
	HAL_SHT_SCK_CLR();						    
	HAL_SHT_SDA_SET();                         

	return val;
}

void transstart(void)
{
	HAL_SHT_SCK_SET();
	SHT11_DELAY(1); 
	HAL_SHT_SDA_CLR();
	SHT11_DELAY(1);  
	HAL_SHT_SCK_CLR();
	SHT11_DELAY(2); 
	HAL_SHT_SCK_SET();
	SHT11_DELAY(1); 

	HAL_SHT_SDA_SET(); 
	SHT11_DELAY(1); 
	HAL_SHT_SCK_CLR();
	SHT11_DELAY(2); 
	HAL_SHT_SDA_CLR();	
}

void connectionreset(void)
{  
	uint8_t i; 
	HAL_SHT_SDA_SET(); 
	HAL_SHT_SCK_CLR();                   
	for(i=0;i<9;i++)                    
	{ 
		HAL_SHT_SCK_SET();
		HAL_SHT_SCK_CLR();
	}
	transstart();                   
}

char s_measure(uint8_t *p_value, uint8_t *p_checksum, uint8_t mode)
{ 
	uint8_t error=0;
	uint16_t i;

	connectionreset();
	switch(mode)
	{                    
		case 0	: error+=write_byte(measure_temp); break;
		case 1	: error+=write_byte(measure_humi); break;
		default     : break;	 
	}
	
	HAL_SHT_SDA_DIR_IN();
	for (i=0;i<6;i++)//�����i<x,x�ʹ������ٶ���أ�������ֵ��Ч����ȷ��
	{
		SHT11_DELAY(10); 
		if(HAL_SHT_SDA_VAL()== 0) break;
	}                               
	if(HAL_SHT_SDA_VAL())
	{
		error+=1;
	}
	
	*(p_value+1)  =read_byte(ACK);   
	*(p_value)=read_byte(ACK);       
	*p_checksum =read_byte(noACK);          
	return error;
}
/******************************************************************************
 * ����       calc_sth11
 * ����       ������ʪ��ֵ
 * ��ڲ���   float *p_humidity ,float *p_temperature
                ʪ��                  �¶�
 * ���ڲ���   ��
 *****************************************************************************/
void calc_sth11(float *p_humidity ,float *p_temperature)      
{ 
	const float C1=-4.0;              
	const float C2=+0.0405;         
	const float C3=-0.0000028;      
	const float T1=+0.01;             
	const float T2=+0.00008;           
	float rh=*p_humidity;            
	float t=*p_temperature;          
	float rh_lin;                     
	float rh_true;                    
	float t_C;                        
	t_C=t*0.01 - 40;                    
	rh_lin=C3*rh*rh + C2*rh + C1;       
	rh_true=(t_C-25)*(T1+T2*rh)+rh_lin; 
	if(rh_true>100)rh_true=100;         
	if(rh_true<0.1)rh_true=0.1;         
	*p_temperature=t_C;                 
	*p_humidity=rh_true;                
}

/******************************************************************************
 * ����       call_sht11
 * ����       ��ȡSH1x��ʪ��ֵ
 * ��ڲ���   void call_sht11(uint16_t *tem_val,uint16_t *hum_val)
            *tem_val:�¶ȴ�ŵ�ַ��0.1�ȷֱ���
            *hum_val:ʪ�ȴ�ŵ�ַ��0.1�ֱ���
 * ���ڲ���  ��  
 *****************************************************************************/
void call_sht11(uint16_t *tem_val,uint16_t *hum_val)
{ 
	value humi_val,temp_val;
	uint8_t error = 0,checksum;
	*tem_val=0;
	*hum_val=0;
	//����SH1x��ʪ�Ȳ���
	error+=s_measure((uint8_t*) &humi_val.i,&checksum,1); 
	//����SH1x���¶Ȳ���
	error+=s_measure((uint8_t*) &temp_val.i,&checksum,0);  
	if(error!=0)
	{   //���Դ��󣬽�����λ
	    connectionreset();                                       
	}
	else
	{   //�������ݴ���
        humi_val.f=(float)humi_val.i;                    
	    temp_val.f=(float)temp_val.i;
        //������ʪ��ʵ��ֵ
	    calc_sth11(&humi_val.f,&temp_val.f);
//        *tem_val=(uint16_t)temp_val.f;//��ǰ�¶�temp_val.f���϶�
//        *hum_val=(uint16_t)(humi_val.f);//ʪ������100����Ҳ���ǵ�ǰ���ʪ��Ϊ��*hum_val��%����λ��%��
        //��������
        *tem_val=(unsigned int)(((temp_val.f*10)+5)/10);
        *hum_val=(unsigned int)(((humi_val.f*10)+5)/10);
	}    
}

#endif

























