;-------------------------------------------------------------------------------
;   File:        Lab6_D3.asm
;   Description: The program demonstrates Press/Release using SW1 and LED1.
;                LED1 is initialized off. The main program enables interrupts
;                from P1.BIT0 (SW1) and remains in an infinite loop doing nothing.
;                P1_ISR implements debouncing and waits for a SW1 to be released.
;
;   Clocks:      ACLK = 32.768kHz, MCLK = SMCLK = default DCO = 2^20=1,048,576 Hz
;   Platform:    TI Experimenter's Board
;
;                 MSP430xG461x
;             -----------------
;         /|\|                 |
;          | |                 |
;          --|RST              |
;            |             P2.2|-->LED1(GREEN)
;            |             P1.0|<--SW1
;
;   Author:  Aleksandar Milenkovic, milenkovic@computer.org
;   Date:    September 14, 2018
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .def    SW1_ISR
            .def	SW2_ISR
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:      mov.w   #__STACK_END, SP        ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW|WDTHOLD, &WDTCTL ; Stop watchdog timer
;-------------------------------------------------------------------------------
Setup:		bis.b	#00000011b, &P1DIR		; Setting direction register for port 1
			bis.b   #00000001b, &P2DIR      ; Setting direction register for port 2
			bis.b	#10000000B, &P4DIR		; Setting direction register for port 4
            bic.b   #00000001b, &P1OUT      ; Turn off LED1
            bic.b	#10000000b, &P4OUT		; Turn off LED2
            bis.w   #GIE, SR                ; Enable Global Interrupts
            bis.b   #00000010b, &P1IE       ; Enable Port 1 interrupt from bit 0
            ;bis.b   #00000001b, &P1IES      ; Set interrupt to call from hi to low
            ;bis.b   #00000001b, &P1IFG      ; Clear interrupt flag

main:		jmp     $                       ; Infinite loop until interrupt

;----------------------------------------------------------------------------
; P1.0 (SW1) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW1_ISR:	bic.b   #00000001b, &P1IFG      ; Clear interrupt flag

ChkSw1:     bit.b   #00000001b, &P1IN       ; Check if SW1 is pressed (0000_0001 on P1IN)
            jnz     ChkSw2                  ; If not zero, SW is not pressed

                                            ; loop and check again
Debounce:   mov.w   #2000, R15              ; Set to (2000 * 10 cc )

SWD20ms:    dec.w   R15                     ; Decrement R15
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            jnz     SWD20ms                 ; Delay over?
            bit.b   #00000001b, &P1IN       ; Is switch 1 still pressed?
            jnz     Return                  ; If not, return from interupt

LEDon:      xor   #0000100b, &P2OUT         ; Toggle LED 2

Return:     reti                            ; Return from interrupt


;----------------------------------------------------------------------------
; P4.7 (SW2) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW2_ISR:	bic.b   #00000001b, &P1IFG      ; Clear interrupt flag

ChkSw2:		bit.b	#00000010b, &P1IN
			jnz		Return2

Debounce2:   mov.w   #2000, R15             ; Set to (2000 * 10 cc )

SWD20ms2:   dec.w   R15                     ; Decrement R15
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            jnz     SWD20ms2                ; Delay over?
            bit.b   #00000010b, &P1IN       ; Is Switch 2 still pressed?
            jnz     Return2                 ; If not, return from interrupt

LEDon2:     XOR		#00000010b, &P2OUT			; Loop for LED 1
			jnz		LEDon2

			XOR   	#1, &P1IES				; Toggle LED 2

SW2wait:	bit.b	#1, &P1IN
			jz		SW2wait
			jmp		Return2

LED3:		bic.b	#00000100b, &P2OUT
			XOR   	#0000001b, &P1IES
			jmp 	Return2

Return2:	reti							; return from interrupt

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
            .sect   ".int20"                ;P1.x Vector
            .short  SW1_ISR
            ;.sect	".int20"
            ;.short	SW2_ISR
            .end
