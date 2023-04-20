; ------------------------------------------------------------------------------
; File:         Lab05_P3.asm
; Function:     multiplies numbers of two arrays and stores them in another array
; Description:  SW_MUL is a subroutune that preforms multiplication on numbers using shift and add
; Input:        R4 -- array 1 starting address
;               R5 -- array 2 starting address
;				R6 -- result array starting address
; Output:       The output is stored in memory at the result array
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 17, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def SW_MUL
SW_MUL:	mov		2(SP), R6			; getting the result array address from the stack
		mov		4(SP), R5			; getting the second array address from the stack
		mov		6(SP), R4			; getting the first array address from the stack
		clr		R15					; clearing R15 for use as integer size counter
		clr		R7					; clearing R7 for use as shift and add accumulator
		clr		R8					; clearing R8 for use as array counter
		mov		@R4, R11			; getting the multiplicand
		mov		@R5, R12			; getting the multiplier

loop:	cmp		#16, R15			; have we reached the end of the shift and add?
		jz		store				; if we have, jump to store the value
		bit		#1, R12				; is the LSB of the multiplier 1?
		jz		skipadd				; if it isn't, skip adding
		add		R11, R7				; if it is, add the multiplicand to the accumulator
skipadd:rla		R11					; rotate the multiplicand for shift and add
		rra		R12					; rotate the multiplier for shift and add
		add		#1, R15				; increase integer counter
		jmp		loop				; preform the next shift and add step

store:	mov 	R7, 0(R6)			; move the accumulated value to memory
		clr		R7					; clear the accumulator
		clr		R15					; clear the integer size counter
		add		#2, R4				; move to next first array address
		add		#2, R5				; move to next second array address
		mov		@R4, R11			; getting the multiplicand
		mov		@R5, R12			; getting the multiplier
		add		#2, R6				; moving to next addressable word for the array in memory
		inc		R8					; increment the array counter
		cmp		#8, R8				; are we at the end of the array?
		jz		end					; if we are, jump to return from the subroutine
		jmp		loop				; get the next number


end:	ret							; return from the subroutine
		.end
