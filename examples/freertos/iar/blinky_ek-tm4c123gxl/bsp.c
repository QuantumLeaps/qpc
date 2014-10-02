/*****************************************************************************
* Product: Simple Blinky example, Tiva EK-TM4C123GXL, QP-FreeRTOS port
* Last Updated for Version: 5.3.1
* Date of the Last Update:  2014-09-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. www.state-machine.com.
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
* Web:   http://www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qp_port.h"
#include "bsp.h"

#include "tm4c_cmsis.h"  /* Tiva-C CMSIS-compliant interface */
#include "sysctl.h"
#include "gpio.h"
#include "rom.h"

Q_DEFINE_THIS_FILE  /* define the name of this file for assertions */

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
enum KernelUnawareISRs { /* see NOTE1 */
    /* ... */
    MAX_KERNEL_UNAWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI
                 <= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

enum KernelAwareISRs {
    SYSTICK_PRIO = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, /* NOTE1 */
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

uint32_t SystemCoreClock;    /* System Clock Frequency (Core Clock) */


/* LEDs and Switches of the EK-TM4C123GXL board ............................*/
#define LED_RED     (1U << 1)
#define LED_GREEN   (1U << 3)
#define LED_BLUE    (1U << 2)

#define USR_SW1     (1U << 4)
#define USR_SW2     (1U << 0)

/*..........................................................................*/
void vApplicationTickHook(void) {
    QF_CRIT_STAT_TYPE intStat;
    BaseType_t lHigherPriorityTaskWoken = pdFALSE;

    QF_ISR_ENTRY(intStat);              /* <=== inform QF about ISR entry */

    QF_TICK_X(0U, (void *)0);  /* process all armed time events */

    QF_ISR_EXIT(intStat, lHigherPriorityTaskWoken); /* <=== ISR exit */

    /* yield only when needed... */
    if (lHigherPriorityTaskWoken != pdFALSE) {
        vTaskMissedYield();
    }
}
/*..........................................................................*/
void vApplicationIdleHook(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
#endif
}
/*..........................................................................*/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    Q_ERROR();
}

/*..........................................................................*/
void BSP_init(void) {
    /* Enable the floating-point unit */
    SCB->CPACR |= (0xFU << 20);

    /* Enable lazy stacking for interrupt handlers. This allows FPU
    * instructions to be used within interrupt handlers, but at the
    * expense of extra stack and CPU usage.
    */
    FPU->FPCCR |= (1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos);

    /* Set the clocking to run directly from the crystal */
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC
                       | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SystemCoreClock = ROM_SysCtlClockGet(); /* get the actual clock */

    /* enable clock to the peripherals used by the application */
    SYSCTL->RCGC2 |= (1U << 5); /* enable clock to GPIOF  */
    __NOP();                    /* wait after enabling clocks */
    __NOP();
    __NOP();

    /* configure the LEDs... */
    GPIOF->DIR |= (LED_RED | LED_GREEN | LED_BLUE);/* set direction: output */
    GPIOF->DEN |= (LED_RED | LED_GREEN | LED_BLUE); /* digital enable */
    GPIOF->DATA_Bits[LED_RED]   = 0; /* turn the LED off */
    GPIOF->DATA_Bits[LED_GREEN] = 0; /* turn the LED off */
    GPIOF->DATA_Bits[LED_BLUE]  = 0; /* turn the LED off */

    /* configure the User Switches... */
    GPIOF->DIR &= ~(USR_SW1 | USR_SW2); /*  set direction: input */
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, (USR_SW1 | USR_SW2),
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
/*..........................................................................*/
void BSP_ledOff() {
    GPIOF->DATA_Bits[LED_GREEN] = 0U;
}
/*..........................................................................*/
void BSP_ledOn() {
    GPIOF->DATA_Bits[LED_GREEN] = LED_GREEN;
}

/*..........................................................................*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE00
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    /* ... */
                                                          /* enable IRQs... */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    assert_failed(file, line);
}
/*..........................................................................*/
/* error routine that is called if the CMSIS library encounters an error    */
void assert_failed(char const *file, int line) {
    (void)file;         /* avoid compiler warning */
    (void)line;         /* avoid compiler warning */
    QF_INT_DISABLE();    /* make sure that all interrupts are disabled */
    ROM_SysCtlReset();   /* reset the system */
}

/*****************************************************************************
* NOTE1:
* The configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY constant from the
* FreeRTOS configuration file specifies the highest ISR priority that
* is disabled by the QF framework. The value is suitable for the
* NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY level (i.e.,
* with the numerical values of priorities equal or higher than
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are allowed to call any
* QP/FreeRTOS services. These ISRs are "kernel-aware".
*
* Conversely, any ISRs prioritized above the
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY priority level (i.e., with
* the numerical values of priorities less than
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are never disabled and are
* not aware of the kernel. Such "kernel-unaware" ISRs cannot call any
* QP/FreeRTOS services. The only mechanism by which a "kernel-unaware" ISR
* can communicate with the QF framework is by triggering a "kernel-aware"
* ISR, which can post/publish events.
*
* For more information, see article "Running the RTOS on a ARM Cortex-M Core"
* http://www.freertos.org/RTOS-Cortex-M3-M4.html
*/
