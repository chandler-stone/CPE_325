;-------------------------------------------------------------------------------
;   File:        Lab6_D2.asm
;   Description: The program demonstrates Press/Release using SW1 and LED1.
;                LED1 is initialized off.
;                When SW1 press is detected, a software delay of 20 ms
;                is used to implement debouncing. The switch is checked
;                again, and if on, LED1 is turned on until SW1 is released.
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
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:      mov.w   #__STACK_END,SP         ; Initialize stack pointer
StopWDT:    mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
SetupP2:
            bis.b   #004h, &P2DIR           ; Set P2.2 to output
                                            ; direction (0000_0100)
            bic.b   #004h, &P2OUT           ; Set P2OUT to 0x0000_0100 (ensure
                                            ; LED1 is off)
ChkSW1:     bit.b   #01h, &P1IN             ; Check if SW1 is pressed
                                            ; (0000_0001 on P1IN)
            jnz     ChkSW1                  ; If not zero, SW is not pressed
                                            ; loop and check again
Debounce:
            mov.w   #2000, R15              ; Set to (2000 * 10 cc = 20,000 cc)
SWD20ms:    dec.w   R15                     ; Decrement R15
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            jnz     SWD20ms                 ; Delay over?
            bit.b   #00000001b, &P1IN       ; Verify SW1 is still pressed
            jnz     ChkSW1                  ; If not, wait for SW1 press

LEDon:      bis.b   #004h, &P2OUT           ; Turn on LED1
SW1wait:    bit.b   #001h, &P1IN            ; Test SW1
            jz      SW1wait                 ; Wait until SW1 is released
            bic.b   #004, &P2OUT            ; Turn off LED1
            jmp     ChkSW1                  ; Loop to beginning
                                            
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
