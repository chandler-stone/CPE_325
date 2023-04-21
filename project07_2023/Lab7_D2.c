/*------------------------------------------------------------------------------
 * File:        Lab7_D2.c (CPE 325 Lab7 Demo code)
 * Function:    Toggling LED1 using WDT ISR (MPS430FG4618)
 * Description: This C program configures the WDT in interval timer mode and
 *              it is clocked with SMCLK. The WDT is configured to give an
 *              interrupt for every 32ms. The WDT ISR is counted for 32 times
 *              (32*32ms = 1sec) before toggling LED1 to get 1 s on/off.
 *              The blinking frequency of LED1 is 0.5Hz.
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
    WDTCTL = WDT_MDLY_32;             // 32ms interval (default)
    P2DIR |= BIT2;                  // Set P2.2 to output direction
    IE1 |= WDTIE;                   // Enable WDT interrupt

    _BIS_SR(LPM0_bits + GIE);       // Enter LPM0 with interrupt
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    static int i = 0;
    i++;
    if (i == 32) {                  // 31.25 * 32 ms = 1s
        P2OUT ^= BIT2;              // Toggle P2.2 using exclusive-OR
                                    // 1s on, 1s off; period = 2s, fre = 1/2s = 0.5Hz
        i = 0;
    }
}
