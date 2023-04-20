/*----------------------------------------------------------------------------------
 * File:        Lab03_P1.c
 * Description: This program blinks LED1 or LED 2 based on switch input.
 *
 * Board:       5529
 * Input:       No inputs.
 * Output:      Lit LEDs based on switch input.
 * Author:      Chandler Stone
 * Date:        February 3, 2023
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

    P1OUT &= ~REDLED;
    P4OUT &= ~GREENLED; //turn off green led

    unsigned int i = 0; //incrementor for de-bouncing loops

    // beginning loop for initial LED conditions
    while(1) {
        P1OUT |= REDLED;
        if((S1) == 0 || (S2) == 0) {
            for(i = 3000; i > 0; i--)
                if((S1) == 0 || (S2) == 0) {
                    goto label;
                }
        }
    }

    //infinite loop for embedded system.
    label:while(1) {

        //if statement block for bonus question
        bonus:if((S1) == 0 && (S2) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if((S1) == 0 && (S2) == 0) {
                P1OUT &= ~REDLED; //turn off red led (saves other outputs)
                P4OUT &= ~GREENLED; //turn off green led (saves other outputs)
            }
            while((S1) == 0 && (S2) == 0) {
                P4OUT ^= GREENLED; //toggle green led
                __delay_cycles(125000); //delay of 0.25 seconds
                P4OUT ^= GREENLED; //toggle green led
                P1OUT ^= REDLED; //toggle red led
                __delay_cycles(125000); //delay of 0.25 seconds
            }
            P1OUT &= ~REDLED; //turn off red led (saves other outputs)
            P4OUT &= ~GREENLED; //turn off green led (saves other outputs)
        }

        //if statement block for holding red light
        else if((S1) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if((S1) == 0) {
                P4OUT |= GREENLED; //turn on green led (saves other outputs)
                P1OUT |= REDLED; //turn on red led (saves other outputs)
            }
            while((S1) == 0) {
                if ((S2) == 0) {
                    goto bonus;
                }
            }
            P1OUT &= ~REDLED; //turn off red led (saves other outputs)
            P4OUT &= ~GREENLED; //turn off green led
        }


        //if statement block for blinking green led
        else if((S2) == 0) {
            for(i = 3000; i > 0; i--) //de-bounce
            if ((S2) == 0) {
                P1OUT &= ~REDLED; //turn off red led (saves other outputs)
                P4OUT &= ~GREENLED; //turn off green led
            }
            while((S2) == 0) {
                if ((S1) == 0) {
                    goto bonus;
                }
                P4OUT ^= GREENLED; //toggle green led
                __delay_cycles(125000); // delay of 0.25 seconds
                P1OUT ^= REDLED;
            }
            P1OUT &= ~REDLED;
            P4OUT &= ~GREENLED;
        }
    }
}
