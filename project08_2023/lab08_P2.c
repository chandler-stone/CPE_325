#include <msp430FG4618.h>
#include <stdio.h>
#include <stdint.h>

volatile float myData;
volatile float modulation;
volatile int periodNumber;
volatile int delay;

void UART_setup(void) {
    P2SEL |= BIT4 | BIT5;       // Set UC0TXD and UC0RXD to transmit and receive data
    UCA0CTL1 |= BIT0;           // Software reset
    UCA0CTL0 = 0;               // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;       // Clock source SMCLK
    UCA0BR0 = 18;             // 1048576 Hz/57600 lower byte
    UCA0BR1 = 0x00;             // Upper byte
    UCA0MCTL = 0x02;            // Modulation
    UCA0CTL1 &= ~BIT0;          // UCSWRST software reset
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait for previous character to transmit
    UCA0TXBUF = c;               // Put character into tx buffer
}

void main() {
    WDTCTL = WDT_ADLY_1_9;
    UART_setup();                // Initialize USCI_A0 module in UART mode
    IE1 |= WDTIE;                // Enable watchdog interrupts

    delay = 0;
    myData = 0.0;
    modulation = 0.0719;
    periodNumber = 0;
    __bis_SR_register(LPM0_bits + GIE);
}

// Sends a ramp signal; amplitude of one period ranges from 0.0 to 9.9
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    if (periodNumber == 12) {
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    }
    char index = 0;
    // Use character pointers to send one byte at a time
    char *myPointer = (char* )&myData;

    UART_putCharacter(0x55);                // Send header
    for(index = 0; index < 4; index++) {    // Send 4-bytes of myData
        UART_putCharacter(myPointer[index]);
    }

    // Update myData for next transmission
    myData = (myData + modulation);
    if (myData + modulation >= 9.0) {
        myData = 9.0;
    }
    if (myData + modulation <= 0) {
        myData = 0;
    }
    if(myData >= 9.0 || myData <= 0) {
        modulation *= -1.0;
        periodNumber++;
    }
}
