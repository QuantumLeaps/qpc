/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 5.6.2
* Date of the Last Update:  2016-03-30
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

/* local "naked" thread object .............................................*/
static QXThread l_test;
static QXMutex l_mutex;

/* global pointer to the test thread .......................................*/
QXThread * const XT_Test = &l_test;


/*..........................................................................*/
static void thread_function(void *par) {
    QXMutex_init(&l_mutex, 3U);

    (void)par;
    for (;;) {

        (void)QXThread_queueGet(BSP_TICKS_PER_SEC*2U, 0U); /* block */

        QXMutex_lock(&l_mutex);
        BSP_ledOn();
        /* NOTE: can't block while holding a mutex... */
        QXMutex_unlock(&l_mutex);

        QXThread_delay(BSP_TICKS_PER_SEC/4U, 0U);  /* block */
        BSP_ledOff();

        QXThread_delay(BSP_TICKS_PER_SEC*3U/4U, 0U);
    }
}

/*..........................................................................*/
void Test_ctor(void) {
    QXThread_ctor(&l_test, &thread_function, 0U);
}
