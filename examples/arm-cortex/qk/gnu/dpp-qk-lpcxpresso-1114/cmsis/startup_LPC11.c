/*****************************************************************************
* Purpose: CMSIS-compliant startup code for LPC1114 Cortex-M0 devices
* Last Updated for Version: 4.1.02
* Date of the Last Update:  Feb 13, 2010
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
void __attribute__ ((weak)) I2C_IRQHandler(void);
void __attribute__ ((weak)) TIMER16_0_IRQHandler(void);
void __attribute__ ((weak)) TIMER16_1_IRQHandler(void);
void __attribute__ ((weak)) TIMER32_0_IRQHandler(void);
void __attribute__ ((weak)) TIMER32_1_IRQHandler(void);
void __attribute__ ((weak)) SSP_IRQHandler(void);
void __attribute__ ((weak)) UART_IRQHandler(void);
void __attribute__ ((weak)) USB_IRQHandler(void);
void __attribute__ ((weak)) USB_FIQHandler(void);
void __attribute__ ((weak)) ADC_IRQHandler(void);
void __attribute__ ((weak)) WDT_IRQHandler(void);
void __attribute__ ((weak)) BOD_IRQHandler(void);
void __attribute__ ((weak)) FMC_IRQHandler(void);
void __attribute__ ((weak)) PIOINT3_IRQHandler(void);
void __attribute__ ((weak)) PIOINT2_IRQHandler(void);
void __attribute__ ((weak)) PIOINT1_IRQHandler(void);
void __attribute__ ((weak)) PIOINT0_IRQHandler(void);
void __attribute__ ((weak)) WAKEUP_IRQHandler(void);

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

#pragma weak I2C_IRQHandler             = Spurious_Handler
#pragma weak TIMER16_0_IRQHandler       = Spurious_Handler
#pragma weak TIMER16_1_IRQHandler       = Spurious_Handler
#pragma weak TIMER32_0_IRQHandler       = Spurious_Handler
#pragma weak TIMER32_1_IRQHandler       = Spurious_Handler
#pragma weak SSP_IRQHandler             = Spurious_Handler
#pragma weak UART_IRQHandler            = Spurious_Handler
#pragma weak USB_IRQHandler             = Spurious_Handler
#pragma weak USB_FIQHandler             = Spurious_Handler
#pragma weak ADC_IRQHandler             = Spurious_Handler
#pragma weak WDT_IRQHandler             = Spurious_Handler
#pragma weak BOD_IRQHandler             = Spurious_Handler
#pragma weak FMC_IRQHandler             = Spurious_Handler
#pragma weak PIOINT3_IRQHandler         = Spurious_Handler
#pragma weak PIOINT2_IRQHandler         = Spurious_Handler
#pragma weak PIOINT1_IRQHandler         = Spurious_Handler
#pragma weak PIOINT0_IRQHandler         = Spurious_Handler
#pragma weak WAKEUP_IRQHandler          = Spurious_Handler

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
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_0  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_1  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_2  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_3  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_4  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_5  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_6  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_7  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_8  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_9  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_10 Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO0_11 Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO1_0  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO1_1  Wakeup            */
    { .handler = WAKEUP_IRQHandler          }, /* PIO1_2  Wakeup            */

    { .handler = I2C_IRQHandler             }, /* I2C0                      */
    { .handler = TIMER16_0_IRQHandler       }, /* CT16B0 (16-bit Timer 0)   */
    { .handler = TIMER16_1_IRQHandler       }, /* CT16B1 (16-bit Timer 1)   */
    { .handler = TIMER32_0_IRQHandler       }, /* CT32B0 (32-bit Timer 0)   */
    { .handler = TIMER32_1_IRQHandler       }, /* CT32B1 (32-bit Timer 1)   */
    { .handler = SSP_IRQHandler             }, /* SSP0                      */
    { .handler = UART_IRQHandler            }, /* UART0                     */

    { .handler = USB_IRQHandler             }, /* USB IRQ                   */
    { .handler = USB_FIQHandler             }, /* USB FIQ                   */

    { .handler = ADC_IRQHandler             }, /* ADC   (A/D Converter)     */
    { .handler = WDT_IRQHandler             }, /* WDT   (Watchdog Timer)    */
    { .handler = BOD_IRQHandler             }, /* BOD   (Brownout Detect)   */
    { .handler = FMC_IRQHandler             }, /* IP2111 Flash Controller)  */
    { .handler = PIOINT3_IRQHandler         }, /* PIO INT3                  */
    { .handler = PIOINT2_IRQHandler         }, /* PIO INT2                  */
    { .handler = PIOINT1_IRQHandler         }, /* PIO INT1                  */
    { .handler = PIOINT0_IRQHandler         }, /* PIO INT0                  */
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

