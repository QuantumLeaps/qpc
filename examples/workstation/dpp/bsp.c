/*****************************************************************************
* Product: DPP example (console)
* Last Updated for Version: 6.9.1
* Date of the Last Update:  2020-09-10
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
#include "dpp.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <stdlib.h>   /* for exit() */

Q_DEFINE_THIS_FILE

/* local variables ---------------------------------------------------------*/
static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };
    static uint8_t const l_clock_tick = 0U;
#endif

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    PRINTF_S("Dining Philosophers Problem example"
           "\nQP %s\n"
           "Press 'p' to pause\n"
           "Press 's' to serve\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    BSP_randomSeed(1234U);

    Q_ALLEGE(QS_INIT((argc > 1) ? argv[1] : (void *)0));
    QS_OBJ_DICTIONARY(&l_clock_tick); /* must be called *after* QF_init() */
    QS_USR_DICTIONARY(PHILO_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
    QF_stop(); /* stop the main "ticker thread" */
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    PRINTF_S("Philosopher %2d is %s\n", (int)n, stat);

     /* application-specific record */
    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio)
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    PRINTF_S("Paused is %s\n", paused ? "ON" : "OFF");
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
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

/****************************************************************************/
/*..........................................................................*/
void QF_onStartup(void) {
    QF_consoleSetup();
    QF_setTickRate(BSP_TICKS_PER_SEC, 50); /* desired tick rate/ticker-prio */
}
/*..........................................................................*/
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
/*..........................................................................*/
void QF_onClockTick(void) {
    QF_TICK_X(0U, &l_clock_tick); /* perform the QF clock tick processing */

    QS_RX_INPUT(); /* handle the QS-RX input */
    QS_OUTPUT();   /* handle the QS output */

    switch (QF_consoleGetKey()) {
        case '\33': { /* see if the ESC key pressed */
            BSP_terminate(0);
            break;
        }
        case 'p': {
            QF_PUBLISH(Q_NEW(QEvt, PAUSE_SIG), &l_clock_tick);
            break;
        }
        case 's': {
            QF_PUBLISH(Q_NEW(QEvt, SERVE_SIG), &l_clock_tick);
            break;
        }
        default: {
            break;
        }
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY /* define QS callbacks */

/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }

    /* unused parameters */
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
    FPRINTF_S(stderr, "Assertion failed in %s:%d", module, loc);
    QF_onCleanup();
    QS_EXIT();
    exit(-1);
}

