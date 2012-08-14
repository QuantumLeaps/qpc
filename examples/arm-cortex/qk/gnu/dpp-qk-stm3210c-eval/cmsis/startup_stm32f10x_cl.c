/*****************************************************************************
* Purpose: CMSIS-compliant startup code for STM32F10x connectivity line
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 04, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/

void __attribute__ ((weak)) Reset_Handler(void);
void __attribute__ ((weak)) NMI_Handler(void);
void __attribute__ ((weak)) HardFault_Handler(void);
void __attribute__ ((weak)) MemManage_Handler(void);
void __attribute__ ((weak)) BusFault_Handler(void);
void __attribute__ ((weak)) UsageFault_Handler(void);
void __attribute__ ((weak)) MemManage_Handler(void);
void __attribute__ ((weak)) SVC_Handler(void);
void __attribute__ ((weak)) DebugMon_Handler(void);
void __attribute__ ((weak)) PendSV_Handler(void);
void __attribute__ ((weak)) SysTick_Handler(void);

                                                  /* external interrupts... */
void __attribute__ ((weak)) WWDG_IRQHandler(void);
void __attribute__ ((weak)) PVD_IRQHandler(void);
void __attribute__ ((weak)) TAMPER_IRQHandler(void);
void __attribute__ ((weak)) RTC_IRQHandler(void);
void __attribute__ ((weak)) FLASH_IRQHandler(void);
void __attribute__ ((weak)) RCC_IRQHandler(void);
void __attribute__ ((weak)) EXTI0_IRQHandler(void);
void __attribute__ ((weak)) EXTI1_IRQHandler(void);
void __attribute__ ((weak)) EXTI2_IRQHandler(void);
void __attribute__ ((weak)) EXTI3_IRQHandler(void);
void __attribute__ ((weak)) EXTI4_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel1_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel2_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel3_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel4_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel5_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel6_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Channel7_IRQHandler(void);
void __attribute__ ((weak)) ADC1_2_IRQHandler(void);
void __attribute__ ((weak)) USB_HP_CAN1_TX_IRQHandler(void);
void __attribute__ ((weak)) USB_LP_CAN1_RX0_IRQHandler(void);
void __attribute__ ((weak)) CAN1_RX1_IRQHandler(void);
void __attribute__ ((weak)) CAN1_SCE_IRQHandler(void);
void __attribute__ ((weak)) EXTI9_5_IRQHandler(void);
void __attribute__ ((weak)) TIM1_BRK_IRQHandler(void);
void __attribute__ ((weak)) TIM1_UP_IRQHandler(void);
void __attribute__ ((weak)) TIM1_TRG_COM_IRQHandler(void);
void __attribute__ ((weak)) TIM1_CC_IRQHandler(void);
void __attribute__ ((weak)) TIM2_IRQHandler(void);
void __attribute__ ((weak)) TIM3_IRQHandler(void);
void __attribute__ ((weak)) TIM4_IRQHandler(void);
void __attribute__ ((weak)) I2C1_EV_IRQHandler(void);
void __attribute__ ((weak)) I2C1_ER_IRQHandler(void);
void __attribute__ ((weak)) I2C2_EV_IRQHandler(void);
void __attribute__ ((weak)) I2C2_ER_IRQHandler(void);
void __attribute__ ((weak)) SPI1_IRQHandler(void);
void __attribute__ ((weak)) SPI2_IRQHandler(void);
void __attribute__ ((weak)) USART1_IRQHandler(void);
void __attribute__ ((weak)) USART2_IRQHandler(void);
void __attribute__ ((weak)) USART3_IRQHandler(void);
void __attribute__ ((weak)) EXTI15_10_IRQHandler(void);
void __attribute__ ((weak)) RTCAlarm_IRQHandler(void);
void __attribute__ ((weak)) USBWakeUp_IRQHandler(void);
void __attribute__ ((weak)) TIM8_BRK_IRQHandler(void);
void __attribute__ ((weak)) TIM8_UP_IRQHandler(void);
void __attribute__ ((weak)) TIM8_TRG_COM_IRQHandler(void);
void __attribute__ ((weak)) TIM8_CC_IRQHandler(void);
void __attribute__ ((weak)) ADC3_IRQHandler(void);
void __attribute__ ((weak)) FSMC_IRQHandler(void);
void __attribute__ ((weak)) SDIO_IRQHandler(void);
void __attribute__ ((weak)) TIM5_IRQHandler(void);
void __attribute__ ((weak)) SPI3_IRQHandler(void);
void __attribute__ ((weak)) UART4_IRQHandler(void);
void __attribute__ ((weak)) UART5_IRQHandler(void);
void __attribute__ ((weak)) TIM6_IRQHandler(void);
void __attribute__ ((weak)) TIM7_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Channel1_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Channel2_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Channel3_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Channel4_5_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Channel5_IRQHandler(void);
void __attribute__ ((weak)) ETH_IRQHandler(void);
void __attribute__ ((weak)) vMAC_IRQHandler(void);
void __attribute__ ((weak)) ETH_WKUP_IRQHandler(void);
void __attribute__ ((weak)) CAN2_TX_IRQHandler(void);
void __attribute__ ((weak)) CAN2_RX0_IRQHandler(void);
void __attribute__ ((weak)) CAN2_RX1_IRQHandler(void);
void __attribute__ ((weak)) CAN2_SCE_IRQHandler(void);
void __attribute__ ((weak)) OTG_FS_IRQHandler(void);

void __attribute__ ((weak)) Spurious_Handler(void);

/*----------------------------------------------------------------------------
* weak aliases for each Exception handler to the Spurious_Handler.
* Any function with the same name will override these definitions.
*/
#pragma weak NMI_Handler                = Spurious_Handler
#pragma weak MemManage_Handler          = Spurious_Handler
#pragma weak BusFault_Handler           = Spurious_Handler
#pragma weak UsageFault_Handler         = Spurious_Handler
#pragma weak SVC_Handler                = Spurious_Handler
#pragma weak DebugMon_Handler           = Spurious_Handler
#pragma weak PendSV_Handler             = Spurious_Handler
#pragma weak SysTick_Handler            = Spurious_Handler
#pragma weak WWDG_IRQHandler            = Spurious_Handler
#pragma weak PVD_IRQHandler             = Spurious_Handler
#pragma weak TAMPER_IRQHandler          = Spurious_Handler
#pragma weak RTC_IRQHandler             = Spurious_Handler
#pragma weak FLASH_IRQHandler           = Spurious_Handler
#pragma weak RCC_IRQHandler             = Spurious_Handler
#pragma weak EXTI0_IRQHandler           = Spurious_Handler
#pragma weak EXTI1_IRQHandler           = Spurious_Handler
#pragma weak EXTI2_IRQHandler           = Spurious_Handler
#pragma weak EXTI3_IRQHandler           = Spurious_Handler
#pragma weak EXTI4_IRQHandler           = Spurious_Handler
#pragma weak DMA1_Channel1_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel2_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel3_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel4_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel5_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel6_IRQHandler   = Spurious_Handler
#pragma weak DMA1_Channel7_IRQHandler   = Spurious_Handler
#pragma weak ADC1_2_IRQHandler          = Spurious_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler  = Spurious_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler = Spurious_Handler
#pragma weak CAN1_RX1_IRQHandler        = Spurious_Handler
#pragma weak CAN1_SCE_IRQHandler        = Spurious_Handler
#pragma weak EXTI9_5_IRQHandler         = Spurious_Handler
#pragma weak TIM1_BRK_IRQHandler        = Spurious_Handler
#pragma weak TIM1_UP_IRQHandler         = Spurious_Handler
#pragma weak TIM1_TRG_COM_IRQHandler    = Spurious_Handler
#pragma weak TIM1_CC_IRQHandler         = Spurious_Handler
#pragma weak TIM2_IRQHandler            = Spurious_Handler
#pragma weak TIM3_IRQHandler            = Spurious_Handler
#pragma weak TIM4_IRQHandler            = Spurious_Handler
#pragma weak I2C1_EV_IRQHandler         = Spurious_Handler
#pragma weak I2C1_ER_IRQHandler         = Spurious_Handler
#pragma weak I2C2_EV_IRQHandler         = Spurious_Handler
#pragma weak I2C2_ER_IRQHandler         = Spurious_Handler
#pragma weak SPI1_IRQHandler            = Spurious_Handler
#pragma weak SPI2_IRQHandler            = Spurious_Handler
#pragma weak USART1_IRQHandler          = Spurious_Handler
#pragma weak USART2_IRQHandler          = Spurious_Handler
#pragma weak USART3_IRQHandler          = Spurious_Handler
#pragma weak EXTI15_10_IRQHandler       = Spurious_Handler
#pragma weak RTCAlarm_IRQHandler        = Spurious_Handler
#pragma weak USBWakeUp_IRQHandler       = Spurious_Handler
#pragma weak TIM8_BRK_IRQHandler        = Spurious_Handler
#pragma weak TIM8_UP_IRQHandler         = Spurious_Handler
#pragma weak TIM8_TRG_COM_IRQHandler    = Spurious_Handler
#pragma weak TIM8_CC_IRQHandler         = Spurious_Handler
#pragma weak ADC3_IRQHandler            = Spurious_Handler
#pragma weak FSMC_IRQHandler            = Spurious_Handler
#pragma weak SDIO_IRQHandler            = Spurious_Handler
#pragma weak TIM5_IRQHandler            = Spurious_Handler
#pragma weak SPI3_IRQHandler            = Spurious_Handler
#pragma weak UART4_IRQHandler           = Spurious_Handler
#pragma weak UART5_IRQHandler           = Spurious_Handler
#pragma weak TIM6_IRQHandler            = Spurious_Handler
#pragma weak TIM7_IRQHandler            = Spurious_Handler
#pragma weak DMA2_Channel1_IRQHandler   = Spurious_Handler
#pragma weak DMA2_Channel2_IRQHandler   = Spurious_Handler
#pragma weak DMA2_Channel3_IRQHandler   = Spurious_Handler
#pragma weak DMA2_Channel4_5_IRQHandler = Spurious_Handler
#pragma weak DMA2_Channel5_IRQHandler   = Spurious_Handler
#pragma weak ETH_IRQHandler             = Spurious_Handler
#pragma weak ETH_WKUP_IRQHandler        = Spurious_Handler
#pragma weak CAN2_TX_IRQHandler         = Spurious_Handler
#pragma weak CAN2_RX0_IRQHandler        = Spurious_Handler
#pragma weak CAN2_RX1_IRQHandler        = Spurious_Handler
#pragma weak CAN2_SCE_IRQHandler        = Spurious_Handler
#pragma weak OTG_FS_IRQHandler          = Spurious_Handler

/* exception and interrupt vector table ------------------------------------*/
typedef void (*ExceptionHandler)(void);
typedef union {
    ExceptionHandler handler;
    void            *pointer;
} VectorTableEntry;

                               /* top of stack defined in the linker script */
extern unsigned __c_stack_top__;

/*..........................................................................*/
__attribute__ ((section(".isr_vector")))
VectorTableEntry const g_pfnVectors[] = {
    { .pointer = &__c_stack_top__           }, /* initial stack pointer     */
    { .handler = &Reset_Handler             }, /* Reset Handler             */
    { .handler = &NMI_Handler               }, /* NMI Handler               */
    { .handler = &HardFault_Handler         }, /* Hard Fault Handler        */
    { .handler = &MemManage_Handler         }, /* MPU Fault Handler         */
    { .handler = &BusFault_Handler          }, /* Bus Fault Handler         */
    { .handler = &UsageFault_Handler        }, /* Usage Fault Handler       */
    { .handler = &Spurious_Handler          }, /* Reserved                  */
    { .handler = &Spurious_Handler          }, /* Reserved                  */
    { .handler = &Spurious_Handler          }, /* Reserved                  */
    { .handler = &Spurious_Handler          }, /* Reserved                  */
    { .handler = &SVC_Handler               }, /* SVCall Handler            */
    { .handler = &DebugMon_Handler          }, /* Debug Monitor Handler     */
    { .handler = &Spurious_Handler          }, /* Reserved                  */
    { .handler = &PendSV_Handler            }, /* PendSV Handler            */
    { .handler = &SysTick_Handler           }, /* SysTick Handler           */
    /* external interrupts (IRQs) ... */
    { .handler = &WWDG_IRQHandler           }, /* Window Watchdog           */
    { .handler = &PVD_IRQHandler            }, /* PVD through EXTI Line     */
    { .handler = &TAMPER_IRQHandler         }, /* Tamper                    */
    { .handler = &RTC_IRQHandler            }, /* RTC                       */
    { .handler = &FLASH_IRQHandler          }, /* Flash                     */
    { .handler = &RCC_IRQHandler            }, /* RCC                       */
    { .handler = &EXTI0_IRQHandler          }, /* EXTI Line 0               */
    { .handler = &EXTI1_IRQHandler          }, /* EXTI Line 1               */
    { .handler = &EXTI2_IRQHandler          }, /* EXTI Line 2               */
    { .handler = &EXTI3_IRQHandler          }, /* EXTI Line 3               */
    { .handler = &EXTI4_IRQHandler          }, /* EXTI Line 4               */
    { .handler = &DMA1_Channel1_IRQHandler  }, /* DMA1 Channel 1            */
    { .handler = &DMA1_Channel2_IRQHandler  }, /* DMA1 Channel 2            */
    { .handler = &DMA1_Channel3_IRQHandler  }, /* DMA1 Channel 3            */
    { .handler = &DMA1_Channel4_IRQHandler  }, /* DMA1 Channel 4            */
    { .handler = &DMA1_Channel5_IRQHandler  }, /* DMA1 Channel 5            */
    { .handler = &DMA1_Channel6_IRQHandler  }, /* DMA1 Channel 6            */
    { .handler = &DMA1_Channel7_IRQHandler  }, /* DMA1 Channel 7            */
    { .handler = &ADC1_2_IRQHandler         }, /* ADC1 & ADC2               */
    { .handler = &USB_HP_CAN1_TX_IRQHandler }, /* USB High-prio or CAN1 TX  */
    { .handler = &USB_LP_CAN1_RX0_IRQHandler}, /* USB Low-prio or CAN1 RX0  */
    { .handler = &CAN1_RX1_IRQHandler       }, /* CAN1 RX1                  */
    { .handler = &CAN1_SCE_IRQHandler       }, /* CAN1 SCE                  */
    { .handler = &EXTI9_5_IRQHandler        }, /* EXTI Line 9..5            */
    { .handler = &TIM1_BRK_IRQHandler       }, /* TIM1 Break                */
    { .handler = &TIM1_UP_IRQHandler        }, /* TIM1 Update               */
    { .handler = &TIM1_TRG_COM_IRQHandler   }, /* TIM1 Trigger and Comm.    */
    { .handler = &TIM1_CC_IRQHandler        }, /* TIM1 Capture Compare      */
    { .handler = &TIM2_IRQHandler           }, /* TIM2                      */
    { .handler = &TIM3_IRQHandler           }, /* TIM3                      */
    { .handler = &TIM4_IRQHandler           }, /* TIM4                      */
    { .handler = &I2C1_EV_IRQHandler        }, /* I2C1 Event                */
    { .handler = &I2C1_ER_IRQHandler        }, /* I2C1 Error                */
    { .handler = &I2C2_EV_IRQHandler        }, /* I2C2 Event                */
    { .handler = &I2C2_ER_IRQHandler        }, /* I2C2 Error                */
    { .handler = &SPI1_IRQHandler           }, /* SPI1                      */
    { .handler = &SPI2_IRQHandler           }, /* SPI2                      */
    { .handler = &USART1_IRQHandler         }, /* USART1                    */
    { .handler = &USART2_IRQHandler         }, /* USART2                    */
    { .handler = &USART3_IRQHandler         }, /* USART3                    */
    { .handler = &EXTI15_10_IRQHandler      }, /* EXTI Line 15..10          */
    { .handler = &RTCAlarm_IRQHandler       }, /*RTC Alarm through EXTI Line*/
    { .handler = &USBWakeUp_IRQHandler      }, /* USB Wakeup from suspend   */
    { .handler = &TIM8_BRK_IRQHandler       }, /* TIM8 Break                */
    { .handler = &TIM8_UP_IRQHandler        }, /* TIM8 Update               */
    { .handler = &TIM8_TRG_COM_IRQHandler   }, /* TIM8 Trigger and Comm.    */
    { .handler = &TIM8_CC_IRQHandler        }, /* TIM8 Capture Compare      */
    { .handler = &ADC3_IRQHandler           }, /* ADC3                      */
    { .handler = &FSMC_IRQHandler           }, /* FSMC                      */
    { .handler = &SDIO_IRQHandler           }, /* SDIO                      */
    { .handler = &TIM5_IRQHandler           }, /* TIM5                      */
    { .handler = &SPI3_IRQHandler           }, /* SPI3                      */
    { .handler = &UART4_IRQHandler          }, /* UART4                     */
    { .handler = &UART5_IRQHandler          }, /* UART5                     */
    { .handler = &TIM6_IRQHandler           }, /* TIM6                      */
    { .handler = &TIM7_IRQHandler           }, /* TIM7                      */
    { .handler = &DMA2_Channel1_IRQHandler  }, /* DMA2 Channel 1            */
    { .handler = &DMA2_Channel2_IRQHandler  }, /* DMA2 Channel 2            */
    { .handler = &DMA2_Channel3_IRQHandler  }, /* DMA2 Channel 3            */
    { .handler = &DMA2_Channel4_5_IRQHandler}, /* DMA2 Channel 4 and 5      */
    { .handler = &DMA2_Channel5_IRQHandler  }, /* connectivity line only    */
    { .handler = &ETH_IRQHandler            }, /* connectivity line only    */
    { .handler = &vMAC_IRQHandler           }, /* connectivity line only    */
    { .handler = &ETH_WKUP_IRQHandler       }, /* connectivity line only    */
    { .handler = &CAN2_TX_IRQHandler        }, /* connectivity line only    */
    { .handler = &CAN2_RX0_IRQHandler       }, /* connectivity line only    */
    { .handler = &CAN2_RX1_IRQHandler       }, /* connectivity line only    */
    { .handler = &CAN2_SCE_IRQHandler       }, /* connectivity line only    */
    { .handler = &OTG_FS_IRQHandler         }  /* connectivity line only    */
};

/* function prototypes -----------------------------------------------------*/
void assert_failed(char const *file, int line);

/*..........................................................................*/
void Reset_Handler(void)    __attribute__((__interrupt__));
void Reset_Handler(void) {
    extern int main(void);
    extern int __libc_init_array(void);
    extern unsigned __data_start;    /* start of .data in the linker script */
    extern unsigned __data_end__;      /* end of .data in the linker script */
    extern unsigned const __data_load;  /* initialization values for .data  */
    extern unsigned __bss_start__;    /* start of .bss in the linker script */
    extern unsigned __bss_end__;        /* end of .bss in the linker script */
    unsigned const *src;
    unsigned *dst;

                 /* copy the data segment initializers from flash to RAM... */
    src = &__data_load;
    for (dst = &__data_start; dst < &__data_end__; ++dst, ++src) {
        *dst = *src;
    }

                                           /* zero fill the .bss segment... */
    for (dst = &__bss_start__; dst < &__bss_end__; ++dst) {
        *dst = 0;
    }
             /* call all static construcors in C++ (harmless in C programs) */
    __libc_init_array();

                                      /* call the application's entry point */
    main();

    /* in a bare-metal system main() has nothing to return to and it should
    * never return. Just in case main() returns, the assert_failed() gives
    * the last opportunity to catch this problem.
    */
    assert_failed("startup_stm32f10x_cl", __LINE__);
}
/*..........................................................................*/
void Spurious_Handler(void) __attribute__((__interrupt__));
void Spurious_Handler(void) {
    assert_failed("startup_stm32f10x_cl", __LINE__);
    /* assert_failed() should not return, but just in case the following
    * enless loop will tie up the CPU.
    */
    for (;;) {
    }
}
/*..........................................................................*/
void _init() {
}

