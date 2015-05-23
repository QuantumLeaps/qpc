;/***************************************************************************/
; * @file     startup_stm32f4xx.s for IAR ARM assembler
; * @brief    CMSIS Cortex-M4F Core Device Startup File for STM32F40xx devices
; * @version  CMSIS v4.2.0
; * @date     2015-03-16
; *
; * @description
; * Created from the CMSIS template for the specified device
; * Quantum Leaps, www.state-machine.com
; *
; * @note
; * The functions assert_failed/Q_onAssert defined at the end of this file
; * define the error/assertion handling policy for the application and might
; * need to be customized for each project. These functions are defined in
; * assembly to avoid accessing the stack, which might be corrupted by
; * the time assert_failed/Q_onAssert are called.
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

;******************************************************************************
;
        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; Reset Handler
        DCD     NMI_Handler                ; NMI Handler
        DCD     HardFault_Handler          ; Hard Fault Handler
        DCD     MemManage_Handler          ; The MPU fault handler
        DCD     BusFault_Handler           ; The bus fault handler
        DCD     UsageFault_Handler         ; The usage fault handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     SVC_Handler                ; SVCall handler
        DCD     DebugMon_Handler           ; Debug monitor handler
        DCD     0                          ; Reserved
        DCD     PendSV_Handler             ; The PendSV handler
        DCD     SysTick_Handler            ; The SysTick handler

        ; IRQ handlers...
        DCD     WWDG_IRQHandler            ; Window WatchDog
        DCD     PVD_IRQHandler             ; PVD through EXTI Line detection
        DCD     TAMP_STAMP_IRQHandler      ; Tamper and TimeStamps through the EXTI line
        DCD     RTC_WKUP_IRQHandler        ; RTC Wakeup through the EXTI line
        DCD     FLASH_IRQHandler           ; FLASH
        DCD     RCC_IRQHandler             ; RCC
        DCD     EXTI0_IRQHandler           ; EXTI Line0
        DCD     EXTI1_IRQHandler           ; EXTI Line1
        DCD     EXTI2_IRQHandler           ; EXTI Line2
        DCD     EXTI3_IRQHandler           ; EXTI Line3
        DCD     EXTI4_IRQHandler           ; EXTI Line4
        DCD     DMA1_Stream0_IRQHandler    ; DMA1 Stream 0
        DCD     DMA1_Stream1_IRQHandler    ; DMA1 Stream 1
        DCD     DMA1_Stream2_IRQHandler    ; DMA1 Stream 2
        DCD     DMA1_Stream3_IRQHandler    ; DMA1 Stream 3
        DCD     DMA1_Stream4_IRQHandler    ; DMA1 Stream 4
        DCD     DMA1_Stream5_IRQHandler    ; DMA1 Stream 5
        DCD     DMA1_Stream6_IRQHandler    ; DMA1 Stream 6
        DCD     ADC_IRQHandler             ; ADC1, ADC2 and ADC3s
        DCD     CAN1_TX_IRQHandler         ; CAN1 TX
        DCD     CAN1_RX0_IRQHandler        ; CAN1 RX0
        DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
        DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
        DCD     EXTI9_5_IRQHandler         ; External Line[9:5]s
        DCD     TIM1_BRK_TIM9_IRQHandler   ; TIM1 Break and TIM9
        DCD     TIM1_UP_TIM10_IRQHandler   ; TIM1 Update and TIM10
        DCD     TIM1_TRG_COM_TIM11_IRQHandler ; TIM1 Trigger and Commutation and TIM11
        DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler            ; TIM2
        DCD     TIM3_IRQHandler            ; TIM3
        DCD     TIM4_IRQHandler            ; TIM4
        DCD     I2C1_EV_IRQHandler         ; I2C1 Event
        DCD     I2C1_ER_IRQHandler         ; I2C1 Error
        DCD     I2C2_EV_IRQHandler         ; I2C2 Event
        DCD     I2C2_ER_IRQHandler         ; I2C2 Error
        DCD     SPI1_IRQHandler            ; SPI1
        DCD     SPI2_IRQHandler            ; SPI2
        DCD     USART1_IRQHandler          ; USART1
        DCD     USART2_IRQHandler          ; USART2
        DCD     USART3_IRQHandler          ; USART3
        DCD     EXTI15_10_IRQHandler       ; External Line[15:10]s
        DCD     RTC_Alarm_IRQHandler       ; RTC Alarm (A and B) through EXTI Line
        DCD     OTG_FS_WKUP_IRQHandler     ; USB OTG FS Wakeup through EXTI line
        DCD     TIM8_BRK_TIM12_IRQHandler  ; TIM8 Break and TIM12
        DCD     TIM8_UP_TIM13_IRQHandler   ; TIM8 Update and TIM13
        DCD     TIM8_TRG_COM_TIM14_IRQHandler ; TIM8 Trigger and Commutation and TIM14
        DCD     TIM8_CC_IRQHandler         ; TIM8 Capture Compare
        DCD     DMA1_Stream7_IRQHandler    ; DMA1 Stream7
        DCD     FSMC_IRQHandler            ; FSMC
        DCD     SDIO_IRQHandler            ; SDIO
        DCD     TIM5_IRQHandler            ; TIM5
        DCD     SPI3_IRQHandler            ; SPI3
        DCD     UART4_IRQHandler           ; UART4
        DCD     UART5_IRQHandler           ; UART5
        DCD     TIM6_DAC_IRQHandler        ; TIM6 and DAC1&2 underrun errors
        DCD     TIM7_IRQHandler            ; TIM7
        DCD     DMA2_Stream0_IRQHandler    ; DMA2 Stream 0
        DCD     DMA2_Stream1_IRQHandler    ; DMA2 Stream 1
        DCD     DMA2_Stream2_IRQHandler    ; DMA2 Stream 2
        DCD     DMA2_Stream3_IRQHandler    ; DMA2 Stream 3
        DCD     DMA2_Stream4_IRQHandler    ; DMA2 Stream 4
        DCD     ETH_IRQHandler             ; Ethernet
        DCD     ETH_WKUP_IRQHandler        ; Ethernet Wakeup through EXTI line
        DCD     CAN2_TX_IRQHandler         ; CAN2 TX
        DCD     CAN2_RX0_IRQHandler        ; CAN2 RX0
        DCD     CAN2_RX1_IRQHandler        ; CAN2 RX1
        DCD     CAN2_SCE_IRQHandler        ; CAN2 SCE
        DCD     OTG_FS_IRQHandler          ; USB OTG FS
        DCD     DMA2_Stream5_IRQHandler    ; DMA2 Stream 5
        DCD     DMA2_Stream6_IRQHandler    ; DMA2 Stream 6
        DCD     DMA2_Stream7_IRQHandler    ; DMA2 Stream 7
        DCD     USART6_IRQHandler          ; USART6
        DCD     I2C3_EV_IRQHandler         ; I2C3 event
        DCD     I2C3_ER_IRQHandler         ; I2C3 error
        DCD     OTG_HS_EP1_OUT_IRQHandler  ; USB OTG HS End Point 1 Out
        DCD     OTG_HS_EP1_IN_IRQHandler   ; USB OTG HS End Point 1 In
        DCD     OTG_HS_WKUP_IRQHandler     ; USB OTG HS Wakeup through EXTI
        DCD     OTG_HS_IRQHandler          ; USB OTG HS
        DCD     DCMI_IRQHandler            ; DCMI
        DCD     CRYP_IRQHandler            ; CRYP crypto
        DCD     HASH_RNG_IRQHandler        ; Hash and Rng
        DCD     FPU_IRQHandler             ; FPU

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
        MOVS    r0,#0
        MOVS    r1,#2       ; NMI exception number
        B       assert_failed
;.............................................................................
        PUBWEAK HardFault_Handler
HardFault_Handler
        MOVS    r0,#0
        MOVS    r1,#3       ; HardFault exception number
        B       assert_failed
;.............................................................................
        PUBWEAK MemManage_Handler
MemManage_Handler
        MOVS    r0,#0
        MOVS    r1,#4       ; MemManage exception number
        B       assert_failed
;.............................................................................
        PUBWEAK BusFault_Handler
BusFault_Handler
        MOVS    r0,#0
        MOVS    r1,#5       ; BusFault exception number
        B       assert_failed
;.............................................................................
        PUBWEAK UsageFault_Handler
UsageFault_Handler
        MOVS    r0,#0
        MOVS    r1,#6       ; UsageFault exception number
        B       assert_failed


;******************************************************************************
;
; Weak non-fault handlers...
;

        PUBWEAK SVC_Handler
SVC_Handler
        MOVS    r0,#0
        MOVS    r1,#11      ; SVCall exception number
        B       assert_failed
;.............................................................................
        PUBWEAK DebugMon_Handler
DebugMon_Handler
        MOVS    r0,#0
        MOVS    r1,#12      ; DebugMon exception number
        B       assert_failed
;.............................................................................
        PUBWEAK PendSV_Handler
PendSV_Handler
        MOVS    r0,#0
        MOVS    r1,#14      ; PendSV exception number
        B       assert_failed
;.............................................................................
        PUBWEAK SysTick_Handler
SysTick_Handler
        MOVS    r0,#0
        MOVS    r1,#15      ; SysTick exception number
        B       assert_failed


;******************************************************************************
;
; Weak IRQ handlers...
;

        PUBWEAK  WWDG_IRQHandler
        PUBWEAK  PVD_IRQHandler
        PUBWEAK  TAMP_STAMP_IRQHandler
        PUBWEAK  RTC_WKUP_IRQHandler
        PUBWEAK  FLASH_IRQHandler
        PUBWEAK  RCC_IRQHandler
        PUBWEAK  EXTI0_IRQHandler
        PUBWEAK  EXTI1_IRQHandler
        PUBWEAK  EXTI2_IRQHandler
        PUBWEAK  EXTI3_IRQHandler
        PUBWEAK  EXTI4_IRQHandler
        PUBWEAK  DMA1_Stream0_IRQHandler
        PUBWEAK  DMA1_Stream1_IRQHandler
        PUBWEAK  DMA1_Stream2_IRQHandler
        PUBWEAK  DMA1_Stream3_IRQHandler
        PUBWEAK  DMA1_Stream4_IRQHandler
        PUBWEAK  DMA1_Stream5_IRQHandler
        PUBWEAK  DMA1_Stream6_IRQHandler
        PUBWEAK  ADC_IRQHandler
        PUBWEAK  CAN1_TX_IRQHandler
        PUBWEAK  CAN1_RX0_IRQHandler
        PUBWEAK  CAN1_RX1_IRQHandler
        PUBWEAK  CAN1_SCE_IRQHandler
        PUBWEAK  EXTI9_5_IRQHandler
        PUBWEAK  TIM1_BRK_TIM9_IRQHandler
        PUBWEAK  TIM1_UP_TIM10_IRQHandler
        PUBWEAK  TIM1_TRG_COM_TIM11_IRQHandler
        PUBWEAK  TIM1_CC_IRQHandler
        PUBWEAK  TIM2_IRQHandler
        PUBWEAK  TIM3_IRQHandler
        PUBWEAK  TIM4_IRQHandler
        PUBWEAK  I2C1_EV_IRQHandler
        PUBWEAK  I2C1_ER_IRQHandler
        PUBWEAK  I2C2_EV_IRQHandler
        PUBWEAK  I2C2_ER_IRQHandler
        PUBWEAK  SPI1_IRQHandler
        PUBWEAK  SPI2_IRQHandler
        PUBWEAK  USART1_IRQHandler
        PUBWEAK  USART2_IRQHandler
        PUBWEAK  USART3_IRQHandler
        PUBWEAK  EXTI15_10_IRQHandler
        PUBWEAK  RTC_Alarm_IRQHandler
        PUBWEAK  OTG_FS_WKUP_IRQHandler
        PUBWEAK  TIM8_BRK_TIM12_IRQHandler
        PUBWEAK  TIM8_UP_TIM13_IRQHandler
        PUBWEAK  TIM8_TRG_COM_TIM14_IRQHandler
        PUBWEAK  TIM8_CC_IRQHandler
        PUBWEAK  DMA1_Stream7_IRQHandler
        PUBWEAK  FSMC_IRQHandler
        PUBWEAK  SDIO_IRQHandler
        PUBWEAK  TIM5_IRQHandler
        PUBWEAK  SPI3_IRQHandler
        PUBWEAK  UART4_IRQHandler
        PUBWEAK  UART5_IRQHandler
        PUBWEAK  TIM6_DAC_IRQHandler
        PUBWEAK  TIM7_IRQHandler
        PUBWEAK  DMA2_Stream0_IRQHandler
        PUBWEAK  DMA2_Stream1_IRQHandler
        PUBWEAK  DMA2_Stream2_IRQHandler
        PUBWEAK  DMA2_Stream3_IRQHandler
        PUBWEAK  DMA2_Stream4_IRQHandler
        PUBWEAK  ETH_IRQHandler
        PUBWEAK  ETH_WKUP_IRQHandler
        PUBWEAK  CAN2_TX_IRQHandler
        PUBWEAK  CAN2_RX0_IRQHandler
        PUBWEAK  CAN2_RX1_IRQHandler
        PUBWEAK  CAN2_SCE_IRQHandler
        PUBWEAK  OTG_FS_IRQHandler
        PUBWEAK  DMA2_Stream5_IRQHandler
        PUBWEAK  DMA2_Stream6_IRQHandler
        PUBWEAK  DMA2_Stream7_IRQHandler
        PUBWEAK  USART6_IRQHandler
        PUBWEAK  I2C3_EV_IRQHandler
        PUBWEAK  I2C3_ER_IRQHandler
        PUBWEAK  OTG_HS_EP1_OUT_IRQHandler
        PUBWEAK  OTG_HS_EP1_IN_IRQHandler
        PUBWEAK  OTG_HS_WKUP_IRQHandler
        PUBWEAK  OTG_HS_IRQHandler
        PUBWEAK  DCMI_IRQHandler
        PUBWEAK  CRYP_IRQHandler
        PUBWEAK  HASH_RNG_IRQHandler
        PUBWEAK  FPU_IRQHandler

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
FSMC_IRQHandler
SDIO_IRQHandler
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
CRYP_IRQHandler
HASH_RNG_IRQHandler
FPU_IRQHandler
        MOV     r0,#0
        MOV     r1,#-1      ; 0xFFFFFFF
        B       assert_failed

;******************************************************************************
;
; The functions assert_failed/Q_onAssert define the error/assertion
; handling policy for the application and might need to be customized
; for each project. These functions are defined in assembly to avoid
; accessing the stack, which might be corrupted by the time assert_failed
; is called. For now the function just resets the CPU.
;
; NOTE: the functions assert_failed/Q_onAssert should NOT return.
;
; The C proptotypes of these functions are as follows:
; void assert_failed(char const *file, int line);
; void Q_onAssert   (char const *file, int line);
;******************************************************************************
        PUBLIC  assert_failed
        PUBLIC  Q_onAssert
Q_onAssert
assert_failed
        ;
        ; NOTE: add here your application-specific error handling
        ;

        ; the following code implements the CMIS function
        ; NVIC_SystemReset() from core_cm4.h
        ; Leave this code if you wish to reset the system after an error.
        DSB                      ; ensure all memory access complete
        LDR    r0,=0x05FA0004    ; (0x5FA << SCB_AIRCR_VECTKEY_Pos)
                                 ; | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk)
                                 ; | SCB_AIRCR_SYSRESETREQ_Msk
        LDR    r1,=0xE000ED0C    ; address of SCB->AIRCR
        STR    r0,[r1]           ; r0 -> SCB->AIRCR
        DSB                      ; ensure all memory access complete
        B      .                 ; wait until reset occurs


        END
