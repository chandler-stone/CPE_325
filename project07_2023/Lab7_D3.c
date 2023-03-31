/*------------------------------------------------------------------------------
 * File:        Lab7_D3.c (CPE 325 Lab7 Demo code)
 * Function:    Toggling LED1 using software polling.
 * Description: This C program configures the WDT in interval timer mode and
 *              it is clocked with ACLK. The WDT sets the interrupt flag (WDTIFG)
 *              every 1 s. LED1 is toggled by verifying whether this flag
 *              is set or not. After it is detected as set, the WDTIFG is cleared.
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
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h>

void main(void)
{
    WDTCTL = WDT_ADLY_1000;           // 1 s interval timer
    P2DIR |= BIT2;                    // Set P2.2 to output direction

    for (;;) {
        // Use software polling
        if ((IFG1 & WDTIFG) == 1) {
            P2OUT ^= BIT2;
            IFG1 &= ~WDTIFG;          // Clear bit WDTIFG in IFG1
        }
    }
}
