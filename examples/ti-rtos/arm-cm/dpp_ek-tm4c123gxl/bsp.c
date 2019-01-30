/*****************************************************************************
* Product: DPP example, EK-TM4C123GLX board, TI-RTOS
* Last Updated for Version: 6.3.8
* Date of the Last Update:  2019-01-30
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "Board.h"                /* the board specific header (TI) */
#include "ti/sysbios/knl/Clock.h" /* the Clock driver (TI) */
#include "ti/drivers/GPIO.h"      /* GPIO driver (TI) */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd;  /* random seed */

/* Clock function used in the application ==================================*/
void clk0Fxn(UArg arg0) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;

    QF_TICK_X(0U, &l_tickHook); /* process time events for rate 0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = GPIO_read(EK_TM4C123GXL_SW1);  /* read SW1 */
    tmp = buttons.depressed;     /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current);  /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if (tmp != 0U) {  /* debounced SW1 state changed? */
        if (buttons.depressed == 0U) { /* is SW1 depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_tickHook);
        }
        else { /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_tickHook);
        }
    }
}

/*..........................................................................*/
void myIdleFunc() { /* idle callback (see dpp.cfg) */
    QF_CRIT_STAT_TYPE key;

    QF_CRIT_ENTRY(key);
    GPIO_write(EK_TM4C123GXL_LED_RED, 1); /* turn the LED on  */
    GPIO_write(EK_TM4C123GXL_LED_RED, 0); /* turn the LED off */
    QF_CRIT_EXIT(key);

#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __asm (" WFI"); /* Wait-For-Interrupt */
#endif
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();

    BSP_randomSeed(1234U);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    /* exercise the FPU with some floating point computations */
    /* NOTE: this code can be only called from a task that created with
    * the option OS_TASK_OPT_SAVE_FP.
    */
    float volatile x;
    x = 3.1415926F;
    x = x + 2.7182818F;

    GPIO_write(EK_TM4C123GXL_LED_BLUE,
               ((stat[0] == 'e')   /* Is Philo[n] eating? */
               ? 1   /* turn the LED1 on  */
               : 0)); /* turn the LED1 off */
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    GPIO_write(EK_TM4C123GXL_LED_GREEN,
               ((paused != 0U)   /* is Eating paused? */
               ? 1   /* turn the LED1 on  */
               : 0)); /* turn the LED1 off */
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    static Clock_Struct clk0Struct;
    Clock_Params clkParams;

    Clock_Params_init(&clkParams);
    clkParams.startFlag = TRUE;

    /* NOTE:
    * The conversion constant 1000 must be consistent with Clock.tickPeriod
    * in the .cfg file for this TI-RTOS application!
    */
    clkParams.period = 1000U/BSP_TICKS_PER_SEC;

    /* Construct a periodic Clock Instance */
    Clock_construct(&clk0Struct, &clk0Fxn, clkParams.period, &clkParams);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void Q_onAssert(char const *module, int loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    //NVIC_SystemReset();
    for (;;) { /* for-ever loop (NOT a good idea for production code!) */
    }
}

/*****************************************************************************
* NOTE01:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
