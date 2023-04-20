; ------------------------------------------------------------------------------
; File:         Lab04_B.asm
; Function:     Counts the number of characters E in a given string
; Description:  Program traverses an input array of characters
;               to detect a character 'E'; exits when a NULL is detected
; Input:        The input string specified in myStr
; Output:       The port P1OUT displays the number of E's in the string
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         September 16, 2022
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-------------------------------------------------------------------------------
		.data
myStr:  .string "Welcome to MSP430!", ''
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
        cmp.b   #'{', R6                ; Is it at least an ascii '{' character
        jge     gCalc                   ; If so, delete it
        cmp.b	#'a', R6				; Is it at least 'a'
        jge		gnext					; If so, get next character
        cmp.b   #'[', R6                ; Is it at least 'Z'
        jge     gCalc                   ; If so delete it
        cmp.b	#'A', R6				; Is it at least 'A'
        jge		gnext					; If so, get the next character
	  	cmp.b   #':', R6                ; Is it at least an ascii ':' character
		jge		gCalc					; If so, delete it
		cmp.b 	#'0', R6				; Is it at least '0'
		jge		gnext					; If so, get the next character


gCalc:	inc.w 	R5						; increase deleted characters incrementer
		mov.w	R4, R15
loop:	cmp		#0, 0(R15)
		jeq		adNull
		mov.b	0(R15), -1(R15)			; move next string member into current character
		add.w	#1, R15
		jmp		loop

adNull:	mov.b	#0x0, -1(R15)
		jmp		gnext

lend:   sub.b	R5, R4
		mov.b	#'', R4
		mov.b   R5, &P4OUT              ; Write result in P1OUT (not visible on port pins)
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
