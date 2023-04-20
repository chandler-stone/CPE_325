; ------------------------------------------------------------------------------
; File:         Lab05_D1.asm
; Function:     Finds a sum of two integer arrays
; Description:  Initializes ports, sums up elements of two integer arrays, and
;               display sums on on parallel port output registers
; Input:        The input arrays are signed 16-bit integers in arr1 and arr2
; Output:       P1OUT&P2OUT displays arr1 sum, P3OUT&P4OUT displays arr2 sum
; Author(s):    Aleksandar Milenkovic, milenkovic@computer.org
; Date:         September 14, 2008
; ------------------------------------------------------------------------------
        .cdecls C, LIST, "msp430.h"     ; Include device header file
;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.
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
        mov.w   #arr1, R4               ; Put address of arr1 into R4
        mov.w   #arr2, R5               ; Put address of arr into R5

        ; Sum arr1 and display
        clr.w   R7                      ; Holds the sum
        mov.w   #8, R10                 ; Number of elements in arr1
lnext1: add.w   @R4+, R7                ; Add the current element to sum
        dec.w   R10                     ; Decrement arr1 length
        jnz     lnext1                  ; Get next element
        mov.b   R7, P1OUT               ; Display lower byte of sum of arr1
        swpb    R7                      ; Swap bytes
        mov.b   R7, P2OUT               ; Display upper byte of sum of arr1
            
        ; Sum arr2 and display
        clr.w   R7                      ; Holds the sum
        mov.w   #7, R10                 ; Number of elements in arr2
lnext2: add.w   @R5+, R7                ; Get next element
        dec.w   R10                     ; Decrement arr2 length
        jnz     lnext2                  ; Get next element
        mov.b   R7, P3OUT               ; Display lower byte of sum of arr2
        swpb    R7                      ; Swap bytes
        mov.b   R7, P4OUT               ; Display upper byte of sum of arr2
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
