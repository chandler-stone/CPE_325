#include <msp430FG4618.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define LED_ON_STATE    0x31      // Character '1'
#define LED_OFF_STATE   0x30      // Character '0'
#define LED_NUL_STATE   0x00      // Character NULL - used for dummy write operation

#define LED_ON          0x01
#define LED_OFF         0x00

unsigned char ch;                 // Hold char from UART RX

char greetMessage[] = "Beacon Pause: ";
char invalidMessage[] = "Invalid pause entered";


char pause[] = {};
char P = 0;                            // Delay time for beacon

void SPI_setup(void) {
    UCB0CTL0 = UCMSB + UCMST + UCSYNC;// Sync. mode, 3-pin SPI, Master mode, 8-bit data
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // SMCLK and Software reset
    UCB0BR0 = 0x02;                // Data rate = SMCLK/2 ~= 500kHz
    UCB0BR1 = 0x00;
    P3SEL |= BIT1 + BIT2 + BIT3;   // P3.1,P3.2,P3.3 option select
    UCB0CTL1 &= ~UCSWRST;          // **Initialize USCI state machine**
}

unsigned char SPI_getState(void) {
    while((P3IN & 0x01));          // Verifies busy flag
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = LED_NUL_STATE;     // Dummy write to start SPI
    while (!(IFG2 & UCB0RXIFG));   // USCI_B0 TX buffer ready?
    return UCB0RXBUF;
}

void SPI_setState(unsigned char State) {
    while(P3IN & 0x01);           // Verifies busy flag
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = State;            // Write new state
    while (!(IFG2 & UCB0RXIFG));  // USCI_B0 TX buffer ready?
}

/**
 * Configures UCI to work in the UART mode at the
 * baud rate of 115,200.
 */
void UART_setup() {
    P2SEL |= BIT4 + BIT5;   // Set USCI_A0 RXD/TXD to receive/transmit data
    UCA0CTL1 |= UCSWRST;    // Set software reset during initialization
    UCA0CTL0 = 0;           // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;   // Clock source SMCLK
    UCA0BR0 = 0x09;         // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;         // upper byte
    UCA0MCTL = 0x02;        // Modulation (UCBRS0=0x01, UCOS16=0)
    UCA0CTL1 &= ~UCSWRST;   // Clear software reset to initialize USCI state machine
}

/**
 * Sends a character via UART
 */
void UART_sendCharacter (char c) {
    while (!(IFG2 & UCA0TXIFG));    // Wait for previous character to transmit
    UCA0TXBUF = c;                  // Put character into tx buffer
}

/**
 * Waits for a character from UART and returns it.
 */
char UART_getCharacter () {
    while(!(IFG2&UCA0TXIFG)); // Wait until TXBUF is free
    return UCA0RXBUF;    // TXBUF <= RXBUF (echo)
}

/**
 * Sends a string via UART using sendCharacter(char c)
 */
void UART_sendString(char* str) {
    int i = 0;
    while(str[i] != 0) {
        UART_sendCharacter(str[i]);
        i++;
    }
}

/**
 * Receives characters via UART using
 * UART_getCharacter()until it finds the new line
 * character or until the limit of characters is exceeded.
 * Writes that string (excluding the new line character)
 * to the buffer allocated outside of the function.
 * Terminates the string with the null character.
 */
void UART_getLine (char* buff, int limit) {
    int i = 0;
    UCA0TXBUF = 0;
    while ((UCA0TXBUF != '\r') && (i < limit)) {
        while(!(IFG2&UCA0RXIFG));
        if (UCA0TXBUF == '\010') {
            if (i > 0) {
                i--;
                buff[i] = NULL;
                UART_sendCharacter('\b');
                UART_sendCharacter(NULL);
            }
        }
        else {
            buff[i] = UART_getCharacter();
            UART_sendCharacter(buff[i]);
            i++;
        }
    }
    if (i >= limit) {
        buff[i+1] = NULL;
    }
    if (buff[i-1] == '\r') {
        buff[i-1] = NULL;
    }
    UART_sendString("\n\r");
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    UART_setup();               // initialize the UART problem
    SPI_setup();

loop:
    UART_sendString(greetMessage);
    UART_getLine(pause, 3);

    if (strcmp("-", pause) == 0) {
        P = 255;
        pause[0] = '2';
        pause[1] = '5';
        pause[2] = '5';
        pause[3] = NULL;
        SPI_setState(P);
    }
    else if (strcmp("?", pause) == 0) {
        SPI_setState(0);
        int i = 0;
        for(i = 100; i > 0; i--);   // Delay
        P = SPI_getState();
        char temp_p = P;
        if (P >= 100) {
            pause[3] = NULL;
            pause[2] = temp_p % 10 + '0';
            temp_p /= 10;
            pause[1] = temp_p % 10 + '0';
            temp_p /= 10;
            pause[0] = temp_p + '0';
        }
        else if (P >= 10) {
            pause[2] = NULL;
            pause[1] = temp_p % 10 + '0';
            temp_p /= 10;
            pause[0] = temp_p + '0';
        }
        else {
            pause[1] = NULL;
            pause[0] = temp_p + '0';
        }
        UART_sendString("Current Pause: ");
        UART_sendString(pause);
        UART_sendString("\r\n");
    }
    else {
        char temp_p = P;
        P = atoi(pause);
        if (P <= 100 && P >= 1) {
            SPI_setState(P);
        }
        else {
            P = temp_p;
            UART_sendString("Invalid Pause entered\r\n");
        }
    }
    goto loop;
}
