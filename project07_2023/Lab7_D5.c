/*------------------------------------------------------------------------------
 * File:        Lab7_D5.c (CPE 325 Lab7 Demo code)
 * Function:    Blinking LED2 using Timer_B in up mode (MPS430FG4618)
 * Description: In this C program, Timer_B is configured for UP mode. In this
 *              mode, the timer TB counts from 0 up to value stored in TB0CCR0.
 *              So, the counter period is CCR0*1us. The TB0 output signal is
 *              configured to toggle every time the counter reaches the value
 *              in TB0CCR0. TB0 is multiplexed with the P2.1, and LED2 is luckily
 *              connected to this output. Thus, the output frequency on P2.1 will
 *              be f = ACLK/(2*CCR0) = 0.5Hz. Please note that once configured,
 *              the Timer_B toggles the LED2 automatically even when the CPU is
 *              in sleep mode.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                          MSP430xG461x
 *                       -----------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |         P2.1/TB0|--> 32768/ACLK (LED2(YELLOW))
 *                      |                 |
 * Input:       None
 * Output:      LED2 blinks at 0.5Hz frequency using hardware TB0
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 

void main(void) 
{ 
    WDTCTL = WDTPW +WDTHOLD;    // Stop WDT
    P2DIR |= BIT1;              // P2.1 output
    P2SEL |= BIT1;              // P2.1 special function (TB0 output)
    TB0CCTL0 = OUTMOD_4;        // TB0 output is in toggle mode
    TB0CTL = TBSSEL_1 + MC_1;   // ACLK is clock source, UP mode
    TB0CCR0 = 32767;
    _BIS_SR(LPM3_bits + GIE);   // Enter Low Power Mode 3
}
