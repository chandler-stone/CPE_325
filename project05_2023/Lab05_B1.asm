; ------------------------------------------------------------------------------
; File:         Lab05_B1.asm
; Function:     Preforms exponentiation on a base and stores the first five results in an array
; Description:  B_HW_MUL is a subroutune that preforms exponentiation on numbers using a hardware peripheral
; Input:        R5 -- result array starting address
;				R6 -- exponent base
; Output:       The output is stored in memory at the result array
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 17, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def B_HW_MUL
B_HW_MUL:	mov		2(SP), R6			; getting the result array address from the stack
			mov		4(SP), R5			; getting the base from the stack
			clr		R7					; clearing R7 for use as exponentiation counter
			mov		#1, R8				; initializing R8 for use as exponent accumulator

loop:		mov		R5, &MPY			; moving the multiplicand into the hardware multiplier
			mov 	R8, &OP2			; moving the multiplier into the hardware multiplier
			nop							; waiting on the hardware multiplier
			nop							; waiting on the hardware multiplier
			nop							; waiting on the hardware multiplier
			mov 	RESLO, R8			; accumulating the result from the hardware multiplier
			mov		R8, 0(R6)			; moving the accumulated result to the array in memory
			add		#2, R6				; move to next addressable word for the array in memory
			inc		R7					; increment exponentiation counter
			cmp		#5, R7				; have we gotten to the highest exponent?
			jz		end					; if we have, jump to return from the subroutine
			jmp		loop				; calculate the next exponent

end:		ret							; return from the subroutine
			.end
