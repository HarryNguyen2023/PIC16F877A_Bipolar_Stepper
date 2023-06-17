#include "UART.h"

// Function to configure UART module
void UART_Init(void) 
{
    // Configure TX and RX pins
    TRISCbits.TRISC6 = 1; // TX Pin set as output
    TRISCbits.TRISC7 = 1; // RX Pin set as input
    // High speed baud rate
    SPBRG = ((_XTAL_FREQ / 16) / Baud_rate) - 1;
    BRGH = 1; 
    // Enable asynchronous mode
    SYNC = 0; 
    SPEN = 1; 
   // Enable UART receiving interrupt 
    RCIE = 1;    
    PEIE = 1;    
    GIE = 1; 
    // Enable transmission and receive
    TXEN = 1; 
    CREN = 1; 
    // 8-bit mode 
    TX9 = 0;
    RX9 = 0;
}

// Function to write data in TXREG register when the TSR register is empty
void UART_sendChar(char data) 
{
   while (!TXIF); 
   TXREG = data; // 
}

// Function to send a byte of data using UART
void UART_sendByte(uint8_t data) 
{
   while (!TXIF); 
   TXREG = data; // 
}

// Function to write string using UART
void UART_sendString(char* str) 
{
   while (*str) 
   {
       UART_sendChar(*str++);
   }
}

// Function to write array of data using UART
void UART_sendData(uint8_t* str, uint8_t len) 
{
   for(int i = 0; i < len; ++i) 
   {
       UART_sendByte(str[i]);
   }
}
