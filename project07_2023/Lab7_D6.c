/*------------------------------------------------------------------------------
 * File:        Lab7_D6.c (CPE 325 Lab7 Demo code)
 * Function:    Blinking LED1 & LED2 using Timer_B with interrupts (MPS430FG4618)
 * Description: In this C program, Timer_B is configured for up/down mode with
 *              ACLK source and interrupts for channel 0 and channel 1 are
 *              enabled. In up/down mode timer TB counts the value from 0 up to
 *              value stored in TB0CCR0 and then counts back to 0. The interrupt
 *              for TB0 is generated when the counter reaches value in TB0CCR0.
 *              The interrupt TB1 is generated whenever the counter reaches value
 *              in TB0CCR1. Thus, TB1 gets two interrupts while counting upwards
 *              and counting downwards. This simulates a PWM control - adjusting
 *              the TB1 and TB0 CCR register values adjusts the duty cycle of the
 *              PWM signal.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                          MSP430xG461x
 *                       -----------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |             P2.1|--> LED2(YELLOW)
 *                      |             P2.2|--> LED1(GREEN)
 * Input:       None
 * Output:      LED1 blinks at 1.64Hz with 20-80% duty cycle and LED2 blinks at
 *              0.82Hz with 50-50% duty cycle.
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 

void main(void) 
{ 
    WDTCTL = WDTPW +WDTHOLD;      // Stop WDT
    _EINT();                      // Enable interrupts
    P2DIR |= BIT1 + BIT2;         // P2.1 and P2.2 set up as output
    P2OUT &= ~(BIT1 + BIT2);      // ensure LED1 and LED2 are off
    TB0CCTL0 = CCIE;              // TB0 count triggers interrupt
    TB0CCR0 = 10000;              // Set TB0 (and maximum) count value
    TB0CCTL1 = CCIE;              // TB1 count triggers interrupt
    TB0CCR1 = 2000;               // Set TB1 count value
    TB0CTL = TBSSEL_1 | MC_3;     // ACLK is clock source, UP/DOWN mode

    _BIS_SR(LPM3);                // Enter Low Power Mode 3
}

#pragma vector = TIMERB0_VECTOR
__interrupt void timerISR(void)
{
    P2OUT ^= BIT1;                // Toggle LED2
}

#pragma vector = TIMERB1_VECTOR
__interrupt void timerISR2(void)
{
    P2OUT ^= BIT2;                // Toggle LED1
    TBCCTL1 &= ~CCIFG;            // Clear interrupt flag
}
