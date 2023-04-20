; ------------------------------------------------------------------------------
; File:         Lab05_D2_main.asm
; Function:     Finds a sum of two integer arrays using a subroutine.
; Description:  The program calls suma_rp to sum up elements of integer arrays
;               and then displays the sum on parallel ports.
;               Parameters to suma_rp are passed through registers, R12, R13.
;               The subroutine suma_rp return the result in register R14.
; Input:        The input arrays are signed 16-bit integers in arr1 and arr2
; Output:       P1OUT&P2OU displays arr1 sum, P3OUT&P4OUT displays arr2 sum
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         August 14, 2008
; Revised:      August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
        .ref    suma_rp
;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory.
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section.
        .retainrefs                     ; And retain any sections that have
                                        ; references to current section.
;-------------------------------------------------------------------------------
RESET:  mov.w   #__STACK_END,SP         ; Initialize stack pointer
        mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main code here
;-------------------------------------------------------------------------------
main:
        mov.w   #arr1, R12              ; Put address of arr1 into R12
        mov.w   #8, R13                 ; Put array length into R13
        call    #suma_rp
; P1OUT is at address 0x02, P2OUT is address 0x03.
; We can write the 16-bit result to both at the same time.
; P2OUT contains the upper byte and P1OUT the lower byte.
        mov.w   R14, &P1OUT             ; Result goes to P2OUT&P1OUT
        mov.w   #arr2, R12              ; Put address of arr2 into R12
        mov.w   #7, R13                 ; Put array length into R13
        mov.w   #1, R14                 ; Display #0 (P3&P4)
        call    #suma_rp
        mov.w   R14, &P3OUT             ; Result goes to P4OUT&P3OUT
        jmp     $

arr1:   .int    1, 2, 3, 4, 1, 2, 3, 4
arr2:   .int    1, 1, 1, 1, -1, -1, -1
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
