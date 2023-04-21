#include <msp430FG4618.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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
        if (UART_getCharacter() == '\b') {
            if (i > 0) {
                i--;
                buff[i] = NULL;
                UART_sendCharacter('\b');
                UART_sendCharacter(' ');
                UART_sendCharacter('\b');
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
    char name[20] = {};
    char fruit[20] = {};
    char number[5] = {};
    int fruitNumber = 0;
    int total = 0;
    char str[6] = {};

start:
    UART_sendString("\033[0;31mMarket Bot:\033[0m Hi, I am Market Bot! What is the name for your order?\n\r\033[0;34mUser: \033[0m ");
    UART_getLine(name, 20);
ask:UART_sendString("\033[0;31mMarket Bot:\033[0m Today we have apples, bananas, and oranges. What would you like to order?\n\r\033[0;34mUser: \033[0m ");
    UART_getLine(fruit, 20);

    UART_sendString("\033[0;31mMarket Bot:\033[0m ");

    if (strcmp("apples", fruit) == 0) {
        UART_sendString("apples cost $2 each, How many would you like?\r\n\033[0;34mUser: \033[0m ");
        UART_getLine(number, 4);
        fruitNumber = atoi(number);
        UART_sendString("\033[0;31mMarket Bot:\033[0m The total for ");
        UART_sendString(number);
        UART_sendString(" apples is $");
        total = fruitNumber * 2;
        if (total >= 10000) {
            str[5] = NULL;
            str[4] = total % 10 + '0';
            total /= 10;
            str[3] = total % 10 + '0';
            total /= 10;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
                }
        else if (total >= 1000) {
            str[4] = NULL;
            str[3] = total % 10 + '0';
            total /= 10;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else if (total >= 100) {
            str[3] = NULL;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else if (total >= 10) {
            str[2] = NULL;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else {
            str[1] = NULL;
            str[0] = total + '0';
            UART_sendString(str);
        }
        UART_sendString(". Thank you for shopping with me ");
        UART_sendString(name);
        UART_sendString("!\r\n");
    }
    else if (strcmp("bananas", fruit) == 0) {
        UART_sendString("bananas cost $1 each, How many would you like?\r\n\033[0;34mUser: \033[0m ");
        UART_getLine(number, 4);
        fruitNumber = atoi(number);
        UART_sendString("\033[0;31mMarket Bot:\033[0m The total for ");
        UART_sendString(number);
        UART_sendString(" bananas is $");
        UART_sendString(number);
        UART_sendString(". Thank you for shopping with me ");
        UART_sendString(name);
        UART_sendString("!\r\n");
    }
    else if (strcmp("oranges", fruit) == 0) {
        UART_sendString("oranges cost $3 each, How many would you like?\r\n\033[0;34mUser: \033[0m ");
        UART_getLine(number, 4);
        fruitNumber = atoi(number);
        UART_sendString("\033[0;31mMarket Bot:\033[0m The total for ");
        UART_sendString(number);
        UART_sendString(" oranges is $");
        total = fruitNumber * 3;
        if (total >= 10000) {
            str[5] = NULL;
            str[4] = total % 10 + '0';
            total /= 10;
            str[3] = total % 10 + '0';
            total /= 10;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else if (total >= 1000) {
            str[4] = NULL;
            str[3] = total % 10 + '0';
            total /= 10;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else if (total >= 100) {
            str[3] = NULL;
            str[2] = total % 10 + '0';
            total /= 10;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else if (total >= 10) {
            str[2] = NULL;
            str[1] = total % 10 + '0';
            total /= 10;
            str[0] = total + '0';
            UART_sendString(str);
        }
        else {
            str[1] = NULL;
            str[0] = total + '0';
            UART_sendString(str);
        }
        UART_sendString(". Thank you for shopping with me ");
        UART_sendString(name);
        UART_sendString("!\r\n");
    }
    else {
        UART_sendString(fruit);
        UART_sendString(" are not available today.\r\n");
        goto ask;
    }
    goto start;
}
