#include <xc.h>
#include "UART.h"
#include "config.h"
#include <stdio.h>

// Define the step of the motor 
#define Step 1.8

// Define pins connect to the motor driver
#define DIR RD0
#define STEP RD2
#define EN RD1

// Define command to control the position of the stepper motor
#define Direc 48
#define Speed1 49
#define Speed2 50
#define Speed3 51
#define Angle30 52
#define Angle45 53
#define Angle90 54
#define Angle180 55
#define Angle270 56
#define Angle360 57

// Declare global variables
char rcvStr[17];
volatile uint8_t command = 0;
volatile uint8_t speedmode = 0;
uint8_t mode = 0;

// Function to configure external and portB interrupt
void ExtInt_Init(){
    TRISB0 = 1;
    // Configure external interrupt
    INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
}

// Function to control the position of the motor
void Position_Control(uint8_t com)
{
    float degree = 0;
    // Get the degree of each command
    if(com == Angle30)
    {
        degree = 30;
    }else if(com == Angle45)
    {
        degree = 45;
    }else if(com == Angle90)
    {
        degree = 90;
    }else if(com == Angle180)
    {
        degree = 180;
    }else if(com == Angle270)
    {
        degree = 270;
    }else if(com == Angle360)
    {
        degree = 360;
    }else{
        UART_sendString("Invalid command\r\n");
        return;
    }
    // Get the number of step required to move to the desired position
    uint16_t step = (degree * 1.0)/Step;
    // Generate pulse
    for(uint16_t i = 0; i < step; ++i)
    {
        STEP = 1;
        __delay_us(500);
        STEP = 0;
        __delay_us(500);
    }
    UART_sendString("Completed\r\n");
}

// Function to control the speed of the motor
void Speed_Control(uint8_t com)
{
    // Run the motor at 500Hz
    if(com == 1)
    {
        STEP = 1;
        __delay_us(1000);
        STEP = 0;
        __delay_us(1000);
    }
    // Run the motor at 750Hz
    else if(com == 2)
    {
        STEP = 1;
        __delay_us(750);
        STEP = 0;
        __delay_us(750);
    }
    // Run the motor at 1000Hz
    else if(com == 3)
    {
        STEP = 1;
        __delay_us(500);
        STEP = 0;
        __delay_us(500);
    }
}

// Function to handle the command send by UART
void Command_Handling(uint8_t com)
{
    // Change direction of the motor
    if(command == Direc)
    {
        DIR = ~DIR;
    }
    // Speed 1 mode
    else if(command == Speed1)
    {
        mode = 0;
        speedmode = 1;
    }
    // Speed 2 mode
    else if(command == Speed2)
    {
        mode = 0;
        speedmode = 2;
    }
    // Speed 3 mode
    else if(command == Speed3)
    {
        mode = 0;
        speedmode = 3;
    }
    else
    {
        mode = 1;
        Position_Control(command);
    }
}

// Interrupt handling service
void __interrupt() ISR(void)
{
    // External interrupt for braking the motor
    if(INTF){
        // De-bounce of button
        __delay_ms(20);
        if(RB0 == 0){
            EN = 1;           
        }
        while(RB0 == 0);
        // Clear flag bit
        INTF = 0;
    }
    // UART interrupt
    if(RCIF){
        // Error handling
        if (OERR) 
        {
            CREN = 0; 
            CREN = 1;  
        }
        command = RCREG;
        sprintf(rcvStr,"Received: %c\r\n",command);
        UART_sendString(rcvStr);
        // Change direction of the motor
        Command_Handling(command);
    }
}

void main(void) 
{
    // GPIO initialization
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    // System initialization
    ExtInt_Init();
    UART_Init();
    __delay_ms(20);
    UART_sendString("System is ready\r\n");
    // Turn on the motor running counterclockwise
    EN = 0;
    DIR = 1;
    STEP = 0;
    // Main duty
    while(1)
    {
        if(mode == 0)
        {
            Speed_Control(speedmode);
        }
    }
    return;
}
