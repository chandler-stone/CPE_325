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
#define SW1 P1IN&BIT0
#define SW2 P1IN&BIT1


#include <msp430xG46x.h>

static const int dutyCycleMax = 528; // changed from 544 to prevent blinking.

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT

    _EINT();                    // Enable global interrupts

    P1IE |= BIT0 + BIT1;        // Enable port 1 interrupts
    P1DIR &= ~BIT0 + ~BIT1;     // Enable detection on switches (Enable low)
    P1IES |= BIT0 + BIT1;       // P1IES.BIT0 hi/low edge
    P1IFG &= ~BIT0;             // clear interrupt flag

    P2DIR |= BIT2;              // P2.2 output
    P2SEL |= BIT2;              // P2.2 special function (TB0 output)

    TBCCTL1 = OUTMOD_7;         // TB0 output is in Set/Reset mode
    TBCTL = TBSSEL_1 + MC_1;    // ACLK is clock source, Up mode. ALCK = 2^15 Hz
    TBCCR0 = dutyCycleMax;      // Setting max for duty cycle
    TBCCR1 = dutyCycleMax/2;    // Setting start for duty cycle (50%)

    _BIS_SR(LPM0_bits + GIE);   // Enter Low Power Mode 0 (Sleep Mode)
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void) {
    if ((SW1) == 0 && (SW2) == 0) {
        __delay_cycles(1000);
        while ((SW1) == 0 && (SW2) == 0) {
            while ((SW1) == 0 && (SW2) == 0 && TBCCR1 < TBCCR0 - 1) {
                __delay_cycles(2750);
                TBCCR1++;
            }
            while ((SW1) == 0 && (SW2) == 0 && TBCCR1 > 1) {
                __delay_cycles(2750);
                TBCCR1--;
            }
        }
        TBCCR1 = dutyCycleMax/2;
    }
    else if ((SW1) == 0) {
        __delay_cycles(1000);
        if ((SW1) == 0) {
            if (TBCCR1 < TBCCR0) {
                TBCCR1 += dutyCycleMax/4;   // changing by 25% of duty cycle.
            }
        }
    }
    else if ((SW2) == 0) {
        __delay_cycles(1000);
        if ((SW2) == 0) {
            if (TBCCR1 > 0) {
                TBCCR1 -= dutyCycleMax/4;   // changing by 25% of duty cycle.
            }
        }
    }
    P1IFG = 0;
}
