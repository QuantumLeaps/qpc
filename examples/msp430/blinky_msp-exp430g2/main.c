/*****************************************************************************
* Product: Blinky example
* Last updated for version 7.1.0
* Last updated on  2022-08-28
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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

Q_DEFINE_THIS_FILE

/*..........................................................................*/
int main() {
    QF_init();    /* initialize the framework and the underlying RT kernel */
    BSP_init();   /* initialize the Board Support Package */

    Blinky_ctor();
    static QEvt const *blinkyeQueueSto[2];
    QACTIVE_START(AO_Blinky,       /* AO to start */
        Q_PRIO(1U, 1U),            /* QF-priority/preemption-thre. */
        blinkyeQueueSto,           /* event queue storage */
        Q_DIM(blinkyeQueueSto),    /* queue length [events] */
        (void *)0,                 /* stack storage (not used) */
        0U,                        /* size of the stack [bytes] */
        (void *)0);                /* initialization param */

    return QF_run(); /* run the QF application */
}

