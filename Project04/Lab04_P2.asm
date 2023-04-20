; ------------------------------------------------------------------------------
; File:         Lab04_P2.asm
; Function:     Replaces lowercase characters in a given string with similar uppercase ones
; Description:  Program traverses an input array of characters
;               to detect a lowercase character; exits when a NULL is detected
; Input:        The input string specified in myStr
; Output:       The port P3OUT displays the number of characters swaped in the string
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         September 16, 2022
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-------------------------------------------------------------------------------
		.data
myStr:  .string "abcdefghijklmnopqrstuvwxyz||~~1686413200", ''
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
main:   mov.w   #myStr, R4              ; Load the starting address of the string into R4
        clr.b   R5                      ; Register R5 will serve as a counter

gnext:  mov.b   @R4+, R6                ; Get a new character
        cmp     #0, R6                  ; Is it a null character
        jeq     lend                    ; If yes, go to the end
        cmp.b   #'a', R6                ; Is it at least an ascii 'a' character
        jn      gnext                   ; If not, go to next letter
        cmp.b	#'{', R6				; Is it at most an ascii 'z' character
        jge		gnext					; If not, get next letter
        inc.w   R5                      ; If yes, increment counter
        sub.w	#0x20, R6				; Subtract 0x20 to get uppercase character
        mov.b	R6, -1(r4)				; Replace lowercase character in memory
        jmp     gnext                   ; Go to the next letter

lend:   mov.b   R5, &P3OUT               ; Write result in P1OUT (not visible on port pins)
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
