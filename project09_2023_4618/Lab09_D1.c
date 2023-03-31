/*------------------------------------------------------------------------------
 * File:        Lab09_D1.c (CPE 325 Lab09 Demo code)
 * Function:    SPI Interface (MPS430Fg4618)
 * Description: Using the MSP-EXP430FG4618 Development Tool establish a data
 *              exchange between the MSP430FG4618 and MSP430F2013 devices using
 *              the SPI mode. The MSP430FG4618 uses the USCI module while the
 *              MSP430F2013 uses the USI module. MSP430FG4618 communicates with
 *              PC via RS232 module using USCI Serial Communication peripheral
 *              interface. This program takes user prompts the user to input a
 *              choice to turn ON or OFF the LED3 located on MSP430F2013. The
 *              user choice is communicated to MSP430FG4618 (master) via  USCI
 *              serial interface and the corresponding action is communicated
 *              to MSP430F2013(slave) via SPI. Based on the user choice,
 *              MSP430F2013 will turn ON or OFF the LED3. This is the master code
 *              that runs on MSP430FG4618.
 *                               Slave                    Master
 *                            MSP430F2013              MSP430FG4618
 *                        -----------------          -----------------
 *                       |              XIN|-    /|\|              XIN|-
 *                       |                 |      | |                 | 32kHz xtal
 *                       |             XOUT|-     --|RST          XOUT|-
 *                       |                 |        |                 |
 *                 LED <-|P1.0             |        |                 |
 *                       |         BF /P1.4|------->|P3.0/BF          |
 *                       |         SDI/P1.7|<-------|P3.1/UCB0SIMO    |
 *                       |         SDO/P1.6|------->|P3.2/UCB0SOMI    |
 *                       |        SCLK/P1.5|<-------|P3.3/UCB0CLK     |
 *
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 * Instructions: 1. Set the following parameters in HyperTerminal
 *                  Port :        COM1
 *                  Baud rate :   19200
 *                  Data bits:    8
 *                  Parity:       None
 *                  Stop bits:    1
 *                  Flow Control: None
 *              2. This lab requires to configure the USI module of MSP430F2013
 *                 as slave and MSP430FG4618 as master in SPI mode.
 *              3. Connect the following jumpers on header 1 (H1) on the
 *                 experimenter's board.  [1-2], [3-4], [5-6], [7-8]
 *                          H1
 *                        _______
 *                      1|-------|2
 *                      3|-------|4
 *                      5|-------|6
 *                      7|-------|8
 *                       |_______|
 *                  Jumper must be present on PWR1, PWR2 and JP2.
 *
 *              4. Download and run this code by the connecting the FET debugger
 *                 to JTAG1 on the experimenter's board.
 *              5. Make sure the device selected is MSP430FG4618 in the General
 *                 Options of CCS.
 *
 * Input:       Character y or n from the user
 * Output:      Turn on or off the LED3 and display the status on HyperTerminal
 *----------------------------------------------------------------------------*/
#include  "msp430xG46x.h"
#include  <stdio.h>

#define LED_ON_STATE    0x31      // Character '1'
#define LED_OFF_STATE   0x30      // Character '0'
#define LED_NUL_STATE   0x00      // Character NULL - used for dummy write operation

#define LED_ON          0x01
#define LED_OFF         0x00

unsigned char ch;                 // Hold char from UART RX

char greetMessage1[] = "Press 'y' to turn ON and 'n' to turn OFF the LED";
char greetMessage2[] = "Type in 'y' or 'n'!";

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

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait for previous character to transmit
    UCA0TXBUF = c;                // Put character into tx buffer
}

void UART_sendMessage(char* message) {
    int i;
    for(i = 0; message[i] != 0; i++) {
        UART_putCharacter(message[i]);
    }
    UART_putCharacter('\n');       // Newline
    UART_putCharacter('\r');       // Carriage return
}

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;          // Set UC0TXD and UC0RXD to transmit and receive data
    UCA0CTL1 |= UCSWRST;           // Software reset
    UCA0CTL0 = 0;                  // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;          // Clock source SMCLK - 1048576 Hz
    UCA0BR0 = 54;                  // Baud rate - 1048576 Hz / 19200
    UCA0BR1 = 0;
    UCA0MCTL = 0x0A;               // Modulation
    UCA0CTL1 &= ~UCSWRST;          // Software reset
    IE2 |= UCA0RXIE;               // Enable USCI_A0 RX interrupt
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;      // Stop watchdog timer
    UART_setup();                  // Setup USCI_A0 module in UART mode
    SPI_setup();                   // Setup USCI_B0 module in SPI mode
    _EINT();                       // Enable global interrupts
    int z, i;

    for(z = 100; z > 0; z--);      // Delay to allow baud rate stabilize

    // Greeting Message
    UART_sendMessage(greetMessage1);
     
    while(1) {
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0

        switch (ch) {
        case  'y':
            SPI_setState(LED_ON_STATE);
            for(i = 1000; i > 0; i--);   // Delay
            UART_putCharacter(SPI_getState()); // Get the current state of LED and print
                                         // '1' - ON ; '0' - OFF
            break;
        case  'n':
            SPI_setState(LED_OFF_STATE);
            for(i = 1000; i > 0; i--);   // Delay
            UART_putCharacter(SPI_getState()); // Get the current state of LED and print
                                         // '1' - ON ; '0' - OFF
            break;
        default :
            UART_sendMessage(greetMessage2);
            break;
        }
    }
}

// Interrupt for USCI Rx
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIB0RX_ISR (void) {
    ch = UCA0RXBUF;                       // Character received is moved to a variable
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
