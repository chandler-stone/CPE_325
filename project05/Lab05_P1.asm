;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .ref	SW_MUL					; Including the software multiplier routine.
            .ref	HW_MUL					; Including the hardware multiplier routine.
            .ref	B_SW_MUL				; Including the bonus software multiplier routine.
        	.ref	B_HW_MUL				; Including the bonus hardware multiplier routine.
;-------------------------------------------------------------------------------
numA:		.int	50						; Declare Multiplicand
numB:		.int	255               		; Declare Multiplier
arr1:		.int	2, 2, 1, 1, 1, 1, 1, 1
arr2:		.int	2, 2, 8, 2, 2, 6, 2, -9
arrLength:	.int	8

			.data
result1:	.int	0
result2:	.int	0
resultB1:	.int	0
resultB2:	.int	0
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT:     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:		push	numA
			push 	numB
			push	#result1
			call	#HW_MUL

			push	numA
			push	numB
			push	#result2
			call	#SW_MUL

			push	#arr1
			push	#arr2
			push	arrLength
			push	#resultB1
			call	#B_SW_MUL

			push	#arr1
			push	#arr2
			push	arrLength
			push	#resultB2
			call	#B_HW_MUL
			nop
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
            
