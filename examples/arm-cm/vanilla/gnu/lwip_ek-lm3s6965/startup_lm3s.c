/*****************************************************************************
* Purpose: CMSIS-compliant startup code for LM3S Cortex-M3 devices, GNU
* Last Updated for Version: 4.1.03
* Date of the Last Update:  Mar 03, 2010
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
void __attribute__ ((weak)) GPIOPortA_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortB_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortC_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortD_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortE_IRQHandler(void);
void __attribute__ ((weak)) UART0_IRQHandler(void);
void __attribute__ ((weak)) UART1_IRQHandler(void);
void __attribute__ ((weak)) SSI0_IRQHandler(void);
void __attribute__ ((weak)) I2C0_IRQHandler(void);
void __attribute__ ((weak)) PWMFault_IRQHandler(void);
void __attribute__ ((weak)) PWMGen0_IRQHandler(void);
void __attribute__ ((weak)) PWMGen1_IRQHandler(void);
void __attribute__ ((weak)) PWMGen2_IRQHandler(void);
void __attribute__ ((weak)) QEI0_IRQHandler(void);
void __attribute__ ((weak)) ADCSeq0_IRQHandler(void);
void __attribute__ ((weak)) ADCSeq1_IRQHandler(void);
void __attribute__ ((weak)) ADCSeq2_IRQHandler(void);
void __attribute__ ((weak)) ADCSeq3_IRQHandler(void);
void __attribute__ ((weak)) Watchdog_IRQHandler(void);
void __attribute__ ((weak)) Timer0A_IRQHandler(void);
void __attribute__ ((weak)) Timer0B_IRQHandler(void);
void __attribute__ ((weak)) Timer1A_IRQHandler(void);
void __attribute__ ((weak)) Timer1B_IRQHandler(void);
void __attribute__ ((weak)) Timer2A_IRQHandler(void);
void __attribute__ ((weak)) Timer2B_IRQHandler(void);
void __attribute__ ((weak)) Comp0_IRQHandler(void);
void __attribute__ ((weak)) Comp1_IRQHandler(void);
void __attribute__ ((weak)) Comp2_IRQHandler(void);
void __attribute__ ((weak)) SysCtrl_IRQHandler(void);
void __attribute__ ((weak)) FlashCtrl_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortF_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortG_IRQHandler(void);
void __attribute__ ((weak)) GPIOPortH_IRQHandler(void);
void __attribute__ ((weak)) UART2_IRQHandler(void);
void __attribute__ ((weak)) SSI1_IRQHandler(void);
void __attribute__ ((weak)) Timer3A_IRQHandler(void);
void __attribute__ ((weak)) Timer3B_IRQHandler(void);
void __attribute__ ((weak)) I2C1_IRQHandler(void);
void __attribute__ ((weak)) QEI1_IRQHandler(void);
void __attribute__ ((weak)) CAN0_IRQHandler(void);
void __attribute__ ((weak)) CAN1_IRQHandler(void);
void __attribute__ ((weak)) CAN2_IRQHandler(void);
void __attribute__ ((weak)) Ethernet_IRQHandler(void);
void __attribute__ ((weak)) Hibernate_IRQHandler(void);

void __attribute__ ((weak)) Spurious_Handler(void);

/*----------------------------------------------------------------------------
* weak aliases for each Exception handler to the Spurious_Handler.
* Any function with the same name will override these definitions.
*/
#pragma weak NMI_Handler            = Spurious_Handler
#pragma weak MemManage_Handler      = Spurious_Handler
#pragma weak BusFault_Handler       = Spurious_Handler
#pragma weak UsageFault_Handler     = Spurious_Handler
#pragma weak SVC_Handler            = Spurious_Handler
#pragma weak DebugMon_Handler       = Spurious_Handler
#pragma weak PendSV_Handler         = Spurious_Handler
#pragma weak SysTick_Handler        = Spurious_Handler

#pragma weak GPIOPortA_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortB_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortC_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortD_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortE_IRQHandler   = Spurious_Handler
#pragma weak UART0_IRQHandler       = Spurious_Handler
#pragma weak UART1_IRQHandler       = Spurious_Handler
#pragma weak SSI0_IRQHandler        = Spurious_Handler
#pragma weak I2C0_IRQHandler        = Spurious_Handler
#pragma weak PWMFault_IRQHandler    = Spurious_Handler
#pragma weak PWMGen0_IRQHandler     = Spurious_Handler
#pragma weak PWMGen1_IRQHandler     = Spurious_Handler
#pragma weak PWMGen2_IRQHandler     = Spurious_Handler
#pragma weak QEI0_IRQHandler        = Spurious_Handler
#pragma weak ADCSeq0_IRQHandler     = Spurious_Handler
#pragma weak ADCSeq1_IRQHandler     = Spurious_Handler
#pragma weak ADCSeq2_IRQHandler     = Spurious_Handler
#pragma weak ADCSeq3_IRQHandler     = Spurious_Handler
#pragma weak Watchdog_IRQHandler    = Spurious_Handler
#pragma weak Timer0A_IRQHandler     = Spurious_Handler
#pragma weak Timer0B_IRQHandler     = Spurious_Handler
#pragma weak Timer1A_IRQHandler     = Spurious_Handler
#pragma weak Timer1B_IRQHandler     = Spurious_Handler
#pragma weak Timer2A_IRQHandler     = Spurious_Handler
#pragma weak Timer2B_IRQHandler     = Spurious_Handler
#pragma weak Comp0_IRQHandler       = Spurious_Handler
#pragma weak Comp1_IRQHandler       = Spurious_Handler
#pragma weak Comp2_IRQHandler       = Spurious_Handler
#pragma weak SysCtrl_IRQHandler     = Spurious_Handler
#pragma weak FlashCtrl_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortF_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortG_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortH_IRQHandler   = Spurious_Handler
#pragma weak UART2_IRQHandler       = Spurious_Handler
#pragma weak SSI1_IRQHandler        = Spurious_Handler
#pragma weak Timer3A_IRQHandler     = Spurious_Handler
#pragma weak Timer3B_IRQHandler     = Spurious_Handler
#pragma weak I2C1_IRQHandler        = Spurious_Handler
#pragma weak QEI1_IRQHandler        = Spurious_Handler
#pragma weak CAN0_IRQHandler        = Spurious_Handler
#pragma weak CAN1_IRQHandler        = Spurious_Handler
#pragma weak CAN2_IRQHandler        = Spurious_Handler
#pragma weak Ethernet_IRQHandler    = Spurious_Handler
#pragma weak Hibernate_IRQHandler   = Spurious_Handler

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
    { .pointer = &__c_stack_top__        }, /* initial stack pointer        */
    { .handler = &Reset_Handler          }, /* Reset Handler                */
    { .handler = &NMI_Handler            }, /* NMI Handler                  */
    { .handler = &HardFault_Handler      }, /* Hard Fault Handler           */
    { .handler = &MemManage_Handler      }, /* MPU Fault Handler            */
    { .handler = &BusFault_Handler       }, /* Bus Fault Handler            */
    { .handler = &UsageFault_Handler     }, /* Usage Fault Handler          */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &SVC_Handler            }, /* SVCall Handler               */
    { .handler = &DebugMon_Handler       }, /* Debug Monitor Handler        */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &PendSV_Handler         }, /* PendSV Handler               */
    { .handler = &SysTick_Handler        }, /* SysTick Handler              */
    /* external interrupts (IRQs) ... */
    { .handler = GPIOPortA_IRQHandler    }, /* GPIO Port A                  */
    { .handler = GPIOPortB_IRQHandler    }, /* GPIO Port B                  */
    { .handler = GPIOPortC_IRQHandler    }, /* GPIO Port C                  */
    { .handler = GPIOPortD_IRQHandler    }, /* GPIO Port D                  */
    { .handler = GPIOPortE_IRQHandler    }, /* GPIO Port E                  */
    { .handler = UART0_IRQHandler        }, /* UART0 Rx and Tx              */
    { .handler = UART1_IRQHandler        }, /* UART1 Rx and Tx              */
    { .handler = SSI0_IRQHandler         }, /* SSI0 Rx and Tx               */
    { .handler = I2C0_IRQHandler         }, /* I2C0 Master and Slave        */
    { .handler = PWMFault_IRQHandler     }, /* PWM Fault                    */
    { .handler = PWMGen0_IRQHandler      }, /* PWM Generator 0              */
    { .handler = PWMGen1_IRQHandler      }, /* PWM Generator 1              */
    { .handler = PWMGen2_IRQHandler      }, /* PWM Generator 2              */
    { .handler = QEI0_IRQHandler         }, /* Quadrature Encoder 0         */
    { .handler = ADCSeq0_IRQHandler      }, /* ADC Sequence 0               */
    { .handler = ADCSeq1_IRQHandler      }, /* ADC Sequence 1               */
    { .handler = ADCSeq2_IRQHandler      }, /* ADC Sequence 2               */
    { .handler = ADCSeq3_IRQHandler      }, /* ADC Sequence 3               */
    { .handler = Watchdog_IRQHandler     }, /* Watchdog timer               */
    { .handler = Timer0A_IRQHandler      }, /* Timer 0 subtimer A           */
    { .handler = Timer0B_IRQHandler      }, /* Timer 0 subtimer B           */
    { .handler = Timer1A_IRQHandler      }, /* Timer 1 subtimer A           */
    { .handler = Timer1B_IRQHandler      }, /* Timer 1 subtimer B           */
    { .handler = Timer2A_IRQHandler      }, /* Timer 2 subtimer A           */
    { .handler = Timer2B_IRQHandler      }, /* Timer 2 subtimer B           */
    { .handler = Comp0_IRQHandler        }, /* Analog Comparator 0          */
    { .handler = Comp1_IRQHandler        }, /* Analog Comparator 1          */
    { .handler = Comp2_IRQHandler        }, /* Analog Comparator 2          */
    { .handler = SysCtrl_IRQHandler      }, /* System Control (PLL,OSC,BO)  */
    { .handler = FlashCtrl_IRQHandler    }, /* FLASH Control                */
    { .handler = GPIOPortF_IRQHandler    }, /* GPIO Port F                  */
    { .handler = GPIOPortG_IRQHandler    }, /* GPIO Port G                  */
    { .handler = GPIOPortH_IRQHandler    }, /* GPIO Port H                  */
    { .handler = UART2_IRQHandler        }, /* UART2 Rx and Tx              */
    { .handler = SSI1_IRQHandler         }, /* SSI1 Rx and Tx               */
    { .handler = Timer3A_IRQHandler      }, /* Timer 3 subtimer A           */
    { .handler = Timer3B_IRQHandler      }, /* Timer 3 subtimer B           */
    { .handler = I2C1_IRQHandler         }, /* I2C1 Master and Slave        */
    { .handler = QEI1_IRQHandler         }, /* Quadrature Encoder 1         */
    { .handler = CAN0_IRQHandler         }, /* CAN0                         */
    { .handler = CAN1_IRQHandler         }, /* CAN1                         */
    { .handler = CAN2_IRQHandler         }, /* CAN2                         */
    { .handler = Ethernet_IRQHandler     }, /* Ethernet                     */
    { .handler = Hibernate_IRQHandler    }, /* Hibernate                    */
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

