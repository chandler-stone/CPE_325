/*------------------------------------------------------------------------------
 * File:        BlinkTheLEDs.c
 * Function:    Blink LEDs on MSP-EXP430F5529LP
 * Description: This program toggles the red and green LEDs on MSP-EXP430F5529LP
 * Clocks:              MSP-EXP430F5529LP
 *                    --------------------
 *                 /|\|               XIN|-
 *                  | |                  | 32kHz
 *                  --|RST           XOUT|-
 *                    |                  |
 *                    |              P1.0|---> Red LED
 *                    |                  |
 *                    |              P4.7|---> Green LED
 * Input:       None
 * Output:      LEDs on MSP-EXP430F5529LP
 * Author(s):   Aleksandar Milenkovic, milenkovic@computer.org
 * Date:        August 2, 2020
 * ---------------------------------------------------------------------------*/
#include <msp430.h>

#define REDLED 0x01             // Mask for BIT0 = 0000_0001b
#define GREENLED 0x80           // Mask for BIT7 = 1000_0000b

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    P1DIR |= REDLED;            // Configure P1.0 as output
    P4DIR |= GREENLED;          // Configure P4.7 as output

    P1OUT = P1OUT & ~REDLED;    // P1.0 is cleared
    P4OUT = P4OUT | GREENLED;   // P4.7 is on
    while(1)                    // Infinite loop
    {
        __delay_cycles(500000); // Delay of ~250 ms
        P1OUT ^= REDLED;        // Toggle P1.0 (REDLED)
        P4OUT ^= GREENLED;      // Toggle P4.7 (GREENLED)
    }
}
