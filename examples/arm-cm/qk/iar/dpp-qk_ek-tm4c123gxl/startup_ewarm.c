//*****************************************************************************
// Added STACK_SIZE macro and calls to Q_onAssert()
// Quantum Leaps on 23-Jan-2013
// www.state-machine.com
//*****************************************************************************

//*****************************************************************************
//
// startup_ewarm.c - Startup code for use with IAR's Embedded Workbench,
//                   version 5.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
//
// Enable the IAR extensions for this source file.
//
//*****************************************************************************
#pragma language=extended

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void NMI_Handler(void);
void HardFault_Handler(void);
__weak void MemManage_Handler(void);
__weak void BusFault_Handler(void);
__weak void UsageFault_Handler(void);
__weak void HardFault_Handler(void);
__weak void SVC_Handler(void);
__weak void DebugMon_Handler(void);
__weak void PendSV_Handler(void);
__weak void SysTick_Handler(void);
__weak void GPIOPortA_IRQHandler(void);
__weak void GPIOPortB_IRQHandler(void);
__weak void GPIOPortC_IRQHandler(void);
__weak void GPIOPortD_IRQHandler(void);
__weak void GPIOPortE_IRQHandler(void);
__weak void UART0_IRQHandler(void);
__weak void UART1_IRQHandler(void);
__weak void SSI0_IRQHandler(void);
__weak void I2C0_IRQHandler(void);
__weak void PWMFault_IRQHandler(void);
__weak void PWMGen0_IRQHandler(void);
__weak void PWMGen1_IRQHandler(void);
__weak void PWMGen2_IRQHandler(void);
__weak void QEI0_IRQHandler(void);
__weak void ADCSeq0_IRQHandler(void);
__weak void ADCSeq1_IRQHandler(void);
__weak void ADCSeq2_IRQHandler(void);
__weak void ADCSeq3_IRQHandler(void);
__weak void Watchdog_IRQHandler(void);
__weak void Timer0A_IRQHandler(void);
__weak void Timer0B_IRQHandler(void);
__weak void Timer1A_IRQHandler(void);
__weak void Timer1B_IRQHandler(void);
__weak void Timer2A_IRQHandler(void);
__weak void Timer2B_IRQHandler(void);
__weak void Comp0_IRQHandler(void);
__weak void Comp1_IRQHandler(void);
__weak void Comp2_IRQHandler(void);
__weak void SysCtrl_IRQHandler(void);
__weak void FlashCtrl_IRQHandler(void);
__weak void GPIOPortF_IRQHandler(void);
__weak void GPIOPortG_IRQHandler(void);
__weak void GPIOPortH_IRQHandler(void);
__weak void UART2_IRQHandler(void);
__weak void SSI1_IRQHandler(void);
__weak void Timer3A_IRQHandler(void);
__weak void Timer3B_IRQHandler(void);
__weak void I2C1_IRQHandler(void);
__weak void QEI1_IRQHandler(void);
__weak void CAN0_IRQHandler(void);
__weak void CAN1_IRQHandler(void);
__weak void CAN2_IRQHandler(void);
__weak void Ethernet_IRQHandler(void);
__weak void Hibernate_IRQHandler(void);
__weak void USB0_IRQHandler(void);
__weak void PWMGen3_IRQHandler(void);
__weak void uDMAST_IRQHandler(void);
__weak void uDMAError_IRQHandler(void);
__weak void ADC1Seq0_IRQHandler(void);
__weak void ADC1Seq1_IRQHandler(void);
__weak void ADC1Seq2_IRQHandler(void);
__weak void ADC1Seq3_IRQHandler(void);
__weak void I2S0_IRQHandler(void);
__weak void EBI0_IRQHandler(void);
__weak void GPIOPortJ_IRQHandler(void);
__weak void GPIOPortK_IRQHandler(void);
__weak void GPIOPortL_IRQHandler(void);
__weak void SSI2_IRQHandler(void);
__weak void SSI3_IRQHandler(void);
__weak void UART3_IRQHandler(void);
__weak void UART4_IRQHandler(void);
__weak void UART5_IRQHandler(void);
__weak void UART6_IRQHandler(void);
__weak void UART7_IRQHandler(void);
__weak void I2C2_IRQHandler(void);
__weak void I2C3_IRQHandler(void);
__weak void Timer4A_IRQHandler(void);
__weak void Timer4B_IRQHandler(void);
__weak void Timer5A_IRQHandler(void);
__weak void Timer5B_IRQHandler(void);
__weak void WideTimer0A_IRQHandler(void);
__weak void WideTimer0B_IRQHandler(void);
__weak void WideTimer1A_IRQHandler(void);
__weak void WideTimer1B_IRQHandler(void);
__weak void WideTimer2A_IRQHandler(void);
__weak void WideTimer2B_IRQHandler(void);
__weak void WideTimer3A_IRQHandler(void);
__weak void WideTimer3B_IRQHandler(void);
__weak void WideTimer4A_IRQHandler(void);
__weak void WideTimer4B_IRQHandler(void);
__weak void WideTimer5A_IRQHandler(void);
__weak void WideTimer5B_IRQHandler(void);
__weak void FPU_IRQHandler(void);
__weak void PECI0_IRQHandler(void);
__weak void LPC0_IRQHandler(void);
__weak void I2C4_IRQHandler(void);
__weak void I2C5_IRQHandler(void);
__weak void GPIOPortM_IRQHandler(void);
__weak void GPIOPortN_IRQHandler(void);
__weak void QEI2_IRQHandler(void);
__weak void Fan0_IRQHandler(void);
__weak void GPIOPortP0_IRQHandler(void);
__weak void GPIOPortP1_IRQHandler(void);
__weak void GPIOPortP2_IRQHandler(void);
__weak void GPIOPortP3_IRQHandler(void);
__weak void GPIOPortP4_IRQHandler(void);
__weak void GPIOPortP5_IRQHandler(void);
__weak void GPIOPortP6_IRQHandler(void);
__weak void GPIOPortP7_IRQHandler(void);
__weak void GPIOPortQ0_IRQHandler(void);
__weak void GPIOPortQ1_IRQHandler(void);
__weak void GPIOPortQ2_IRQHandler(void);
__weak void GPIOPortQ3_IRQHandler(void);
__weak void GPIOPortQ4_IRQHandler(void);
__weak void GPIOPortQ5_IRQHandler(void);
__weak void GPIOPortQ6_IRQHandler(void);
__weak void GPIOPortQ7_IRQHandler(void);
__weak void GPIOPortR_IRQHandler(void);
__weak void GPIOPortS_IRQHandler(void);
__weak void PWM1Gen0_IRQHandler(void);
__weak void PWM1Gen1_IRQHandler(void);
__weak void PWM1Gen2_IRQHandler(void);
__weak void PWM1Gen3_IRQHandler(void);
__weak void PWM1Fault_IRQHandler(void);


//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static unsigned long pulStack[STACK_SIZE/sizeof(unsigned long)] @ ".noinit";

//*****************************************************************************
//
// A union that describes the entries of the vector table.  The union is needed
// since the first entry is the stack pointer and the remainder are function
// pointers.
//
//*****************************************************************************
typedef union {
    void (*pfnHandler)(void);
    unsigned long ulPtr;
}
uVectorEntry;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__root const uVectorEntry __vector_table[] @ ".intvec" = {
    { .ulPtr = (unsigned long)pulStack + sizeof(pulStack) },
                                            // The initial stack pointer
    __iar_program_start,                    // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    MemManage_Handler,                      // The MPU fault handler
    BusFault_Handler,                       // The bus fault handler
    UsageFault_Handler,                     // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVC_Handler,                            // SVCall handler
    DebugMon_Handler,                       // Debug monitor handler
    0,                                      // Reserved
    PendSV_Handler,                         // The PendSV handler
    SysTick_Handler,                        // The SysTick handler

    //
    // External Interrupts
    //
    GPIOPortA_IRQHandler,                   // GPIO Port A
    GPIOPortB_IRQHandler,                   // GPIO Port B
    GPIOPortC_IRQHandler,                   // GPIO Port C
    GPIOPortD_IRQHandler,                   // GPIO Port D
    GPIOPortE_IRQHandler,                   // GPIO Port E
    UART0_IRQHandler,                       // UART0 Rx and Tx
    UART1_IRQHandler,                       // UART1 Rx and Tx
    SSI0_IRQHandler,                        // SSI0 Rx and Tx
    I2C0_IRQHandler,                        // I2C0 Master and Slave
    PWMFault_IRQHandler,                    // PWM Fault
    PWMGen0_IRQHandler,                     // PWM Generator 0
    PWMGen1_IRQHandler,                     // PWM Generator 1
    PWMGen2_IRQHandler,                     // PWM Generator 2
    QEI0_IRQHandler,                        // Quadrature Encoder 0
    ADCSeq0_IRQHandler,                     // ADC Sequence 0
    ADCSeq1_IRQHandler,                     // ADC Sequence 1
    ADCSeq2_IRQHandler,                     // ADC Sequence 2
    ADCSeq3_IRQHandler,                     // ADC Sequence 3
    Watchdog_IRQHandler,                    // Watchdog timer
    Timer0A_IRQHandler,                     // Timer 0 subtimer A
    Timer0B_IRQHandler,                     // Timer 0 subtimer B
    Timer1A_IRQHandler,                     // Timer 1 subtimer A
    Timer1B_IRQHandler,                     // Timer 1 subtimer B
    Timer2A_IRQHandler,                     // Timer 2 subtimer A
    Timer2B_IRQHandler,                     // Timer 2 subtimer B
    Comp0_IRQHandler,                       // Analog Comparator 0
    Comp1_IRQHandler,                       // Analog Comparator 1
    Comp2_IRQHandler,                       // Analog Comparator 2
    SysCtrl_IRQHandler,                     // System Control (PLL, OSC, BO)
    FlashCtrl_IRQHandler,                   // FLASH Control
    GPIOPortF_IRQHandler,                   // GPIO Port F
    GPIOPortG_IRQHandler,                   // GPIO Port G
    GPIOPortH_IRQHandler,                   // GPIO Port H
    UART2_IRQHandler,                       // UART2 Rx and Tx
    SSI1_IRQHandler,                        // SSI1 Rx and Tx
    Timer3A_IRQHandler,                     // Timer 3 subtimer A
    Timer3B_IRQHandler,                     // Timer 3 subtimer B
    I2C1_IRQHandler,                        // I2C1 Master and Slave
    QEI1_IRQHandler,                        // Quadrature Encoder 1
    CAN0_IRQHandler,                        // CAN0
    CAN1_IRQHandler,                        // CAN1
    CAN2_IRQHandler,                        // CAN2
    Ethernet_IRQHandler,                    // Ethernet
    Hibernate_IRQHandler,                   // Hibernate

    USB0_IRQHandler,                        // USB0
    PWMGen3_IRQHandler,                     // PWM Generator 3
    uDMAST_IRQHandler,                      // uDMA Software Transfer
    uDMAError_IRQHandler,                   // uDMA Error
    ADC1Seq0_IRQHandler,                    // ADC1 Sequence 0
    ADC1Seq1_IRQHandler,                    // ADC1 Sequence 1
    ADC1Seq2_IRQHandler,                    // ADC1 Sequence 2
    ADC1Seq3_IRQHandler,                    // ADC1 Sequence 3
    I2S0_IRQHandler,                        // I2S0
    EBI0_IRQHandler,                        // External Bus Interface 0
    GPIOPortJ_IRQHandler,                   // GPIO Port J
    GPIOPortK_IRQHandler,                   // GPIO Port K
    GPIOPortL_IRQHandler,                   // GPIO Port L
    SSI2_IRQHandler,                        // SSI2 Rx and Tx
    SSI3_IRQHandler,                        // SSI3 Rx and Tx
    UART3_IRQHandler,                       // UART3 Rx and Tx
    UART4_IRQHandler,                       // UART4 Rx and Tx
    UART5_IRQHandler,                       // UART5 Rx and Tx
    UART6_IRQHandler,                       // UART6 Rx and Tx
    UART7_IRQHandler,                       // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    I2C2_IRQHandler,                        // I2C2 Master and Slave
    I2C3_IRQHandler,                        // I2C3 Master and Slave
    Timer4A_IRQHandler,                     // Timer 4 subtimer A
    Timer4B_IRQHandler,                     // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    Timer5A_IRQHandler,                     // Timer 5 subtimer A
    Timer5B_IRQHandler,                     // Timer 5 subtimer B
    WideTimer0A_IRQHandler,                 // Wide Timer 0 subtimer A
    WideTimer0B_IRQHandler,                 // Wide Timer 0 subtimer B
    WideTimer1A_IRQHandler,                 // Wide Timer 1 subtimer A
    WideTimer1B_IRQHandler,                 // Wide Timer 1 subtimer B
    WideTimer2A_IRQHandler,                 // Wide Timer 2 subtimer A
    WideTimer2B_IRQHandler,                 // Wide Timer 2 subtimer B
    WideTimer3A_IRQHandler,                 // Wide Timer 3 subtimer A
    WideTimer3B_IRQHandler,                 // Wide Timer 3 subtimer B
    WideTimer4A_IRQHandler,                 // Wide Timer 4 subtimer A
    WideTimer4B_IRQHandler,                 // Wide Timer 4 subtimer B
    WideTimer5A_IRQHandler,                 // Wide Timer 5 subtimer A
    WideTimer5B_IRQHandler,                 // Wide Timer 5 subtimer B
    FPU_IRQHandler,                         // FPU
    PECI0_IRQHandler,                       // PECI 0
    LPC0_IRQHandler,                        // LPC 0
    I2C4_IRQHandler,                        // I2C4 Master and Slave
    I2C5_IRQHandler,                        // I2C5 Master and Slave
    GPIOPortM_IRQHandler,                   // GPIO Port M
    GPIOPortN_IRQHandler,                   // GPIO Port N
    QEI2_IRQHandler,                        // Quadrature Encoder 2
    Fan0_IRQHandler,                        // Fan 0
    0,                                      // Reserved
    GPIOPortP0_IRQHandler,                  // GPIO Port P (Summary or P0)
    GPIOPortP1_IRQHandler,                  // GPIO Port P1
    GPIOPortP2_IRQHandler,                  // GPIO Port P2
    GPIOPortP3_IRQHandler,                  // GPIO Port P3
    GPIOPortP4_IRQHandler,                  // GPIO Port P4
    GPIOPortP5_IRQHandler,                  // GPIO Port P5
    GPIOPortP6_IRQHandler,                  // GPIO Port P6
    GPIOPortP7_IRQHandler,                  // GPIO Port P7
    GPIOPortQ0_IRQHandler,                  // GPIO Port Q (Summary or Q0)
    GPIOPortQ1_IRQHandler,                  // GPIO Port Q1
    GPIOPortQ2_IRQHandler,                  // GPIO Port Q2
    GPIOPortQ3_IRQHandler,                  // GPIO Port Q3
    GPIOPortQ4_IRQHandler,                  // GPIO Port Q4
    GPIOPortQ5_IRQHandler,                  // GPIO Port Q5
    GPIOPortQ6_IRQHandler,                  // GPIO Port Q6
    GPIOPortQ7_IRQHandler,                  // GPIO Port Q7
    GPIOPortR_IRQHandler,                   // GPIO Port R
    GPIOPortS_IRQHandler,                   // GPIO Port S
    PWM1Gen0_IRQHandler,                    // PWM 1 Generator 0
    PWM1Gen1_IRQHandler,                    // PWM 1 Generator 1
    PWM1Gen2_IRQHandler,                    // PWM 1 Generator 2
    PWM1Gen3_IRQHandler,                    // PWM 1 Generator 3
    PWM1Fault_IRQHandler                    // PWM 1 Fault
};

/* function prototypes -----------------------------------------------------*/
void Q_onAssert(char const *file, int line);


/****************************************************************************/
void NMI_Handler(void) {
    Q_onAssert("NMI_Handler", __LINE__);

    // Q_onAssert() should not return, this infinite loop is just a precaution
    while (1) {
    }
}

/****************************************************************************/
void HardFault_Handler(void) {
    Q_onAssert("HardFault_Handler", __LINE__);

    // Q_onAssert() should not return, this infinite loop is just a precaution
    while (1) {
    }
}
