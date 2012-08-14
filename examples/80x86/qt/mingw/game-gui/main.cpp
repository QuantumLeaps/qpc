//////////////////////////////////////////////////////////////////////////////
// Product: QP/C++ example
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Jul 05, 2012
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
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
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qp_app.h"
#include "gui.h"
//-----------------
extern "C" {
    #include "qp_port.h"
    #include "game.h"
    #include "bsp.h"
}

/* Local-scope objects -----------------------------------------------------*/

/* storage for event pools... */
static QF_MPOOL_EL(QEvt)           l_smlPoolSto[30];          /* small pool */
static QF_MPOOL_EL(ObjectImageEvt) l_medPoolSto[2*GAME_MINES_MAX + 20];

static QSubscrList l_subscrSto[MAX_PUB_SIG];

//............................................................................
int main(int argc, char *argv[]) {
    QPApp app(argc, argv);
    Gui  gui;

    gui.show();

                          /* explicitly invoke the active objects' ctors... */
    Missile_ctor();
    Ship_ctor();
    Tunnel_ctor();

    QF_init();     /* initialize the framework and the underlying RT kernel */

    BSP_init();                     /* initialize the Board Support Package */
                                           /* initialize the event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
    QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                            /* send object dictionaries for event queues... */

                             /* send object dictionaries for event pools... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_medPoolSto);

               /* send signal dictionaries for globally published events... */
    QS_SIG_DICTIONARY(TIME_TICK_SIG,      (void *)0);
    QS_SIG_DICTIONARY(PLAYER_TRIGGER_SIG, (void *)0);
    QS_SIG_DICTIONARY(PLAYER_QUIT_SIG,    (void *)0);
    QS_SIG_DICTIONARY(GAME_OVER_SIG,      (void *)0);

                                             /* start the active objects... */
    QActive_start(AO_Missile,
                  1U,                                           /* priority */
                  (QEvt const **)0, 0U,           /* no private event queue */
                  (void *)0, 0U, (QEvt *)0);            /* no private stack */
    QActive_start(AO_Ship,
                  2U,                                           /* priority */
                  (QEvt const **)0, 0U,           /* no private event queue */
                  (void *)0, 0U, (QEvt *)0);            /* no private stack */
    QActive_start(AO_Tunnel,
                  3U,                                           /* priority */
                  (QEvt const **)0, 0U,           /* no private event queue */
                  (void *)0, 0U, (QEvt *)0);            /* no private stack */

    return QF_run();                                  /* calls qApp->exec() */
}
