;/***************************************************************************/
; * @file     startup_stm32f746xx.s for IAR ARM assembler
; * @brief    CMSIS Cortex-M7 Core Device Startup File for TM4C123GH6PM
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
        DCD     WWDG_IRQHandler             ; Window WatchDog
        DCD     PVD_IRQHandler              ; PVD through EXTI Line detection
        DCD     TAMP_STAMP_IRQHandler       ; Tamper and TimeStamps through the EXTI line
        DCD     RTC_WKUP_IRQHandler         ; RTC Wakeup through the EXTI line
        DCD     FLASH_IRQHandler            ; FLASH
        DCD     RCC_IRQHandler              ; RCC
        DCD     EXTI0_IRQHandler            ; EXTI Line0
        DCD     EXTI1_IRQHandler            ; EXTI Line1
        DCD     EXTI2_IRQHandler            ; EXTI Line2
        DCD     EXTI3_IRQHandler            ; EXTI Line3
        DCD     EXTI4_IRQHandler            ; EXTI Line4
        DCD     DMA1_Stream0_IRQHandler     ; DMA1 Stream 0
        DCD     DMA1_Stream1_IRQHandler     ; DMA1 Stream 1
        DCD     DMA1_Stream2_IRQHandler     ; DMA1 Stream 2
        DCD     DMA1_Stream3_IRQHandler     ; DMA1 Stream 3
        DCD     DMA1_Stream4_IRQHandler     ; DMA1 Stream 4
        DCD     DMA1_Stream5_IRQHandler     ; DMA1 Stream 5
        DCD     DMA1_Stream6_IRQHandler     ; DMA1 Stream 6
        DCD     ADC_IRQHandler              ; ADC1, ADC2 and ADC3s
        DCD     CAN1_TX_IRQHandler          ; CAN1 TX
        DCD     CAN1_RX0_IRQHandler         ; CAN1 RX0
        DCD     CAN1_RX1_IRQHandler         ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler         ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler          ; External Line[9:5]s
        DCD     TIM1_BRK_TIM9_IRQHandler    ; TIM1 Break and TIM9
        DCD     TIM1_UP_TIM10_IRQHandler    ; TIM1 Update and TIM10
        DCD     TIM1_TRG_COM_TIM11_IRQHandler ; TIM1 Trigger and Commutation and TIM11
        DCD     TIM1_CC_IRQHandler          ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler             ; TIM2
        DCD     TIM3_IRQHandler             ; TIM3
        DCD     TIM4_IRQHandler             ; TIM4
        DCD     I2C1_EV_IRQHandler          ; I2C1 Event
        DCD     I2C1_ER_IRQHandler          ; I2C1 Error
        DCD     I2C2_EV_IRQHandler          ; I2C2 Event
        DCD     I2C2_ER_IRQHandler          ; I2C2 Error
        DCD     SPI1_IRQHandler             ; SPI1
        DCD     SPI2_IRQHandler             ; SPI2
        DCD     USART1_IRQHandler           ; USART1
        DCD     USART2_IRQHandler           ; USART2
        DCD     USART3_IRQHandler           ; USART3
        DCD     EXTI15_10_IRQHandler        ; External Line[15:10]s
        DCD     RTC_Alarm_IRQHandler        ; RTC Alarm (A and B) through EXTI Line
        DCD     OTG_FS_WKUP_IRQHandler      ; USB OTG FS Wakeup through EXTI line
        DCD     TIM8_BRK_TIM12_IRQHandler   ; TIM8 Break and TIM12
        DCD     TIM8_UP_TIM13_IRQHandler    ; TIM8 Update and TIM13
        DCD     TIM8_TRG_COM_TIM14_IRQHandler ; TIM8 Trigger and Commutation and TIM14
        DCD     TIM8_CC_IRQHandler          ; TIM8 Capture Compare
        DCD     DMA1_Stream7_IRQHandler     ; DMA1 Stream7
        DCD     FMC_IRQHandler              ; FMC
        DCD     SDMMC1_IRQHandler           ; SDMMC1
        DCD     TIM5_IRQHandler             ; TIM5
        DCD     SPI3_IRQHandler             ; SPI3
        DCD     UART4_IRQHandler            ; UART4
        DCD     UART5_IRQHandler            ; UART5
        DCD     TIM6_DAC_IRQHandler         ; TIM6 and DAC1&2 underrun errors
        DCD     TIM7_IRQHandler             ; TIM7
        DCD     DMA2_Stream0_IRQHandler     ; DMA2 Stream 0
        DCD     DMA2_Stream1_IRQHandler     ; DMA2 Stream 1
        DCD     DMA2_Stream2_IRQHandler     ; DMA2 Stream 2
        DCD     DMA2_Stream3_IRQHandler     ; DMA2 Stream 3
        DCD     DMA2_Stream4_IRQHandler     ; DMA2 Stream 4
        DCD     ETH_IRQHandler              ; Ethernet
        DCD     ETH_WKUP_IRQHandler         ; Ethernet Wakeup through EXTI line
        DCD     CAN2_TX_IRQHandler          ; CAN2 TX
        DCD     CAN2_RX0_IRQHandler         ; CAN2 RX0
        DCD     CAN2_RX1_IRQHandler         ; CAN2 RX1
        DCD     CAN2_SCE_IRQHandler         ; CAN2 SCE
        DCD     OTG_FS_IRQHandler           ; USB OTG FS
        DCD     DMA2_Stream5_IRQHandler     ; DMA2 Stream 5
        DCD     DMA2_Stream6_IRQHandler     ; DMA2 Stream 6
        DCD     DMA2_Stream7_IRQHandler     ; DMA2 Stream 7
        DCD     USART6_IRQHandler           ; USART6
        DCD     I2C3_EV_IRQHandler          ; I2C3 event
        DCD     I2C3_ER_IRQHandler          ; I2C3 error
        DCD     OTG_HS_EP1_OUT_IRQHandler   ; USB OTG HS End Point 1 Out
        DCD     OTG_HS_EP1_IN_IRQHandler    ; USB OTG HS End Point 1 In
        DCD     OTG_HS_WKUP_IRQHandler      ; USB OTG HS Wakeup through EXTI
        DCD     OTG_HS_IRQHandler           ; USB OTG HS
        DCD     DCMI_IRQHandler             ; DCMI
        DCD     0                           ; Reserved
        DCD     RNG_IRQHandler              ; Rng
        DCD     FPU_IRQHandler              ; FPU
        DCD     UART7_IRQHandler            ; UART7
        DCD     UART8_IRQHandler            ; UART8
        DCD     SPI4_IRQHandler             ; SPI4
        DCD     SPI5_IRQHandler             ; SPI5
        DCD     SPI6_IRQHandler             ; SPI6
        DCD     SAI1_IRQHandler             ; SAI1
        DCD     LTDC_IRQHandler             ; LTDC
        DCD     LTDC_ER_IRQHandler          ; LTDC error
        DCD     DMA2D_IRQHandler            ; DMA2D
        DCD     SAI2_IRQHandler             ; SAI2
        DCD     QUADSPI_IRQHandler          ; QUADSPI
        DCD     LPTIM1_IRQHandler           ; LPTIM1
        DCD     CEC_IRQHandler              ; HDMI_CEC
        DCD     I2C4_EV_IRQHandler          ; I2C4 Event
        DCD     I2C4_ER_IRQHandler          ; I2C4 Error
        DCD     SPDIF_RX_IRQHandler         ; SPDIF_RX

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

        PUBWEAK WWDG_IRQHandler
        PUBWEAK PVD_IRQHandler
        PUBWEAK TAMP_STAMP_IRQHandler
        PUBWEAK RTC_WKUP_IRQHandler
        PUBWEAK FLASH_IRQHandler
        PUBWEAK RCC_IRQHandler
        PUBWEAK EXTI0_IRQHandler
        PUBWEAK EXTI1_IRQHandler
        PUBWEAK EXTI2_IRQHandler
        PUBWEAK EXTI3_IRQHandler
        PUBWEAK EXTI4_IRQHandler
        PUBWEAK DMA1_Stream0_IRQHandler
        PUBWEAK DMA1_Stream1_IRQHandler
        PUBWEAK DMA1_Stream2_IRQHandler
        PUBWEAK DMA1_Stream3_IRQHandler
        PUBWEAK DMA1_Stream4_IRQHandler
        PUBWEAK DMA1_Stream5_IRQHandler
        PUBWEAK DMA1_Stream6_IRQHandler
        PUBWEAK ADC_IRQHandler
        PUBWEAK CAN1_TX_IRQHandler
        PUBWEAK CAN1_RX0_IRQHandler
        PUBWEAK CAN1_RX1_IRQHandler
        PUBWEAK CAN1_SCE_IRQHandler
        PUBWEAK EXTI9_5_IRQHandler
        PUBWEAK TIM1_BRK_TIM9_IRQHandler
        PUBWEAK TIM1_UP_TIM10_IRQHandler
        PUBWEAK TIM1_TRG_COM_TIM11_IRQHandler
        PUBWEAK TIM1_CC_IRQHandler
        PUBWEAK TIM2_IRQHandler
        PUBWEAK TIM3_IRQHandler
        PUBWEAK TIM4_IRQHandler
        PUBWEAK I2C1_EV_IRQHandler
        PUBWEAK I2C1_ER_IRQHandler
        PUBWEAK I2C2_EV_IRQHandler
        PUBWEAK I2C2_ER_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI2_IRQHandler
        PUBWEAK USART1_IRQHandler
        PUBWEAK USART2_IRQHandler
        PUBWEAK USART3_IRQHandler
        PUBWEAK EXTI15_10_IRQHandler
        PUBWEAK RTC_Alarm_IRQHandler
        PUBWEAK OTG_FS_WKUP_IRQHandler
        PUBWEAK TIM8_BRK_TIM12_IRQHandler
        PUBWEAK TIM8_UP_TIM13_IRQHandler
        PUBWEAK TIM8_TRG_COM_TIM14_IRQHandler
        PUBWEAK TIM8_CC_IRQHandler
        PUBWEAK DMA1_Stream7_IRQHandler
        PUBWEAK FMC_IRQHandler
        PUBWEAK SDMMC1_IRQHandler
        PUBWEAK TIM5_IRQHandler
        PUBWEAK SPI3_IRQHandler
        PUBWEAK UART4_IRQHandler
        PUBWEAK UART5_IRQHandler
        PUBWEAK TIM6_DAC_IRQHandler
        PUBWEAK TIM7_IRQHandler
        PUBWEAK DMA2_Stream0_IRQHandler
        PUBWEAK DMA2_Stream1_IRQHandler
        PUBWEAK DMA2_Stream2_IRQHandler
        PUBWEAK DMA2_Stream3_IRQHandler
        PUBWEAK DMA2_Stream4_IRQHandler
        PUBWEAK ETH_IRQHandler
        PUBWEAK ETH_WKUP_IRQHandler
        PUBWEAK CAN2_TX_IRQHandler
        PUBWEAK CAN2_RX0_IRQHandler
        PUBWEAK CAN2_RX1_IRQHandler
        PUBWEAK CAN2_SCE_IRQHandler
        PUBWEAK OTG_FS_IRQHandler
        PUBWEAK DMA2_Stream5_IRQHandler
        PUBWEAK DMA2_Stream6_IRQHandler
        PUBWEAK DMA2_Stream7_IRQHandler
        PUBWEAK USART6_IRQHandler
        PUBWEAK I2C3_EV_IRQHandler
        PUBWEAK I2C3_ER_IRQHandler
        PUBWEAK OTG_HS_EP1_OUT_IRQHandler
        PUBWEAK OTG_HS_EP1_IN_IRQHandler
        PUBWEAK OTG_HS_WKUP_IRQHandler
        PUBWEAK OTG_HS_IRQHandler
        PUBWEAK DCMI_IRQHandler
        PUBWEAK RNG_IRQHandler
        PUBWEAK FPU_IRQHandler
        PUBWEAK UART7_IRQHandler
        PUBWEAK UART8_IRQHandler
        PUBWEAK SPI4_IRQHandler
        PUBWEAK SPI5_IRQHandler
        PUBWEAK SPI6_IRQHandler
        PUBWEAK SAI1_IRQHandler
        PUBWEAK LTDC_IRQHandler
        PUBWEAK LTDC_ER_IRQHandler
        PUBWEAK DMA2D_IRQHandler
        PUBWEAK SAI2_IRQHandler
        PUBWEAK QUADSPI_IRQHandler
        PUBWEAK LPTIM1_IRQHandler
        PUBWEAK CEC_IRQHandler
        PUBWEAK I2C4_EV_IRQHandler
        PUBWEAK I2C4_ER_IRQHandler
        PUBWEAK SPDIF_RX_IRQHandler


WWDG_IRQHandler
PVD_IRQHandler
TAMP_STAMP_IRQHandler
RTC_WKUP_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Stream0_IRQHandler
DMA1_Stream1_IRQHandler
DMA1_Stream2_IRQHandler
DMA1_Stream3_IRQHandler
DMA1_Stream4_IRQHandler
DMA1_Stream5_IRQHandler
DMA1_Stream6_IRQHandler
ADC_IRQHandler
CAN1_TX_IRQHandler
CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_TIM9_IRQHandler
TIM1_UP_TIM10_IRQHandler
TIM1_TRG_COM_TIM11_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTC_Alarm_IRQHandler
OTG_FS_WKUP_IRQHandler
TIM8_BRK_TIM12_IRQHandler
TIM8_UP_TIM13_IRQHandler
TIM8_TRG_COM_TIM14_IRQHandler
TIM8_CC_IRQHandler
DMA1_Stream7_IRQHandler
FMC_IRQHandler
SDMMC1_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_DAC_IRQHandler
TIM7_IRQHandler
DMA2_Stream0_IRQHandler
DMA2_Stream1_IRQHandler
DMA2_Stream2_IRQHandler
DMA2_Stream3_IRQHandler
DMA2_Stream4_IRQHandler
ETH_IRQHandler
ETH_WKUP_IRQHandler
CAN2_TX_IRQHandler
CAN2_RX0_IRQHandler
CAN2_RX1_IRQHandler
CAN2_SCE_IRQHandler
OTG_FS_IRQHandler
DMA2_Stream5_IRQHandler
DMA2_Stream6_IRQHandler
DMA2_Stream7_IRQHandler
USART6_IRQHandler
I2C3_EV_IRQHandler
I2C3_ER_IRQHandler
OTG_HS_EP1_OUT_IRQHandler
OTG_HS_EP1_IN_IRQHandler
OTG_HS_WKUP_IRQHandler
OTG_HS_IRQHandler
DCMI_IRQHandler
RNG_IRQHandler
FPU_IRQHandler
UART7_IRQHandler
UART8_IRQHandler
SPI4_IRQHandler
SPI5_IRQHandler
SPI6_IRQHandler
SAI1_IRQHandler
LTDC_IRQHandler
LTDC_ER_IRQHandler
DMA2D_IRQHandler
SAI2_IRQHandler
QUADSPI_IRQHandler
LPTIM1_IRQHandler
CEC_IRQHandler
I2C4_EV_IRQHandler
I2C4_ER_IRQHandler
SPDIF_RX_IRQHandler
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

