/*****************************************************************************
* Product: "Orthogonal Component" example, Console based
* Last updated for version 6.4.0
* Last updated on  2019-02-08
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "alarm.h"
#include "clock.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
int main(int argc, char *argv[]) {
    static QEvt const *l_alarmClockQSto[10]; /* queue storage for AlarmClock */
    static QF_MPOOL_EL(TimeEvt) l_smlPoolSto[10]; /* storage for small pool */

    PRINTF_S("Orthogonal Component pattern\nQP version: %s\n"
           "Press 'o' to turn the Alarm ON\n"
           "Press 'f' to turn the Alarm OFF\n"
           "Press '0'..'9' to set the Alarm time\n"
           "Press 'a' to set the Clock in 12-hour mode\n"
           "Press 'b' to set the Clock in 24-hour mode\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    BSP_init(argc, argv); /* initialize the BSP */

    QF_init(); /* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* instantiate and start the active objects... */
    AlarmClock_ctor();
    QACTIVE_START(APP_alarmClock, 1U,
                  l_alarmClockQSto, Q_DIM(l_alarmClockQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run(); /* run the QF application */
}
