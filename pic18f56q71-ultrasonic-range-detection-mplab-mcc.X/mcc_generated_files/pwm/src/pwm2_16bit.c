/**
  * PWM2_16BIT Generated Driver File.
  *
  * @file pwm2_16bit.c
  *
  * @ingroup pwm2_16bit
  *
  * @brief This file contains the API implementation for the PWM2_16BIT module.
  *
  * version PWM2_16BIT Driver Version 1.0.1
*/

/*
� [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

/**
 * @section: Included Files
 */

#include <xc.h>
#include "../pwm2_16bit.h"

//Pointers to PWM2_16BIT interrupt handlers
//User can use them in application code to initialize with custom ISRs
static void (*PWM_CONTROL_Slice1Output1_InterruptHandler)(void);   //SaP1IF and hence PWMxIF is set
static void (*PWM_CONTROL_Slice1Output2_InterruptHandler)(void);   //SaP2IF and hence PWMxIF is set
static void (*PWM_CONTROL_Period_InterruptHandler)(void);          //PWMxPIF is set
static void PWM_CONTROL_Slice1Output1_DefaultInterruptHandler(void);
static void PWM_CONTROL_Slice1Output2_DefaultInterruptHandler(void);
static void PWM_CONTROL_Period_DefaultInterruptHandler(void);

void PWM_CONTROL_Initialize(void)
{
    //PWMERS External Reset Disabled; 
    PWM2ERS = 0x0;

    //PWMCLK LFINTOSC; 
    PWM2CLK = 0x4;

    //PWMLDS Autoload disabled; 
    PWM2LDS = 0x0;

    //PWMPRL 107; 
    PWM2PRL = 0x6B;

    //PWMPRH 2; 
    PWM2PRH = 0x2;

    //PWMCPRE No prescale; 
    PWM2CPRE = 0x0;

    //PWMPIPOS No postscale; 
    PWM2PIPOS = 0x0;

    //PWMS1P1IF PWM1 output match did not occur; PWMS1P2IF PWM2 output match did not occur; 
    PWM2GIR = 0x0;

    //PWMS1P1IE disabled; PWMS1P2IE disabled; 
    PWM2GIE = 0x0;

    //PWMPOL1 disabled; PWMPOL2 enabled; PWMPPEN disabled; PWMMODE Left aligned mode; 
    PWM2S1CFG = 0x80;

    //PWMS1P1L 9; 
    PWM2S1P1L = 0x9;

    //PWMS1P1H 0; 
    PWM2S1P1H = 0x0;

    //PWMS1P2L 12; 
    PWM2S1P2L = 0xC;

    //PWMS1P2H 0; 
    PWM2S1P2H = 0x0;
    
    //Clear PWM2_16BIT period interrupt flag
    PIR5bits.PWM2PIF = 0;
    
    //Clear PWM2_16BIT interrupt flag
    PIR5bits.PWM2IF = 0;
    
    //Clear PWM2_16BIT slice 1, output 1 interrupt flag
    PWM2GIRbits.S1P1IF = 0;
    
    //Clear PWM2_16BIT slice 1, output 2 interrupt flag
    PWM2GIRbits.S1P2IF = 0;
    
    //PWM2_16BIT interrupt enable bit
    PIE5bits.PWM2IE = 1;
    
    //PWM2_16BIT period interrupt enable bit
    PIE5bits.PWM2PIE = 1;
    
    //Set default interrupt handlers
    PWM_CONTROL_Slice1Output1_SetInterruptHandler(PWM_CONTROL_Slice1Output1_DefaultInterruptHandler);
    PWM_CONTROL_Slice1Output2_SetInterruptHandler(PWM_CONTROL_Slice1Output2_DefaultInterruptHandler);
    PWM_CONTROL_Period_SetInterruptHandler(PWM_CONTROL_Period_DefaultInterruptHandler);

    //PWMEN enabled; PWMLD disabled; PWMERSPOL disabled; PWMERSNOW disabled; 
    PWM2CON = 0x80;
}

void PWM_CONTROL_Enable()
{
    PWM2CON |= _PWM2CON_EN_MASK;
}

void PWM_CONTROL_Disable()
{
    PWM2CON &= (~_PWM2CON_EN_MASK);
}

void PWM_CONTROL_WritePeriodRegister(uint16_t periodCount)
{
    PWM2PRL = (uint8_t)periodCount;
    PWM2PRH= (uint8_t)(periodCount >> 8);
}

void PWM_CONTROL_SetSlice1Output1DutyCycleRegister(uint16_t registerValue)
{    
    PWM2S1P1L = (uint8_t)(registerValue);
    PWM2S1P1H = (uint8_t)(registerValue >> 8);
}

void PWM_CONTROL_SetSlice1Output2DutyCycleRegister(uint16_t registerValue)
{        
    PWM2S1P2L = (uint8_t)(registerValue);
    PWM2S1P2H = (uint8_t)(registerValue >> 8);
}

void PWM_CONTROL_LoadBufferRegisters(void)
{
    //Load the period and duty cycle registers on the next period event
    PWM2CONbits.LD = 1;
}

void __interrupt(irq(PWM2),base(8)) PWM_CONTROL_PWMI_ISR()
{
    PIR5bits.PWM2IF = 0;
    if((PWM2GIEbits.S1P1IE == 1) && (PWM2GIRbits.S1P1IF == 1))
    {
        PWM2GIRbits.S1P1IF = 0;
        if(PWM_CONTROL_Slice1Output1_InterruptHandler != NULL)
            PWM_CONTROL_Slice1Output1_InterruptHandler();
    }
    else if((PWM2GIEbits.S1P2IE == 1) && (PWM2GIRbits.S1P2IF == 1))
    {
        PWM2GIRbits.S1P2IF = 0;
        if(PWM_CONTROL_Slice1Output2_InterruptHandler != NULL)
            PWM_CONTROL_Slice1Output2_InterruptHandler();
    }
}

void __interrupt(irq(PWM2PR),base(8)) PWM_CONTROL_PWMPI_ISR()
{
    PIR5bits.PWM2PIF = 0;
    if(PWM_CONTROL_Period_InterruptHandler != NULL)
        PWM_CONTROL_Period_InterruptHandler();
}

void PWM_CONTROL_Slice1Output1_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM_CONTROL_Slice1Output1_InterruptHandler = InterruptHandler;
}

void PWM_CONTROL_Slice1Output2_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM_CONTROL_Slice1Output2_InterruptHandler = InterruptHandler;
}

void PWM_CONTROL_Period_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM_CONTROL_Period_InterruptHandler = InterruptHandler;
}

static void PWM_CONTROL_Slice1Output1_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM_CONTROL_Slice1Output1_SetInterruptHandler() function to use Custom ISR
}

static void PWM_CONTROL_Slice1Output2_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM_CONTROL_Slice1Output2_SetInterruptHandler() function to use Custom ISR
}

static void PWM_CONTROL_Period_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use PWM_CONTROL_Period_SetInterruptHandler() function to use Custom ISR
}
