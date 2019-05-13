;/***************************************************************************/
; * @file     startup_TM4C123GH6PM.s for IAR ARM assembler
; * @brief    CMSIS Cortex-M4F Core Device Startup File for TM4C123GH6PM
; * @version  CMSIS 5.5.1
; * @date     13 May 2019
; *
; * @description
; * Created from the CMSIS template for the specified device
; * Quantum Leaps, www.state-machine.com
; *
; * @note
; * The function assert_failed defined at the end of this file defines
; * the error/assertion handling policy for the application and might
; * need to be customized for each project. This function is defined in
; * assembly to re-set the stack pointer, in case it is corrupted by the
; * time assert_failed is called.
; *
; ***************************************************************************/
;/* Copyright (c) 2012 ARM LIMITED
;
;  All rights reserved.
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are met:
;  - Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;  - Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
;  - Neither the name of ARM nor the names of its contributors may be used
;    to endorse or promote products derived from this software without
;    specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;  POSSIBILITY OF SUCH DAMAGE.
;---------------------------------------------------------------------------*/

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;
        DATA
__vector_table
        DCD     sfe(CSTACK)
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

        ; IRQ handlers...
        DCD     WDT_IRQHandler              ; WDT
        DCD     TIMER0_IRQHandler           ; TIMER0
        DCD     TIMER1_IRQHandler           ; TIMER1
        DCD     TIMER2_IRQHandler           ; TIMER2
        DCD     TIMER3_IRQHandler           ; TIMER3
        DCD     UART0_IRQHandler            ; UART0
        DCD     UART1_IRQHandler            ; UART1
        DCD     UART2_IRQHandler            ; UART2
        DCD     UART3_IRQHandler            ; UART3
        DCD     PWM1_IRQHandler             ; PWM1
        DCD     I2C0_IRQHandler             ; I2C0
        DCD     I2C1_IRQHandler             ; I2C1
        DCD     I2C2_IRQHandler             ; I2C2
        DCD     SPI_IRQHandler              ; SPI
        DCD     SSP0_IRQHandler             ; SSP0
        DCD     SSP1_IRQHandler             ; SSP1
        DCD     PLL0_IRQHandler             ; PLL0 (Main PLL)
        DCD     RTC_IRQHandler              ; RTC
        DCD     EINT0_IRQHandler            ; EINT0
        DCD     EINT1_IRQHandler            ; EINT1
        DCD     EINT2_IRQHandler            ; EINT2
        DCD     EINT3_IRQHandler            ; EINT3
        DCD     ADC_IRQHandler              ; ADC
        DCD     BOD_IRQHandler              ; BOD
        DCD     USB_IRQHandler              ; USB
        DCD     CAN_IRQHandler              ; CAN
        DCD     DMA_IRQHandler              ; GP DMA
        DCD     I2S_IRQHandler              ; I2S
        DCD     ENET_IRQHandler             ; Ethernet
        DCD     RIT_IRQHandler              ; RITINT
        DCD     MCPWM_IRQHandler            ; Motor Control PWM
        DCD     QEI_IRQHandler              ; Quadrature Encoder
        DCD     PLL1_IRQHandler             ; PLL1 (USB PLL)
        DCD     USBActivity_IRQHandler      ; USB Activity interrupt to wakeup
        DCD     CANActivity_IRQHandler      ; CAN Activity interrupt to wakeup

__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

;******************************************************************************
;
; Weak fault handlers...
;
        SECTION .text:CODE:REORDER:NOROOT(2)

;.............................................................................
        PUBWEAK Reset_Handler
        EXTERN  SystemInit
        EXTERN  __iar_program_start
Reset_Handler
        BL      SystemInit  ; CMSIS system initialization
        BL      __iar_program_start ; IAR startup code
;.............................................................................
        PUBWEAK NMI_Handler
NMI_Handler
        LDR     r0,=str_NMI
        MOVS    r1,#1
        B       assert_failed
str_NMI
        DCB     "NMI"
        ALIGNROM 2
;.............................................................................
        PUBWEAK HardFault_Handler
HardFault_Handler
        LDR     r0,=str_HardFault
        MOVS    r1,#1
        B       assert_failed
str_HardFault
        DCB     "HardFault"
        ALIGNROM 2
;.............................................................................
        PUBWEAK MemManage_Handler
MemManage_Handler
        LDR     r0,=str_MemManage
        MOVS    r1,#1
        B       assert_failed
str_MemManage
        DCB     "MemManage"
        ALIGNROM 2
;.............................................................................
        PUBWEAK BusFault_Handler
BusFault_Handler
        LDR     r0,=str_BusFault
        MOVS    r1,#1
        B       assert_failed
str_BusFault
        DCB     "BusFault"
        ALIGNROM 2
;.............................................................................
        PUBWEAK UsageFault_Handler
UsageFault_Handler
        LDR     r0,=str_UsageFault
        MOVS    r1,#1
        B       assert_failed
str_UsageFault
        DCB     "UsageFault"
        ALIGNROM 2

;******************************************************************************
;
; Weak non-fault handlers...
;

        PUBWEAK SVC_Handler
SVC_Handler
        LDR     r0,=str_SVC
        MOVS    r1,#1
        B       assert_failed
str_SVC
        DCB     "SVC"
        ALIGNROM 2
;.............................................................................
        PUBWEAK DebugMon_Handler
DebugMon_Handler
        LDR     r0,=str_DebugMon
        MOVS    r1,#1
        B       assert_failed
str_DebugMon
        DCB     "DebugMon"
        ALIGNROM 2
;.............................................................................
        PUBWEAK PendSV_Handler
PendSV_Handler
        LDR     r0,=str_PendSV
        MOVS    r1,#1
        B       assert_failed
str_PendSV
        DCB     "PendSV"
        ALIGNROM 2
;.............................................................................
        PUBWEAK SysTick_Handler
SysTick_Handler
        LDR     r0,=str_SysTick
        MOVS    r1,#1
        B       assert_failed
str_SysTick
        DCB     "SysTick"
        ALIGNROM 2

;******************************************************************************
;
; Weak IRQ handlers...
;

        PUBWEAK  WDT_IRQHandler
        PUBWEAK  TIMER0_IRQHandler
        PUBWEAK  TIMER1_IRQHandler
        PUBWEAK  TIMER2_IRQHandler
        PUBWEAK  TIMER3_IRQHandler
        PUBWEAK  UART0_IRQHandler
        PUBWEAK  UART1_IRQHandler
        PUBWEAK  UART2_IRQHandler
        PUBWEAK  UART3_IRQHandler
        PUBWEAK  PWM1_IRQHandler
        PUBWEAK  I2C0_IRQHandler
        PUBWEAK  I2C1_IRQHandler
        PUBWEAK  I2C2_IRQHandler
        PUBWEAK  SPI_IRQHandler
        PUBWEAK  SSP0_IRQHandler
        PUBWEAK  SSP1_IRQHandler
        PUBWEAK  PLL0_IRQHandler
        PUBWEAK  RTC_IRQHandler
        PUBWEAK  EINT0_IRQHandler
        PUBWEAK  EINT1_IRQHandler
        PUBWEAK  EINT2_IRQHandler
        PUBWEAK  EINT3_IRQHandler
        PUBWEAK  ADC_IRQHandler
        PUBWEAK  BOD_IRQHandler
        PUBWEAK  USB_IRQHandler
        PUBWEAK  CAN_IRQHandler
        PUBWEAK  DMA_IRQHandler
        PUBWEAK  I2S_IRQHandler
        PUBWEAK  ENET_IRQHandler
        PUBWEAK  RIT_IRQHandler
        PUBWEAK  MCPWM_IRQHandler
        PUBWEAK  QEI_IRQHandler
        PUBWEAK  PLL1_IRQHandler
        PUBWEAK  USBActivity_IRQHandler
        PUBWEAK  CANActivity_IRQHandler

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler
        LDR     r0,=str_Undefined
        MOVS    r1,#1
        B       assert_failed
str_Undefined
        DCB     "Undefined"
        ALIGNROM 2

;*****************************************************************************
; The function assert_failed defines the error/assertion handling policy
; for the application. After making sure that the stack is OK, this function
; calls Q_onAssert, which should NOT return (typically reset the CPU).
;
; NOTE: the function Q_onAssert should NOT return.
;
; The C proptotype of the assert_failed() and Q_onAssert() functions are:
; void assert_failed(char const *file, int line);
; void Q_onAssert   (char const *file, int line);
;*****************************************************************************
        PUBLIC  assert_failed
        EXTERN  Q_onAssert
assert_failed
        LDR    r2,=sfe(CSTACK)   ; load the original top of stack
        MOV    sp,r2             ; re-set the SP in case of stack overflow
        BL     Q_onAssert        ; call the application-specific handler

        B      .                 ; should not be reached, but just in case...


        END                      ; end of module

