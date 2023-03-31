/*------------------------------------------------------------------------------
 * File:        Demo8_D4.c
 * Function:    Send floating data to Serial port
 * Description: UAH serial app expects lower byte first so send each byte at a
 *              time sending Lowest byte first
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO
 *
 * Instructions: Set the following parameters in putty
 * Port: COM1
 * Baud rate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1
 * Flow Control: None
 *
 *        MSP430xG461x
 *     -----------------
 * /|\ |            XIN|-
 *  |  |               | 32kHz
 *  |--|RST        XOUT|-
 *     |               |
 *     |   P2.4/UCA0TXD|------------>
 *     |               | 115200 - 8N1
 *     |   P2.5/UCA0RXD|<------------
 *     |               |
 * Input:     None
 * Output:    Ramp signal in UAH Serial app
 * Author:    Prawar Poudel
 * Date:      October 2018
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdint.h>

volatile float myData;

void UART_setup(void) {
    P2SEL |= BIT4 | BIT5;       // Set UC0TXD and UC0RXD to transmit and receive data
    UCA0CTL1 |= BIT0;           // Software reset
    UCA0CTL0 = 0;               // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;       // Clock source SMCLK
    UCA0BR0 = 0x09;             // 1048576 Hz/115200 lower byte
    UCA0BR1 = 0x00;             // Upper byte
    UCA0MCTL = 0x02;            // Modulation
    UCA0CTL1 &= ~BIT0;          // UCSWRST software reset
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait for previous character to transmit
    UCA0TXBUF = c;               // Put character into tx buffer
}

int main() {
    WDTCTL = WDT_ADLY_1000;
    UART_setup();                // Initialize USCI_A0 module in UART mode
    IE1 |= WDTIE;                // Enable watchdog interrupts
   
    myData = 0.0;
    __bis_SR_register(LPM0_bits + GIE);
}

// Sends a ramp signal; amplitude of one period ranges from 0.0 to 9.9
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    char index = 0;
    // Use character pointers to send one byte at a time
    char *myPointer = (char* )&myData;

    UART_putCharacter(0x55);                // Send header
    for(index = 0; index < 4; index++) {    // Send 4-bytes of myData
        UART_putCharacter(myPointer[index]);
    }

    // Update myData for next transmission
    myData = (myData + 0.1);     
    if(myData >= 10.0) {
        myData = 0.0;
    }
}
