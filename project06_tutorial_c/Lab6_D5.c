/********************************************************************************
 *   File:        Lab6_D5.c
 *   Description: The program detects when SW1 is pressed and turns on LED1.
 *                LED1 is kept on as long as SW1 is pressed.
 *                P1_ISR is used to detect both SW1 presses and releases.
 *   Board:       MSP430FG461x/F20xx Experimenter Board
 *   Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO
 *
 *                 MSP430FG461x
 *            +-----------------+
 *            |                 |
 *            |                 |
 *            |                 |
 *            |                 |
 *            |             P2.2|--> LED1
 *            |             P1.0|<-- SW1
 *
 *   Author: Aleksandar Milenkovic, milenkovic@computer.org
 *   Date:   September 2010
********************************************************************************/
#include  <msp430.h>

unsigned char SW1pressed = 0;       // SW1 status (0 not pressed, 1 pressed)

void main(void) {
    WDTCTL = WDTPW+WDTHOLD;         // Stop WDT
    P2DIR |= BIT2;                  // Set LED1 as output
    P2OUT = 0x00;                   // Clear LED1 status
    SW1pressed = 0;
    _EINT();                        // Enable interrupts
    P1IE |= BIT0;                   // P1IE.BIT0 interrupt enabled
    P1IES |= BIT0;                  // P1IES.BIT0 hi/low edge
    P1IFG &= ~BIT0;                 // P1IFG.BIT0 is cleared
    _BIS_SR(LPM0_bits + GIE);       // Enter LPM0(CPU is off); Enable interrupts
}

// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void) {
    if (SW1pressed == 0) {
        SW1pressed = 1;
        P2OUT |= BIT2;                // LED1 is turned ON
        P1IFG &= ~BIT0;               // P1IFG.BIT0 is cleared
        P1IES &= ~BIT0;               // P1IES.BIT0 low/high edge
    } else if (SW1pressed == 1) {
        SW1pressed = 0;
        P2OUT &= ~BIT2;                // LED1 is turned ON
        P1IFG &= ~BIT0;                // P1IFG.BIT0 is cleared
        P1IES |= BIT0;                 // P1IES.BIT0 hi/low edge
    }
}
