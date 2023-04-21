#include <msp430.h>
#include <stdio.h>



static const char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char rotor1[26] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
static const char rotor2[26] = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
static const char rotor3[26] = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
static const char rotor4[26] = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
static const char rotor5[26] = "VZBRGITYUPSDNHLXAWMJQOFECK";
static const char reflector[] = "EJMZALYXVBWFCRQUONTSPIKHGD";

static const char rotor1notch = 'Q';
static const char rotor2notch = 'E';
static const char rotor3notch = 'V';
static const char rotor4notch = 'J';
static const char rotor5notch = 'Z';


char plugboard[26] = "..........................";

char* selection1 = rotor1;
char* selection2 = rotor2;
char* selection3 = rotor3;

unsigned char i = 0;
unsigned char j = 0;
unsigned char k = 0;

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
            if (buff[i] > 'a' && buff[i] < 'z') {
                buff[i] -= 32;
            }
            if (buff[i] > 'A' && buff[i] < 'Z') {
                UART_sendCharacter(buff[i]);
                i++;
            }
            else {
                buff[i] = NULL;
            }
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

char plugboardAlgorithm(char* c) {
    char tempc = *c;
    if (plugboard[tempc] != '.') {
        tempc = plugboard[tempc];
    }
    return tempc;
}

/**
 * This program preforms the job
 * of the reflector on the enigma
 * machine.
 */
char reflectorAlgorithm(char* c) {
    char tempc = *c;

    return tempc;
}

char enigmaAlgorithm(char c) {
    char tempc = c - 65;
    tempc = plugboardAlgorithm(&tempc);
    tempc = selection1[(tempc + i) % 26];
    tempc = selection2[(tempc + j) % 26];
    tempc = selection3[(tempc + k) % 26];
    tempc = reflectorAlgorithm(&tempc);
    tempc = selection3[(tempc + k) % 26];
    tempc = selection2[(tempc + j) % 26];
    tempc = selection1[(tempc + i) % 26];
    if (i > 234) i = 0;
    if (j > 234) j = 0;
    if (k > 234) k = 0;
    return tempc;
}


void main () {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    UART_setup();
}
