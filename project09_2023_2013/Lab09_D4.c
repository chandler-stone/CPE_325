/*------------------------------------------------------------------------------
 * File:        Lab09_D4.c (CPE 325 Lab09 Demo code)
 * Function:    Timed Hello World message in HyperTerminal by using interrupts
 *              (MPS430FG4618)
 * Description: This C program maintains real-time clock and sends "Hello World"
 *              message along with the time in second to the workstation through
 *              UART. It uses interrupt from USCIAB0TX to transmitting characters.
 *              Watchdog in interval mode triggers the transmission every 1s.
 *              The format of the message displayed in HyperTerminal is
 *              "sssss s:Hello World!".
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 * Instructions:Set the following parameters in HyperTerminal
 *                  Port :        COM1
 *                  Baud rate :   115200
 *                  Data bits:    8
 *                  Parity:       None
 *                  Stop bits:    1
 *                  Flow Control: None
 *
 *                         MSP430xG461x
 *                      -------------------
 *                   /|\|              XIN|-
 *                    | |                 | 32kHz
 *                    --|RST          XOUT|-
 *                      |                 |
 *                      |     P2.4/UCA0TXD|------------> "ssss s:Hello World"
 *                      |                 | 115200 - 8N1
 *                      |     P2.5/UCA0RXD|<------------
 *                      |                 |
 * Input:       None
 * Output:      Displays "ssss s:Hello World" in HyperTerminal
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *              Priyanka Madhushri
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 
#include <stdio.h>

char helloMsg[] = "Hello World!\n\r";
char timeMsg[25];                 // String for time message
unsigned int sec = 0;             // variable for measuring time
int i = 0;                        // Character counter

void UART_setup(void) {
    P2SEL |= BIT4;                // Set UC0TXD to transmit data
    UCA0CTL1 |= UCSWRST;          // Software reset
    UCA0CTL0 = 0;                 // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;         // Clock source SMCLK - 1048576 Hz
    UCA0BR0 = 0x09;               // 1MHz/115200   (lower byte)
    UCA0BR1 = 0;                  // 1MHz/115200   (upper byte)
    UCA0MCTL = 0x02;              // Modulation (UCBRS0=0x01)(UCOS16=0)
    UCA0CTL1 &= ~UCSWRST;         // Software reset
}

void main(void) {
    WDTCTL = WDT_ADLY_1000;       // WDT 1000ms, ACLK, interval timer
    IE1 |= WDTIE;                 // Enable WDT interrupt
    UART_setup();                 // Setup USCI_A0 module in UART mode

    for (;;) {
        _BIS_SR(LPM0_bits + GIE); // Enter LPM0, enable interrupts
        sec++;                    // Increment time
        sprintf(timeMsg, "%6d s: %s", sec, helloMsg); // Prepare time message
        i = 0;                    // Character counter
        IE2 |= UCA0TXIE;          // Enable transmit interrupts
    }
}

#pragma vector = WDT_VECTOR     
__interrupt void WDT_ISR(void) {
    __bic_SR_register_on_exit(CPUOFF);// Exit LPM mode
}

#pragma vector = USCIAB0TX_VECTOR  // Transmit ISR
__interrupt void TX_ISR(void) {
    UCA0TXBUF = timeMsg[i++];      // Send the next character
    if (i == 25) IE2 &= ~UCA0TXIE; // If all characters are sent disable interrupts
}
