; ------------------------------------------------------------------------------
; File:         Lab04_B.asm
; Function:     Preforms addition and subtraction on single digit numbers
; Description:  Program traverses an input array of characters
;               to detect a number; exits when a NULL is detected
; Input:        The input string specified in myStr
; Output:       The integer sum is written in memory
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 10, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-------------------------------------------------------------------------------
		.data
sum:	.int 0
myStr:  .string "-3+5-4", ''
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
        clr.b   R5                      ; Register R5 will serve as an accumulator
        mov.w	#sum, R15				; Load address to store memory

gnext:  mov.b   @R4+, R6                ; Get a new character
        cmp     #0, R6                  ; Is it a null character
        jeq     end                     ; If yes, go to the end
        cmp.b   #'-', R6                ; Is it an ascii '-' character
        jz      sub	                    ; If so subtract
        cmp.b	#'+', R6				; Is it an ascii '+' character
        jz		add						; If so, add
        sub.w	#1, R4
        jmp		add

sub:	mov.b   @R4+, R6						; get next item in array
		cmp.b	#'0', R6
		jz		gnext
		cmp.b	#'1', R6
		jz		sub1
		cmp.b	#'2', R6
		jz		sub2
		cmp.b	#'3', R6
		jz		sub3
		cmp.b	#'4', R6
		jz		sub4
		cmp.b	#'5', R6
		jz		sub5
		cmp.b	#'6', R6
		jz		sub6
		cmp.b	#'7', R6
		jz		sub7
		cmp.b	#'8', R6
		jz		sub8
		cmp.b	#'9', R6
		jz		sub9
		jmp gnext

sub1:	sub.w	#1,	R5
		jmp		gnext

sub2:	sub.w	#2,	R5
		jmp		gnext

sub3:	sub.w	#3,	R5
		jmp		gnext

sub4:	sub.w	#4,	R5
		jmp		gnext

sub5:	sub.w	#5,	R5
		jmp		gnext

sub6:	sub.w	#6,	R5
		jmp		gnext

sub7:	sub.w	#7,	R5
		jmp		gnext

sub8:	sub.w	#8,	R5
		jmp		gnext

sub9:	sub.w	#9,	R5
		jmp		gnext


add:	mov.b   @R4+, R6						; get next item in array
		cmp.b	#'0', R6
		jz		gnext
		cmp.b	#'1', R6
		jz		add1
		cmp.b	#'2', R6
		jz		add2
		cmp.b	#'3', R6
		jz		add3
		cmp.b	#'4', R6
		jz		add4
		cmp.b	#'5', R6
		jz		add5
		cmp.b	#'6', R6
		jz		add6
		cmp.b	#'7', R6
		jz		add7
		cmp.b	#'8', R6
		jz		add8
		cmp.b	#'9', R6
		jz		add9
		jmp gnext

add1:	add.w	#1,	R5
		jmp		gnext

add2:	add.w	#2,	R5
		jmp		gnext

add3:	add.w	#3,	R5
		jmp		gnext

add4:	add.w	#4,	R5
		jmp		gnext

add5:	add.w	#5,	R5
		jmp		gnext

add6:	add.w	#6,	R5
		jmp		gnext

add7:	add.w	#7,	R5
		jmp		gnext

add8:	add.w	#8,	R5
		jmp		gnext

add9:	add.w	#9,	R5
		jmp		gnext


end:    mov.w	R5, 0(R15)
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
