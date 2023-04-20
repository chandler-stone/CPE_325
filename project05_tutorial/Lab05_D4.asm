; ------------------------------------------------------------------------------
; File:         Lab05_D4.asm
; Function:     Multiply two integers
; Description:  This program defines two integer variables
;               and performs multiplication of these two intergers and
;               and stores the result in another variable
; Clocks:       ACLK = 32.768kHz, MCLK = SMCLK = 1 Mhz
;                       MSP-EXP430F5529LP
;                      --------------------
;                   /|\|               XIN|-
;                    | |                  | 32kHz
;                    --|RST           XOUT|-
;                      |                  |
;                      |                  |
;                      |                  |
;                      |                  |
; Input:        None
; Output:       None
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         September 14, 2018
; Modified:     Prawar Poudel, prawar.poudel@uah.edu
; Date:         August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-----------------------------------------------------------------------------
;            USER DATA
;-----------------------------------------------------------------------------
        .data                           ; So that it goes to data section
val1:   .int    45                      ; val1 is an integer whose value is 45
val2:   .int    3                       ; val2 is an integer whose value is 3
val3:   .int    0                       ; val3 will hold result of val1*val2
;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory.
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section.
        .retainrefs                     ; And retain any sections that have
                                        ; references to current section.
;-------------------------------------------------------------------------------
RESET:  mov.w   #__STACK_END,SP         ; Initialize stack pointer
        mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
        mov     val1, &MPY              ; moves val1 to R5
        mov     val2, &OP2              ; moves val2 to R6
; Since we have both numbers, let us get the result after the three clock cycles
; required for for 16X16 hardware multiplication
        nop
        nop
        nop
        mov     RESLO, &val3
        jmp     $                       ; Infinite loop
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
        .end
