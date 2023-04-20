/******************************************************************************
 File:          ToggleLEDs.c
 Description:   Program toggles LED1 and LED2 by 
                xoring port pins inside of an infinite loop.
 Board:         MSP430FG461x/F20XX Experimenter Board
 Clocks:        ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz

      MSP430xG461x
     -----------------
 /|\ |            XIN|-
  |  |               | 32kHz
  |--|RST        XOUT|-
     |               |
     |           P2.1|- Led2
     |           P2.2|- Led1
     |               |

 Authors:   A. Milenkovic, milkovic@computer.org
 Date:      September 2010
******************************************************************************/
#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
    P2DIR |= (BIT1 | BIT2);
    P2OUT = 0x00;               // Clear output port P2, P2OUT=0000_0000b 

    for(;;) {
        unsigned int i;         // Toggle P2.1 and P2.2 using exclusive-OR
        P2OUT ^= (BIT1 | BIT2); // Software delay (13 cc per iteration)
        for(i = 0; i < 50000; i++);
        /* Total delay on average 13 cc*50,000 = 750,000; 750,000 * 1us = 0.75 s */
    }
    return 0;
}
