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
        .def HW_MUL
HW_MUL:	add		#2, SP
		pop		R6
		pop		R5
		pop		R4
		mov     R4, &MPY              ; moves val1 to R5
        mov     R5, &OP2              ; moves val2 to R6
; Since we have both numbers, let us get the result after the three clock cycles
; required for for 16X16 hardware multiplication
        nop
        nop
        nop
        mov     RESLO, R9
        add		R9, 0(R6)
        sub		#8, SP
		ret
		.end
