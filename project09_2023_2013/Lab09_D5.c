/*------------------------------------------------------------------------------
 * File:        Lab09_D5.c (CPE 325 Lab09 Demo code)
 * Function:    Timed Hello World message in HyperTerminal by using DMA
 *              (MPS430FG4618)
 * Description: This C program maintains real-time clock and sends "Hello World"
 *              message along with the time in second to the workstation through
 *              UART. DMA0 is used to transfer a string as a block to UCA0TXBUF.
 *              DMAREQ will trigger the DMA0. Watchdog in interval mode triggers
 *              block transfer every 1s. The format of the message displayed
 *              in HyperTerminal is "sssss s:Hello World!".
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
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 
#include <stdio.h>

char helloMsg[] = "Hello World!\n\r";
char timeMsg[25];                 // String for time message
unsigned int sec = 0;             // Variable for measuring time

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

void DMA_setup(void) {
    DMACTL0 = DMA0TSEL_4;             // DMAREQ, software trigger, TX is ready
    DMA0SA = (__SFR_FARPTR)timeMsg;   // Source block address
    DMA0DA = (__SFR_FARPTR)&UCA0TXBUF;// Destination single address
    DMA0SZ = sizeof(timeMsg);         // Length of the String
    DMA0CTL = DMASRCINCR_3 + DMASBDB + DMALEVEL; // Increment src address
}

void main(void) {
    WDTCTL = WDT_ADLY_1000;       // WDT 1000ms, ACLK, interval timer
    IE1 |= WDTIE;                 // Enable WDT interrupt
    UART_setup();                 // Setup USCI_A0 module in UART mode
    DMA_setup();                  // Setup DMA to transfer the time message

    _BIS_SR(LPM0_bits + GIE);     // Enter LPM0, interrupts enabled
}

#pragma vector = WDT_VECTOR       // Trigger DMA block transfer
__interrupt void WDT_ISR(void) {
    sec++;
    sprintf(timeMsg, "%6d s: %s", sec, helloMsg);
    DMA0CTL |= DMAEN;             // Enable DMA transfer
}
