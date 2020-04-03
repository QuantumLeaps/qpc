/* File: startup_efm32pg1b.c
 * Purpose: startup file for EFM32PG1B devices.
 *          Should be used with GCC 'GNU Tools ARM Embedded'
 * Version: CMSIS 5.0.1
 * Date: 2017-09-13
 *
 * Created from the CMSIS template for the specified device
 * Quantum Leaps, www.state-machine.com
 *
 * NOTE:
 * The function assert_failed defined at the end of this file defines
 * the error/assertion handling policy for the application and might
 * need to be customized for each project. This function is defined in
 * assembly to re-set the stack pointer, in case it is corrupted by the
 * time assert_failed is called.
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
void EMU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void WDOG0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void LDMA_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO_EVEN_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void USART0_RX_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void USART0_TX_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void ACMP0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void IDAC0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO_ODD_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_RX_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_TX_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void LEUART0_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void PCNT0_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void CMU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void MSC_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler")));
void CRYPTO_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void LETIMER0_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler")));
void RTCC_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler")));
void CRYOTIMER_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void FPUEH_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));

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
    (int)&EMU_IRQHandler,         /*  0 - EMU                        */
    (int)&Default_Handler,        /*  1 - Reserved                   */
    (int)&WDOG0_IRQHandler,       /*  2 - WDOG0                      */
    (int)&Default_Handler,        /*  3 - Reserved                   */
    (int)&Default_Handler,        /*  4 - Reserved                   */
    (int)&Default_Handler,        /*  5 - Reserved                   */
    (int)&Default_Handler,        /*  6 - Reserved                   */
    (int)&Default_Handler,        /*  7 - Reserved                   */
    (int)&LDMA_IRQHandler,        /*  8 - LDMA                       */
    (int)&GPIO_EVEN_IRQHandler,   /*  9 - GPIO_EVEN                  */
    (int)&TIMER0_IRQHandler,      /* 10 - TIMER0                     */
    (int)&USART0_RX_IRQHandler,   /* 11 - USART0_RX                  */
    (int)&USART0_TX_IRQHandler,   /* 12 - USART0_TX                  */
    (int)&ACMP0_IRQHandler,       /* 13 - ACMP0                      */
    (int)&ADC0_IRQHandler,        /* 14 - ADC0                       */
    (int)&IDAC0_IRQHandler,       /* 15 - IDAC0                      */
    (int)&I2C0_IRQHandler,        /* 16 - I2C0                       */
    (int)&GPIO_ODD_IRQHandler,    /* 17 - GPIO_ODD                   */
    (int)&TIMER1_IRQHandler,      /* 18 - TIMER1                     */
    (int)&USART1_RX_IRQHandler,   /* 19 - USART1_RX                  */
    (int)&USART1_TX_IRQHandler,   /* 20 - USART1_TX                  */
    (int)&LEUART0_IRQHandler,     /* 21 - LEUART0                    */
    (int)&PCNT0_IRQHandler,       /* 22 - PCNT0                      */
    (int)&CMU_IRQHandler,         /* 23 - CMU                        */
    (int)&MSC_IRQHandler,         /* 24 - MSC                        */
    (int)&CRYPTO_IRQHandler,      /* 25 - CRYPTO                     */
    (int)&LETIMER0_IRQHandler,    /* 26 - LETIMER0                   */
    (int)&Default_Handler,        /* 27 - Reserved                   */
    (int)&Default_Handler,        /* 28 - Reserved                   */
    (int)&RTCC_IRQHandler,        /* 29 - RTCC                       */
    (int)&Default_Handler,        /* 30 - Reserved                   */
    (int)&CRYOTIMER_IRQHandler,   /* 31 - CRYOTIMER                  */
    (int)&Default_Handler,        /* 32 - Reserved                   */
    (int)&FPUEH_IRQHandler,       /* 33 - FPUEH                      */
};


/* reset handler -----------------------------------------------------------*/
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
