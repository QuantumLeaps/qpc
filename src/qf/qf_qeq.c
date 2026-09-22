//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License, version 3 or later (GPL-3.0-or-later), or,
// alternatively, under the terms of one of the closed-source Quantum Leaps
// commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
//
// NOTE:
// The GPL does NOT permit distributing this code as part of proprietary
// (closed-source) programs. For such use, please contact Quantum Leaps for
// a commercial license, which is an alternative to the GPL designed
// explicitly for closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#define QP_IMPL             // this is QP implementation
#include "qp_port.h"        // QP port
#include "qp_pkg.h"         // QP package-scope interface
#include "qsafe.h"          // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY                // QS software tracing enabled?
    #include "qs_port.h"    // QS port
    #include "qs_pkg.h"     // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.h"   // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qf_qeq")

//............................................................................
//! @public @memberof QEQueue
bool QEQueue_post(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast16_t const margin,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    // the event to post must not be NULL
    Q_REQUIRE_INCRIT(100, e != (QEvt *)0);

    QEQueueCtr nFree = me->nFree; // get member into temporary

    bool const status = ((margin == QF_NO_MARGIN)
        || ((uint_fast16_t)nFree > margin));
    if (status) { // should try to post the event?

        // the queue must have a free slot
        Q_ASSERT_INCRIT(130, nFree != 0U);

#if (QF_MAX_EPOOL > 0U)
        if (e->poolNum_ != 0U) { // is it a mutable event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }
#endif // (QF_MAX_EPOOL > 0U)

        --nFree; // one free entry just used up
        me->nFree = nFree; // update the original
        if (me->nMin > nFree) { // is this the new minimum?
            me->nMin = nFree; // update minimum so far
        }

        QS_BEGIN_PRE(QS_QF_EQUEUE_POST, qsId)
            QS_TIME_PRE();        // timestamp
            QS_SIG_PRE(e->sig);   // the signal of the event
            QS_OBJ_PRE(me);       // this queue object
            QS_2U8_PRE(e->poolNum_, e->refCtr_);
            QS_EQC_PRE(nFree);    // # free entries
            QS_EQC_PRE(me->nMin); // min # free entries
        QS_END_PRE()

        if (me->frontEvt.e == (QEvt *)0) { // is the queue empty?
            me->frontEvt.e = e; // deliver event directly
        }
        else { // queue was not empty, insert event into the ring-buffer
            QEQueueCtr head = me->head; // get member into temporary
            me->ring[head].e = e; // insert e into buffer

            if (head == 0U) { // need to wrap the head?
                head = me->end;
            }
            --head; // advance head (counter-clockwise)
            me->head = head; // update the original
        }
    }
    else { // event cannot be posted
        QS_BEGIN_PRE(QS_QF_EQUEUE_POST_ATTEMPT, qsId)
            QS_TIME_PRE();       // timestamp
            QS_SIG_PRE(e->sig);  // the signal of this event
            QS_OBJ_PRE(me);      // this queue object
            QS_2U8_PRE(e->poolNum_, e->refCtr_);
            QS_EQC_PRE(nFree);   // # free entries
            QS_EQC_PRE(margin);  // margin requested
        QS_END_PRE()
    }

    QF_CRIT_EXIT();

    return status;
}

//............................................................................
//! @public @memberof QEQueue
void QEQueue_postLIFO(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    // event e to be posted must be valid
    Q_REQUIRE_INCRIT(200, e != (QEvt *)0);

    QEQueueCtr nFree = me->nFree; // get member into temporary

    // must be able to LIFO-post the event
    Q_REQUIRE_INCRIT(230, nFree != 0U);

#if (QF_MAX_EPOOL > 0U)
    if (e->poolNum_ != 0U) { // is it a mutable event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }
#endif // (QF_MAX_EPOOL > 0U)

    --nFree; // one free entry just used up
    me->nFree = nFree; // update the original

    if (me->nMin > nFree) { // is this the new minimum?
        me->nMin = nFree; // update minimum so far
    }

    QS_BEGIN_PRE(QS_QF_EQUEUE_POST_LIFO, qsId)
        QS_TIME_PRE();        // timestamp
        QS_SIG_PRE(e->sig);   // the signal of this event
        QS_OBJ_PRE(me);       // this queue object
        QS_2U8_PRE(e->poolNum_, e->refCtr_);
        QS_EQC_PRE(nFree);    // # free entries
        QS_EQC_PRE(me->nMin); // min # free entries
    QS_END_PRE()

    QEvt const * const frontEvt = me->frontEvt.e; // get member into temporary
    me->frontEvt.e = e; // deliver the event directly to the front

    if (frontEvt != (QEvt *)0) { // was the queue NOT empty?
        QEQueueCtr tail = me->tail; // get member into temporary
        ++tail;
        if (tail == me->end) { // need to wrap the tail?
            tail = 0U; // wrap around
        }
        me->tail = tail; // update the original
        me->ring[tail].e = frontEvt;
    }

    QF_CRIT_EXIT();
}

//............................................................................
//! @public @memberof QEQueue
struct QEvt const * QEQueue_get(QEQueue * const me,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    // always remove event from the front
    QEvt const * const e = me->frontEvt.e;

    if (e != (QEvt *)0) { // is the queue NOT empty?
        QEQueueCtr nFree = me->nFree; // get member into temporary

        ++nFree; // one more free event in the queue
        me->nFree = nFree; // update the # free

        if (nFree <= me->end) { // any events in the ring buffer?
            // remove event from the tail
            QEQueueCtr tail = me->tail; // get member into temporary

            QEvt const * const frontEvt = me->ring[tail].e;

            // the queue must have at least one event (at the front)
            Q_ASSERT_INCRIT(350, frontEvt != (QEvt *)0);

            QS_BEGIN_PRE(QS_QF_EQUEUE_GET, qsId)
                QS_TIME_PRE();      // timestamp
                QS_SIG_PRE(e->sig); // the signal of this event
                QS_OBJ_PRE(me);     // this queue object
                QS_2U8_PRE(e->poolNum_, e->refCtr_);
                QS_EQC_PRE(nFree);  // # free entries
            QS_END_PRE()

            me->frontEvt.e = frontEvt; // update the original

            if (tail == 0U) { // need to wrap the tail?
                tail = me->end; // wrap around
            }
            --tail; // advance the tail (counter-clockwise)
            me->tail = tail; // update the original
        }
        else {
            me->frontEvt.e = (QEvt *)0; // queue becomes empty

            // all entries in the queue must be free (+1 for frontEvt)
            Q_INVARIANT_INCRIT(370, nFree == (me->end + 1U));

            QS_BEGIN_PRE(QS_QF_EQUEUE_GET_LAST, qsId)
                QS_TIME_PRE();      // timestamp
                QS_SIG_PRE(e->sig); // the signal of this event
                QS_OBJ_PRE(me);     // this queue object
                QS_2U8_PRE(e->poolNum_, e->refCtr_);
            QS_END_PRE()
        }
    }

    QF_CRIT_EXIT();

    return e;
}
//............................................................................
//! @public @memberof QEQueue
void QEQueue_init(QEQueue * const me,
    QEvtPtr * const qSto,
    uint_fast16_t const qLen)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    // the qLen parameter must fit in the dynamic range of QEQueueCtr
    // (including the +1 for the front event)
#if (QF_EQUEUE_CTR_SIZE == 1U)
    Q_REQUIRE_INCRIT(400, qLen < 0xFFU);
#elif (QF_EQUEUE_CTR_SIZE == 2U)
    Q_REQUIRE_INCRIT(400, qLen < 0xFFFFU);
#endif

    // the queue storage must be provided, unless qLen is zero
    Q_REQUIRE_INCRIT(410, (qSto != (QEvtPtr *)0) || (qLen == 0U));

    me->frontEvt.e = (QEvt *)0; // no events in the queue
    me->ring = qSto;      // the beginning of the ring buffer
    me->end  = (QEQueueCtr)qLen; // index of the last element
    // NOTE: with qLen == 0 the head/tail members are NOT initialized,
    // because QTicker reuses them (see QTicker_ctor()/QTicker_init_())
    if (qLen > 0U) { // queue buffer storage provided?
        me->head = 0U; // head index: for removing events
        me->tail = 0U; // tail index: for inserting events
    }
    me->nFree    = (QEQueueCtr)(qLen + 1U); // +1 for frontEvt
    me->nMin     = me->nFree; // minimum so far

    QF_CRIT_EXIT();
}

//............................................................................
//! @public @memberof QEQueue
uint16_t QEQueue_getUse(QEQueue const * const me) {
    // NOTE: this function does NOT apply critical section, so it can
    // be safely called from an already established critical section.
    uint16_t nUse = 0U;
    if (me->frontEvt.e != (QEvt *)0) { // queue not empty?
        nUse = (uint16_t)((uint16_t)me->end + 1U - (uint16_t)me->nFree);
    }
    return nUse;
}
//............................................................................
//! @public @memberof QEQueue
uint16_t QEQueue_getFree(QEQueue const * const me) {
    // NOTE: this function does NOT apply critical section, so it can
    // be safely called from an already established critical section.
    return (uint16_t)me->nFree;
}
//............................................................................
//! @public @memberof QEQueue
uint16_t QEQueue_getMin(QEQueue const * const me) {
    // NOTE: this function does NOT apply critical section, so it can
    // be safely called from an already established critical section.
    return (uint16_t)me->nMin;
}
//............................................................................
//! @public @memberof QEQueue
bool QEQueue_isEmpty(QEQueue const * const me) {
    // NOTE: this function does NOT apply critical section, so it can
    // be safely called from an already established critical section.
    return me->frontEvt.e == (struct QEvt *)0;
}
