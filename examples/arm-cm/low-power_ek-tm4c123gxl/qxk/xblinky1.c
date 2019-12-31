/*****************************************************************************
* Product: "Low-Power" example, EK-TM4C123GXL board, QXK kernel
* Last updated for version 6.4.0
* Last updated on  2019-02-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "low_power.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

/* local objects -----------------------------------------------------------*/
static void XBlinky1_run(QXThread * const me);

/* global objects ----------------------------------------------------------*/
QXThread XT_Blinky1;
QXSemaphore XSEM_sw1;

/*..........................................................................*/
void XBlinky1_ctor(void) {
    QXThread_ctor(&XT_Blinky1,
                  &XBlinky1_run, /* thread routine */
                  1U); /* associate the thread with tick rate-1 */
}

/*..........................................................................*/
static void XBlinky1_run(QXThread * const me) {
    bool isActive = false;

    (void)me; /* unused parameter */
    for (;;) {
        if (!isActive) {
            QXSemaphore_wait(&XSEM_sw1, QXTHREAD_NO_TIMEOUT);
            isActive = true;
        }
        else {
            uint8_t count;
            BSP_tickRate1_on();
            for (count = 13U; count > 0U; --count) {
                BSP_led1_on();
                QXThread_delay(1U);
                BSP_led1_off();
                QXThread_delay(1U);
            }
            isActive = false;
        }
    }
}
