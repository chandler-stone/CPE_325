			.cdecls C,LIST,"msp430.h"
			.def	RESET

			.text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs

RESET:      mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW | WDTHOLD, &WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main code here
;-------------------------------------------------------------------------------
            sub.w   #4, SP                  ; allocate space for ascii chars
            mov.w   SP, R14                 ; R14 points to the allocated area
            mov.w   myI, R4                 ; integer is passed through R4
            push.w  R14                     ; push the starting address on the stack
            call    #i2a_s                  ; call subroutine
            add.w   #2, SP                  ; free space on the stack
lend:       jmp $

myI:        .word     0x34AE

i2a_s:		add	#2, SP
			pop R10

			mov	#0x000F, R6
			mov	#0x00F0, R7
			mov	#0x0F00, R8
			mov	#0xF000, R9

			and	R4, R6
			and R4, R7
			and R4, R8
			and R4, R9

			rra	R7
			rra	R7
			rra	R7
			rra	R7

			rra	R8
			rra	R8
			rra	R8
			rra	R8
			rra	R8
			rra	R8
			rra	R8
			rra	R8

			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9
			rra	R9

			mov.b	R6, 0(R10)
			mov.b	R7, 1(R10)
			mov.b	R8, 2(R10)
			mov.b	R9, 3(R10)

			sub	#4, SP
			ret


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


;i2a_s:		pop R10
;			mov	#0x000F, R6
;			mov	#0x00F0, R7
;			mov	#0x0F00, R8
;			mov	#0xF000, R9
;			and	R4, R6
;			and R4, R7
;			and R4, R8
;			and R4, R9
;			mov	R6, 0(R10)
;			mov	R6, 1(R10)
;			mov	R6, 2(R10)
;			mov	R6, 3(R10)
;			ret
