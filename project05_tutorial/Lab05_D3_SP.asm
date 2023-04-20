; ------------------------------------------------------------------------------
; File:         Lab5_D3_SP.asm
; Function:     Finds a sum of an input integer array
; Description:  suma_sp is a subroutine that sums elements of an integer array
; Input:        The input parameters are on the stack pushed as follows:
;               starting address of the array, array length
; Output:       The result is returned through the stack
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         September 14, 2008
; Revised:      August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
; ------------------------------------------------------------------------------
        .def    suma_sp
        .text
suma_sp:                                ; Save the registers on the stack
        push    R7                      ; Save R7, temporal sum
        push    R6                      ; Save R6, array length
        push    R4                      ; Save R5, pointer to array
        clr.w   R7                      ; Clear R7
        mov.w   10(SP), R6              ; Retrieve array length
        mov.w   12(SP), R4              ; Retrieve starting address

lnext:  add.w   @R4+, R7                ; Add next element
        dec.w   R6                      ; Decrement array length
        jnz     lnext                   ; Repeat if not done
        mov.w   R7, 8(SP)               ; Store the sum on the stack

lend:   pop     R4                      ; Restore R4
        pop     R6                      ; Restore R6
        pop     R7                      ; Restore R7
        ret                             ; Return
        .end
