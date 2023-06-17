#include <xc.h>
#include "UART.h"
#include "config.h"
#include <stdio.h>

// Define maximum and minimum PWM frequency of the motor
#define PWM_FREQ_MAX 10000
#define PWM_FREQ_MIN 1500

// Define the step of the motor 
#define Step 1.8

// Define pins connect to the motor driver
#define DIR RD0
#define STEP RC2
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
volatile uint8_t mode = 0;
volatile uint8_t command;
volatile uint16_t motor_freq = 2000;

// Function to configure external and portB interrupt
void ExtInt_Init(){
    TRISB0 = 1;
    // Configure external interrupt
    INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
}

// Function to configure PWM mode for CCP1
void PWM_Init(void){
    // Configure RC2 as output pin
    TRISC2 = 0;
    // Configure PWM mode in CCPCON
    CCP1M3 = 1;
    CCP1M2 = 1;
}

// Function to configure Timer2 
void Speed_Control(uint16_t PWM_FREQ, uint8_t State){
    uint16_t dutyCycle;
    if(PWM_FREQ > PWM_FREQ_MAX || PWM_FREQ < PWM_FREQ_MIN)
    {
        UART_sendString("ERROR: invalid frequency!");
        return;
    }
    uint16_t PWM_RANGE = (_XTAL_FREQ/(16*PWM_FREQ)) & 0xFFFF;
    PR2 = ((PWM_RANGE/4) - 1) & 0xFF;
    // Use pre-scaler 1:16
    T2CKPS1 = 1;
    T2CKPS0 = 1;
    // Control the speed of the motor
    if(State == 0)
    {
        dutyCycle = 0;
    }
    else if(State == 1)
    {
        dutyCycle = PWM_RANGE;
    }
    // Control the duty cycle of the PWM module
    CCP1CONbits.CCP1Y = (dutyCycle) & 1;
    CCP1CONbits.CCP1X = (dutyCycle) & 2;
    // Move 8 MSB-bit to CCPRL
    CCPR1L = (dutyCycle) >> 2;
    // Activate Timer 2
    TMR2ON = 1;
}

// Interrupt handling service
void __interrupt() ISR(void)
{
    // External interrupt for braking the motor
    if(INTF){
        // De-bounce of button
        __delay_ms(20);
        if(RB0 == 0){
            EN = 0;
            Speed_Control(PWM_FREQ_MIN, 0);
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
        sprintf(rcvStr,"Received: [%c]\r\n",command);
        UART_sendString(rcvStr);
        // Change direction of the motor
        if(command == Direc)
        {
            DIR = ~DIR;
        }
        else if(command == Speed1)
        {
            motor_freq = 4000;
            UART_sendString("Speed 1 running");
        }
        else if(command == Speed2)
        {
            motor_freq = 6000;
            UART_sendString("Speed 2 running");
        }
        else if(command == Speed3)
        {
            motor_freq = 8000;
            UART_sendString("Speed 3 running");
        }
        else
        {
            mode = 1;
            //Command_Handling(command);
        }
    }
}

void main(void) 
{
    // System initialization
    ExtInt_Init();
    PWM_Init();
    UART_Init();
    // Turn on the motor running counterclockwise
    EN = 1;
    DIR = 1;
    // 
    while(1)
    {
        if(mode == 0)
        {
            Speed_Control(motor_freq, 1);
        }
    }
    return;
}
