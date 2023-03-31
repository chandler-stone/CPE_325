/*------------------------------------------------------------------------------
 * File:        Lab09_D3.c (CPE 325 Lab09 Demo code)
 * Function:    Timed Hello World message in HyperTerminal by using software
 *              polling (MPS430FG4618)
 * Description: This C program maintains real-time clock and sends "Hello World"
 *              message along with the time in second to the workstation through
 *              UART. Watchdog is configured for 1s in interval mode. The
 *              processor continuously polls to check whether the WDTIFG is set
 *              or not. When the flag is set it increments the time and prepares
 *              the new message to transmit. The format of the message displayed
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
 *              Priyanka Madhushri
 *------------------------------------------------------------------------------*/
#include <msp430xG46x.h> 
#include <stdio.h>

char helloMsg[] = "Hello World!\r\n";
char timeMsg[25];                // String for time message
unsigned int sec = 0;            // Variable for measuring time

void UART_setup(void) {
    P2SEL |= BIT4;               // Set UC0TXD to transmit data
    UCA0CTL1 |= UCSWRST;         // Software reset
    UCA0CTL0 = 0;                // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;        // Clock source SMCLK - 1048576 Hz
    UCA0BR0 = 0x09;              // 1MHz/115200   (lower byte)
    UCA0BR1 = 0;                 // 1MHz/115200   (upper byte)
    UCA0MCTL = 0x02;             // Modulation (UCBRS0=0x01)(UCOS16=0)
    UCA0CTL1 &= ~UCSWRST;        // Software reset
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait for previous character to transmit
    UCA0TXBUF = c;               // Put character into tx buffer
}

void main(void) {
    int i;
    WDTCTL = WDT_ADLY_1000;       // WDT 1000ms, ACLK, interval timer
    UART_setup();                 // Setup USCI_A0 module in UART mode
    for (;;) {
        while (!(IFG1 & WDTIFG)); // Wait for 1 second from WDT
        sec++;                    // Increment time
        sprintf(timeMsg, "%6d s: %s", sec, helloMsg); // Prepare time message
        for (i = 0; i < sizeof(timeMsg); i++) {       // Send time message
            UART_putCharacter(timeMsg[i]);
        }
        IFG1 &= ~WDTIFG;          // Clear watchdog interrupt flag
    }
}
