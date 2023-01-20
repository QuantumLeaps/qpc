//============================================================================
// Product: "Fly 'n' Shoot" game example
// Last updated for version 7.3.0
// Last updated on  2023-09-06
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"
#include "game.h"

//Q_DEFINE_THIS_FILE

static QTicker l_ticker0; // ticker active object for tick rate 0
QActive * const the_Ticker0 = &l_ticker0.super;

//............................................................................
int main() {
    QF_init();  // initialize the framework and the underlying RT kernel
    BSP_init(); // initialize the Board Support Package

    // init publish-subscribe...
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // initialize the event pools...

    //static QF_MPOOL_EL(QEvt) smlPoolSto[10];
    //QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));
    // NOTE:
    // After new rounding up, the "small" memory pool has the same block-size
    // as the next "medium" pool. If such "small" pool is initialized,
    // the next "medium" pool will casuse an assertion (the same block-size
    // pool already exists).

    static QF_MPOOL_EL(ObjectImageEvt) medPoolSto[2*GAME_MINES_MAX + 20];
    QF_poolInit(medPoolSto, sizeof(medPoolSto), sizeof(medPoolSto[0]));

    // object dictionaries for AOs...
    QS_OBJ_DICTIONARY(AO_Missile);
    QS_OBJ_DICTIONARY(AO_Ship);
    QS_OBJ_DICTIONARY(AO_Tunnel);

    // signal dictionaries for globally published events...
    QS_SIG_DICTIONARY(TIME_TICK_SIG,      (void *)0);
    QS_SIG_DICTIONARY(PLAYER_TRIGGER_SIG, (void *)0);
    QS_SIG_DICTIONARY(PLAYER_QUIT_SIG,    (void *)0);
    QS_SIG_DICTIONARY(GAME_OVER_SIG,      (void *)0);

    // start the active objects...
    QTicker_ctor(&l_ticker0, 0U); // active object for tick rate 0
    QACTIVE_START(the_Ticker0,
                  1U,                // QP priority
                  0, 0, 0, 0, 0);    // no queue, no stack , no init. event

    static QEvt const *tunnelQueueSto[GAME_MINES_MAX + 5];
    Tunnel_ctor_call();
    QACTIVE_START(AO_Tunnel,
                  2U,                // QP priority
                  tunnelQueueSto,  Q_DIM(tunnelQueueSto), // evt queue
                  (void *)0, 0U,     // no per-thread stack
                  (QEvt *)0);        // no initialization event

    static QEvt const *shipQueueSto[3];
    Ship_ctor_call();
    QACTIVE_START(AO_Ship,
                  3U,                // QP priority
                  shipQueueSto,    Q_DIM(shipQueueSto), // evt queue
                  (void *)0, 0U,     // no per-thread stack
                  (QEvt *)0);        // no initialization event

    static QEvt const *missileQueueSto[2];
    Missile_ctor_call();
    QACTIVE_START(AO_Missile,
                  4U,                // QP priority
                  missileQueueSto, Q_DIM(missileQueueSto), // evt queue
                  (void *)0, 0U,     // no per-thread stack
                  (QEvt *)0);        // no initialization event

    return QF_run(); // run the QF application
}

