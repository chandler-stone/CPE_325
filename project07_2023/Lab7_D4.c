/*------------------------------------------------------------------------------
 * File:        Lab7_D4.c (CPE 325 Lab7 Demo code)
 * Function:    Blinking LED2 using Timer_B in continuous mode (MPS430FG4618)
 * Description: In this C program, Timer_B is configured for continuous mode. In
 *              this mode, the timer TB counts from 0 up to 0xFFFF (default 2^16).
 *              So, the counter period is 65,536*1us = 65 ms. The TB0 output signal
 *              is configured to toggle every time the counter reaches the maximum
 *              value, which corresponds to 65 ms. TB0 is multiplexed with the
 *              P2.1, and LED2 is luckily connected to this output. Thus the
 *              output frequency on P2.1 will be f = SMCLK/(2*65536) ~ 7.63 Hz.
 *              Please note that once configured, the Timer_B toggles the LED2
 *              automatically even when the CPU is in sleep mode.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                          MSP430xG461x
 *                       -----------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |         P2.1/TB0|-->65536/SMCLK (LED2(YELLOW))
 *                      |                 |
 * Input:       None
 * Output:      LED2 blinks at 7.63Hz frequency using hardware TB0
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 

void main(void) 
{ 
    WDTCTL = WDTPW +WDTHOLD;  // Stop WDT
    P2DIR |= BIT1;            // P2.1 output
    P2SEL |= BIT1;            // P2.1 special function (TB0 output)
    TB0CCTL0 = OUTMOD_4;      // TB0 output is in toggle mode
    TB0CTL = TBSSEL_2 + MC_2; // SMCLK is clock source, Continuous mode
    _BIS_SR(LPM0_bits + GIE); // Enter Low Power Mode 0
}
