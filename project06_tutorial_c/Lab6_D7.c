/******************************************************************************
 *  File:        Lab6_D7.c
 *  Description: MSP430xG46x Demo - FLL+, Runs Internal DCO at 8MHz
 *               This program demonstrates setting the internal DCO to run at
 *               8MHz with auto-calibration by the FLL+ circuitry.
 *  Clocks:      ACLK = LFXT1 = 32768Hz,
 *               MCLK = SMCLK = DCO = (121+1) x 2 x ACLK = 7995392Hz
 *               An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                MSP430xG461x
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 | 32kHz
 *         --|RST          XOUT|-
 *           |                 |
 *           |             P1.1|--> MCLK = 8MHz
 *           |                 |
 *           |             P1.4|--> SMCLK = 8MHz
 *           |             P1.5|--> ACLK = 32kHz
 *           |                 |
 *
 *  Author: Aleksandar Milenkovic, milenkovic@computer.og
 *  Date:   September 2010
 ******************************************************************************/

#include  <msp430.h>

void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer
   FLL_CTL0 |= DCOPLUS + XCAP18PF; // DCO+ set, freq = xtal x D x N+1
   SCFI0 |= FN_4 + FLLD_2;         // DCO range control, multiplier D
   SCFQCTL = 121;                  // (121+1) x 32768 x 2 = 7.99 MHz
   P1DIR |= 0x32;                  // P1.1, P1.4 & P1.5 to output direction
   P1SEL |= 0x32;                  // P1.1, P1.4 & P1.5 to output MCLK, SMCLK & ACLK
   while(1);                       // Loop in place
}
