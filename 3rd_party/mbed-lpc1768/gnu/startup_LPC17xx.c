/* File: startup_LPC17xx.c
 * Purpose: startup file for LPC17xx Cortex-M3 devices.
 *          Should be used with GCC 'GNU Tools ARM Embedded'
 * Version: CMSIS 5.0.1
 * Date: 2017-09-13
 *
 * Created from the CMSIS template for the specified device
 * Quantum Leaps, www.state-machine.com
 *
 * NOTE: The function assert_failed defined at the end of this file
 * determines the error/assertion handling policy for the application and
 * might need to be customized for each project. This function is defined
 * using the GNU-ARM language extensions to avoid accessing the stack,
 * which might be corrupted by the time assert_failed is called.
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 ---------------------------------------------------------------------------*/

/* start and end of stack defined in the linker script ---------------------*/
/*extern int __stack_start__;*/
extern int __stack_end__;

/* Weak prototypes for error handlers --------------------------------------*/
/**
* \note
* The function assert_failed defined at the end of this file defines
* the error/assertion handling policy for the application and might
* need to be customized for each project. This function is defined in
* assembly to avoid accessing the stack, which might be corrupted by
* the time assert_failed is called.
*/
__attribute__ ((naked, noreturn))
void assert_failed(char const *module, int loc);

/* Function prototypes -----------------------------------------------------*/
void Default_Handler(void);  /* Default empty handler */
void Reset_Handler(void);    /* Reset Handler */
void SystemInit(void);       /* CMSIS system initialization */
__attribute__ ((noreturn))
void Q_onAssert(char const *module, int loc); /* QP assertion handler */

/*----------------------------------------------------------------------------
* weak aliases for each Exception handler to the Default_Handler.
* Any function with the same name will override these definitions.
*/
/* Cortex-M Processor fault exceptions... */
void NMI_Handler           (void) __attribute__ ((weak));
void HardFault_Handler     (void) __attribute__ ((weak));
void MemManage_Handler     (void) __attribute__ ((weak));
void BusFault_Handler      (void) __attribute__ ((weak));
void UsageFault_Handler    (void) __attribute__ ((weak));

/* Cortex-M Processor non-fault exceptions... */
void SVC_Handler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));

/* external interrupts...   */
void WDT_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER3_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void UART0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UART1_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UART2_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UART3_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void PWM1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SSP0_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SSP1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void PLL0_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void EINT0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void EINT1_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void EINT2_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void EINT3_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void BOD_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void USB_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void I2S_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void ENET_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void RIT_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void MCPWM_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void QEI_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler")));
void PLL1_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void USBActivity_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void CANActivity_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));


/*..........................................................................*/
__attribute__ ((section(".isr_vector")))
int const g_pfnVectors[] = {
    (int)&__stack_end__,          /* Top of Stack                    */
    (int)&Reset_Handler,          /* Reset Handler                   */
    (int)&NMI_Handler,            /* NMI Handler                     */
    (int)&HardFault_Handler,      /* Hard Fault Handler              */
    (int)&MemManage_Handler,      /* The MPU fault handler           */
    (int)&BusFault_Handler,       /* The bus fault handler           */
    (int)&UsageFault_Handler,     /* The usage fault handler         */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&SVC_Handler,            /* SVCall handler                  */
    (int)&DebugMon_Handler,       /* Debug monitor handler           */
    0,                            /* Reserved                        */
    (int)&PendSV_Handler,         /* The PendSV handler              */
    (int)&SysTick_Handler,        /* The SysTick handler             */

    /*IRQ handlers... */
    (int)&WDT_IRQHandler,         /* WDT                              */
    (int)&TIMER0_IRQHandler,      /* TIMER0                           */
    (int)&TIMER1_IRQHandler,      /* TIMER1                           */
    (int)&TIMER2_IRQHandler,      /* TIMER2                           */
    (int)&TIMER3_IRQHandler,      /* TIMER3                           */
    (int)&UART0_IRQHandler,       /* UART0                            */
    (int)&UART1_IRQHandler,       /* UART1                            */
    (int)&UART2_IRQHandler,       /* UART2                            */
    (int)&UART3_IRQHandler,       /* UART3                            */
    (int)&PWM1_IRQHandler,        /* PWM1                             */
    (int)&I2C0_IRQHandler,        /* I2C0                             */
    (int)&I2C1_IRQHandler,        /* I2C1                             */
    (int)&I2C2_IRQHandler,        /* I2C2                             */
    (int)&SPI_IRQHandler,         /* SPI                              */
    (int)&SSP0_IRQHandler,        /* SSP0                             */
    (int)&SSP1_IRQHandler,        /* SSP1                             */
    (int)&PLL0_IRQHandler,        /* PLL0 (Main PLL)                  */
    (int)&RTC_IRQHandler,         /* RTC                              */
    (int)&EINT0_IRQHandler,       /* EINT0                            */
    (int)&EINT1_IRQHandler,       /* EINT1                            */
    (int)&EINT2_IRQHandler,       /* EINT2                            */
    (int)&EINT3_IRQHandler,       /* EINT3                            */
    (int)&ADC_IRQHandler,         /* ADC                              */
    (int)&BOD_IRQHandler,         /* BOD                              */
    (int)&USB_IRQHandler,         /* USB                              */
    (int)&CAN_IRQHandler,         /* CAN                              */
    (int)&DMA_IRQHandler,         /* GP DMA                           */
    (int)&I2S_IRQHandler,         /* I2S                              */
    (int)&ENET_IRQHandler,        /* Ethernet                         */
    (int)&RIT_IRQHandler,         /* RITINT                           */
    (int)&MCPWM_IRQHandler,       /* Motor Control PWM                */
    (int)&QEI_IRQHandler,         /* Quadrature Encoder               */
    (int)&PLL1_IRQHandler,        /* PLL1 (USB PLL)                   */
    (int)&USBActivity_IRQHandler, /* USB Activity interrupt to wakeup */
    (int)&CANActivity_IRQHandler, /* CAN Activity interrupt to wakeup */
};


/* reset handler -----------------------------------------------------------*/
__attribute__((naked)) void Reset_Handler(void);
void Reset_Handler(void) {
    extern int main(void);
    extern int __libc_init_array(void);
    extern unsigned __data_start;  /* start of .data in the linker script */
    extern unsigned __data_end__;  /* end of .data in the linker script */
    extern unsigned const __data_load; /* initialization values for .data  */
    extern unsigned __bss_start__; /* start of .bss in the linker script */
    extern unsigned __bss_end__;   /* end of .bss in the linker script */
    extern void software_init_hook(void) __attribute__((weak));

    unsigned const *src;
    unsigned *dst;

    SystemInit(); /* CMSIS system initialization */

    /* copy the data segment initializers from flash to RAM... */
    src = &__data_load;
    for (dst = &__data_start; dst < &__data_end__; ++dst, ++src) {
        *dst = *src;
    }

    /* zero fill the .bss segment in RAM... */
    for (dst = &__bss_start__; dst < &__bss_end__; ++dst) {
        *dst = 0;
    }

    /* init hook provided? */
    if (&software_init_hook != (void (*)(void))(0)) {
        /* give control to the RTOS */
        software_init_hook(); /* this will also call __libc_init_array */
    }
    else {
        /* call all static constructors in C++ (comment out in C programs) */
        //__libc_init_array();
        (void)main(); /* application's entry point; should never return! */
    }

    /* the previous code should not return, but assert just in case... */
    assert_failed("Reset_Handler", __LINE__);
}


/* fault exception handlers ------------------------------------------------*/
__attribute__((naked)) void NMI_Handler(void);
void NMI_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_nmi\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_nmi: .asciz \"NMI\"\n\t"
        "  .align 2\n\t"
    );
}
/*..........................................................................*/
__attribute__((naked)) void MemManage_Handler(void);
void MemManage_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_mem\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_mem: .asciz \"MemManage\"\n\t"
        "  .align 2\n\t"
    );
}
/*..........................................................................*/
__attribute__((naked)) void HardFault_Handler(void);
void HardFault_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_hrd\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_hrd: .asciz \"HardFault\"\n\t"
        "  .align 2\n\t"
    );
}
/*..........................................................................*/
__attribute__((naked)) void BusFault_Handler(void);
void BusFault_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_bus\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_bus: .asciz \"BusFault\"\n\t"
        "  .align 2\n\t"
    );
}
/*..........................................................................*/
__attribute__((naked)) void UsageFault_Handler(void);
void UsageFault_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_usage\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_usage: .asciz \"UsageFault\"\n\t"
        "  .align 2\n\t"
    );
}
/*..........................................................................*/
__attribute__((naked)) void Default_Handler(void);
void Default_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_dflt\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_dflt: .asciz \"Default\"\n\t"
        "  .align 2\n\t"
    );
}


/*****************************************************************************
* The function assert_failed defines the error/assertion handling policy
* for the application. After making sure that the stack is OK, this function
* calls Q_onAssert, which should NOT return (typically reset the CPU).
*
* NOTE: the function Q_onAssert should NOT return.
*****************************************************************************/
__attribute__ ((naked, noreturn))
void assert_failed(char const *module, int loc) {
    /* re-set the SP in case of stack overflow */
    __asm volatile (
        "  MOV sp,%0\n\t"
        : : "r" (&__stack_end__));

    Q_onAssert(module, loc); /* call the application-specific QP handler */

    for (;;) { /* should not be reached, but just in case loop forever... */
    }
}

/****** End Of File *********************************************************/
