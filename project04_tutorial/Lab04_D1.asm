; ------------------------------------------------------------------------------
; File:         Lab04_D1.asm
; Function:     Counts the number of characters E in a given string
; Description:  Program traverses an input array of characters
;               to detect a character 'E'; exits when a NULL is detected    
; Input:        The input string specified in myStr
; Output:       The port P1OUT displays the number of E's in the string
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         August 14, 2008
; Revised:      August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-------------------------------------------------------------------------------
myStr:  .string "HELLO WORLD, I AM THE MSP430!", ''
        ; .string does not add NULL at the end of the string;
        ; '' ensures that a NULL follows the string.
        ; You can alternatively use .cstring "HELLO WORLD, I AM THE MSP430!"
        ; that adds a NULL character at the end of the string automatically.
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
main:   ; bis.b   #0FFh, &P1DIR         ; Do not output the result on port pins
        mov.w   #myStr, R4              ; Load the starting address of the string into R4
        clr.b   R5                      ; Register R5 will serve as a counter
gnext:  mov.b   @R4+, R6                ; Get a new character
        cmp     #0, R6                  ; Is it a null character
        jeq     lend                    ; If yes, go to the end
        cmp.b   #'E', R6                ; Is it an 'E' character
        jne     gnext                   ; If not, go to the next
        inc.w   R5                      ; If yes, increment counter
        jmp     gnext                   ; Go to the next character

lend:   mov.b   R5,&P1OUT               ; Write result in P1OUT (not visible on port pins)
        bis.w   #LPM4, SR               ; LPM4
        nop                             ; Required only for debugger
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
