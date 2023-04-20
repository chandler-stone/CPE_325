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
base:		.int	2						; Base for exponentiation
arr1:		.int	2, 2, 1, 1, 1, 1, 1, 1	; Multiplicand array
arr2:		.int	2, 2, 8, 2, 2, 6, 2, -9	; Multiplier array

			.data
result1:	.int	0, 0, 0, 0, 0, 0, 0, 0	; result array for hardware multiplier
result2:	.int	0, 0, 0, 0, 0, 0, 0, 0	; result array for software shift and add
resultB1:	.int	0, 0, 0, 0, 0			; result array for bonus hardware multiplier
resultB2:	.int	0, 0, 0, 0, 0			; result array for bonus software shift and add
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP        ; Initialize stackpointer
StopWDT:     mov.w   #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:		push	#arr1					; pass array 1 address parameter through stack
			push 	#arr2					; pass array 2 address parameter through stack
			push	#result1				; pass result array address parameter through stack
			call	#HW_MUL					; call hardware multiplier subroutine
			add		#6, SP					; collapse stack

			push	#arr1					; pass array 1 address parameter through stack
			push 	#arr2					; pass array 2 address parameter through stack
			push	#result2				; pass result array address parameter through stack
			call	#SW_MUL					; call software shift and add subroutine
			add		#6, SP					; collapse stack

			push	base					; pass base number parameter through stack
			push	#resultB1				; pass result array address parameter through stack
			call	#B_HW_MUL				; call bonus hardware multiplier subroutine
			add		#4, SP					; collapse stack

			push	base					; pass base number parameter through stack
			push	#resultB2				; pass result array address parameter through stack
			call	#B_SW_MUL				; call bonus software shift and add subroutine
			add		#4, SP					; collapse stack
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
            
