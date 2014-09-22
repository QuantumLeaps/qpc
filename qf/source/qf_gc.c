/**
* \file
* \brief QF_gc() definition.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.1
* Last updated on  2014-09-19
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

Q_DEFINE_THIS_MODULE("qf_gc")

/****************************************************************************/
/**
* \description
* This function implements a simple garbage collector for the dynamic events.
* Only dynamic events are candidates for recycling. (A dynamic event is one
* that is allocated from an event pool, which is determined as non-zero
* e->poolId_ attribute.) Next, the function decrements the reference counter
* of the event (e->refCtr_), and recycles the event only if the counter drops
* to zero (meaning that no more references are outstanding for this event).
* The dynamic event is recycled by returning it to the pool from which
* it was originally allocated.
*
* \arguments
* \arg[in]  \c e  pointer to the event to recycle
*
* \note QF invokes the garbage collector at all appropriate contexts, when
* an event can become garbage (automatic garbage collection), so the
* application code should have no need to call QF_gc() directly. The QF_gc()
* function is exposed only for special cases when your application sends
* dynamic events to the "raw" thread-safe queues (see ::QEQueue). Such
* queues are processed outside of QF and the automatic garbage collection
* is **NOT** performed for these events. In this case you need to call
* QF_gc() explicitly.
*/
void QF_gc(QEvt const * const e) {

    /* is it a dynamic event? */
    if (e->poolId_ != (uint8_t)0) {
        QF_CRIT_STAT_
        QF_CRIT_ENTRY_();

        /* isn't this the last ref? */
        if (e->refCtr_ > (uint8_t)1) {
            QF_EVT_REF_CTR_DEC_(e); /* decrements the ref counter */

            QS_BEGIN_NOCRIT_(QS_QF_GC_ATTEMPT, (void *)0, (void *)0)
                QS_TIME_();         /* timestamp */
                QS_SIG_(e->sig);    /* the signal of the event */
                QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_()

            QF_CRIT_EXIT_();
        }
        /* this is the last reference to this event, recycle it */
        else {
            uint_fast8_t idx = (uint_fast8_t)e->poolId_ - (uint_fast8_t)1;

            QS_BEGIN_NOCRIT_(QS_QF_GC, (void *)0, (void *)0)
                QS_TIME_();         /* timestamp */
                QS_SIG_(e->sig);    /* the signal of the event */
                QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_()

            QF_CRIT_EXIT_();

            /* pool ID must be in range */
            Q_ASSERT_ID(110, idx < QF_maxPool_);

            /* casting const away is legitimate, because it's a pool event */
            QF_EPOOL_PUT_(QF_pool_[idx], (QEvt *)e);
        }
    }
}
