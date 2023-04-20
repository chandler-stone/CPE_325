; ------------------------------------------------------------------------------
; File:         Lab04_P2.asm
; Function:     determines whether a number is odd or even
; Description:  Program determines whether a number is even or odd, writes string in memory accordingly
; Input:        The input integer specified by VarA
; Output:       The variable mystr is changed in memory
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 10, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
;-------------------------------------------------------------------------------
		.data
VarA:	.int 21
myStr:  .string "xxxx", ''
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
main:   mov.w	VarA, R15				; Load the integer into R15
		mov.w   #myStr, R4              ; Load the starting address of the string into R4
		and.w	#1, R15					; Test the LSB
		jz		even					; jump to even

odd:	mov.b	#'O', 0(R4)
		mov.b	#'d', 1(R4)
		mov.b	#'d', 2(R4)
		mov.b	#'', 3(R4)
		jmp		end

even:  	mov.b	#'E', 0(R4)
		mov.b	#'v', 1(R4)
		mov.b	#'e', 2(R4)
		mov.b	#'n', 3(R4)

end:	bis.w   #LPM4, SR               ; LPM4
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
