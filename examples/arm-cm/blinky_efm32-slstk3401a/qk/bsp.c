/*****************************************************************************
* Product: "Blinky" on EFM32-SLSTK3401A board, preemptive QK kernel
* Last updated for version 6.9.1
* Last updated on  2020-09-22
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"

#include "em_device.h"  /* the device specific header (SiLabs) */
#include "em_cmu.h"     /* Clock Management Unit (SiLabs) */
#include "em_gpio.h"    /* GPIO (SiLabs) */
/* add other drivers if necessary... */

//Q_DEFINE_THIS_FILE

#ifdef Q_SPY
    #error Simple Blinky Application does not provide Spy build configuration
#endif

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);

/* Local-scope objects -----------------------------------------------------*/
#define LED0_PIN    4
#define LED0_PORT   gpioPortF

#define LED1_PIN    5
#define LED1_PORT   gpioPortF

#define BTN_SW1     (1U << 4)
#define BTN_SW2     (1U << 0)

/* ISRs used in this project ===============================================*/
void SysTick_Handler(void) {
    QK_ISR_ENTRY();   /* inform QK about entering an ISR */
    QF_TICK_X(0U, (void *)0); /* process time events for rate 0 */
    QK_ISR_EXIT();  /* inform QK about exiting an ISR */
}


/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* configure the FPU usage by choosing one of the options... */
#if 1
    /* OPTION 1:
    * Use the automatic FPU state preservation and the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in more than one task or
    * in any ISRs. This setting is the safest and recommended, but requires
    * extra stack space and CPU cycles.
    */
    FPU->FPCCR |= (1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos);
#else
    /* OPTION 2:
    * Do NOT to use the automatic FPU state preservation and
    * do NOT to use the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in ONE task only and not
    * in any ISR. This setting is very efficient, but if more than one task
    * (or ISR) start using the FPU, this can lead to corruption of the
    * FPU registers. This option should be used with CAUTION.
    */
    FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos)
                   | (1U << FPU_FPCCR_LSPEN_Pos));
#endif

    /* enable clock for to the peripherals used by this application... */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* configure the LEDs */
    GPIO_PinModeSet(LED0_PORT, LED0_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(LED1_PORT, LED1_PIN, gpioModePushPull, 0);
    GPIO_PinOutClear(LED0_PORT, LED0_PIN);
    GPIO_PinOutClear(LED1_PORT, LED1_PIN);

    /* configure the Buttons */
    //...
}

/*..........................................................................*/
void BSP_ledOff(void) {
    //GPIO_PinOutClear(LED0_PORT, LED0_PIN);
    GPIO->P[LED0_PORT].DOUT &= ~(1U << LED0_PIN);
}
/*..........................................................................*/
void BSP_ledOn(void) {
    /* exercise the FPU with some floating point computations */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    //GPIO_PinOutSet(LED0_PORT, LED0_PIN);
    GPIO->P[LED0_PORT].DOUT |= (1U << LED0_PIN);
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE1
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI);
    /* ... */

    /* enable IRQs... */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {
    /* toggle LED1 on and then off, see NOTE01 */
    QF_INT_DISABLE();
    GPIO->P[LED1_PORT].DOUT |=  (1U << LED1_PIN);
    GPIO->P[LED1_PORT].DOUT &= ~(1U << LED1_PIN);
    QF_INT_ENABLE();

#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
#endif
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
    NVIC_SystemReset();
}


/*****************************************************************************
* NOTE1:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
* are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF services. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE2:
* One of the LEDs is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
