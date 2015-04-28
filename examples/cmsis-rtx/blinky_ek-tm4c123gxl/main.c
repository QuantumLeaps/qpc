/*****************************************************************************
* Product: "Blinky" example, CMSIS-RTX kernel
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-03-27
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
* Web  : http://www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"

/*..........................................................................*/
int main() {
    static QEvt const *l_blinkyQSto[10]; /* Event queue storage for Blinky */

    /* in C you need to explicitly instantiate all AOs...  */
    Blinky_ctor();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

    /* start the active objects...
    * NOTE: Remember to configure sufficient number of concurrent running
    * user threads OS_TASKCNT in RTX_Conf_CM.c. The number also must
    * take into account the RTX timer thread used for QP system tick.
    */

    /* Blinky AO uses the custom stack size.
    * NOTE: Remember to configure sufficient number of threads with
    * user-provided stack size OS_PRIVCNT in RTX_Conf_CM.c
    */
    QF_setRtxPrio(AO_Blinky, osPriorityHigh); /* set RTX priority */
    QACTIVE_START(AO_Blinky,           /* AO pointer to start */
                  1U,                  /* unique QP priority of the AO */
                  l_blinkyQSto,        /* storage for the AO's queue */
                  Q_DIM(l_blinkyQSto), /* queue's length [entries] */
                  (void *)0,           /* no stack storage, (must be 0) */
                  128U*4U,             /* stack size [bytes] (or 0) */
                  (QEvt const *)0);    /* initialization event (or 0) */

    return QF_run(); /* run the QF application */
}
