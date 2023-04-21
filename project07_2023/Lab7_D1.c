/*------------------------------------------------------------------------------
 * File:        Lab7_D1.c (CPE 325 Lab7 Demo code)
 * Function:    Toggling LED1 using WDT ISR (MPS430FG4618)
 * Description: This C program configures the WDT in interval timer mode and
 *              it is clocked with ACLK. The WDT is configured to give an
 *              interrupt for every 1s. LED1 is toggled in the WDT ISR
 *              by xoring P2.2. The blinking frequency of LED1 is 0.5Hz.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                          MSP430xG461x
 *                       -----------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |             P2.2|-->LED1(GREEN)
 *                      |                 |
 * Input:       None
 * Output:      LED1 blinks at 0.5Hz frequency
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Max Avula, ma0004@uah.edu
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h>

void main(void)
{
    WDTCTL = WDT_ADLY_1000;           // 1 s interval timer
    P2DIR |= BIT2;                    // Set P2.2 to output direction
    IE1 |= WDTIE;                     // Enable WDT interrupt
    _BIS_SR(LPM0_bits + GIE);         // Enter LPM0 w/ interrupt
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    P2OUT ^= BIT2;                    // Toggle P2.2 using exclusive-OR
}
