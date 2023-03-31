/*------------------------------------------------------------------------------
 * File:        Lab10_D1.c (CPE 325 Lab10 Demo code)
 * Function:    Measuring the temperature (MPS430FG4618)
 * Description: This C program samples the on-chip temperature sensor and
 *              converts the sampled voltage from the sensor to temperature in
 *              degrees Celsius and Fahrenheit. The converted temperature is
 *              sent to HyperTerminal over the UART by using RS-232 cable.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
 * Instructions:Set the following parameters in HyperTerminal
 *                  Port :        COM1
 *                  Baud rate :   38400
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
 *                      |     P2.4/UCA0TXD|------------>
 *                      |                 | 38400 - 8N1
 *                      |     P2.5/UCA0RXD|<------------
 *                      |                 |
 * Input:       Character Y or y or N or n
 * Output:      Displays Temperature in Celsius and Fahrenheit in HyperTerminal
 * Author:      Aleksandar Milenkovic, milenkovic@computer.org
 *------------------------------------------------------------------------------*/

#include  <msp430xG46x.h>
#include  <stdio.h>

char ch;                   // Holds the received char from UART
unsigned char rx_flag;     // Status flag to indicate new char is received

char gm1[] = "Hello! I am an MSP430. Would you like to know my temperature? (Y|N)";
char gm2[] = "Bye, bye!";
char gm3[] = "Type in Y or N!";

long int temp;                      // Holds the output of ADC
long int IntDegF;                   // Temperature in degrees Fahrenheit
long int IntDegC;                   // Temperature in degrees Celsius

char NewTem[25];

void UART_setup(void) {
    P2SEL |= BIT4+BIT5;             // Set UC0TXD and UC0RXD a
    UCA0CTL1 |= BIT0;               // software reset
    UCA0CTL0 = 0;                   // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;           // Clock source SMCLK - 1048576 Hz
    UCA0BR0 = 27;                   // Baud rate - 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL = 0x94;                // Modulation
    UCA0CTL1 &= ~BIT0;              // Software reset
    IE2 |= UCA0RXIE;                // Enable USCI_A0 RX interrupt
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG));    // Wait for TX to be ready
    UCA0TXBUF = c;                  // Put character into TX buffer
}

void sendMessage(char* msg, int len) {
    int i;
    for(i = 0; i < len; i++) {
        UART_putCharacter(msg[i]);
    }
    UART_putCharacter('\n');        // Newline
    UART_putCharacter('\r');        // Carriage return
}

void ADC_setup(void) {
    unsigned int i;
    ADC12CTL0 = SHT0_8 + REFON + ADC12ON; // (256*1/5MHz) > 30 us, 1.5 V
    ADC12CTL1 = SHP;                // Enable sample timer
    ADC12MCTL0 = INCH_10 + SREF_1;  // Channel 10, Vref+
    ADC12IE = ADC12IE + BIT0;       // Enable interrupt
    for (i = 0; i < 0x3600; i++);   // SW delay for reference start-up
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;         // Stop watchdog timer
    UART_setup();                     // Setup USCI_A0 module in UART mode
    ADC_setup();                      // Setup ADC12

    rx_flag = 0;                      // RX default state "empty"
    _EINT();                          // Enable global interrupts
    while(1) {
        sendMessage(gm1, sizeof(gm1));// Send a greetings message

        while(!(rx_flag&0x01));       // Wait for input
        rx_flag = 0;                  // Clear rx_flag
        sendMessage(&ch, 1);          // Send received char

        // Character input validation
        if ((ch == 'y') || (ch == 'Y')) {
            ADC12CTL0 |= ENC + ADC12SC; // Sampling and conversion start
            _BIS_SR(CPUOFF + GIE);      // LPM0 with interrupts enabled
            //  oC = ((x/4095)*1500mV)-986mV)*1/3.55mV
            //  IntDegC = (ADCMEM0 - 2692)* 423/4095
            IntDegC = ((temp - 2692) * 423)/4095;
            IntDegF = IntDegC*(9/5) + 32;
            // Printing the temperature on HyperTerminal/Putty
            sprintf(NewTem, "T(F)=%ld\tT(C)=%ld\n", IntDegF, IntDegC);
            sendMessage(NewTem, sizeof(NewTem));
        }
        else if ((ch == 'n') || (ch == 'N')) {
            sendMessage(gm2, sizeof(gm2));
            break;                      // Get out
        }
        else {
            sendMessage(gm3, sizeof(gm3));
        }
    }                                   // End of while
    while(1);                           // Stay here forever
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR (void) {
    ch = UCA0RXBUF;                 // Copy the received char
    rx_flag = 0x01;                 // Signal to main 
    LPM0_EXIT;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR (void) {
    temp = ADC12MEM0;               // Move results, IFG is cleared
    _BIC_SR_IRQ(CPUOFF);            // Clear CPUOFF bit from 0(SR)
}
