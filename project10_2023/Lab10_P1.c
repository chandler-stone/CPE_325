/*------------------------------------------------------------------------------
 * File:        Lab10_D2.c (CPE 325 Lab10 Demo code)
 * Function:    Interfacing thumbstick (MPS430FG4618)
 * Description: This C program interfaces with a thumbstick sensor that has
 *              x (HORZ) and y (VERT) axis and outputs from 0 to 3V.
 *              The value of x and y axis
 *              is sent as the percentage of power to the UAH Serial App.
 * Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
 *              An external watch crystal between XIN & XOUT is required for ACLK
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
 * Input:       Connect thumbstick to the board
 * Output:      Displays % of power in UAH serial app
 * Author:      Micah Harvey
 *------------------------------------------------------------------------------*/

#include <msp430xG46x.h>
#include <math.h>


volatile long int ADCXval, ADCYval, ADCZval;
volatile float Xgs, Ygs, Zgs, Tgs, Tgs2, Deg;

const float A = 0.9398086351723256;
const float B = 0.38928148272372454;
const float Y = 0.298706187614;

void TimerA_setup(void) {
    TACCR0 = 6554;                      // 6554 / 32768 Hz = 0.2s
    TACTL = TASSEL_1 + MC_1;            // ACLK, up mode
    TACCTL0 = CCIE;                     // Enabled interrupt
}

void ADC_setup(void) {
    int i =0;

    P6DIR &= ~BIT3 + ~BIT7 + ~BIT5;             // Configure P6.3 and P6.7 as input pins
    P6SEL |= BIT3 + BIT7 + BIT5;               // Configure P6.3 and P6.7 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC; // configure ADC converter
    ADC12CTL1 = SHP + CONSEQ_1;         // Use sample timer, single sequence
    ADC12MCTL0 = INCH_3;                // ADC A3 pin - Accelerometer X-axis
    ADC12MCTL1 = INCH_7;                // ADC A7 pin - Accelerometer Y-axis
    ADC12MCTL2 = INCH_5 + EOS;          // ADC A5 pin - Accelerometer Z-axis
                                        // EOS - End of Sequence for Conversions
    ADC12IE |= 0x02;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ENC;                   // Enable conversions
}

void UART_putCharacter(char c) {
    while(!(IFG2 & UCA0TXIFG));         // Wait for previous character to be sent
    UCA0TXBUF = c;                      // Send byte to the buffer for transmitting
}

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;               // Set up Rx and Tx bits
    UCA0CTL0 = 0;                       // Set up default RS-232 protocol
    UCA0CTL1 |= BIT0 + UCSSEL_2;        // Disable device, set clock
    UCA0BR0 = 27;                       // 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL = 0x94;
    UCA0CTL1 &= ~BIT0;                  // Start UART device
}

void sendData(void) {
    int i;
    Xgs = (((ADCXval*3.0/4095) - 1.5)/0.3);    // Calculate gravity outputs (in gs)
    Ygs = (((ADCYval*3.0/4095) - 1.5)/0.3);
    Zgs = (2*(((ADCZval*3.0/4095) - 1.5)/0.3) + 4.9);
    Tgs = sqrt(Xgs*Xgs + Ygs*Ygs + Zgs*Zgs);

    Deg = 180*atan2f(Zgs, Xgs)/3.1415926535879323 + 90;

    if ((fabs(Xgs) > fabs(Ygs)) && (fabs(Xgs) > fabs(Zgs))) {
        if (fabs(Ygs) > fabs(Zgs)) {
            Tgs2 = fmaxf(fabs(Xgs), fabs(Xgs)*Y + fabs(Ygs)*B + fabs(Zgs)*A);
        }
        else if (fabs(Zgs) > fabs(Ygs)) {
            Tgs2 = fmaxf(fabs(Xgs), fabs(Xgs)*Y + fabs(Zgs)*B + fabs(Ygs)*A);
        }
    }
    else if ((fabs(Ygs) > fabs(Xgs)) && (fabs(Ygs) > fabs(Zgs))) {
        if (fabs(Xgs) > fabs(Zgs)) {
            Tgs2 = fmaxf(fabs(Ygs), fabs(Ygs)*Y + fabs(Xgs)*B + fabs(Zgs)*A);
        }
        else if (fabs(Zgs) > fabs(Xgs)) {
            Tgs2 = fmaxf(fabs(Ygs), fabs(Ygs)*Y + fabs(Zgs)*B + fabs(Xgs)*A);
        }
    }
    else if ((fabs(Zgs) > fabs(Ygs)) && (fabs(Zgs) > fabs(Xgs))) {
        if (fabs(Ygs) > fabs(Xgs)) {
            Tgs2 = fmaxf(fabs(Zgs), fabs(Zgs)*Y + fabs(Ygs)*B + fabs(Xgs)*A);
        }
        else if (fabs(Xgs) > fabs(Ygs)) {
            Tgs2 = fmaxf(fabs(Zgs), fabs(Zgs)*Y + fabs(Xgs)*B + fabs(Ygs)*A);
        }
    }

    // Use character pointers to send one byte at a time
    char *xpointer=(char *)&Xgs;
    char *ypointer=(char *)&Ygs;
    char *zpointer=(char *)&Zgs;
    char *tpointer=(char *)&Tgs;
    char *t2pointer=(char *)&Tgs2;
    char *dpointer=(char *)&Deg;

    UART_putCharacter(0x55);            // Send header
    for(i = 0; i < 4; i++) {            // Send x acceleration - one byte at a time
        UART_putCharacter(xpointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send y acceleration - one byte at a time
        UART_putCharacter(ypointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send z acceleration - one byte at a time
        UART_putCharacter(zpointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send total acceleration - one byte at a time
        UART_putCharacter(tpointer[i]);
    }
    for (i = 0; i < 4; i++) {           // Send total acceleration - one byte at a time
        UART_putCharacter(t2pointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send Degree deviation - one byte at a time
        UART_putCharacter(dpointer[i]);
    }

}

void main(void) {
    WDTCTL = WDTPW +WDTHOLD;            // Stop WDT
    TimerA_setup();                     // Setup timer to send ADC data
    ADC_setup();                        // Setup ADC
    UART_setup();                       // Setup UART for RS-232
    _EINT();
    P2DIR |= BIT2 + BIT1;              // P2.2 output
    while (1){
        ADC12CTL0 |= ADC12SC;               // Start conversions
        if (Deg > 15) {
            P2OUT |= BIT2;
            P2OUT &= ~BIT1;
        }
        else if (Deg < -15) {
            P2OUT |= BIT1;
            P2OUT &= ~BIT2;
        }
        else {
            P2OUT &= ~BIT1;
            P2OUT &= ~BIT2;
        }
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCXval = ADC12MEM0;                  // Move results, IFG is cleared
    ADCYval = ADC12MEM1;
    ADCZval = ADC12MEM2;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timerA_isr() {
    sendData();                           // Send data to serial app
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
