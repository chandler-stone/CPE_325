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
        .def B_SW_MUL
B_SW_MUL:	add		#2, SP
			pop		R6
			pop		R7
			pop		R5
			pop		R4
			clr		R15
			clr		R11

			sub		#0x2, R4
			sub		#0x2, R5

gnext:		;add		#0x1, R15
			cmp		R7, R15
			jz		end
			add		#0x2, R4
			mov		@R4, R8
			add		#0x2, R5
			mov		@R5, R9

			clr 	R14
gnext2:	;add		#0x1, R15
		cmp		#0xF, R14
		jge		fin
		bit		#0x1, R8
		jnz		addn
		rla		R8
		rra		R9
		add		#0x1, R14
		jmp		gnext2

addn:	add		R8, R11
		;mov		R7, 0(R6)
		;mov		#0x1, R10
		rla		R8
		rra		R9
		jmp		gnext2


fin: 	mov		R11, 0(R6)
		add		#0x1, R15
		jmp		gnext


end:		sub		#10, SP
			ret
			.end
