/*****************************************************************************
* Product: Simple Blinky example, Stellaris EK-LM3S811, QP-FreeRTOS port
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

#include "lm3s_cmsis.h"

Q_DEFINE_THIS_FILE

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

//uint32_t SystemCoreClock;

/* Local-scope objects -----------------------------------------------------*/
#define PUSH_BUTTON             (1U << 4)
#define USER_LED                (1U << 5)

/*..........................................................................*/
void vApplicationTickHook(void) {
    QF_CRIT_STAT_TYPE intStat;
    BaseType_t lHigherPriorityTaskWoken = pdFALSE;

    QF_ISR_ENTRY(intStat);              /* <=== inform QF about ISR entry */

    QF_TICK_X(0U, &l_SysTick_Handler); /* process all time events at rate 0 */

    QF_ISR_EXIT(intStat, lHigherPriorityTaskWoken); /* <=== ISR exit */

    /* yield only when needed... */
    if (lHigherPriorityTaskWoken != pdFALSE) {
        vTaskMissedYield();
    }
}

/*..........................................................................*/
void BSP_init(void) {
    /* set the system clock as specified in lm3s_config.h (20MHz from PLL)  */
    SystemInit();

    /* enable clock to the peripherals used by the application */
    SYSCTL->RCGC2 |= (1 <<  0) | (1 <<  2); /* enable clock to GPIOA & C  */
    __NOP();                                /* wait after enabling clocks */
    __NOP();
    __NOP();

    /* configure the LED and push button */
    GPIOC->DIR |= USER_LED;          /* set direction: output */
    GPIOC->DEN |= USER_LED;          /* digital enable */
    GPIOC->DATA_Bits[USER_LED] = 0U; /* turn the User LED off */

    GPIOC->DIR &= ~PUSH_BUTTON;      /*  set direction: input */
    GPIOC->DEN |= PUSH_BUTTON;       /* digital enable */
}
/*..........................................................................*/
void BSP_ledOff(void) {
    GPIOC->DATA_Bits[USER_LED] = 0U; /* turn the User LED off */
}
/*..........................................................................*/
void BSP_ledOn(void) {
    GPIOC->DATA_Bits[USER_LED] = USER_LED; /* turn the User LED on */
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
void vApplicationIdleHook(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
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
void Q_onAssert(char const Q_ROM * const file, int_t line) {
    assert_failed(file, line);
}
/*..........................................................................*/
/* error routine that is called if the CMSIS library encounters an error    */
void assert_failed(char const *file, int line) {
    (void)file;        /* avoid compiler warning */
    (void)line;        /* avoid compiler warning */
    QF_INT_DISABLE();   /* make sure that all interrupts are disabled */
    NVIC_SystemReset(); /* perform system reset */
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
