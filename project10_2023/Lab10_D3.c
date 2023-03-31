/*------------------------------------------------------------------------------
 * File:        Lab10_D3.c (CPE 325 Lab10 Demo code)
 * Function:    Sinusoidal wave with DAC (MPS430FG4618)
 * Description: This C program reconstructs the sinusoidal wave (y=1.25(1+sin(x)))
 *              from the samples using DAC and outputs at P6.6. WDT is used to
 *              give an interrupt for every ~0.064ms to wake up the CPU and
 *              feed the DAC with new value. Connect the oscilloscope to P6.6
 *              to observe the signal. The interval used to read the samples
 *              controls the frequency of output signal.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 *
 *                         MSP430xG461x
 *                      -------------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |        DAC0/P6.6|--> sine (10Hz)
 *                      |                 |
 * Input:       None
 * Output:      Sinusoidal wave with 10Hz frequency at P6.6
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Max Avula, ma0004@uah.edu
 *------------------------------------------------------------------------------*/
#include <msp430.h>
#include <sine_lut_256.h>  /*256 samples are stored in this table */

void TimerA_setup(void) {
    TACTL = TASSEL_2 + MC_1;              // SMCLK, up mode
    TACCR0 = 410;                         // Sets Timer Freq (1048576*0.1sec/256)
    TACCTL0 = CCIE;                       // CCR0 interrupt enabled
}

void DAC_setup(void) {
    ADC12CTL0 = REF2_5V + REFON;          // Turn on 2.5V internal ref volage
    unsigned int i = 0;
    for (i = 50000; i > 0; i--);          // Delay to allow Ref to settle
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;   //Sets DAC12
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;             // Stop WDT
    TimerA_setup();                       // Set timer to uniformly distribute the samples
    DAC_setup();                          // Setup DAC
    unsigned int i = 0;
    while (1) {
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
        DAC12_0DAT = LUT256[i];
        i=(i+1)%256;
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void) {
    __bic_SR_register_on_exit(LPM0_bits);  // Exit LPMx, interrupts enabled
}
