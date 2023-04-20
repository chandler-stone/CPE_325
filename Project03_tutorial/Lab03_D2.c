/*------------------------------------------------------------------------------
 * File:        Lab03_D2.c
 * Function:    Blinking LED1 and LED2
 * Description: This C program toggle LED1 and LED2 at 1Hz by xoring P1.0 and
 *              P4.7 inside a loop. The LEDs are on when P1.0=1 and P4.7=1.
 *              The LED1 is initialized to be off and LED2 to be on.
 * Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO (~1 MHz)
 *                      MSP-EXP430F5529LP
 *                      -----------------
 *                   /|\|               |
 *                    | |               |
 *                    --|RST            |
 *                      |           P1.0|-->LED1(RED)
 *                      |           P4.7|-->LED2(GREEN)
 *                      |               |
 * Input:       None
 * Output:      LED1 and LED2 blink alternately at a frequency of 1 Hz
 * Author(s):   Aleksandar Milenkovic, milenkovic@computer.org
 *              Prawar Poudel, prawar.poudel@uah.edu
 * Date:        
 * ---------------------------------------------------------------------------*/
#include <msp430.h>

#define REDLED 0x01                     // Mask for BIT0 = 0000_0001b
#define GREENLED 0x80                   // Mask for BIT7 = 1000_0000b

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    P1DIR |= REDLED;                    // Set P1.0 to output direction
    P4DIR |= GREENLED;                  // Set P4.7 to output direction
    P1OUT &= ~REDLED;                   // LED1 is OFF
    P4OUT |= GREENLED;                  // LED2 is ON
    unsigned int i = 0;
    while(1)                            // Infinite loop
    {
        for (i = 0; i < 50000; i++);    // Delay 0.5s
                                        // 0.5s on, 0.5s off => 1/(1s) = 1Hz
        P1OUT ^= REDLED;                // Toggle LED1
        P4OUT ^= GREENLED;              // Toggle LED2
    }
}
