/*****************************************************************************
// This LM3S startup file has been adapted from startup_ewarm.c
// Quantum Leaps on 28-Jul-2014
// www.state-machine.com
*****************************************************************************/
//*****************************************************************************
//
// startup_ewarm.c - Startup code for use with IAR's Embedded Workbench,
//                   version 5.
//
// Copyright (c) 2012-2013 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 2.0.1.11577 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************
#include "lm3s_cmsis.h"

/* Enable the IAR extensions for this source file */
#pragma language=extended

/* Function prototypes -----------------------------------------------------*/
void __iar_program_start(void);   /* IAR startup code */
void Spurious_Handler(void);      /* spurious exception/interrupt handler */


/* CSTACK section limits (created by the IAR linker) -----------------------*/
extern int CSTACK$$Base;  /* symbol created by the IAR linker */
extern int CSTACK$$Limit; /* symbol created by the IAR linker */

/* exception and interrupt vector table ------------------------------------*/
typedef void (*handler)(void); /* pointer to handler function type */

/*..........................................................................*/
__root handler const  __vector_table[] @ ".intvec" = {
    (handler)&CSTACK$$Limit,  /* The initial stack pointer    */
    &__iar_program_start,     /* Reset Handler                */
    &NMI_Handler,             /* NMI Handler                  */
    &HardFault_Handler,       /* Hard Fault Handler           */
    &MemManage_Handler,       /* MPU Fault Handler            */
    &BusFault_Handler,        /* Bus Fault Handler            */
    &UsageFault_Handler,      /* Usage Fault Handler          */
    &Spurious_Handler,        /* Reserved                     */
    &Spurious_Handler,        /* Reserved                     */
    &Spurious_Handler,        /* Reserved                     */
    &Spurious_Handler,        /* Reserved                     */
    &SVC_Handler,             /* SVCall Handler               */
    &DebugMon_Handler,        /* Debug Monitor Handler        */
    &Spurious_Handler,        /* Reserved                     */
    &PendSV_Handler,          /* PendSV Handler               */
    &SysTick_Handler,         /* SysTick Handler              */

    /* external interrupts (IRQs) ... */
    &GPIOPortA_IRQHandler,    /* GPIO Port A                  */
    &GPIOPortB_IRQHandler,    /* GPIO Port B                  */
    &GPIOPortC_IRQHandler,    /* GPIO Port C                  */
    &GPIOPortD_IRQHandler,    /* GPIO Port D                  */
    &GPIOPortE_IRQHandler,    /* GPIO Port E                  */
    &UART0_IRQHandler,        /* UART0 Rx and Tx              */
    &UART1_IRQHandler,        /* UART1 Rx and Tx              */
    &SSI0_IRQHandler,         /* SSI0 Rx and Tx               */
    &I2C0_IRQHandler,         /* I2C0 Master and Slave        */
    &PWMFault_IRQHandler,     /* PWM Fault                    */
    &PWMGen0_IRQHandler,      /* PWM Generator 0              */
    &PWMGen1_IRQHandler,      /* PWM Generator 1              */
    &PWMGen2_IRQHandler,      /* PWM Generator 2              */
    &QEI0_IRQHandler,         /* Quadrature Encoder 0         */
    &ADCSeq0_IRQHandler,      /* ADC Sequence 0               */
    &ADCSeq1_IRQHandler,      /* ADC Sequence 1               */
    &ADCSeq2_IRQHandler,      /* ADC Sequence 2               */
    &ADCSeq3_IRQHandler,      /* ADC Sequence 3               */
    &Watchdog_IRQHandler,     /* Watchdog timer               */
    &Timer0A_IRQHandler,      /* Timer 0 subtimer A           */
    &Timer0B_IRQHandler,      /* Timer 0 subtimer B           */
    &Timer1A_IRQHandler,      /* Timer 1 subtimer A           */
    &Timer1B_IRQHandler,      /* Timer 1 subtimer B           */
    &Timer2A_IRQHandler,      /* Timer 2 subtimer A           */
    &Timer2B_IRQHandler,      /* Timer 2 subtimer B           */
    &Comp0_IRQHandler,        /* Analog Comparator 0          */
    &Comp1_IRQHandler,        /* Analog Comparator 1          */
    &Comp2_IRQHandler,        /* Analog Comparator 2          */
    &SysCtrl_IRQHandler,      /* System Control (PLL,OSC,BO)  */
    &FlashCtrl_IRQHandler,    /* FLASH Control                */
    &GPIOPortF_IRQHandler,    /* GPIO Port F                  */
    &GPIOPortG_IRQHandler,    /* GPIO Port G                  */
    &GPIOPortH_IRQHandler,    /* GPIO Port H                  */
    &UART2_IRQHandler,        /* UART2 Rx and Tx              */
    &SSI1_IRQHandler ,        /* SSI1 Rx and Tx               */
    &Timer3A_IRQHandler,      /* Timer 3 subtimer A           */
    &Timer3B_IRQHandler,      /* Timer 3 subtimer B           */
    &I2C1_IRQHandler,         /* I2C1 Master and Slave        */
    &QEI1_IRQHandler,         /* Quadrature Encoder 1         */
    &CAN0_IRQHandler,         /* CAN0                         */
    &CAN1_IRQHandler,         /* CAN1                         */
    &CAN2_IRQHandler,         /* CAN2                         */
    &Spurious_Handler,        /* Reserved                     */
    &Hibernate_IRQHandler,    /* Hibernate                    */
};

/* exception handlers ------------------------------------------------------*/
/*..........................................................................*/
__stackless void NMI_Handler(void) {
    assert_failed("NMI", __LINE__); /* should never return! */
}
/*..........................................................................*/
__stackless void MemManage_Handler(void) {
    assert_failed("MemManage", __LINE__); /* should never return! */
}
/*..........................................................................*/
__stackless void HardFault_Handler(void) {
    unsigned old_sp = __get_SP();

    if (old_sp < (unsigned)&CSTACK$$Base) { /* stack overflow? */
        __set_SP((unsigned)&CSTACK$$Limit); /* initial stack pointer */
        assert_failed("StackOverflow", old_sp); /* should never return! */
    }
    else {
        assert_failed("HardFault", __LINE__); /* should never return! */
    }
}
/*..........................................................................*/
__stackless void BusFault_Handler(void) {
    assert_failed("BusFault", __LINE__); /* should never return! */
}
/*..........................................................................*/
__stackless void UsageFault_Handler(void) {
    assert_failed("UsageFault", __LINE__); /* should never return! */
}
/*..........................................................................*/
__stackless void Spurious_Handler(void) {
    assert_failed("Spurious", __LINE__); /* should never return! */
}

/*----------------------------------------------------------------------------
* Weak aliases for each Exception handler to the Spurious_Handler.
*/
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
#pragma weak Hibernate_IRQHandler   = Spurious_Handler


