/*****************************************************************************
* Product: CMSIS-compliant startup code for LM3S Cortex-M3, IAR-ARM
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 24, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
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

/* Enable the IAR extensions for this source file */
#pragma language=extended

/* Function prototypes -----------------------------------------------------*/
void __iar_program_start(void);                         /* IAR startup code */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void HardFault_Handler(void);
void Spurious_Handler(void);

void assert_failed(char const *file, int line);       /* defined in the BSP */

/*----------------------------------------------------------------------------
* weak aliases for each Exception handler to the Spurious_Handler.
* Any function with the same name will override these definitions.
*/
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
__weak void Hibernate_IRQHandler(void);
__weak void Ethernet_IRQHandler(void);


/* CSTACK section limits (created by the IAR linker) -----------------------*/
extern int CSTACK$$Base;                /* symbol created by the IAR linker */
extern int CSTACK$$Limit;               /* symbol created by the IAR linker */

/* exception and interrupt vector table ------------------------------------*/
typedef union VectorTableEntryTag {
    void *pointer;                 /* used only to initialize the stack top */
    void (*handler)(void); /* used for all exception and interrupt handlers */
} VectorTableEntry;

/*..........................................................................*/
__root const VectorTableEntry __vector_table[] @ ".intvec" = {
    { .pointer = &CSTACK$$Limit          }, /* The initial stack pointer    */
    { .handler = &__iar_program_start    }, /* IAR Reset Handler            */
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
    { .handler = &GPIOPortA_IRQHandler   }, /* GPIO Port A                  */
    { .handler = &GPIOPortB_IRQHandler   }, /* GPIO Port B                  */
    { .handler = &GPIOPortC_IRQHandler   }, /* GPIO Port C                  */
    { .handler = &GPIOPortD_IRQHandler   }, /* GPIO Port D                  */
    { .handler = &GPIOPortE_IRQHandler   }, /* GPIO Port E                  */
    { .handler = &UART0_IRQHandler       }, /* UART0 Rx and Tx              */
    { .handler = &UART1_IRQHandler       }, /* UART1 Rx and Tx              */
    { .handler = &SSI0_IRQHandler        }, /* SSI0 Rx and Tx               */
    { .handler = &I2C0_IRQHandler        }, /* I2C0 Master and Slave        */
    { .handler = &PWMFault_IRQHandler    }, /* PWM Fault                    */
    { .handler = &PWMGen0_IRQHandler     }, /* PWM Generator 0              */
    { .handler = &PWMGen1_IRQHandler     }, /* PWM Generator 1              */
    { .handler = &PWMGen2_IRQHandler     }, /* PWM Generator 2              */
    { .handler = &QEI0_IRQHandler        }, /* Quadrature Encoder 0         */
    { .handler = &ADCSeq0_IRQHandler     }, /* ADC Sequence 0               */
    { .handler = &ADCSeq1_IRQHandler     }, /* ADC Sequence 1               */
    { .handler = &ADCSeq2_IRQHandler     }, /* ADC Sequence 2               */
    { .handler = &ADCSeq3_IRQHandler     }, /* ADC Sequence 3               */
    { .handler = &Watchdog_IRQHandler    }, /* Watchdog timer               */
    { .handler = &Timer0A_IRQHandler     }, /* Timer 0 subtimer A           */
    { .handler = &Timer0B_IRQHandler     }, /* Timer 0 subtimer B           */
    { .handler = &Timer1A_IRQHandler     }, /* Timer 1 subtimer A           */
    { .handler = &Timer1B_IRQHandler     }, /* Timer 1 subtimer B           */
    { .handler = &Timer2A_IRQHandler     }, /* Timer 2 subtimer A           */
    { .handler = &Timer2B_IRQHandler     }, /* Timer 2 subtimer B           */
    { .handler = &Comp0_IRQHandler       }, /* Analog Comparator 0          */
    { .handler = &Comp1_IRQHandler       }, /* Analog Comparator 1          */
    { .handler = &Comp2_IRQHandler       }, /* Analog Comparator 2          */
    { .handler = &SysCtrl_IRQHandler     }, /* System Control (PLL,OSC,BO)  */
    { .handler = &FlashCtrl_IRQHandler   }, /* FLASH Control                */
    { .handler = &GPIOPortF_IRQHandler   }, /* GPIO Port F                  */
    { .handler = &GPIOPortG_IRQHandler   }, /* GPIO Port G                  */
    { .handler = &GPIOPortH_IRQHandler   }, /* GPIO Port H                  */
    { .handler = &UART2_IRQHandler       }, /* UART2 Rx and Tx              */
    { .handler = &SSI1_IRQHandler        }, /* SSI1 Rx and Tx               */
    { .handler = &Timer3A_IRQHandler     }, /* Timer 3 subtimer A           */
    { .handler = &Timer3B_IRQHandler     }, /* Timer 3 subtimer B           */
    { .handler = &I2C1_IRQHandler        }, /* I2C1 Master and Slave        */
    { .handler = &QEI1_IRQHandler        }, /* Quadrature Encoder 1         */
    { .handler = &CAN0_IRQHandler        }, /* CAN0                         */
    { .handler = &CAN1_IRQHandler        }, /* CAN1                         */
    { .handler = &CAN2_IRQHandler        }, /* CAN2                         */
    { .handler = &Ethernet_IRQHandler    }, /* Ethernet                     */
    { .handler = &Hibernate_IRQHandler   }, /* Hibernate                    */
};


/* exception handlers ------------------------------------------------------*/
__stackless void NMI_Handler(void) {
    assert_failed("NMI", __LINE__);                 /* should never return! */
}
/*..........................................................................*/
__stackless void MemManage_Handler(void) {
    assert_failed("MemManage", __LINE__);           /* should never return! */
}
/*..........................................................................*/
__stackless void HardFault_Handler(void) {
    unsigned old_sp = __get_SP();

    if (old_sp < (unsigned)&CSTACK$$Base) {              /* stack overflow? */
        __set_SP((unsigned)&CSTACK$$Limit);        /* initial stack pointer */
        assert_failed("StackOverflow", old_sp);     /* should never return! */
    }
    else {
        assert_failed("HardFault", __LINE__);       /* should never return! */
    }
}
/*..........................................................................*/
__stackless void BusFault_Handler(void) {
    assert_failed("BusFault", __LINE__);            /* should never return! */
}
/*..........................................................................*/
__stackless void UsageFault_Handler(void) {
    assert_failed("UsageFault", __LINE__);          /* should never return! */
}
/*..........................................................................*/
__stackless void Spurious_Handler(void) {
    assert_failed("Spurious", __LINE__);            /* should never return! */
}


