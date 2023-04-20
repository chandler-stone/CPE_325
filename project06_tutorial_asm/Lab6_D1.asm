;-------------------------------------------------------------------------------
;   File:        Lab6_D1.asm
;   Description: The program toggles LEDs periodically.
;                The LEDs are initialized off. An endless loop is entered.
;                A SWDelay1 loop creates 1s delay before toggling the LEDs.
;                LEDs will toggle: off for 1s and on for 1s.
;
;   Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO = 2^20=1,048,576 Hz
;   Platform:    TI Experimenter's Board
;
;                 MSP430xG461x
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |             P2.2|-->LED1(GREEN)
;            |             P2.1|-->LED2(YELLOW)
;
;   Author:  Aleksandar Milenkovic, milenkovic@computer.org
;   Date:    September 14, 2018
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:      mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
Setup:      bis.b   #0x06,&P2DIR            ; Set P2.2 and P2.1 to output
                                            ; direction (0000_0110)
            bic.b   #0x04,&P2OUT             ; Set P2OUT to 0x0000_0100 (LEDS off)
InfLoop:    mov.w    #0xFFFF, R5            ; Software delay (65,535*16cc/2^20 ~ 1s)
SWDelay1:   nop                             ; 1cc (total delay is 16 cc)
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            dec.w  R5                       ; 1cc
            jnz    SWDelay1                 ; 2cc
            xor.b  #0x06, P2OUT             ; toggle LEDs
            jmp    InfLoop                  ; goto InfLoop
                                            
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
