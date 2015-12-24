/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 5.6.0
* Date of the Last Update:  2015-11-24
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

static QXMutex l_mux;

/* global pointer to the test thread .......................................*/
QXThread * const XT_Test = &l_test;

/*..........................................................................*/
static void thread_function(void *par) {
    (void)par;

    QXMutex_init(&l_mux, 8U);

    for (;;) {

        (void)QXThread_queueGet(BSP_TICKS_PER_SEC*2U, 0U);

        QXMutex_lock(&l_mux);
        BSP_ledOn();
        QXMutex_unlock(&l_mux);

        QXThread_delay(BSP_TICKS_PER_SEC/4U, 0U);
        BSP_ledOff();
        QXThread_delay(BSP_TICKS_PER_SEC*3U/4U, 0U);
    }
}

/*..........................................................................*/
void Test_ctor(void) {
    QXThread_ctor(&l_test, &thread_function, 0U);
}
