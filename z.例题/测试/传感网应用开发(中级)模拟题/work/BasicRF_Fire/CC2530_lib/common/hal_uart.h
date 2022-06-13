//-------------------------------------------------------------------
// Filename: hal_uart.h
// Description: hal UART library header file
//-------------------------------------------------------------------
#ifndef _HAL_UART_H_
#define _HAL_UART_H_
//------------------------------------------------------------------

//-------------------------------------------------------------------
// Initialize UART at the startup
//-------------------------------------------------------------------
void halUartInit(uint32 baud);

//-------------------------------------------------------------------
// Read a buffer from the UART
//-------------------------------------------------------------------
extern uint16 halUartRead(uint8 *pBuffer, uint16 length);

//-------------------------------------------------------------------
// Write a buff to the uart 
//-------------------------------------------------------------------
extern uint16 halUartWrite(uint8 *pBuffer, uint16 length);

//-------------------------------------------------------------------
// Return the number of bytes in the Rx buffer
//-------------------------------------------------------------------
extern uint16 halUartRxLen(void);

//-------------------------------------------------------------------
// Abort UART when entering sleep mode
//-------------------------------------------------------------------
extern void halUartSuspend(void);

//-------------------------------------------------------------------
// Resume UART after wakeup from sleep
//-------------------------------------------------------------------
extern void halUartResume(void);

#endif
