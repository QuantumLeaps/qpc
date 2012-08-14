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
#include "gui.h"
#include "qp_app.h"
//-----------------
extern "C" {
    #include "qp_port.h"
    #include "dpp.h"
    #include "bsp.h"
}

/* Local-scope objects -----------------------------------------------------*/
static QSubscrList l_subscrSto[MAX_PUB_SIG];

/* storage for event pools... */
static QF_MPOOL_EL(TableEvt) l_smlPoolSto[2*N_PHILO];         /* small pool */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    uint8_t n;

    Philo_ctor();             /* instantiate all Philosopher active objects */
    Table_ctor();                    /* instantiate the Table active object */

    QPApp app(argc, argv);
    Gui  gui;

    gui.show();

    QF_init();     /* initialize the framework and the underlying RT kernel */
    BSP_init();                                       /* initialize the BSP */

                                                  /* object dictionaries... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                               /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    for (n = 0; n < N_PHILO; ++n) {          /* start the active objects... */
        QActive_start(AO_Philo[n], (uint8_t)(n + 1),
                      (QEvt const **)0, 0U,       /* no private event queue */
                      (void *)0, 0U, (QEvt *)0);        /* no private stack */
    }
    QActive_start(AO_Table, (uint8_t)(N_PHILO + 1),
                  (QEvt const **)0, 0U,           /* no private event queue */
                  (void *)0, 0U, (QEvt *)0);            /* no private stack */

    return QF_run();                                  /* calls qApp->exec() */
}
