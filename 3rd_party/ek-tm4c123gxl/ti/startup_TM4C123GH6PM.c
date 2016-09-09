/* File: startup_TM4C123GH6PM.c
 * Purpose: startup file for TM4C123GH6PM Cortex-M4 device.
 *          Should be used with TI CCS-ARM Compiler
 * Version: CMSIS 4.3.0
 * Date: 2016-Sep-09
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
//****************************************************************************
//
// Startup code for use with TI's Code Composer Studio.
//
// Copyright (c) 2011-2014 Texas Instruments Inc. All rights reserved.
// Software License Agreement
//
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//****************************************************************************

/* top of stack defined in the linker script -------------------------------*/
extern int __STACK_TOP;

/* Function prototypes -----------------------------------------------------*/
void Reset_Handler  (void);  /* Reset Handler */
void Default_Handler(void);  /* Default empty handler */

/*--------------------------------------------------------------------------*/
/* Cortex-M Processor fault exceptions... */
void NMI_Handler           (void);
void HardFault_Handler     (void);
void MemManage_Handler     (void);
void BusFault_Handler      (void);
void UsageFault_Handler    (void);

/* Cortex-M Processor non-fault exceptions... */
void SVC_Handler           (void);
void DebugMon_Handler      (void);
void PendSV_Handler        (void);
void SysTick_Handler       (void);

/* external interrupts...   */
void GPIOPortA_IRQHandler  (void);
void GPIOPortB_IRQHandler  (void);
void GPIOPortC_IRQHandler  (void);
void GPIOPortD_IRQHandler  (void);
void GPIOPortE_IRQHandler  (void);
void UART0_IRQHandler      (void);
void UART1_IRQHandler      (void);
void SSI0_IRQHandler       (void);
void I2C0_IRQHandler       (void);
void PWMFault_IRQHandler   (void);
void PWMGen0_IRQHandler    (void);
void PWMGen1_IRQHandler    (void);
void PWMGen2_IRQHandler    (void);
void QEI0_IRQHandler       (void);
void ADCSeq0_IRQHandler    (void);
void ADCSeq1_IRQHandler    (void);
void ADCSeq2_IRQHandler    (void);
void ADCSeq3_IRQHandler    (void);
void Watchdog_IRQHandler   (void);
void Timer0A_IRQHandler    (void);
void Timer0B_IRQHandler    (void);
void Timer1A_IRQHandler    (void);
void Timer1B_IRQHandler    (void);
void Timer2A_IRQHandler    (void);
void Timer2B_IRQHandler    (void);
void Comp0_IRQHandler      (void);
void Comp1_IRQHandler      (void);
void Comp2_IRQHandler      (void);
void SysCtrl_IRQHandler    (void);
void FlashCtrl_IRQHandler  (void);
void GPIOPortF_IRQHandler  (void);
void GPIOPortG_IRQHandler  (void);
void GPIOPortH_IRQHandler  (void);
void UART2_IRQHandler      (void);
void SSI1_IRQHandler       (void);
void Timer3A_IRQHandler    (void);
void Timer3B_IRQHandler    (void);
void I2C1_IRQHandler       (void);
void QEI1_IRQHandler       (void);
void CAN0_IRQHandler       (void);
void CAN1_IRQHandler       (void);
void CAN2_IRQHandler       (void);
void Hibernate_IRQHandler  (void);
void USB0_IRQHandler       (void);
void PWMGen3_IRQHandler    (void);
void uDMAST_IRQHandler     (void);
void uDMAError_IRQHandler  (void);
void ADC1Seq0_IRQHandler   (void);
void ADC1Seq1_IRQHandler   (void);
void ADC1Seq2_IRQHandler   (void);
void ADC1Seq3_IRQHandler   (void);
void GPIOPortJ_IRQHandler  (void);
void GPIOPortK_IRQHandler  (void);
void GPIOPortL_IRQHandler  (void);
void SSI2_IRQHandler       (void);
void SSI3_IRQHandler       (void);
void UART3_IRQHandler      (void);
void UART4_IRQHandler      (void);
void UART5_IRQHandler      (void);
void UART6_IRQHandler      (void);
void UART7_IRQHandler      (void);
void I2C2_IRQHandler       (void);
void I2C3_IRQHandler       (void);
void Timer4A_IRQHandler    (void);
void Timer4B_IRQHandler    (void);
void Timer5A_IRQHandler    (void);
void Timer5B_IRQHandler    (void);
void WideTimer0A_IRQHandler(void);
void WideTimer0B_IRQHandler(void);
void WideTimer1A_IRQHandler(void);
void WideTimer1B_IRQHandler(void);
void WideTimer2A_IRQHandler(void);
void WideTimer2B_IRQHandler(void);
void WideTimer3A_IRQHandler(void);
void WideTimer3B_IRQHandler(void);
void WideTimer4A_IRQHandler(void);
void WideTimer4B_IRQHandler(void);
void WideTimer5A_IRQHandler(void);
void WideTimer5B_IRQHandler(void);
void FPU_IRQHandler        (void);
void I2C4_IRQHandler       (void);
void I2C5_IRQHandler       (void);
void GPIOPortM_IRQHandler  (void);
void GPIOPortN_IRQHandler  (void);
void QEI2_IRQHandler       (void);
void GPIOPortP0_IRQHandler (void);
void GPIOPortP1_IRQHandler (void);
void GPIOPortP2_IRQHandler (void);
void GPIOPortP3_IRQHandler (void);
void GPIOPortP4_IRQHandler (void);
void GPIOPortP5_IRQHandler (void);
void GPIOPortP6_IRQHandler (void);
void GPIOPortP7_IRQHandler (void);
void GPIOPortQ0_IRQHandler (void);
void GPIOPortQ1_IRQHandler (void);
void GPIOPortQ2_IRQHandler (void);
void GPIOPortQ3_IRQHandler (void);
void GPIOPortQ4_IRQHandler (void);
void GPIOPortQ5_IRQHandler (void);
void GPIOPortQ6_IRQHandler (void);
void GPIOPortQ7_IRQHandler (void);
void GPIOPortR_IRQHandler  (void);
void GPIOPortS_IRQHandler  (void);
void PWM1Gen0_IRQHandler   (void);
void PWM1Gen1_IRQHandler   (void);
void PWM1Gen2_IRQHandler   (void);
void PWM1Gen3_IRQHandler   (void);
void PWM1Fault_IRQHandler  (void);


//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000 or at the start of
// the program if located at a start address other than 0.
//
//                      !!!!!!  NOTE  !!!!!!
// You must MANUALLY remove the (int)&Default_Handler from the interrupts
// that you actually wish to handle in your application!
//*****************************************************************************
#pragma DATA_SECTION(g_pfnVectors, ".intvecs")
int const g_pfnVectors[] = {
    (int)&__STACK_TOP,            /* The initial stack pointer       */
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

    /* IRQ handlers... */
    (int)&Default_Handler, //(int)&GPIOPortA_IRQHandler,   /* GPIO Port A                     */
    (int)&Default_Handler, //(int)&GPIOPortB_IRQHandler,   /* GPIO Port B                     */
    (int)&Default_Handler, //(int)&GPIOPortC_IRQHandler,   /* GPIO Port C                     */
    (int)&Default_Handler, //(int)&GPIOPortD_IRQHandler,   /* GPIO Port D                     */
    (int)&Default_Handler, //(int)&GPIOPortE_IRQHandler,   /* GPIO Port E                     */
    (int)&UART0_IRQHandler,       /* UART0 Rx and Tx                 */
    (int)&Default_Handler, //(int)&UART1_IRQHandler,       /* UART1 Rx and Tx                 */
    (int)&Default_Handler, //(int)&SSI0_IRQHandler,        /* SSI0 Rx and Tx                  */
    (int)&Default_Handler, //(int)&I2C0_IRQHandler,        /* I2C0 Master and Slave           */
    (int)&Default_Handler, //(int)&PWMFault_IRQHandler,    /* PWM Fault                       */
    (int)&Default_Handler, //(int)&PWMGen0_IRQHandler,     /* PWM Generator 0                 */
    (int)&Default_Handler, //(int)&PWMGen1_IRQHandler,     /* PWM Generator 1                 */
    (int)&Default_Handler, //(int)&PWMGen2_IRQHandler,     /* PWM Generator 2                 */
    (int)&Default_Handler, //(int)&QEI0_IRQHandler,        /* Quadrature Encoder 0            */
    (int)&Default_Handler, //(int)&ADCSeq0_IRQHandler,     /* ADC Sequence 0                  */
    (int)&Default_Handler, //(int)&ADCSeq1_IRQHandler,     /* ADC Sequence 1                  */
    (int)&Default_Handler, //(int)&ADCSeq2_IRQHandler,     /* ADC Sequence 2                  */
    (int)&Default_Handler, //(int)&ADCSeq3_IRQHandler,     /* ADC Sequence 3                  */
    (int)&Default_Handler, //(int)&Watchdog_IRQHandler,    /* Watchdog timer                  */
    (int)&Default_Handler, //(int)&Timer0A_IRQHandler,     /* Timer 0 subtimer A              */
    (int)&Default_Handler, //(int)&Timer0B_IRQHandler,     /* Timer 0 subtimer B              */
    (int)&Default_Handler, //(int)&Timer1A_IRQHandler,     /* Timer 1 subtimer A              */
    (int)&Default_Handler, //(int)&Timer1B_IRQHandler,     /* Timer 1 subtimer B              */
    (int)&Default_Handler, //(int)&Timer2A_IRQHandler,     /* Timer 2 subtimer A              */
    (int)&Default_Handler, //(int)&Timer2B_IRQHandler,     /* Timer 2 subtimer B              */
    (int)&Default_Handler, //(int)&Comp0_IRQHandler,       /* Analog Comparator 0             */
    (int)&Default_Handler, //(int)&Comp1_IRQHandler,       /* Analog Comparator 1             */
    (int)&Default_Handler, //(int)&Comp2_IRQHandler,       /* Analog Comparator 2             */
    (int)&Default_Handler, //(int)&SysCtrl_IRQHandler,     /* System Control (PLL, OSC, BO)   */
    (int)&Default_Handler, //(int)&FlashCtrl_IRQHandler,   /* FLASH Control                   */
    (int)&Default_Handler, //(int)&GPIOPortF_IRQHandler,   /* GPIO Port F                     */
    (int)&Default_Handler, //(int)&GPIOPortG_IRQHandler,   /* GPIO Port G                     */
    (int)&Default_Handler, //(int)&GPIOPortH_IRQHandler,   /* GPIO Port H                     */
    (int)&Default_Handler, //(int)&UART2_IRQHandler,       /* UART2 Rx and Tx                 */
    (int)&Default_Handler, //(int)&SSI1_IRQHandler,        /* SSI1 Rx and Tx                  */
    (int)&Default_Handler, //(int)&Timer3A_IRQHandler,     /* Timer 3 subtimer A              */
    (int)&Default_Handler, //(int)&Timer3B_IRQHandler,     /* Timer 3 subtimer B              */
    (int)&Default_Handler, //(int)&I2C1_IRQHandler,        /* I2C1 Master and Slave           */
    (int)&Default_Handler, //(int)&QEI1_IRQHandler,        /* Quadrature Encoder 1            */
    (int)&Default_Handler, //(int)&CAN0_IRQHandler,        /* CAN0                            */
    (int)&Default_Handler, //(int)&CAN1_IRQHandler,        /* CAN1                            */
    (int)&Default_Handler, //(int)&CAN2_IRQHandler,        /* CAN2                            */
    (int)&Default_Handler, //0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&Hibernate_IRQHandler,   /* Hibernate                       */
    (int)&Default_Handler, //(int)&USB0_IRQHandler,        /* USB0                            */
    (int)&Default_Handler, //(int)&PWMGen3_IRQHandler,     /* PWM Generator 3                 */
    (int)&Default_Handler, //(int)&uDMAST_IRQHandler,      /* uDMA Software Transfer          */
    (int)&Default_Handler, //(int)&uDMAError_IRQHandler,   /* uDMA Error                      */
    (int)&Default_Handler, //(int)&ADC1Seq0_IRQHandler,    /* ADC1 Sequence 0                 */
    (int)&Default_Handler, //(int)&ADC1Seq1_IRQHandler,    /* ADC1 Sequence 1                 */
    (int)&Default_Handler, //(int)&ADC1Seq2_IRQHandler,    /* ADC1 Sequence 2                 */
    (int)&Default_Handler, //(int)&ADC1Seq3_IRQHandler,    /* ADC1 Sequence 3                 */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&GPIOPortJ_IRQHandler,   /* GPIO Port J                     */
    (int)&Default_Handler, //(int)&GPIOPortK_IRQHandler,   /* GPIO Port K                     */
    (int)&Default_Handler, //(int)&GPIOPortL_IRQHandler,   /* GPIO Port L                     */
    (int)&Default_Handler, //(int)&SSI2_IRQHandler,        /* SSI2 Rx and Tx                  */
    (int)&Default_Handler, //(int)&SSI3_IRQHandler,        /* SSI3 Rx and Tx                  */
    (int)&Default_Handler, //(int)&UART3_IRQHandler,       /* UART3 Rx and Tx                 */
    (int)&Default_Handler, //(int)&UART4_IRQHandler,       /* UART4 Rx and Tx                 */
    (int)&Default_Handler, //(int)&UART5_IRQHandler,       /* UART5 Rx and Tx                 */
    (int)&Default_Handler, //(int)&UART6_IRQHandler,       /* UART6 Rx and Tx                 */
    (int)&Default_Handler, //(int)&UART7_IRQHandler,       /* UART7 Rx and Tx                 */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&I2C2_IRQHandler,        /* I2C2 Master and Slave           */
    (int)&Default_Handler, //(int)&I2C3_IRQHandler,        /* I2C3 Master and Slave           */
    (int)&Default_Handler, //(int)&Timer4A_IRQHandler,     /* Timer 4 subtimer A              */
    (int)&Default_Handler, //(int)&Timer4B_IRQHandler,     /* Timer 4 subtimer B              */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&Timer5A_IRQHandler,     /* Timer 5 subtimer A              */
    (int)&Default_Handler, //(int)&Timer5B_IRQHandler,     /* Timer 5 subtimer B              */
    (int)&Default_Handler, //(int)&WideTimer0A_IRQHandler, /* Wide Timer 0 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer0B_IRQHandler, /* Wide Timer 0 subtimer B         */
    (int)&Default_Handler, //(int)&WideTimer1A_IRQHandler, /* Wide Timer 1 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer1B_IRQHandler, /* Wide Timer 1 subtimer B         */
    (int)&Default_Handler, //(int)&WideTimer2A_IRQHandler, /* Wide Timer 2 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer2B_IRQHandler, /* Wide Timer 2 subtimer B         */
    (int)&Default_Handler, //(int)&WideTimer3A_IRQHandler, /* Wide Timer 3 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer3B_IRQHandler, /* Wide Timer 3 subtimer B         */
    (int)&Default_Handler, //(int)&WideTimer4A_IRQHandler, /* Wide Timer 4 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer4B_IRQHandler, /* Wide Timer 4 subtimer B         */
    (int)&Default_Handler, //(int)&WideTimer5A_IRQHandler, /* Wide Timer 5 subtimer A         */
    (int)&Default_Handler, //(int)&WideTimer5B_IRQHandler, /* Wide Timer 5 subtimer B         */
    (int)&Default_Handler, //(int)&FPU_IRQHandler,         /* FPU                             */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&I2C4_IRQHandler,        /* I2C4 Master and Slave           */
    (int)&Default_Handler, //(int)&I2C5_IRQHandler,        /* I2C5 Master and Slave           */
    (int)&Default_Handler, //(int)&GPIOPortM_IRQHandler,   /* GPIO Port M                     */
    (int)&Default_Handler, //(int)&GPIOPortN_IRQHandler,   /* GPIO Port N                     */
    (int)&Default_Handler, //(int)&QEI2_IRQHandler,        /* Quadrature Encoder 2            */
    0,                            /* Reserved                        */
    0,                            /* Reserved                        */
    (int)&Default_Handler, //(int)&GPIOPortP0_IRQHandler,  /* GPIO Port P (Summary or P0)     */
    (int)&Default_Handler, //(int)&GPIOPortP1_IRQHandler,  /* GPIO Port P1                    */
    (int)&Default_Handler, //(int)&GPIOPortP2_IRQHandler,  /* GPIO Port P2                    */
    (int)&Default_Handler, //(int)&GPIOPortP3_IRQHandler,  /* GPIO Port P3                    */
    (int)&Default_Handler, //(int)&GPIOPortP4_IRQHandler,  /* GPIO Port P4                    */
    (int)&Default_Handler, //(int)&GPIOPortP5_IRQHandler,  /* GPIO Port P5                    */
    (int)&Default_Handler, //(int)&GPIOPortP6_IRQHandler,  /* GPIO Port P6                    */
    (int)&Default_Handler, //(int)&GPIOPortP7_IRQHandler,  /* GPIO Port P7                    */
    (int)&Default_Handler, //(int)&GPIOPortQ0_IRQHandler,  /* GPIO Port Q (Summary or Q0)     */
    (int)&Default_Handler, //(int)&GPIOPortQ1_IRQHandler,  /* GPIO Port Q1                    */
    (int)&Default_Handler, //(int)&GPIOPortQ2_IRQHandler,  /* GPIO Port Q2                    */
    (int)&Default_Handler, //(int)&GPIOPortQ3_IRQHandler,  /* GPIO Port Q3                    */
    (int)&Default_Handler, //(int)&GPIOPortQ4_IRQHandler,  /* GPIO Port Q4                    */
    (int)&Default_Handler, //(int)&GPIOPortQ5_IRQHandler,  /* GPIO Port Q5                    */
    (int)&Default_Handler, //(int)&GPIOPortQ6_IRQHandler,  /* GPIO Port Q6                    */
    (int)&Default_Handler, //(int)&GPIOPortQ7_IRQHandler,  /* GPIO Port Q7                    */
    (int)&Default_Handler, //(int)&GPIOPortR_IRQHandler,   /* GPIO Port R                     */
    (int)&Default_Handler, //(int)&GPIOPortS_IRQHandler,   /* GPIO Port S                     */
    (int)&Default_Handler, //(int)&PWM1Gen0_IRQHandler,    /* PWM 1 Generator 0               */
    (int)&Default_Handler, //(int)&PWM1Gen1_IRQHandler,    /* PWM 1 Generator 1               */
    (int)&Default_Handler, //(int)&PWM1Gen2_IRQHandler,    /* PWM 1 Generator 2               */
    (int)&Default_Handler, //(int)&PWM1Gen3_IRQHandler,    /* PWM 1 Generator 3               */
    (int)&Default_Handler, //(int)&PWM1Fault_IRQHandler,   /* PWM 1 Fault                     */
};

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void Reset_Handler(void) {
    // First, call the CMSIS SystemInit() to set up the CPU clock.
    // Next, jump to the CCS C initialization routine. This will enable the
    // floating-point unit as well, so that does not need to be done here.
    //
    __asm volatile(
        "  .global _c_int00\n\r"
        "  .global SystemInit\n\r"
        "  bl.w    SystemInit\n\r"
        "  b.w     _c_int00\n\r");
}

/* fault exception handlers ------------------------------------------------*/
#pragma WEAK (Default_Handler)
void Default_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#0\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (NMI_Handler)
void NMI_Handler(void) {
    __asm(
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#1\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (MemManage_Handler)
void MemManage_Handler(void) {
    __asm(
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#2\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (HardFault_Handler)
void HardFault_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#3\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (BusFault_Handler)
void BusFault_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#4\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (UsageFault_Handler)
void UsageFault_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#5\n\r"
        "  b.w assert_failed\n\r"
    );
}

/*..........................................................................*/
#pragma WEAK (SVC_Handler)
void SVC_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#11\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (DebugMon_Handler)
void DebugMon_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#12\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (PendSV_Handler)
void PendSV_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#13\n\r"
        "  b.w assert_failed\n\r"
    );
}
/*..........................................................................*/
#pragma WEAK (SysTick_Handler)
void SysTick_Handler(void) {
    __asm volatile (
        "  .global assert_failed\n\r"
        "  mov r0,#0\n\r"
        "  mov r1,#14\n\r"
        "  b.w assert_failed\n\r"
    );
}

/*..........................................................................*/
void _init(void) { /* dummy */
}
/*..........................................................................*/
void _fini(void) { /* dummy */
}
