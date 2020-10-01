/*****************************************************************************
* Product: Console-based BSP
* Last Updated for Version: 6.9.1
* Date of the Last Update:  2020-09-21
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
* <www.state-machine.com/>
* <info@state-machine.com/>
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <stdlib.h>   /* for exit() */

Q_DEFINE_THIS_FILE

#ifdef Q_SPY
static uint8_t const l_QF_onClockTick = 0;
#endif

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    QS_OBJ_DICTIONARY(&l_QF_onClockTick);

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
}
/*..........................................................................*/
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC, 30); /* set the desired tick rate */
    QF_consoleSetup();
}
/*..........................................................................*/
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
/*..........................................................................*/
void QF_onClockTick(void) {
    int key;
    QF_TICK_X(0U, &l_QF_onClockTick); /* perform QF clock tick processing */

    QS_RX_INPUT(); /* handle the QS-RX input */
    QS_OUTPUT();   /* handle the QS output */

    key = QF_consoleGetKey();
    if (key != 0) { /* any key pressed? */
        BSP_onKeyboardInput(key);
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
Q_NORETURN Q_onAssert(char_t const * const file, int_t const line) {
    FPRINTF_S(stderr, "Assertion failed in %s, line %d", file, line);
    QF_onCleanup();
    exit(-1);
}

