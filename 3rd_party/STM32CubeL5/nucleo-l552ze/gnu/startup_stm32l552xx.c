/* File: startup_stm32l552xx.c
 * Purpose: startup file for STM32H74xx devices
 *          Should be used with GCC 'GNU Tools ARM Embedded'
 * Version: CMSIS 5.x
 * Date:    2022-02-26
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
void NMI_Handler             (void) __attribute__ ((weak));
void HardFault_Handler       (void) __attribute__ ((weak));
void MemManage_Handler       (void) __attribute__ ((weak));
void BusFault_Handler        (void) __attribute__ ((weak));
void UsageFault_Handler      (void) __attribute__ ((weak));
void SecureFault_Handler     (void) __attribute__ ((weak));

/* Cortex-M Processor non-fault exceptions... */
void SVC_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));

/* external interrupts...   */
void WWDG_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_S_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TAMP_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TAMP_S_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_S_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void GTZC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_S_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI6_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI8_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI10_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI11_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI12_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI13_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI14_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMAMUX1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMAMUX1_S_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel8_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DAC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FDCAN1_IT0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FDCAN1_IT1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void LPTIM1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void LPTIM2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM15_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM16_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM17_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void COMP_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void USB_FS_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void CRS_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FMC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void OCTOSPI1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SDMMC1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel6_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel8_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SAI2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void TSC_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void RNG_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void HASH_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void LPTIM3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C4_ER_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C4_EV_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void UCPD1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));
void ICACHE_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler")));

/*..........................................................................*/
__attribute__ ((section(".isr_vector")))
int const g_pfnVectors[] = {
    (int)&__stack_end__,            /* Top of Stack                        */
    (int)&Reset_Handler,            /* Reset Handler                       */
    (int)&NMI_Handler,              /* NMI Handler                         */
    (int)&HardFault_Handler,        /* Hard Fault Handler                  */
    (int)&MemManage_Handler,        /* The MPU fault handler               */
    (int)&BusFault_Handler,         /* The bus fault handler               */
    (int)&UsageFault_Handler,       /* The usage fault handler             */
    (int)&SecureFault_Handler,      /* Secure Fault Handler                */
    (int)0,                         /* Reserved                            */
    (int)0,                         /* Reserved                            */
    (int)0,                         /* Reserved                            */
    (int)&SVC_Handler,              /* SVCall handler                      */
    (int)&DebugMon_Handler,         /* Debug monitor handler               */
    (int)0,                         /* Reserved                            */
    (int)&PendSV_Handler,           /* The PendSV handler                  */
    (int)&SysTick_Handler,          /* The SysTick handler                 */

    /*IRQ handlers... */
    (int)&WWDG_IRQHandler,
    (int)&PVD_PVM_IRQHandler,
    (int)&RTC_IRQHandler,
    (int)&RTC_S_IRQHandler,
    (int)&TAMP_IRQHandler,
    (int)&TAMP_S_IRQHandler,
    (int)&FLASH_IRQHandler,
    (int)&FLASH_S_IRQHandler,
    (int)&GTZC_IRQHandler,
    (int)&RCC_IRQHandler,
    (int)&RCC_S_IRQHandler,
    (int)&EXTI0_IRQHandler,
    (int)&EXTI1_IRQHandler,
    (int)&EXTI2_IRQHandler,
    (int)&EXTI3_IRQHandler,
    (int)&EXTI4_IRQHandler,
    (int)&EXTI5_IRQHandler,
    (int)&EXTI6_IRQHandler,
    (int)&EXTI7_IRQHandler,
    (int)&EXTI8_IRQHandler,
    (int)&EXTI9_IRQHandler,
    (int)&EXTI10_IRQHandler,
    (int)&EXTI11_IRQHandler,
    (int)&EXTI12_IRQHandler,
    (int)&EXTI13_IRQHandler,
    (int)&EXTI14_IRQHandler,
    (int)&EXTI15_IRQHandler,
    (int)&DMAMUX1_IRQHandler,
    (int)&DMAMUX1_S_IRQHandler,
    (int)&DMA1_Channel1_IRQHandler,
    (int)&DMA1_Channel2_IRQHandler,
    (int)&DMA1_Channel3_IRQHandler,
    (int)&DMA1_Channel4_IRQHandler,
    (int)&DMA1_Channel5_IRQHandler,
    (int)&DMA1_Channel6_IRQHandler,
    (int)&DMA1_Channel7_IRQHandler,
    (int)&DMA1_Channel8_IRQHandler,
    (int)&ADC1_2_IRQHandler,
    (int)&DAC_IRQHandler,
    (int)&FDCAN1_IT0_IRQHandler,
    (int)&FDCAN1_IT1_IRQHandler,
    (int)&TIM1_BRK_IRQHandler,
    (int)&TIM1_UP_IRQHandler,
    (int)&TIM1_TRG_COM_IRQHandler,
    (int)&TIM1_CC_IRQHandler,
    (int)&TIM2_IRQHandler,
    (int)&TIM3_IRQHandler,
    (int)&TIM4_IRQHandler,
    (int)&TIM5_IRQHandler,
    (int)&TIM6_IRQHandler,
    (int)&TIM7_IRQHandler,
    (int)&TIM8_BRK_IRQHandler,
    (int)&TIM8_UP_IRQHandler,
    (int)&TIM8_TRG_COM_IRQHandler,
    (int)&TIM8_CC_IRQHandler,
    (int)&I2C1_EV_IRQHandler,
    (int)&I2C1_ER_IRQHandler,
    (int)&I2C2_EV_IRQHandler,
    (int)&I2C2_ER_IRQHandler,
    (int)&SPI1_IRQHandler,
    (int)&SPI2_IRQHandler,
    (int)&USART1_IRQHandler,
    (int)&USART2_IRQHandler,
    (int)&USART3_IRQHandler,
    (int)&UART4_IRQHandler,
    (int)&UART5_IRQHandler,
    (int)&LPUART1_IRQHandler,
    (int)&LPTIM1_IRQHandler,
    (int)&LPTIM2_IRQHandler,
    (int)&TIM15_IRQHandler,
    (int)&TIM16_IRQHandler,
    (int)&TIM17_IRQHandler,
    (int)&COMP_IRQHandler,
    (int)&USB_FS_IRQHandler,
    (int)&CRS_IRQHandler,
    (int)&FMC_IRQHandler,
    (int)&OCTOSPI1_IRQHandler,
    (int)0,
    (int)&SDMMC1_IRQHandler,
    (int)0,
    (int)&DMA2_Channel1_IRQHandler,
    (int)&DMA2_Channel2_IRQHandler,
    (int)&DMA2_Channel3_IRQHandler,
    (int)&DMA2_Channel4_IRQHandler,
    (int)&DMA2_Channel5_IRQHandler,
    (int)&DMA2_Channel6_IRQHandler,
    (int)&DMA2_Channel7_IRQHandler,
    (int)&DMA2_Channel8_IRQHandler,
    (int)&I2C3_EV_IRQHandler,
    (int)&I2C3_ER_IRQHandler,
    (int)&SAI1_IRQHandler,
    (int)&SAI2_IRQHandler,
    (int)&TSC_IRQHandler,
    (int)0,
    (int)&RNG_IRQHandler,
    (int)&FPU_IRQHandler,
    (int)&HASH_IRQHandler,
    (int)0,
    (int)&LPTIM3_IRQHandler,
    (int)&SPI3_IRQHandler,
    (int)&I2C4_ER_IRQHandler,
    (int)&I2C4_EV_IRQHandler,
    (int)&DFSDM1_FLT0_IRQHandler,
    (int)&DFSDM1_FLT1_IRQHandler,
    (int)&DFSDM1_FLT2_IRQHandler,
    (int)&DFSDM1_FLT3_IRQHandler,
    (int)&UCPD1_IRQHandler,
    (int)&ICACHE_IRQHandler,
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
        /* call all static constructors in C++ (harmless in C programs) */
        __libc_init_array();
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
__attribute__((naked)) void SecureFault_Handler(void);
void SecureFault_Handlerr(void) {
    __asm volatile (
        "    ldr r0,=str_secure\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_secure: .asciz \"SecureFault\"\n\t"
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
