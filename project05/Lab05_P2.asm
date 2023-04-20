; ------------------------------------------------------------------------------
; File:         Lab05_D2_RP.asm
; Function:     Finds a sum of an input integer array
; Description:  suma_rp is a subroutine that sums elements of an integer array
; Input:        R12 -- array starting address
;               R13 -- the number of elements (>= 1)
; Output:       The output result is returned in register R14
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         September 14, 2008
; Revised:      August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def SW_MUL
SW_MUL:
		add		#2, SP
		pop		R6
		pop		R5
		pop		R4
		clr		R15
		clr		R7

gnext:	;add		#0x1, R15
		cmp		#0xF, R15
		jge		end
		bit		#0x1, R5
		jnz		addn
		rla		R4
		rra		R5
		add		#0x1, R15
		jmp		gnext

addn:	add		R4, R7
		;mov		R7, 0(R6)
		;mov		#0x1, R10
		rla		R4
		rra		R5
		jmp		gnext

end:
		mov		R7, 0(R6)
        sub		#8, SP
		ret
		.end
