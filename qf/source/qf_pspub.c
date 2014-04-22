/**
* \file
* \brief QF_publish_() implementation.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_pspub")

/****************************************************************************/
/**
* \description
* This function posts (using the FIFO policy) the event \a e to ALL
* active objects that have subscribed to the signal \a e->sig.
* This function is designed to be callable from any part of the system,
* including ISRs, device drivers, and active objects.
*
* \note
* In the general case, event publishing requires multicasting the
* event to multiple subscribers. This happens in the caller's thread with
* the scheduler locked to prevent preemption during the multicasting
* process. (Please note that the interrupts are not locked.)
*
* \attention this function should be called only via the macro QF_PUBLISH()
*/
#ifndef Q_SPY
void QF_publish_(QEvt const * const e)
#else
void QF_publish_(QEvt const * const e, void const * const sender)
#endif
{
    QF_CRIT_STAT_

    /** \pre the published signal must be within the configured range */
    Q_REQUIRE_ID(100, e->sig < (QSignal)QF_maxSignal_);

    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_PUBLISH, (void *)0, (void *)0)
        QS_TIME_();          /* the timestamp */
        QS_OBJ_(sender);     /* the sender object */
        QS_SIG_(e->sig);     /* the signal of the event */
        QS_2U8_(e->poolId_, e->refCtr_);/* pool Id & ref Count of the event */
    QS_END_NOCRIT_()

    /* is it a dynamic event? */
    if (e->poolId_ != (uint8_t)0) {
        QF_EVT_REF_CTR_INC_(e); /* increment reference counter, NOTE01 */
    }
    QF_CRIT_EXIT_();

#if (QF_MAX_ACTIVE <= 8)
    {
        uint8_t tmp = QF_subscrList_[e->sig].bits[0];
        while (tmp != (uint8_t)0) {
            uint8_t p = QF_LOG2(tmp);
            tmp &= Q_ROM_BYTE(QF_invPwr2Lkup[p]); /* clear subscriber bit */
            /* the priority of the AO must be registered with the framework */
            Q_ASSERT_ID(110, QF_active_[p] != (QActive *)0);

            /* QACTIVE_POST() asserts internally if the queue overflows */
            QACTIVE_POST(QF_active_[p], e, sender);
        }
    }
#else
    {
        uint_fast8_t i = (uint_fast8_t)Q_DIM(QF_subscrList_[0].bits);
        /* go through all bytes in the subscription list */
        do {
            uint_fast8_t tmp;
            --i;
            tmp = (uint_fast8_t)QF_PTR_AT_(QF_subscrList_, e->sig).bits[i];
            while (tmp != (uint_fast8_t)0) {
                uint_fast8_t p = (uint_fast8_t)QF_LOG2(tmp);

                /* clear the subscriber bit */
                tmp &= (uint_fast8_t)Q_ROM_BYTE(QF_invPwr2Lkup[p]);
                p = (uint_fast8_t)p + (uint_fast8_t)(i << 3);/*adj priority */

                /* the priority level be registered with the framework */
                Q_ASSERT_ID(120, QF_active_[p] != (QActive *)0);

                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(QF_active_[p], e, sender);
            }
        } while (i != (uint_fast8_t)0);
    }
#endif

    /* run the garbage collector */
    QF_gc(e);

    /* NOTE: QF_publish_() increments the reference counter to prevent
    * premature recycling of the event while the multicasting is still
    * in progress. At the end of the function, the garbage collector step
    * decrements the reference counter and recycles the event if the
    * counter drops to zero. This covers the case when the event was
    * published without any subscribers.
    */
}
