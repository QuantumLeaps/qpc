/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief ::QEQueue implementation (QP native thread-safe queue)
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_qeq")

/*..........................................................................*/
void QEQueue_init(QEQueue * const me, QEvt const * * const qSto,
                  uint_fast16_t const qLen)
{
    me->frontEvt = (QEvt *)0; /* no events in the queue */
    me->ring     = qSto;      /* the beginning of the ring buffer */
    me->end      = (QEQueueCtr)qLen;
    if (qLen != 0U) {
        me->head = 0U;
        me->tail = 0U;
    }
    me->nFree    = (QEQueueCtr)(qLen + 1U); /* +1 for frontEvt */
    me->nMin     = me->nFree;
}

/*..........................................................................*/
bool QEQueue_post(QEQueue * const me, QEvt const * const e,
                  uint_fast16_t const margin, uint_fast8_t const qs_id)
{
    /* @pre event must be valid */
    Q_REQUIRE_ID(200, e != (QEvt *)0);

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEQueueCtr nFree  = me->nFree; /* get volatile into the temporary */

    /* required margin available? */
    bool status;
    if (((margin == QF_NO_MARGIN) && (nFree > 0U))
        || (nFree > (QEQueueCtr)margin))
    {
        /* is it a dynamic event? */
        if (e->poolId_ != 0U) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        --nFree; /* one free entry just used up */
        me->nFree = nFree; /* update the volatile */
        if (me->nMin > nFree) {
            me->nMin = nFree; /* update minimum so far */
        }

        QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST, qs_id)
            QS_TIME_PRE_();          /* timestamp */
            QS_SIG_PRE_(e->sig);     /* the signal of this event */
            QS_OBJ_PRE_(me);         /* this queue object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);      /* number of free entries */
            QS_EQC_PRE_(me->nMin);   /* min number of free entries */
        QS_END_NOCRIT_PRE_()

        /* was the queue empty? */
        if (me->frontEvt == (QEvt *)0) {
            me->frontEvt = e; /* deliver event directly */
        }
        /* queue was not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO)... */
            QF_PTR_AT_(me->ring, me->head) = e; /* insert e into buffer */
            /* need to wrap the head? */
            if (me->head == 0U) {
                me->head = me->end; /* wrap around */
            }
            --me->head;
        }
        status = true; /* event posted successfully */
    }
    else {
        /*! @note assert if event cannot be posted and dropping events is
        * not acceptable
        */
        Q_ASSERT_CRIT_(210, margin != QF_NO_MARGIN);

        QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST_ATTEMPT, qs_id)
            QS_TIME_PRE_();          /* timestamp */
            QS_SIG_PRE_(e->sig);     /* the signal of this event */
            QS_OBJ_PRE_(me);         /* this queue object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);      /* number of free entries */
            QS_EQC_PRE_(margin);     /* margin requested */
        QS_END_NOCRIT_PRE_()

        status = false;
    }
    QF_CRIT_X_();

    return status;
}

/*..........................................................................*/
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e,
                      uint_fast8_t const qs_id)
{
    (void)qs_id; /* unused parameter, if Q_SPY not defined */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEQueueCtr nFree = me->nFree; /* get volatile into the temporary */

    /*! @pre the queue must be able to accept the event (cannot overflow) */
    Q_REQUIRE_CRIT_(300, nFree != 0U);

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        QF_EVT_REF_CTR_INC_(e);  /* increment the reference counter */
    }

    --nFree;  /* one free entry just used up */
    me->nFree = nFree; /* update the volatile */
    if (me->nMin > nFree) {
        me->nMin = nFree; /* update minimum so far */
    }

    QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST_LIFO, qs_id)
        QS_TIME_PRE_();         /* timestamp */
        QS_SIG_PRE_(e->sig);    /* the signal of this event */
        QS_OBJ_PRE_(me);        /* this queue object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count of event */
        QS_EQC_PRE_(nFree);     /* number of free entries */
        QS_EQC_PRE_(me->nMin);  /* min number of free entries */
    QS_END_NOCRIT_PRE_()

    QEvt const * const frontEvt = me->frontEvt; /* read volatile into temp */
    me->frontEvt = e; /* deliver event directly to the front of the queue */

    /* was the queue not empty? */
    if (frontEvt != (QEvt *)0) {
        ++me->tail;
        if (me->tail == me->end) {     /* need to wrap the tail? */
            me->tail = 0U;  /* wrap around */
        }
        QF_PTR_AT_(me->ring, me->tail) = frontEvt; /* save old front evt */
    }
    QF_CRIT_X_();
}

/*..........................................................................*/
QEvt const *QEQueue_get(QEQueue * const me, uint_fast8_t const qs_id) {
    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEvt const * const e = me->frontEvt; /* remove event from the front */

    /* was the queue not empty? */
    if (e != (QEvt *)0) {
        /* use a temporary variable to increment volatile me->nFree */
        QEQueueCtr const nFree = me->nFree + 1U;
        me->nFree = nFree; /* update the number of free */

        /* any events in the ring buffer? */
        if (nFree <= me->end) {
            me->frontEvt = QF_PTR_AT_(me->ring, me->tail); /* get from tail */
            if (me->tail == 0U) { /* need to wrap the tail? */
                me->tail = me->end; /* wrap around */
            }
            --me->tail;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_GET, qs_id)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(me);     /* this queue object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count */
                QS_EQC_PRE_(nFree);  /* number of free entries */
            QS_END_NOCRIT_PRE_()
        }
        else {
            me->frontEvt = (QEvt *)0; /* queue becomes empty */

            /* all entries in the queue must be free (+1 for fronEvt) */
            Q_ASSERT_CRIT_(410, nFree == (me->end + 1U));

            QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_GET_LAST, qs_id)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(me);     /* this queue object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()
        }
    }
    QF_CRIT_X_();
    return e;
}
