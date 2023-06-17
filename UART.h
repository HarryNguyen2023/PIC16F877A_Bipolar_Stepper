// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>

#define _XTAL_FREQ 16000000
// Specify the baud rate of the UART 
#define Baud_rate 9600
// Initiate functions
void UART_Init(void);
void UART_sendChar(char data);
void UART_sendByte(uint8_t data);
void UART_sendString(char* str);
void UART_sendData(uint8_t* str, uint8_t len);

#endif	/* XC_HEADER_TEMPLATE_H */
 


