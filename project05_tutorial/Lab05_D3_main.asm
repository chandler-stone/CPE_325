; ------------------------------------------------------------------------------
; File:         Lab05_D3_main.asm
; Function:     Finds a sum of two integer arrays using a subroutine suma_sp
; Description:  The program calls suma_sp to sum up elements of integer arrays
;               and stores the respective sums in parallel ports' output
;               registers. Parameters to suma_sp are passed through the stack.
; Input:        The input arrays are signed 16-bit integers in arr1 and arr2
; Output:       P2OUT&P1OUT stores the sum of arr1
;               P4OUT&P3OUT stores the sum of arr2
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         September 14, 2008
; Revised:      August 5, 2020
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
        .ref    suma_sp
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
        push    #arr1                   ; Push the address of arr1
        push    #8                      ; Push the number of elements
        sub.w   #2, SP                  ; Allocate space for the sum
        call    #suma_sp
        mov.w   @SP, &P1OUT             ; Store the sum in P2OUT&P1OUT
        add.w   #6, SP                  ; Collapse the stack

        push    #arr2                   ; Push the address of arr1
        push    #7                      ; Push the number of elements
        sub     #2, SP                  ; Allocate space for the sum
        call    #suma_sp
        mov.w   @SP, &P3OUT             ; Store the sume in P4OUT&P3OUT
        add.w   #6, SP                  ; Collapse the stack

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
