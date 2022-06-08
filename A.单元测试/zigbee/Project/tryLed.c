#include "hal_defs.h"  
#include "hal_cc8051.h"  
#include "hal_int.h"  
#include "hal_mcu.h"  
#include "hal_board.h"  
#include "hal_led.h"  
#include "hal_rf.h"  
#include "basic_rf.h"  
#include "hal_uart.h"   
#include "hal_pwm.h"   
#include "UART_PRINT.h"  
#include <stdlib.h>  
#include <string.h>  
#include <stdio.h>   


#define LED0 3
#define LED1 4
#define LED2 1

uint32 rx_cnt = 0;

void setLedAll(uint8 a,uint8 b,uint8 c)
{
  if(a)
  {
    halLedSet(LED0);
  }
  else
  {
    halLedClear(LED0);
  }
  if (b)
  {
    halLedSet(LED1);
  }
  else
  {
    halLedClear(LED1);
  }
  if (c)
  {
    halLedSet(LED2);
  }
  else
  {
    halLedClear(LED2);
  }
}

void setLedWhile(uint32 ledFlag)
{

  switch(ledFlag%8)
  {
  case 0:
    setLedAll(0, 0, 0);
    break;
  case 1:
    setLedAll(1, 0, 0); 
    break;
  case 2:
    setLedAll(0, 1, 0); 
    break;
  case 3:
    setLedAll(1, 1, 0); 
    break;
  case 4:
    setLedAll(0, 0, 1); 
    break;
  case 5:
    setLedAll(1, 0, 1); 
    break;
  case 6:
    setLedAll(0, 1, 1); 
    break;
  case 7:
    setLedAll(1, 1, 1); 
    break;
  default:
    break;
  }
}





void main(void)
{
  halBoardInit();  //模块相关资源的初始化  
  uart_printf("开始了\r\n");
  while(1)
  {
       halMcuWaitMs(1000);  
   setLedWhile(rx_cnt++);
    
  }
}