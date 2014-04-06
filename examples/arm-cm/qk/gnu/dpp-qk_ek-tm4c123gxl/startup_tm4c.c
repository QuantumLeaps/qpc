/*****************************************************************************
* Product: CMSIS-compliant startup code for TM4C Cortex-M4F, GNU-ARM
* Last updated for version 5.3.0
* Last updated on  2014-02-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include <stdint.h>
#include "sysctl.h"
#include "rom.h"

/* Function prototypes -----------------------------------------------------*/
void Reset_Handler(void);
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
__attribute__ ((weak)) void SVC_Handler(void);
__attribute__ ((weak)) void DebugMon_Handler(void);
__attribute__ ((weak)) void PendSV_Handler(void);
__attribute__ ((weak)) void SysTick_Handler(void);
__attribute__ ((weak)) void Spurious_Handler(void);

/* external interrupts... */
__attribute__ ((weak)) void GPIOPortA_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortB_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortC_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortD_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortE_IRQHandler(void);
__attribute__ ((weak)) void UART0_IRQHandler(void);
__attribute__ ((weak)) void UART1_IRQHandler(void);
__attribute__ ((weak)) void SSI0_IRQHandler(void);
__attribute__ ((weak)) void I2C0_IRQHandler(void);
__attribute__ ((weak)) void PWMFault_IRQHandler(void);
__attribute__ ((weak)) void PWMGen0_IRQHandler(void);
__attribute__ ((weak)) void PWMGen1_IRQHandler(void);
__attribute__ ((weak)) void PWMGen2_IRQHandler(void);
__attribute__ ((weak)) void QEI0_IRQHandler(void);
__attribute__ ((weak)) void ADCSeq0_IRQHandler(void);
__attribute__ ((weak)) void ADCSeq1_IRQHandler(void);
__attribute__ ((weak)) void ADCSeq2_IRQHandler(void);
__attribute__ ((weak)) void ADCSeq3_IRQHandler(void);
__attribute__ ((weak)) void Watchdog_IRQHandler(void);
__attribute__ ((weak)) void Timer0A_IRQHandler(void);
__attribute__ ((weak)) void Timer0B_IRQHandler(void);
__attribute__ ((weak)) void Timer1A_IRQHandler(void);
__attribute__ ((weak)) void Timer1B_IRQHandler(void);
__attribute__ ((weak)) void Timer2A_IRQHandler(void);
__attribute__ ((weak)) void Timer2B_IRQHandler(void);
__attribute__ ((weak)) void Comp0_IRQHandler(void);
__attribute__ ((weak)) void Comp1_IRQHandler(void);
__attribute__ ((weak)) void Comp2_IRQHandler(void);
__attribute__ ((weak)) void SysCtrl_IRQHandler(void);
__attribute__ ((weak)) void FlashCtrl_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortF_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortG_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortH_IRQHandler(void);
__attribute__ ((weak)) void UART2_IRQHandler(void);
__attribute__ ((weak)) void SSI1_IRQHandler(void);
__attribute__ ((weak)) void Timer3A_IRQHandler(void);
__attribute__ ((weak)) void Timer3B_IRQHandler(void);
__attribute__ ((weak)) void I2C1_IRQHandler(void);
__attribute__ ((weak)) void QEI1_IRQHandler(void);
__attribute__ ((weak)) void CAN0_IRQHandler(void);
__attribute__ ((weak)) void CAN1_IRQHandler(void);
__attribute__ ((weak)) void CAN2_IRQHandler(void);
__attribute__ ((weak)) void Ethernet_IRQHandler(void);
__attribute__ ((weak)) void Hibernate_IRQHandler(void);

__attribute__ ((weak)) void USB0_IRQHandler(void);
__attribute__ ((weak)) void PWMGen3_IRQHandler(void);
__attribute__ ((weak)) void uDMAST_IRQHandler(void);
__attribute__ ((weak)) void uDMAError_IRQHandler(void);
__attribute__ ((weak)) void ADC1Seq0_IRQHandler(void);
__attribute__ ((weak)) void ADC1Seq1_IRQHandler(void);
__attribute__ ((weak)) void ADC1Seq2_IRQHandler(void);
__attribute__ ((weak)) void ADC1Seq3_IRQHandler(void);
__attribute__ ((weak)) void I2S0_IRQHandler(void);
__attribute__ ((weak)) void EBI0_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortJ_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortK_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortL_IRQHandler(void);
__attribute__ ((weak)) void SSI2_IRQHandler(void);
__attribute__ ((weak)) void SSI3_IRQHandler(void);
__attribute__ ((weak)) void UART3_IRQHandler(void);
__attribute__ ((weak)) void UART4_IRQHandler(void);
__attribute__ ((weak)) void UART5_IRQHandler(void);
__attribute__ ((weak)) void UART6_IRQHandler(void);
__attribute__ ((weak)) void UART7_IRQHandler(void);
__attribute__ ((weak)) void I2C2_IRQHandler(void);
__attribute__ ((weak)) void I2C3_IRQHandler(void);
__attribute__ ((weak)) void Timer4A_IRQHandler(void);
__attribute__ ((weak)) void Timer4B_IRQHandler(void);
__attribute__ ((weak)) void Timer5A_IRQHandler(void);
__attribute__ ((weak)) void Timer5B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer0A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer0B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer1A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer1B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer2A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer2B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer3A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer3B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer4A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer4B_IRQHandler(void);
__attribute__ ((weak)) void WideTimer5A_IRQHandler(void);
__attribute__ ((weak)) void WideTimer5B_IRQHandler(void);
__attribute__ ((weak)) void FPU_IRQHandler(void);
__attribute__ ((weak)) void PECI0_IRQHandler(void);
__attribute__ ((weak)) void LPC0_IRQHandler(void);
__attribute__ ((weak)) void I2C4_IRQHandler(void);
__attribute__ ((weak)) void I2C5_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortM_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortN_IRQHandler(void);
__attribute__ ((weak)) void QEI2_IRQHandler(void);
__attribute__ ((weak)) void Fan0_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP0_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP1_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP2_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP3_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP4_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP5_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP6_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortP7_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ0_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ1_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ2_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ3_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ4_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ5_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ6_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortQ7_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortR_IRQHandler(void);
__attribute__ ((weak)) void GPIOPortS_IRQHandler(void);
__attribute__ ((weak)) void PWM1Gen0_IRQHandler(void);
__attribute__ ((weak)) void PWM1Gen1_IRQHandler(void);
__attribute__ ((weak)) void PWM1Gen2_IRQHandler(void);
__attribute__ ((weak)) void PWM1Gen3_IRQHandler(void);
__attribute__ ((weak)) void PWM1Fault_IRQHandler(void);


/*----------------------------------------------------------------------------
* weak aliases for each Exception handler to the Spurious_Handler.
* Any function with the same name will override these definitions.
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
#pragma weak Ethernet_IRQHandler    = Spurious_Handler
#pragma weak Hibernate_IRQHandler   = Spurious_Handler

#pragma weak USB0_IRQHandler        = Spurious_Handler
#pragma weak PWMGen3_IRQHandler     = Spurious_Handler
#pragma weak uDMAST_IRQHandler      = Spurious_Handler
#pragma weak uDMAError_IRQHandler   = Spurious_Handler
#pragma weak ADC1Seq0_IRQHandler    = Spurious_Handler
#pragma weak ADC1Seq1_IRQHandler    = Spurious_Handler
#pragma weak ADC1Seq2_IRQHandler    = Spurious_Handler
#pragma weak ADC1Seq3_IRQHandler    = Spurious_Handler
#pragma weak I2S0_IRQHandler        = Spurious_Handler
#pragma weak EBI0_IRQHandler        = Spurious_Handler
#pragma weak GPIOPortJ_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortK_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortL_IRQHandler   = Spurious_Handler
#pragma weak SSI2_IRQHandler        = Spurious_Handler
#pragma weak SSI3_IRQHandler        = Spurious_Handler
#pragma weak UART3_IRQHandler       = Spurious_Handler
#pragma weak UART4_IRQHandler       = Spurious_Handler
#pragma weak UART5_IRQHandler       = Spurious_Handler
#pragma weak UART6_IRQHandler       = Spurious_Handler
#pragma weak UART7_IRQHandler       = Spurious_Handler
#pragma weak I2C2_IRQHandler        = Spurious_Handler
#pragma weak I2C3_IRQHandler        = Spurious_Handler
#pragma weak Timer4A_IRQHandler     = Spurious_Handler
#pragma weak Timer4B_IRQHandler     = Spurious_Handler
#pragma weak Timer5A_IRQHandler     = Spurious_Handler
#pragma weak Timer5B_IRQHandler     = Spurious_Handler
#pragma weak WideTimer0A_IRQHandler = Spurious_Handler
#pragma weak WideTimer0B_IRQHandler = Spurious_Handler
#pragma weak WideTimer1A_IRQHandler = Spurious_Handler
#pragma weak WideTimer1B_IRQHandler = Spurious_Handler
#pragma weak WideTimer2A_IRQHandler = Spurious_Handler
#pragma weak WideTimer2B_IRQHandler = Spurious_Handler
#pragma weak WideTimer3A_IRQHandler = Spurious_Handler
#pragma weak WideTimer3B_IRQHandler = Spurious_Handler
#pragma weak WideTimer4A_IRQHandler = Spurious_Handler
#pragma weak WideTimer4B_IRQHandler = Spurious_Handler
#pragma weak WideTimer5A_IRQHandler = Spurious_Handler
#pragma weak WideTimer5B_IRQHandler = Spurious_Handler
#pragma weak FPU_IRQHandler         = Spurious_Handler
#pragma weak PECI0_IRQHandler       = Spurious_Handler
#pragma weak LPC0_IRQHandler        = Spurious_Handler
#pragma weak I2C4_IRQHandler        = Spurious_Handler
#pragma weak I2C5_IRQHandler        = Spurious_Handler
#pragma weak GPIOPortM_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortN_IRQHandler   = Spurious_Handler
#pragma weak QEI2_IRQHandler        = Spurious_Handler
#pragma weak Fan0_IRQHandler        = Spurious_Handler
#pragma weak GPIOPortP0_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP1_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP2_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP3_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP4_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP5_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP6_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortP7_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ0_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ1_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ2_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ3_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ4_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ5_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ6_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortQ7_IRQHandler  = Spurious_Handler
#pragma weak GPIOPortR_IRQHandler   = Spurious_Handler
#pragma weak GPIOPortS_IRQHandler   = Spurious_Handler
#pragma weak PWM1Gen0_IRQHandler    = Spurious_Handler
#pragma weak PWM1Gen1_IRQHandler    = Spurious_Handler
#pragma weak PWM1Gen2_IRQHandler    = Spurious_Handler
#pragma weak PWM1Gen3_IRQHandler    = Spurious_Handler
#pragma weak PWM1Fault_IRQHandler   = Spurious_Handler


/* start and end of stack defined in the linker script ---------------------*/
extern unsigned __stack_start__;
extern unsigned __stack_end__;

/* exception and interrupt vector table ------------------------------------*/
typedef union VectorTableEntryTag {
    void *pointer;                 /* used only to initialize the stack top */
    void (*handler)(void); /* used for all exception and interrupt handlers */
} VectorTableEntry;

/*..........................................................................*/
__attribute__ ((section(".isr_vector")))
VectorTableEntry const g_pfnVectors[] = {
    { .pointer = &__stack_end__          }, /* initial stack pointer        */
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
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Hibernate_IRQHandler   }, /* Hibernate                    */
    { .handler = &USB0_IRQHandler        }, /* USB0                         */
    { .handler = &PWMGen3_IRQHandler     }, /* PWM Generator 3              */
    { .handler = &uDMAST_IRQHandler      }, /* uDMA Software Transfer       */
    { .handler = &uDMAError_IRQHandler   }, /* uDMA Error                   */
    { .handler = &ADC1Seq0_IRQHandler    }, /* ADC1 Sequence 0              */
    { .handler = &ADC1Seq1_IRQHandler    }, /* ADC1 Sequence 1              */
    { .handler = &ADC1Seq2_IRQHandler    }, /* ADC1 Sequence 2              */
    { .handler = &ADC1Seq3_IRQHandler    }, /* ADC1 Sequence 3              */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &GPIOPortJ_IRQHandler   }, /* GPIO Port J                  */
    { .handler = &GPIOPortK_IRQHandler   }, /* GPIO Port K                  */
    { .handler = &GPIOPortL_IRQHandler   }, /* GPIO Port L                  */
    { .handler = &SSI2_IRQHandler        }, /* SSI2 Rx and Tx               */
    { .handler = &SSI3_IRQHandler        }, /* SSI3 Rx and Tx               */
    { .handler = &UART3_IRQHandler       }, /* UART3 Rx and Tx              */
    { .handler = &UART4_IRQHandler       }, /* UART4 Rx and Tx              */
    { .handler = &UART5_IRQHandler       }, /* UART5 Rx and Tx              */
    { .handler = &UART6_IRQHandler       }, /* UART6 Rx and Tx              */
    { .handler = &UART7_IRQHandler       }, /* UART7 Rx and Tx              */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &I2C2_IRQHandler        }, /* I2C2 Master and Slave        */
    { .handler = &I2C3_IRQHandler        }, /* I2C3 Master and Slave        */
    { .handler = &Timer4A_IRQHandler     }, /* Timer 4 subtimer A           */
    { .handler = &Timer4B_IRQHandler     }, /* Timer 4 subtimer B           */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Timer5A_IRQHandler     }, /* Timer 5 subtimer A           */
    { .handler = &Timer5B_IRQHandler     }, /* Timer 5 subtimer B           */
    { .handler = &WideTimer0A_IRQHandler }, /* Wide Timer 0 subtimer A      */
    { .handler = &WideTimer0B_IRQHandler }, /* Wide Timer 0 subtimer B      */
    { .handler = &WideTimer1A_IRQHandler }, /* Wide Timer 1 subtimer A      */
    { .handler = &WideTimer1B_IRQHandler }, /* Wide Timer 1 subtimer B      */
    { .handler = &WideTimer2A_IRQHandler }, /* Wide Timer 2 subtimer A      */
    { .handler = &WideTimer2B_IRQHandler }, /* Wide Timer 2 subtimer B      */
    { .handler = &WideTimer3A_IRQHandler }, /* Wide Timer 3 subtimer A      */
    { .handler = &WideTimer3B_IRQHandler }, /* Wide Timer 3 subtimer B      */
    { .handler = &WideTimer4A_IRQHandler }, /* Wide Timer 4 subtimer A      */
    { .handler = &WideTimer4B_IRQHandler }, /* Wide Timer 4 subtimer B      */
    { .handler = &WideTimer5A_IRQHandler }, /* Wide Timer 5 subtimer A      */
    { .handler = &WideTimer5B_IRQHandler }, /* Wide Timer 5 subtimer B      */
    { .handler = &FPU_IRQHandler         }, /* FPU                          */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &I2C4_IRQHandler        }, /* I2C4 Master and Slave        */
    { .handler = &I2C5_IRQHandler        }, /* I2C5 Master and Slave        */
    { .handler = &GPIOPortM_IRQHandler   }, /* GPIO Port M                  */
    { .handler = &GPIOPortN_IRQHandler   }, /* GPIO Port N                  */
    { .handler = &QEI2_IRQHandler        }, /* Quadrature Encoder 2         */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &Spurious_Handler       }, /* Reserved                     */
    { .handler = &GPIOPortP0_IRQHandler  }, /* GPIO Port P (Summary or P0)  */
    { .handler = &GPIOPortP1_IRQHandler  }, /* GPIO Port P1                 */
    { .handler = &GPIOPortP2_IRQHandler  }, /* GPIO Port P2                 */
    { .handler = &GPIOPortP3_IRQHandler  }, /* GPIO Port P3                 */
    { .handler = &GPIOPortP4_IRQHandler  }, /* GPIO Port P4                 */
    { .handler = &GPIOPortP5_IRQHandler  }, /* GPIO Port P5                 */
    { .handler = &GPIOPortP6_IRQHandler  }, /* GPIO Port P6                 */
    { .handler = &GPIOPortP7_IRQHandler  }, /* GPIO Port P7                 */
    { .handler = &GPIOPortQ0_IRQHandler  }, /* GPIO Port Q (Summary or Q0)  */
    { .handler = &GPIOPortQ1_IRQHandler  }, /* GPIO Port Q1                 */
    { .handler = &GPIOPortQ2_IRQHandler  }, /* GPIO Port Q2                 */
    { .handler = &GPIOPortQ3_IRQHandler  }, /* GPIO Port Q3                 */
    { .handler = &GPIOPortQ4_IRQHandler  }, /* GPIO Port Q4                 */
    { .handler = &GPIOPortQ5_IRQHandler  }, /* GPIO Port Q5                 */
    { .handler = &GPIOPortQ6_IRQHandler  }, /* GPIO Port Q6                 */
    { .handler = &GPIOPortQ7_IRQHandler  }, /* GPIO Port Q7                 */
    { .handler = &GPIOPortR_IRQHandler   }, /* GPIO Port R                  */
    { .handler = &GPIOPortS_IRQHandler   }, /* GPIO Port S                  */
    { .handler = &PWM1Gen0_IRQHandler    }, /* PWM 1 Generator 0            */
    { .handler = &PWM1Gen1_IRQHandler    }, /* PWM 1 Generator 1            */
    { .handler = &PWM1Gen2_IRQHandler    }, /* PWM 1 Generator 2            */
    { .handler = &PWM1Gen3_IRQHandler    }, /* PWM 1 Generator 3            */
    { .handler = &PWM1Fault_IRQHandler   }  /* PWM 1 Fault                  */
};


/* exception handlers ------------------------------------------------------*/
__attribute__((naked)) void Reset_Handler(void);
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

    /* call all static constructors in C++ (harmless in C programs) */
    __libc_init_array();

    /* Enable the floating-point unit. This must be done here to handle the
    * case where main() uses floating-point and the function prologue saves
    * floating-point registers (which will fault if floating-point is not
    * enabled). Any configuration of the floating-point unit using DriverLib
    * APIs must be done here prior to the floating-point unit being enabled.
    */
    ROM_FPUEnable();

    (void)main();  /* application's entry point, which should never return! */
}
/*..........................................................................*/
__attribute__((naked)) void NMI_Handler(void);
void NMI_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_nmi\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_nmi: .asciz \"NMI\"\n\t"
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
    );
}
/*..........................................................................*/
__attribute__((naked)) void HardFault_Handler(void);
void HardFault_Handler(void) {
    __asm volatile (
        "    mov r0,sp\n\t"
        "    ldr r1,=__stack_start__\n\t"
        "    cmp r0,r1\n\t"
        "    bcs stack_ok\n\t"
        "    ldr r0,=__stack_end__\n\t"
        "    mov sp,r0\n\t"
        "    ldr r0,=str_overflow\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "stack_ok:\n\t"
        "    ldr r0,=str_hardfault\n\t"
        "    mov r1,#2\n\t"
        "    b assert_failed\n\t"
        "str_overflow:  .asciz \"StackOverflow\"\n\t"
        "str_hardfault: .asciz \"HardFault\"\n\t"
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
    );
}
/*..........................................................................*/
__attribute__((naked)) void Spurious_Handler(void);
void Spurious_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_spur\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_spur: .asciz \"Spurious\"\n\t"
    );
}
/*..........................................................................*/
void _init() {
}


