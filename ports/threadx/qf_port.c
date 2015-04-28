/**
* @file
* @brief QF/C, port to ThreadX
* @ingroup ports
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")

/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();
    return (int_t)0; /* return success */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
static void thread_function(ULONG thread_input) { /* ThreadX signature */
    QActive *act = (QActive *)thread_input;
    act->osObject = (uint8_t)1;/* enable thread-loop */
    while (act->osObject) {
        QEvt const *e = QActive_get_(act);
        QMSM_DISPATCH(&act->super, e);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }

    QF_remove_(act); /* remove this object from QF */
    Q_ALLEGE(tx_queue_delete(&act->eQueue) == TX_SUCCESS); /* cleanup queue */
    Q_ALLEGE(tx_thread_delete(&act->thread) == TX_SUCCESS);/*cleanup thread */
}
/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    UINT tx_prio; /* ThreadX priority corresponding to the QF priority prio */

    /* allege that the ThreadX queue is created successfully */
    Q_ALLEGE(tx_queue_create(&me->eQueue,
                 "Q",
                 TX_1_ULONG,
                 (VOID *)qSto,
                 (ULONG)(qLen * sizeof(ULONG)))
             == TX_SUCCESS);

    me->prio = prio;  /* save the QF priority */
    QF_add_(me);      /* make QF aware of this active object */
    QMSM_INIT(&me->super, ie); /* take the top-most initial tran. */

    QS_FLUSH(); /* flush the trace buffer to the host */

    /* convert QF priority to the ThreadX priority */
    tx_prio = QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - prio;

    Q_ALLEGE(tx_thread_create(&me->thread, /* ThreadX thread control block */
                 "AO",                     /* thread name */
                 &thread_function,         /* thread function */
                 (ULONG)me,                /* thread input */
                 stkSto,                   /* stack start */
                 stkSize,                  /* stack size in bytes */
                 tx_prio,                  /* ThreadX priority */
                 tx_prio,/*preemption threshold disabled (same as priority) */
                 TX_NO_TIME_SLICE,
                 TX_AUTO_START)
             == TX_SUCCESS);
}
/*..........................................................................*/
void QActive_stop(QActive * const me) {
    me->osObject = (uint8_t)0; /* stop the thread loop */
}
/*..........................................................................*/
#ifndef Q_SPY
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin)
#else
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
#endif /* Q_SPY */
{
    uint_fast16_t nFree;
    bool status;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    nFree = (uint_fast16_t)me->eQueue.tx_queue_available_storage;

    if (nFree > margin) {
        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_priv_.aoObjFilter, me)
            QS_TIME_();             /* timestamp */
            QS_OBJ_(sender);        /* the sender object */
            QS_SIG_(e->sig);        /* the signal of the event */
            QS_OBJ_(me);            /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_((QEQueueCtr)nFree); /* # free entries available */
            QS_EQC_((QEQueueCtr)0); /* min # free entries (unknown) */
        QS_END_NOCRIT_()

        if (e->poolId_ != (uint8_t)0) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }
        /* posting to the ThreadX message queue must succeed, see NOTE1 */
        Q_ALLEGE(tx_queue_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
                 == TX_SUCCESS);

        status = true; /* return success */
    }
    else {
        /* can tolerate dropping evts? */
        Q_ASSERT(margin != (uint_fast16_t)0);

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_ATTEMPT, QS_priv_.aoObjFilter, me)
            QS_TIME_();             /* timestamp */
            QS_OBJ_(sender);        /* the sender object */
            QS_SIG_(e->sig);        /* the signal of the event */
            QS_OBJ_(me);            /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_((QEQueueCtr)nFree); /* # free entries available */
            QS_EQC_((QEQueueCtr)0); /* min # free entries (unknown) */
        QS_END_NOCRIT_()

        status = false; /* return failure */
    }
    QF_CRIT_EXIT_();

    return status;
}
/*..........................................................................*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_priv_.aoObjFilter, me)
        QS_TIME_();             /* timestamp */
        QS_SIG_(e->sig);        /* the signal of this event */
        QS_OBJ_(me);            /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        /* # free entries */
        QS_EQC_((QEQueueCtr)me->eQueue.tx_queue_available_storage);
        QS_EQC_((QEQueueCtr)0); /* min # free entries (unknown) */
    QS_END_NOCRIT_()

    if (e->poolId_ != (uint8_t)0) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    /* LIFO posting must succeed, see NOTE1 */
    Q_ALLEGE(tx_queue_front_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
             == TX_SUCCESS);

    QF_CRIT_EXIT_();
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QS_CRIT_STAT_

    Q_ALLEGE(tx_queue_receive(&me->eQueue, (VOID *)&e, TX_WAIT_FOREVER)
             == TX_SUCCESS);

    QS_BEGIN_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, me)
        QS_TIME_();             /* timestamp */
        QS_SIG_(e->sig);        /* the signal of this event */
        QS_OBJ_(me);            /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        /* # free entries */
        QS_EQC_((QEQueueCtr)me->eQueue.tx_queue_available_storage);
    QS_END_()

    return e;
}

/*****************************************************************************
* NOTE1:
* The event posting to ThreadX message queue occurs inside a critical section,
* but this is OK, because the QF/ThreadX critical sections are designed
* to nest.
*/

