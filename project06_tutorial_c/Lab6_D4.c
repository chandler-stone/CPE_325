/********************************************************************************
 *   File:        Lab6_D4.c
 *   Description: The program detects when SW1 is pressed and turns on LED1.
 *                LED1 is kept on as long as SW1 is pressed.
 *                P1_ISR is used to detect when SW1 is pressed.
 *                Main program polls SW1 and turns off when a release is detected.
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
#define   SW1_PRESSED ((BIT0&P1IN)==0)

void main(void) {
    WDTCTL = WDTPW+WDTHOLD;         // Stop WDT
    P2DIR |= BIT2;                  // Set LED1 as output
    P2OUT = 0x00;                   // clear LED1 status
    _EINT();                        // enable interrupts
    P1IE |= BIT0;                   // P1.0 interrupt enabled
    P1IES |= BIT0;                  // P1.0 hi/low edge
    P1IFG &= ~BIT0;                 // P1.0 IFG cleared
    for(;;) {
        while(SW1_PRESSED);         // Wait until SW1 is released
        P2OUT &= ~BIT2;             // LED1 is turned off
    }
}

// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void) {
    P2OUT |= BIT2;                // LED1 is turned ON
    P1IFG &= ~BIT0;               // P1.0 IFG cleared
}
