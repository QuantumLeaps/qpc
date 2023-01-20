//============================================================================
// QXThread example
// Last updated for version 7.3.0
// Last updated on  2023-08-12
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
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
#include "qpc.h"               // QP/C real-time embedded framework
#include "dpp.h"               // DPP Application interface
#include "bsp.h"               // Board Support Package

//----------------------------------------------------------------------------
//Q_DEFINE_THIS_FILE

//............................................................................
typedef struct XThread1 {
// protected:
    QXThread super;

// private:
    // NOTE: data needed by this thread should be members of
    // the thread class. That way they are in the memory region
    // accessible from this thread.
    uint8_t foo;

// public:
} XThread1;

// private:
static void XThread1_run(QXThread * const thr);
extern XThread1 XThread1_inst;

//............................................................................
QXThread * const TH_XThread1 = &XThread1_inst.super;

//............................................................................
void XThread1_ctor(void) {
    XThread1 *me = &XThread1_inst;
    QXThread_ctor(&me->super, &XThread1_run, 0U);
}

//............................................................................
XThread1 XThread1_inst;

//............................................................................
static void XThread1_run(QXThread * const thr) {
    QS_OBJ_DICTIONARY(TH_XThread1);
    QS_OBJ_DICTIONARY(&TH_XThread1->timeEvt);

    // downcast the generic thr pointer to the specific thread
    XThread1 *me = (XThread1 *)thr;

    // subscribe to the EAT signal (from the application)
    QActive_subscribe(&me->super.super, EAT_SIG);

    for (;;) {
        QEvt const *e = QXThread_queueGet(BSP_TICKS_PER_SEC/4U);
        if (e) {
            QXSemaphore_signal(&TH_sema); // signal Thread2
            QF_gc(e); // must explicitly recycle the received event!
        }

        QXMutex_lock(&TH_mutex, QXTHREAD_NO_TIMEOUT); // lock the mutex
        BSP_ledOn();
        if (QXMutex_tryLock(&TH_mutex)) { // exercise the mutex
            // some floating point code to exercise the VFP...
            float volatile x = 1.4142135F;
            x = x * 1.4142135F;
            QXThread_delay(10U);  // BLOCK while holding a mutex
            QXMutex_unlock(&TH_mutex);
        }
        QXMutex_unlock(&TH_mutex);
        BSP_ledOff();
    }
}
