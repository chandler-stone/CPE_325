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
#define   SW1_PRESSED ((BIT0&P1IN) == 0)
#define   SW2_PRESSED ((BIT1&P1IN) == 0)

#define   LED2  0x02
#define   LED1  0x04

#define   SW2   0x02
#define   SW1   0x01

void main(void) {
    WDTCTL = WDTPW+WDTHOLD;         // Stop WDT

    FLL_CTL0 = XCAP18PF;
    SCFI0 |= BIT0;
    SCFQCTL = 15;

    P2DIR |= LED2 +LED1;            // Set LED1 as output
    P2OUT = 0x00;                   // clear LED1 status
    _EINT();                        // enable interrupts
    P1IE = SW1 + SW2;              // P1.0 interrupt enabled
    P1IES |= BIT0 + BIT1;                  // P1.0 hi/low edge
    P1IFG &= ~BIT0;                 // P1.0 IFG cleared
    while (1) {
        if (!(SW1_PRESSED || SW2_PRESSED)) {
            SCFQCTL = 15;
        }
        P2OUT ^= LED1; //toggle red led
        __delay_cycles(1000000);
    }
}

// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void) {
    //P1IE = SW1 + SW2;              // P1.0 interrupt enabled
    P1IFG = 0;
    if (SW1_PRESSED && SW2_PRESSED) {
        SCFQCTL = 121; // (121+1) *32768 * 2 = 8,000,000
    }
    else if (SW1_PRESSED) {
        SCFQCTL = 60; // (60+1) *32768 * 2 = 4,000,000
    }
    else if (SW2_PRESSED) {
        SCFQCTL = 30; // (30+1) *32768 * 2 = 2,000,000
    }
}
