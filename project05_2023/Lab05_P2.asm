; ------------------------------------------------------------------------------
; File:         Lab05_P2.asm
; Function:     multiplies numbers of two arrays and stores them in another array
; Description:  HW_MUL is a subroutune that preforms multiplication on numbers using a hardware peripheral
; Input:        R4 -- array 1 starting address
;               R5 -- array 2 starting address
;				R6 -- result array starting address
; Output:       The output is stored in memory at the result array
; Author(s):    Chandler Stone, crs0074@uah.edu
; Date:         February 17, 2023
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def HW_MUL
HW_MUL:	mov		2(SP), R6			; getting the result array address from the stack
		mov		4(SP), R5			; getting the second array address from the stack
		mov		6(SP), R4			; getting the first array address from the stack
		clr		R7					; clearing R7 for use as array counter

loop:	mov     @R4+, &MPY			; moving the multiplicand into the hardware multiplier (and incrementing for the loop)
        mov     @R5+, &OP2          ; moving the multiplier into the hardware multiplier (and incrementing for the loop)
        nop							; waiting on the hardware multiplier
        nop							; waiting on the hardware multiplier
        nop							; waiting on the hardware multiplier
        mov     RESLO, R9			; grabbing the result from the hardware multiplier
        mov		R9, 0(R6)			; storing the result in memory
        add		#2, R6				; moving to next addressable word for the array in memory
        inc		R7					; increment the array counter
		cmp		#8, R7				; are we at the end of the array?
        jz		end					; if we are, jump to return from the subroutine
        jmp		loop				; preform the next multiplication

end:	ret							; return from the subroutine
		.end
