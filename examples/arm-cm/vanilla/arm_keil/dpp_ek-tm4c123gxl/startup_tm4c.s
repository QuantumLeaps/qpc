;******************************************************************************
; Interrupt vector table and startup for DPP example, Vanilla kernel
;
; Added CMSIS-compliant exception handlers, added conditional FPU
; initialization and calls to 'assert_failed' in the exception handlers.
;
; Quantum Leaps on 08-Oct-2013
; www.state-machine.com
;******************************************************************************
;
; Startup.s - Startup code for use with Keil's uVision.
;
; Copyright (c) 2007-2011 Texas Instruments Incorporated.  All rights reserved.
; Software License Agreement
;
; Texas Instruments (TI) is supplying this software for use solely and
; exclusively on TI's microcontroller products. The software is owned by
; TI and/or its suppliers, and is protected under applicable copyright
; laws. You may not combine this software with "viral" open-source
; software in order to form a larger program.
;
; THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
; NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
; NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
; CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
; DAMAGES, FOR ANY REASON WHATSOEVER.
;
; This is part of revision 7860 of the EK-LM3S811 Firmware Package.
;
;******************************************************************************

;******************************************************************************
;
; <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Stack   EQU     0x00000200

;******************************************************************************
;
; <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Heap    EQU     0x00000000

;******************************************************************************
;
; Allocate space for the stack.
;
;******************************************************************************
        AREA    STACK, NOINIT, READWRITE, ALIGN=3
StackMem
        SPACE   Stack
__initial_sp

;******************************************************************************
;
; Allocate space for the heap.
;
;******************************************************************************
        AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
HeapMem
        SPACE   Heap
__heap_limit

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
        PRESERVE8

;******************************************************************************
;
; Place code into the reset code section.
;
;******************************************************************************
        AREA   RESET, CODE, READONLY
        THUMB

        IMPORT assert_failed
        ;IMPORT SVC_Handler
        ;IMPORT PendSV_Handler
        IMPORT SysTick_Handler
        IMPORT GPIOPortA_IRQHandler

        ; add any used interrupt handlers here and in the right
        ; slot of the vector table below...

;******************************************************************************
;
; The vector table.
;
;******************************************************************************
        EXPORT  __Vectors
__Vectors
        DCD     StackMem + Stack            ; Top of Stack
        DCD     Reset_Handler               ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; The MPU fault handler
        DCD     BusFault_Handler            ; The bus fault handler
        DCD     UsageFault_Handler          ; The usage fault handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall handler
        DCD     DebugMon_Handler            ; Debug monitor handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; The PendSV handler
        DCD     SysTick_Handler             ; The SysTick handler

        ; External interrupts...
        DCD     GPIOPortA_IRQHandler        ; GPIO Port A
        DCD     Unused_IRQHandler           ; GPIO Port B
        DCD     Unused_IRQHandler           ; GPIO Port C
        DCD     Unused_IRQHandler           ; GPIO Port D
        DCD     Unused_IRQHandler           ; GPIO Port E
        DCD     Unused_IRQHandler           ; UART0 Rx and Tx
        DCD     Unused_IRQHandler           ; UART1 Rx and Tx
        DCD     Unused_IRQHandler           ; SSI0 Rx and Tx
        DCD     Unused_IRQHandler           ; I2C0 Master and Slave
        DCD     Unused_IRQHandler           ; PWM Fault
        DCD     Unused_IRQHandler           ; PWM Generator 0
        DCD     Unused_IRQHandler           ; PWM Generator 1
        DCD     Unused_IRQHandler           ; PWM Generator 2
        DCD     Unused_IRQHandler           ; Quadrature Encoder 0
        DCD     Unused_IRQHandler           ; ADC Sequence 0
        DCD     Unused_IRQHandler           ; ADC Sequence 1
        DCD     Unused_IRQHandler           ; ADC Sequence 2
        DCD     Unused_IRQHandler           ; ADC Sequence 3
        DCD     Unused_IRQHandler           ; Watchdog timer
        DCD     Unused_IRQHandler           ; Timer 0 subtimer A
        DCD     Unused_IRQHandler           ; Timer 0 subtimer B
        DCD     Unused_IRQHandler           ; Timer 1 subtimer A
        DCD     Unused_IRQHandler           ; Timer 1 subtimer B
        DCD     Unused_IRQHandler           ; Timer 2 subtimer A
        DCD     Unused_IRQHandler           ; Timer 2 subtimer B
        DCD     Unused_IRQHandler           ; Analog Comparator 0
        DCD     Unused_IRQHandler           ; Analog Comparator 1
        DCD     Unused_IRQHandler           ; Analog Comparator 2
        DCD     Unused_IRQHandler           ; System Control (PLL, OSC, BO)
        DCD     Unused_IRQHandler           ; FLASH Control
        DCD     Unused_IRQHandler           ; GPIO Port F
        DCD     Unused_IRQHandler           ; GPIO Port G
        DCD     Unused_IRQHandler           ; GPIO Port H
        DCD     Unused_IRQHandler           ; UART2 Rx and Tx
        DCD     Unused_IRQHandler           ; SSI1 Rx and Tx
        DCD     Unused_IRQHandler           ; Timer 3 subtimer A
        DCD     Unused_IRQHandler           ; Timer 3 subtimer B
        DCD     Unused_IRQHandler           ; I2C1 Master and Slave
        DCD     Unused_IRQHandler           ; Quadrature Encoder 1
        DCD     Unused_IRQHandler           ; CAN0
        DCD     Unused_IRQHandler           ; CAN1
        DCD     Unused_IRQHandler           ; CAN2
        DCD     Unused_IRQHandler           ; Ethernet
        DCD     Unused_IRQHandler           ; Hibernate

        DCD     Unused_IRQHandler           ; USB0
        DCD     Unused_IRQHandler           ; PWM Generator 3
        DCD     Unused_IRQHandler           ; uDMA Software Transfer
        DCD     Unused_IRQHandler           ; uDMA Error
        DCD     Unused_IRQHandler           ; ADC1 Sequence 0
        DCD     Unused_IRQHandler           ; ADC1 Sequence 1
        DCD     Unused_IRQHandler           ; ADC1 Sequence 2
        DCD     Unused_IRQHandler           ; ADC1 Sequence 3
        DCD     Unused_IRQHandler           ; I2S0
        DCD     Unused_IRQHandler           ; External Bus Interface 0
        DCD     Unused_IRQHandler           ; GPIO Port J
        DCD     Unused_IRQHandler           ; GPIO Port K
        DCD     Unused_IRQHandler           ; GPIO Port L
        DCD     Unused_IRQHandler           ; SSI2 Rx and Tx
        DCD     Unused_IRQHandler           ; SSI3 Rx and Tx
        DCD     Unused_IRQHandler           ; UART3 Rx and Tx
        DCD     Unused_IRQHandler           ; UART4 Rx and Tx
        DCD     Unused_IRQHandler           ; UART5 Rx and Tx
        DCD     Unused_IRQHandler           ; UART6 Rx and Tx
        DCD     Unused_IRQHandler           ; UART7 Rx and Tx
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     Unused_IRQHandler           ; I2C2 Master and Slave
        DCD     Unused_IRQHandler           ; I2C3 Master and Slave
        DCD     Unused_IRQHandler           ; Timer 4 subtimer A
        DCD     Unused_IRQHandler           ; Timer 4 subtimer B
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     Unused_IRQHandler           ; Timer 5 subtimer A
        DCD     Unused_IRQHandler           ; Timer 5 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 0 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 0 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 1 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 1 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 2 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 2 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 3 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 3 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 4 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 4 subtimer B
        DCD     Unused_IRQHandler           ; Wide Timer 5 subtimer A
        DCD     Unused_IRQHandler           ; Wide Timer 5 subtimer B
        DCD     Unused_IRQHandler           ; FPU
        DCD     Unused_IRQHandler           ; PECI 0
        DCD     Unused_IRQHandler           ; LPC 0
        DCD     Unused_IRQHandler           ; I2C4 Master and Slave
        DCD     Unused_IRQHandler           ; I2C5 Master and Slave
        DCD     Unused_IRQHandler           ; GPIO Port M
        DCD     Unused_IRQHandler           ; GPIO Port N
        DCD     Unused_IRQHandler           ; Quadrature Encoder 2
        DCD     Unused_IRQHandler           ; Fan 0
        DCD     0                           ; Reserved
        DCD     Unused_IRQHandler           ; GPIO Port P (Summary or P0)
        DCD     Unused_IRQHandler           ; GPIO Port P1
        DCD     Unused_IRQHandler           ; GPIO Port P2
        DCD     Unused_IRQHandler           ; GPIO Port P3
        DCD     Unused_IRQHandler           ; GPIO Port P4
        DCD     Unused_IRQHandler           ; GPIO Port P5
        DCD     Unused_IRQHandler           ; GPIO Port P6
        DCD     Unused_IRQHandler           ; GPIO Port P7
        DCD     Unused_IRQHandler           ; GPIO Port Q (Summary or Q0)
        DCD     Unused_IRQHandler           ; GPIO Port Q1
        DCD     Unused_IRQHandler           ; GPIO Port Q2
        DCD     Unused_IRQHandler           ; GPIO Port Q3
        DCD     Unused_IRQHandler           ; GPIO Port Q4
        DCD     Unused_IRQHandler           ; GPIO Port Q5
        DCD     Unused_IRQHandler           ; GPIO Port Q6
        DCD     Unused_IRQHandler           ; GPIO Port Q7
        DCD     Unused_IRQHandler           ; GPIO Port R
        DCD     Unused_IRQHandler           ; GPIO Port S
        DCD     Unused_IRQHandler           ; PWM 1 Generator 0
        DCD     Unused_IRQHandler           ; PWM 1 Generator 1
        DCD     Unused_IRQHandler           ; PWM 1 Generator 2
        DCD     Unused_IRQHandler           ; PWM 1 Generator 3
        DCD     Unused_IRQHandler           ; PWM 1 Fault


;******************************************************************************
;
; This is the code that gets called when the processor first starts execution
; following a reset event.
;
;******************************************************************************
        EXPORT  Reset_Handler
Reset_Handler
        ;
        ; Call the C library enty point that handles startup.  This will copy
        ; the .data section initializers from flash to SRAM and zero fill the
        ; .bss section.
        ;
        IMPORT  __main

  IF {FPU} != "SoftVFP"       ; If software FPU not used...
        ; NOTE: The FPU must be enabled before jumping to __main, because
        ; the initialization code downstream assumes that the FPU is present
        ; and a fault exception would result if the FPU was not enabled.
        ; In other words, enabling the FPU from main() would be too late.
        LDR     r0,=0xE000ED88
        LDR     r1,[r0]
        ORR     r1,r1,#0xF00000
        STR     r1,[r0]
  ENDIF

        B       __main

;******************************************************************************
;
; The NMI handler
;
;******************************************************************************
NMI_Handler
        MOV     r0,#0
        MOV     r1,#2       ; NMI exception number
        B       assert_failed

;******************************************************************************
;
; The Hard Fault handler
;
;******************************************************************************
HardFault_Handler
        MOV     r0,#0
        MOV     r1,#3       ; HardFault exception number
        B       assert_failed

;******************************************************************************
;
; The MPU fault handler
;
;******************************************************************************
MemManage_Handler
        MOV     r0,#0
        MOV     r1,#4       ; MemManage exception number
        B       assert_failed

;******************************************************************************
;
; The Bus Fault handler
;
;******************************************************************************
BusFault_Handler
        MOV     r0,#0
        MOV     r1,#5       ; BusFault exception number
        B       assert_failed

;******************************************************************************
;
; The Usage Fault handler
;
;******************************************************************************
UsageFault_Handler
        MOV     r0,#0
        MOV     r1,#6       ; UsageFault exception number
        B       assert_failed

;******************************************************************************
;
; The SVC handler (uncommend if _not_ used)
;
;******************************************************************************
SVC_Handler
        MOV     r0,#0
        MOV     r1,#11      ; SVCall exception number
        B       assert_failed

;******************************************************************************
;
; The Debug Monitor handler
;
;******************************************************************************
DebugMon_Handler
        MOV     r0,#0
        MOV     r1,#12      ; DebugMon exception number
        B       assert_failed

;******************************************************************************
;
; The PendSV handler (uncommend if _not_ used)
;
;******************************************************************************
PendSV_Handler
        MOV     r0,#0
        MOV     r1,#14      ; PendSV exception number
        B       assert_failed

;******************************************************************************
;
; The SysTick handler (uncommend if _not_ used)
;
;******************************************************************************
;SysTick_Handler
;        MOV     r0,#0
;        MOV     r1,#15      ; SysTick exception number
;        B       assert_failed

;******************************************************************************
;
; The Unused IRQ handler (called when an unused IRQ occurs)
;
;******************************************************************************
Unused_IRQHandler
        MOV     r0,#0
        MOV     r1,#-1      ; 0xFFFFFFF
        B       assert_failed

        ALIGN               ; make sure the end of this section is aligned

;******************************************************************************
;
; Some code in the normal code section for initializing the heap and stack.
;
;******************************************************************************
        AREA    |.text|, CODE, READONLY

;******************************************************************************
;
; The function expected of the C library startup code for defining the stack
; and heap memory locations.  For the C library version of the startup code,
; provide this function so that the C library initialization code can find out
; the location of the stack and heap.
;
;******************************************************************************
    IF :DEF: __MICROLIB
        EXPORT  __initial_sp
        EXPORT  __heap_base
        EXPORT  __heap_limit
    ELSE
        IMPORT  __use_two_region_memory
        EXPORT  __user_initial_stackheap
__user_initial_stackheap
        LDR     R0, =HeapMem
        LDR     R1, =(StackMem + Stack)
        LDR     R2, =(HeapMem + Heap)
        LDR     R3, =StackMem
        BX      LR
    ENDIF

        ALIGN               ; make sure the end of this section is aligned

        END                 ; end of module
