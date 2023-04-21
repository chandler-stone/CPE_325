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

void main(void)
{
    WDTCTL = WDT_ADLY_1000;     // Stop WDT
    IE1 |= WDTIE;

    _EINT();                    // Enable global interrupts

    P2DIR |= BIT2;              // P2.2 output

    P3DIR |= BIT5;
    P3SEL |= BIT5;
    P3OUT &= ~BIT5;


    TBCCTL4 = OUTMOD_4;         // TB0 output is in Set/Reset mode
    TBCTL = TBSSEL_1 + MC_1;    // ACLK is clock source, Continuous mode//SMLCK = 1MHz
    TBCCR0 = 33;                // Setting min value of timer to 50 (half brightness)
    TBCCR4 = 16;

    _BIS_SR(LPM0_bits + GIE);   // Enter Low Power Mode 0 (Sleep Mode)
}




#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer (void) {
    P2OUT ^= BIT2;
    P3DIR ^= BIT5;
    IFG1 &= ~WDTIFG;
}
