/*****************************************************************************
* Purpose: CMSIS-compliant startup code for LPC13xx Cortex-M3 devices
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Nov 07, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
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

void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_0 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_1 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_2 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_3 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_4 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_5 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_6 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_7 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_8 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_9 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_10(void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO0_11(void);

void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_0 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_1 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_2 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_3 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_4 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_5 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_6 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_7 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_8 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_9 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_10(void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO1_11(void);

void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_0 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_1 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_2 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_3 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_4 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_5 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_6 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_7 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_8 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_9 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_10(void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO2_11(void);

void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO3_0 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO3_1 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO3_2 (void);
void __attribute__ ((weak)) WAKEUP_IRQHandlerPIO3_3 (void);

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

#pragma weak WAKEUP_IRQHandlerPIO0_0    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_1    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_2    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_3    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_4    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_5    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_6    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_7    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_8    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_9    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_10   = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO0_11   = Spurious_Handler

#pragma weak WAKEUP_IRQHandlerPIO1_0    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_1    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_2    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_3    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_4    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_5    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_6    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_7    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_8    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_9    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_10   = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO1_11   = Spurious_Handler

#pragma weak WAKEUP_IRQHandlerPIO2_0    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_1    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_2    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_3    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_4    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_5    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_6    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_7    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_8    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_9    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_10   = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO2_11   = Spurious_Handler

#pragma weak WAKEUP_IRQHandlerPIO3_0    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO3_1    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO3_2    = Spurious_Handler
#pragma weak WAKEUP_IRQHandlerPIO3_3    = Spurious_Handler

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
    { .handler = &WAKEUP_IRQHandlerPIO0_0   }, /* PIO0_0  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_1   }, /* PIO0_1  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_2   }, /* PIO0_2  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_3   }, /* PIO0_3  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_4   }, /* PIO0_4  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_5   }, /* PIO0_5  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_6   }, /* PIO0_6  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_7   }, /* PIO0_7  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_8   }, /* PIO0_8  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_9   }, /* PIO0_9  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_10  }, /* PIO0_10 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO0_11  }, /* PIO0_11 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_0   }, /* PIO1_0  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_1   }, /* PIO1_1  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_2   }, /* PIO1_2  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_3   }, /* PIO1_3  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_4   }, /* PIO1_4  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_5   }, /* PIO1_5  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_6   }, /* PIO1_6  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_7   }, /* PIO1_7  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_8   }, /* PIO1_8  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_9   }, /* PIO1_9  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_10  }, /* PIO1_10 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO1_11  }, /* PIO1_11 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_0   }, /* PIO2_0  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_1   }, /* PIO2_1  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_2   }, /* PIO2_2  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_3   }, /* PIO2_3  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_4   }, /* PIO2_4  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_5   }, /* PIO2_5  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_6   }, /* PIO2_6  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_7   }, /* PIO2_7  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_8   }, /* PIO2_8  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_9   }, /* PIO2_9  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_10  }, /* PIO2_10 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO2_11  }, /* PIO2_11 Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO3_0   }, /* PIO3_0  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO3_1   }, /* PIO3_1  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO3_2   }, /* PIO3_2  Wakeup            */
    { .handler = &WAKEUP_IRQHandlerPIO3_3   }, /* PIO3_3  Wakeup            */
    { .handler = &I2C_IRQHandler            }, /* I2C0                      */
    { .handler = &TIMER16_0_IRQHandler      }, /* CT16B0 (16-bit Timer 0)   */
    { .handler = &TIMER16_1_IRQHandler      }, /* CT16B1 (16-bit Timer 1)   */
    { .handler = &TIMER32_0_IRQHandler      }, /* CT32B0 (32-bit Timer 0)   */
    { .handler = &TIMER32_1_IRQHandler      }, /* CT32B1 (32-bit Timer 1)   */
    { .handler = &SSP_IRQHandler            }, /* SSP0                      */
    { .handler = &UART_IRQHandler           }, /* UART0                     */
    { .handler = &USB_IRQHandler            }, /* USB IRQ                   */
    { .handler = &USB_FIQHandler            }, /* USB FIQ                   */
    { .handler = &ADC_IRQHandler            }, /* ADC   (A/D Converter)     */
    { .handler = &WDT_IRQHandler            }, /* WDT   (Watchdog Timer)    */
    { .handler = &BOD_IRQHandler            }, /* BOD   (Brownout Detect)   */
    { .handler = &FMC_IRQHandler            }, /* Flash (IP2111 Flash Ctrl) */
    { .handler = &PIOINT3_IRQHandler        }, /* PIO INT3                  */
    { .handler = &PIOINT2_IRQHandler        }, /* PIO INT2                  */
    { .handler = &PIOINT1_IRQHandler        }, /* PIO INT1                  */
    { .handler = &PIOINT0_IRQHandler        }, /* PIO INT0                  */
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

