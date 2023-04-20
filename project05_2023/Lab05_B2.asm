; ------------------------------------------------------------------------------
; File:         Lab05_B2.asm
; Function:     Preforms exponentiation on a base and stores the first five results in an array
; Description:  B_SW_MUL is a subroutune that preforms exponentiation on numbers using shift and add
; Input:        R5 -- result array starting address
;				R6 -- exponent base
; Output:       The output is stored in memory at the result array
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 17, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
        .ref	HW_MUL
;-------------------------------------------------------------------------------
        .def B_SW_MUL
B_SW_MUL:	mov		2(SP), R6			; getting the result array address from the stack
			mov		4(SP), R5			; getting the base from the stack
			clr		R7					; clearing R7 for use as exponentiation counter
			mov		#1, R8				; initializing R8 for use as shift and add multiplier
			clr		R9					; clearing R9 for use as exponentiation accumulator
			clr 	R15					; clearing R15 for use as integer size counter

loop:		cmp		#16, R15			; have we reached the end of the shift and add?
			jz		store				; if we have, jump to store the value
			bit		#1, R8				; is the LSB of the multiplier 1?
			jz		skipadd				; if it isn't, skip adding
			add		R5, R9				; if it is, add the multiplicand to the accumulator
skipadd:	rla		R5					; rotate the multiplicand for shift and add
			rra		R8					; rotate the multiplier for shift and add
			add		#1, R15				; increase integer counter
			jmp		loop

store:		mov 	R9, 0(R6)			; move the accumulated value to memory
			mov		4(SP), R5			; move the base value to the multiplicand
			mov		R9, R8				; move the accumulated value to the multiplier
			clr		R9					; clear accumulator
			clr		R15					; clear the integer size counter
			add		#2, R6				; moving to next addressable word for the array in memory
			inc		R7					; increment the array counter
			cmp		#5, R7				; are we at the end of the array?
			jz		end					; if we are, jump to return from the subroutine
			jmp		loop				; calculate the next exponent

end:		ret
			.end
