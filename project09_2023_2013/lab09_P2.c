#include "msp430F2013.h"

#define LED_OFF_STATE   0x30      // Character '0'

#define SET_BUSY_FLAG()   P1OUT |= 0x10
#define RESET_BUSY_FLAG() P1OUT &= ~0x10

unsigned char LEDState ;
unsigned char NextState;

unsigned char P = 1;
unsigned char PCounter = 0;

void SPI_setup(void) {
    USICTL0 |= USISWRST;          // Set UCSWRST -- needed for re-configuration process
    USICTL0 |= USIPE5 + USIPE6 + USIPE7 + USIOE; // SCLK-SDO-SDI port enable,MSB first
    USICTL1 = USIIE;              // USI  Counter Interrupt enable
    USICTL0 &= ~USISWRST;         // **Initialize USCI state machine**
}

void SPI_initComm(void) {
    USICNT = 8;                   // Load bit counter, clears IFG
    USISRL = LEDState;            // Set LED state
    RESET_BUSY_FLAG();            // Reset busy flag
}

void initLED(unsigned char state) {
    P1DIR |= BIT0;                // P1.0 as output - LED3
    P1DIR |= BIT4;                // P1.4 as output - Busy flag
    P1OUT &= ~BIT0;               // turns LED3 off
}

void systemInitialize() {
    WDTCTL = WDT_MDLY_32;     // Stop watchdog timer
    IE1 |= WDTIE;                   // Enable WDT interrupt
    BCSCTL1 = CALBC1_1MHZ;        // Set DCO
    DCOCTL = CALDCO_1MHZ;
}

void main(void) {
    systemInitialize();
    initLED(LED_OFF_STATE);       // LED state initialization
    SPI_setup();                  // Setup USI module in SPI mode
    SPI_initComm();               // Initialization communication

    while(1) {
        _BIS_SR(LPM0_bits + GIE );// Enter LPM0 with interrupt

        switch (NextState) {
            case 0x00:            // Dummy operation; no change in the state
                USISRL = P;        // Prepares reply to master with new state
                break;
            default  :
                P = NextState;   // New state
                break;
         }
        RESET_BUSY_FLAG();        // Clears busy flag - ready for new communication
   }
}

#pragma vector = USI_VECTOR
__interrupt void USI_ISR(void) {
    SET_BUSY_FLAG();              // Set busy flag - busy with new communication
    NextState = USISRL;           // Read new command
    USICNT = 8;                   // Load bit counter for next TX
    _BIC_SR_IRQ(LPM0_bits);       // Exit from LPM0 on RETI
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void) {
    if (P == 255) {
        P1OUT &= ~BIT0;
    }
    else if (PCounter >= P) {
        P1OUT |= BIT0;
        PCounter = 0;
    }
    else {
        P1OUT &= ~BIT0;
        PCounter++;
    }
}
