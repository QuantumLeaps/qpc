//*****************************************************************************
// Modified for CMSIS compliance by Quantum Leaps
// Preemptive QK kernel
// Jan 26, 2013
//*****************************************************************************

//*****************************************************************************
//
// startup_ccs.c - Startup code for use with TI's Code Composer Studio.
//
// Copyright (c) 2005-2010 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 5961 of the DK-LM3S811 Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
// prototypes of the IRQ handlers defined in the application
//*****************************************************************************
void SysTick_Handler(void);
void GPIOPortA_IRQHandler(void);

//*****************************************************************************
// prototypes of the default fault handlers.
//*****************************************************************************
void ResetISR(void);
static void NMI_Handler(void);
static void HardFault_Handler(void);
static void MemManage_Handler(void);
static void BusFault_Handler(void);
static void UsageFault_Handler(void);
static void Reserved_Handler(void);
static void MemManage_Handler(void);
static void Reserved_Handler(void);
void SVC_Handler(void);
static void DebugMon_Handler(void);
void PendSV_Handler(void);

static void Unused_IRQHandler(void);

//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// Linker variable that marks the top of the stack.
//
//*****************************************************************************
extern unsigned long __STACK_TOP;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000 or at the start of
// the program if located at a start address other than 0.
//
//*****************************************************************************
#pragma DATA_SECTION(g_pfnVectors, ".intvecs")
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)&__STACK_TOP),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    MemManage_Handler,                      // The MPU fault handler
    BusFault_Handler,                       // The bus fault handler
    UsageFault_Handler,                     // The usage fault handler
    Reserved_Handler,                       // Reserved
    Reserved_Handler,                       // Reserved
    Reserved_Handler,                       // Reserved
    Reserved_Handler,                       // Reserved
    SVC_Handler,                            // SVCall handler
    DebugMon_Handler,                       // Debug monitor handler
    Reserved_Handler,                       // Reserved
    PendSV_Handler,                         // The PendSV handler
    SysTick_Handler,                        // The SysTick handler
    GPIOPortA_IRQHandler,                   // GPIO Port A
    Unused_IRQHandler,                      // GPIO Port B
    Unused_IRQHandler,                      // GPIO Port C
    Unused_IRQHandler,                      // GPIO Port D
    Unused_IRQHandler,                      // GPIO Port E
    Unused_IRQHandler,                      // UART0 Rx and Tx
    Unused_IRQHandler,                      // UART1 Rx and Tx
    Unused_IRQHandler,                      // SSI0 Rx and Tx
    Unused_IRQHandler,                      // I2C0 Master and Slave
    Unused_IRQHandler,                      // PWM Fault
    Unused_IRQHandler,                      // PWM Generator 0
    Unused_IRQHandler,                      // PWM Generator 1
    Unused_IRQHandler,                      // PWM Generator 2
    Unused_IRQHandler,                      // Quadrature Encoder 0
    Unused_IRQHandler,                      // ADC Sequence 0
    Unused_IRQHandler,                      // ADC Sequence 1
    Unused_IRQHandler,                      // ADC Sequence 2
    Unused_IRQHandler,                      // ADC Sequence 3
    Unused_IRQHandler,                      // Watchdog timer
    Unused_IRQHandler,                      // Timer 0 subtimer A
    Unused_IRQHandler,                      // Timer 0 subtimer B
    Unused_IRQHandler,                      // Timer 1 subtimer A
    Unused_IRQHandler,                      // Timer 1 subtimer B
    Unused_IRQHandler,                      // Timer 2 subtimer A
    Unused_IRQHandler,                      // Timer 2 subtimer B
    Unused_IRQHandler,                      // Analog Comparator 0
    Unused_IRQHandler,                      // Analog Comparator 1
    Unused_IRQHandler,                      // Analog Comparator 2
    Unused_IRQHandler,                      // System Control (PLL, OSC, BO)
    Unused_IRQHandler                       // FLASH Control
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
void ResetISR(void) {
    //
    // Jump to the CCS C Initialization Routine.
    //
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}

//*****************************************************************************
// These dummy handlers simply enter an infinite loop, preserving the system
// state for examination by a debugger.
//*****************************************************************************

static void NMI_Handler(void) {
    while(1) {
    }
}

static void HardFault_Handler(void) {
    while(1) {
    }
}


static void MemManage_Handler(void) {
    while(1) {
    }
}

static void BusFault_Handler(void) {
    while(1) {
    }
}

static void UsageFault_Handler(void) {
    while(1) {
    }
}

static void Reserved_Handler(void) {
    while(1) {
    }
}

//static void SVC_Handler(void) {
//    while(1) {
//    }
//}

static void DebugMon_Handler(void) {
    while(1) {
    }
}

//static void PendSV_Handler(void) {
//    while(1) {
//    }
//}

static void Unused_IRQHandler(void) {
    while(1) {
    }
}
