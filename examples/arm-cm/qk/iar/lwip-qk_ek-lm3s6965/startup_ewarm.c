//*****************************************************************************
// Added STACK_SIZE macro and calls to Q_onAssert()
// Quantum Leaps on 25-Jan-2012
// www.state-machine.com
//*****************************************************************************

//*****************************************************************************
//
// startup_ewarm.c - Startup code for use with IAR's Embedded Workbench,
//                   version 5.
//
// Copyright (c) 2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
//
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
//
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 32 of the Stellaris CMSIS Package.
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
typedef union
{
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
__root const uVectorEntry __vector_table[] @ ".intvec" =
{
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
    Hibernate_IRQHandler                    // Hibernate
};

/* function prototypes -----------------------------------------------------*/
void Q_onAssert(char const *file, int line);


/****************************************************************************/
void
NMI_Handler(void)
{
    Q_onAssert("NMI_Handler", __LINE__);
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

/****************************************************************************/
void
HardFault_Handler(void)
{
    Q_onAssert("HardFault_Handler", __LINE__);
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}
