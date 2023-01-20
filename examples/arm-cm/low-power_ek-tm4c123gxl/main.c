//============================================================================
// Product: "Low-Power" example
// Last updated for version 7.3.0
// Last updated on  2023-08-25
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
#include "low_power.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

//............................................................................
int main() {
    QF_init();  // initialize the framework and the underlying RT kernel
    BSP_init(); // initialize the Board Support Package

    // initialize event pools...
    //QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto)); // init publish-subscribe

    // instantiate and start the active objects...
    Blinky0_ctor();
    static QEvt const *l_blinky0QSto[10];  // queue storage for Blinky0
    QACTIVE_START(AO_Blinky0,     // AO pointer
                  1U,             // unique QP priority of the AO
                  l_blinky0QSto,  // storage for the AO's queue
                  Q_DIM(l_blinky0QSto), // length of the queue [entries]
                  (void *)0,      // stack storage (not used in QK)
                  0U,             // stack size [bytes] (not used in QK)
                  (void *)0);     // initial param (not used)

#ifdef QXK_H_ // QXK kernel?
    XBlinky1_ctor();
    static uint32_t const *l_xblinky1Stack[64]; // stack for XBlinky1
    QXSemaphore_init(&XSEM_sw1, 0U, 1U); // signaling binary semaphore
    QXTHREAD_START(&XT_Blinky1,   // extended thread pointer
                  2U,             // unique QP priority of the AO
                  (QEvt const **)0, // storage for the AO's queue (not used)
                  0U,             // length of the queue [entries]
                  l_xblinky1Stack,  // stack storage (must provide in QXK)
                  sizeof(l_xblinky1Stack), // stack size [bytes]
                  (void *)0);     // initial param (not used)
#else // QV or QK kernels
    Blinky1_ctor();
    static QEvt const *l_blinky1QSto[10]; // queue storage for Blinky1
    QACTIVE_START(AO_Blinky1,     // AO pointer
                  2U,             // unique QP priority of the AO
                  l_blinky1QSto,  // storage for the AO's queue
                  Q_DIM(l_blinky1QSto), // length of the queue [entries]
                  (void *)0,      // stack storage (not used in QK)
                  0U,             // stack size [bytes] (not used in QK)
                  (void *)0);     // initial param (not used)
#endif // QXK_H_

    return QF_run(); // run the QF application
}
