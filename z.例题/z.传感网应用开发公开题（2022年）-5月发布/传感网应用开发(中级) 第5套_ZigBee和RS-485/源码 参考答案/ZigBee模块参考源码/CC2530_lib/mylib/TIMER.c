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
    /****代码完善：完成定时两秒的代码******/
    /****对相应的计数单位进行累加并计算得到时间   **/
    /*对相应的标志位进行置位，使得main中可以参照进行相应功能开发*/
    NUM ++;
    if(NUM == 3000) //定时3s，翻转一次（1/250000*256）这是进一次中断所需要的时间，进3000次中断大约是3s
    {
        NUM = 0;
        SEND_DATA_FLAG = 1;
        //halLedToggle(1);
        Timer4_Off();
    }  
    else
    {
        SEND_DATA_FLAG = 0; 
    }
    /**********定时两秒代码结束**********/
}