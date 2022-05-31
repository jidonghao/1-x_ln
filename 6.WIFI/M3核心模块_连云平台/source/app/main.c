
#include "stm32f103_config.h"
#include "stm32App.h"

int main(void)
{	
	delay_init();
	delay_ms(100);//��ʱ100Ms��ϵͳ�ȶ�
	USART1_Init(BAUD_RATE);
	UART4_Init(BAUD_RATE);
	Buzzer_Init();//��LED_Init();IO�ڶ���PA8
	TIM2_Init();
	printf("Cortex-M3����WiFiͨѶģ��������ƽ̨\r\n");
	HalBuzzerBlink (HAL_BUZZER_0, 1, 50, 500);

	userApp();
}




