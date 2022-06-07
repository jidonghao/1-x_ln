#include "hal_defs.h"
#include "hal_led.h"
#include "TIMER.h"


uint8 SEND_DATA_FLAG = 0;
uint16 NUM = 0;

void Timer4_Init(void)
{
    // Set prescaler divider value to 128 (32M/128 = 250KHZ)
    T4CTL |= 0xE0;
    T4CTL &= ~(0x10); // Stop timer
    T4CTL &= ~(0x08); // 禁止溢出中断
    T4CTL |= 0x04;    //计数器清零
    T4IE = 0;         // Disable interrupt    
}

void Timer4_On(void)
{    
    T4CTL |= 0x08; //使能溢出中断
    T4CTL &= ~(0x03);//0x00-0xFF
    T4CTL |= 0x10; // Start timer
    T4IE = 1; // Enable interrupt
    SEND_DATA_FLAG = 0;
}

void Timer4_Off(void)
{
    T4CTL &= ~(0x08); // 禁止溢出中断
    T4CTL &= ~(0x10); // Stop timer
    T4IE = 0; // Disable interrupt
}

uint8 GetSendDataFlag(void)
{
    return SEND_DATA_FLAG;
}



HAL_ISR_FUNCTION(T4_ISR, T4_VECTOR)
{
    T4OVFIF = 0;
    T4IF = 0;
    
    /*完善代码：完成三秒的定时代码逻辑*/


   /*3秒定时代码结束*/
}