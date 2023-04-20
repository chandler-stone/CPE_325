/*----------------------------------------------------------------------------------
 * File:        Lab03_P1.c
 * Description: This program blinks LED1 or LED 2 based on switch input.
 *
 * Board:       5529
 * Input:       No inputs.
 * Output:      Lit LEDs based on switch input.
 * Author:      Chandler Stone
 * Date:        September 10, 2022
 *----------------------------------------------------------------------------*/
#include <msp430.h>

#define S1 P2IN&BIT1 // xxxx_xxxx && 0000_0010 = 0000_0010
#define S2 P1IN&BIT1 // xxxx_xxxx && 0000_0010 = 0000_0010
#define REDLED 0x01 // 0000_0001
#define GREENLED 0x80 //1000_0000

int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

    P1DIR = REDLED + BIT1; //Sets direction register to 0000_0001
    P4DIR = GREENLED; //Sets direction register to 1000_0000
    P2DIR = BIT1;

    P2REN = BIT1; //enables resistor 0000_0010 on Port 2
    P1REN = BIT1; //enables resistor 0000_0010 on Port 1

    P2OUT |= BIT1;
    P1OUT |= BIT1;

    unsigned int i = 0; //incrementor for de-bouncing loops

    //infinite loop for embedded system.
    while(1) {

        //if statement block for bonus question
        if((S1) == 0 && (S2) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if((S1) == 0 && (S2) == 0) {
                P1OUT &= ~REDLED; //turn off red led (saves other outputs)
                P4OUT &= ~GREENLED; //turn off green led (saves other outputs)
            }
            while((S1) == 0 && (S2) == 0) {
                __delay_cycles(500000); //delay of 1 second
                P4OUT ^= GREENLED; //toggle green led
                P1OUT ^= REDLED; //toggle red led
            }
            P1OUT &= ~REDLED; //turn off red led (saves other outputs)
            P4OUT &= ~GREENLED; //turn off green led (saves other outputs)
        }

        //if statement block for holding red light
        else if((S1) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if((S1) == 0) {
                P4OUT &= ~GREENLED; //turn off green led
                P1OUT |= REDLED; //turn on red led (saves other outputs)
            }
            while((S1) == 0);
            P1OUT &= ~REDLED; //turn off red led (saves other outputs)
        }


        //if statement block for blinking green led
        else if((S2) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if((S2) == 0) {
                P1OUT &= ~REDLED; //turn off red led
            }
            while((S2) == 0) {
                __delay_cycles(250000); // delay of 0.5 seconds
                P4OUT ^= GREENLED; //toggle green led
            }
            P4OUT &= ~GREENLED; //turn off green led
        }

        //else statement block for blinking red led
        else {
            __delay_cycles(100000); //delay of 0.2 seconds
            P1OUT ^= REDLED; //toggle red led
        }
    }
}
