/*****************************************************************************
* Product: "Fly 'n' Shoot" game example for Windows
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
#include "bsp.h"
#include "game.h"

Q_DEFINE_THIS_FILE

/* "fudge factor" for Windows, see NOTE1 */
enum { WIN_FUDGE_FACTOR = 10 };

/*..........................................................................*/
int main() {
    static QEvt const *missileQueueSto[2*WIN_FUDGE_FACTOR];
    static QEvt const *shipQueueSto[3*WIN_FUDGE_FACTOR];
    static QEvt const *tunnelQueueSto[(GAME_MINES_MAX + 5)*WIN_FUDGE_FACTOR];
    static QF_MPOOL_EL(QEvt) smlPoolSto[10*WIN_FUDGE_FACTOR];
    static QF_MPOOL_EL(ObjectImageEvt)
           medPoolSto[(2*GAME_MINES_MAX + 10)*WIN_FUDGE_FACTOR];

    static QSubscrList subscrSto[MAX_PUB_SIG];

    /* explicitly invoke the active objects' ctors... */
    Missile_ctor();
    Ship_ctor();
    Tunnel_ctor();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* init publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize the event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));
    QF_poolInit(medPoolSto, sizeof(medPoolSto), sizeof(medPoolSto[0]));

    /* object dictionaries for AOs... */
    QS_OBJ_DICTIONARY(AO_Missile);
    QS_OBJ_DICTIONARY(AO_Ship);
    QS_OBJ_DICTIONARY(AO_Tunnel);

    /* signal dictionaries for globally published events... */
    QS_SIG_DICTIONARY(TIME_TICK_SIG,      (void *)0);
    QS_SIG_DICTIONARY(PLAYER_TRIGGER_SIG, (void *)0);
    QS_SIG_DICTIONARY(PLAYER_QUIT_SIG,    (void *)0);
    QS_SIG_DICTIONARY(GAME_OVER_SIG,      (void *)0);

    /* start the active objects... */
    QACTIVE_START(AO_Tunnel,
                  1U,                /* QP priority */
                  tunnelQueueSto,  Q_DIM(tunnelQueueSto), /* evt queue */
                  (void *)0, 0U,     /* no per-thread stack */
                  (QEvt *)0);        /* no initialization event */
    QACTIVE_START(AO_Ship,
                  2U,                /* QP priority */
                  shipQueueSto,    Q_DIM(shipQueueSto), /* evt queue */
                  (void *)0, 0U,     /* no per-thread stack */
                  (QEvt *)0);        /* no initialization event */
    QACTIVE_START(AO_Missile,
                  3U,                /* QP priority */
                  missileQueueSto, Q_DIM(missileQueueSto), /* evt queue */
                  (void *)0, 0U,     /* no per-thread stack */
                  (QEvt *)0);        /* no initialization event */

    return QF_run(); /* run the QF application */
}

/*****************************************************************************
* NOTE1:
* Windows is not a deterministic real-time system, which means that the
* system can occasionally and unexpectedly "choke and freeze" for a number
* of seconds. The designers of Windows have dealt with these sort of issues
* by massively oversizing the resources available to the applications. For
* example, the default Windows GUI message queues size is 10,000 entries,
* which can dynamically grow to an even larger number. Also the stacks of
* Win32 threads can dynamically grow to several megabytes.
*
* In contrast, the event queues, event pools, and stack size inside the
* real-time embedded (RTE) systems can be (and must be) much smaller,
* because you typically can put an upper bound on the real-time behavior
* and the resulting delays.
*
* To be able to run the unmodified applications designed originally for
* RTE systems on Windows, and to reduce the odds of resource shortages in
* this case, the generous WIN_FUDGE_FACTOR is used to oversize the
* event queues and event pools.
*/
