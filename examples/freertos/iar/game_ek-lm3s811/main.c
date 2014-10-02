/*****************************************************************************
* Product: "Fly 'n' Shoot" game example, QP-FreeRTOS port
* Last Updated for Version: 5.3.1
* Date of the Last Update:  2014-09-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. www.state-machine.com.
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
* Web:   http://www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qp_port.h"
#include "bsp.h"
#include "game.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvt const * l_missileQueueSto[2];
static QEvt const * l_shipQueueSto[3];
static QEvt const * l_tunnelQueueSto[GAME_MINES_MAX + 5];

static QF_MPOOL_EL(QEvt)           l_smlPoolSto[10];
static QF_MPOOL_EL(ObjectImageEvt) l_medPoolSto[2*GAME_MINES_MAX + 10];

static QSubscrList    l_subscrSto[MAX_PUB_SIG];

/*..........................................................................*/
int main() {
    /* explicitly invoke the active objects' ctors... */
    Missile_ctor();
    Ship_ctor();
    Tunnel_ctor();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* initialize the event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
    QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto)); /* init publish-subscribe */

    /* send object dictionaries for event queues... */
    QS_OBJ_DICTIONARY(l_missileQueueSto);
    QS_OBJ_DICTIONARY(l_shipQueueSto);
    QS_OBJ_DICTIONARY(l_tunnelQueueSto);

    /* send object dictionaries for event pools... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_medPoolSto);

    /* send signal dictionaries for globally published events... */
    QS_SIG_DICTIONARY(TIME_TICK_SIG,      (void *)0);
    QS_SIG_DICTIONARY(PLAYER_TRIGGER_SIG, (void *)0);
    QS_SIG_DICTIONARY(PLAYER_QUIT_SIG,    (void *)0);
    QS_SIG_DICTIONARY(GAME_OVER_SIG,      (void *)0);

    /* start the active objects... */
    QACTIVE_START(AO_Missile,
                  1U,                 /* priority */
                  l_missileQueueSto, Q_DIM(l_missileQueueSto), /* evt queue */
                  (void *)0, 512U,    /* per-task stack */
                  (QEvt *)0);         /* no initialization event */
    QACTIVE_START(AO_Ship,
                  2U,                 /* priority */
                  l_shipQueueSto,    Q_DIM(l_shipQueueSto), /* evt queue */
                  (void *)0, 512U,    /* per-task stack */
                  (QEvt *)0);         /* no initialization event */
    QACTIVE_START(AO_Tunnel,
                  3U,                 /* priority */
                  l_tunnelQueueSto,  Q_DIM(l_tunnelQueueSto), /* evt queue */
                  (void *)0, 512U,    /* per-task stack */
                  (QEvt *)0);         /* no initialization event */

    return QF_run(); /* run the QF application */
}

