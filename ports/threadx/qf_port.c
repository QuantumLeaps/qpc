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
* @date Last updated on: 2022-08-19
* @version Last updated for: @ref qpc_7_1_0
*
* @file
* @brief QF/C, port to ThreadX
* @ingroup ports
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS package-scope internal interface */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")

/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
int_t QF_run(void) {
    QS_CRIT_STAT_

    QF_onStartup();  /* QF callback to configure and start interrupts */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    return 0; /* return success */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
static void thread_function(ULONG thread_input) { /* ThreadX signature */
    QActive *act = (QActive *)thread_input;

    /* event-loop */
    for (;;) { /* for-ever */
        QEvt const *e = QActive_get_(act);
        QHSM_DISPATCH(&act->super, e, act->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    /* allege that the ThreadX queue is created successfully */
    Q_ALLEGE_ID(210,
        tx_queue_create(&me->eQueue,
            me->thread.tx_thread_name,
            TX_1_ULONG,
            (VOID *)qSto,
            (ULONG)(qLen * sizeof(ULONG)))
        == TX_SUCCESS);

    me->prio  = (uint8_t)(prioSpec & 0xFFU); /* QF-priority */
    me->pthre = (uint8_t)(prioSpec >> 8U); /* preemption-threshold */
    QActive_register_(me); /* register this AO */

    QHSM_INIT(&me->super, par, me->prio); /* initial tran. (virtual) */
    QS_FLUSH(); /* flush the trace buffer to the host */

    Q_ALLEGE_ID(220,
        tx_thread_create(
            &me->thread, /* ThreadX thread control block */
            me->thread.tx_thread_name, /* unique thread name */
            &thread_function, /* thread function */
            (ULONG)me, /* thread parameter */
            stkSto,    /* stack start */
            stkSize,   /* stack size in bytes */
            QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - me->prio, /* ThreadX prio */
            QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - me->pthre, /* preempt-thre */
            TX_NO_TIME_SLICE,
            TX_AUTO_START)
        == TX_SUCCESS);
}
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    /* this function must be called before QACTIVE_START(),
    * which implies that me->thread.tx_thread_name must not be used yet;
    */
    Q_REQUIRE_ID(300, me->thread.tx_thread_name == (char *)0);
    switch (attr1) {
        case THREAD_NAME_ATTR:
            /* temporarily store the name, cast 'const' away */
            me->thread.tx_thread_name = (char *)attr2;
            break;
        /* ... */
    }
}
/*..........................................................................*/
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    uint_fast16_t nFree;
    bool status;
    QF_CRIT_STAT_

    QF_CRIT_E_();
    nFree = (uint_fast16_t)me->eQueue.tx_queue_available_storage;

    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post */
            Q_ERROR_ID(510); /* must be able to post the event */
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true; /* can post */
    }
    else {
        status = false; /* cannot post */
    }

    if (status) { /* can post the event? */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();       /* timestamp */
            QS_OBJ_PRE_(sender);  /* the sender object */
            QS_SIG_PRE_(e->sig);  /* the signal of the event */
            QS_OBJ_PRE_(me);      /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);   /* # free entries available */
            QS_EQC_PRE_(0U);      /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        QF_CRIT_X_();

        /* posting to the ThreadX message queue must succeed, see NOTE1 */
        Q_ALLEGE_ID(520,
            tx_queue_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
            == TX_SUCCESS);
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();       /* timestamp */
            QS_OBJ_PRE_(sender);  /* the sender object */
            QS_SIG_PRE_(e->sig);  /* the signal of the event */
            QS_OBJ_PRE_(me);      /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);   /* # free entries available */
            QS_EQC_PRE_(0U);      /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        QF_CRIT_X_();
    }

    return status;
}
/*..........................................................................*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();       /* timestamp */
        QS_SIG_PRE_(e->sig);  /* the signal of this event */
        QS_OBJ_PRE_(me);      /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(me->eQueue.tx_queue_available_storage); /* # free */
        QS_EQC_PRE_(0U);      /* min # free entries (unknown) */
    QS_END_NOCRIT_PRE_()

    if (e->poolId_ != 0U) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    QF_CRIT_X_();

    /* LIFO posting must succeed, see NOTE1 */
    Q_ALLEGE_ID(610,
        tx_queue_front_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
        == TX_SUCCESS);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QS_CRIT_STAT_

    Q_ALLEGE_ID(710,
        tx_queue_receive(&me->eQueue, (VOID *)&e, TX_WAIT_FOREVER)
        == TX_SUCCESS);

    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();       /* timestamp */
        QS_SIG_PRE_(e->sig);  /* the signal of this event */
        QS_OBJ_PRE_(me);      /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(me->eQueue.tx_queue_available_storage);/* # free */
    QS_END_PRE_()

    return e;
}

/*..........................................................................*/
void QFSchedLock_(QFSchedLock * const lockStat, uint_fast8_t prio) {
    UINT tx_err;

    lockStat->lockHolder = tx_thread_identify();

    /*! @pre must be thread level, so current TX thread must be valid */
    Q_REQUIRE_ID(800, lockStat->lockHolder != (TX_THREAD *)0);

    /* change the preemption threshold of the current thread */
    tx_err = tx_thread_preemption_change(lockStat->lockHolder,
                     (QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - prio),
                     &lockStat->prevThre);

    if (tx_err == TX_SUCCESS) {
        QS_CRIT_STAT_
        lockStat->lockPrio = prio;

        QS_BEGIN_PRE_(QS_SCHED_LOCK, 0U)
            QS_TIME_PRE_(); /* timestamp */
            QS_2U8_PRE_((QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE
                         - lockStat->prevThre),
                        prio); /* new lock prio */
        QS_END_PRE_()
    }
    else if (tx_err == TX_THRESH_ERROR) {
        /* threshold was greater than (lower prio) than the current prio */
        lockStat->lockPrio = 0U; /* threshold not changed */
    }
    else {
        /* no other errors are tolerated */
        Q_ERROR_ID(810);
    }
}

/*..........................................................................*/
void QFSchedUnlock_(QFSchedLock const * const lockStat) {
    UINT old_thre;
    QS_CRIT_STAT_

    /*! @pre the lock holder must be valid and the scheduler must be locked */
    Q_REQUIRE_ID(900, (lockStat->lockHolder != (TX_THREAD *)0)
                      && (lockStat->lockPrio != 0U));

    QS_BEGIN_PRE_(QS_SCHED_LOCK, 0U)
        QS_TIME_PRE_(); /* timestamp */
        QS_2U8_PRE_(lockStat->lockPrio, /* prev lock prio */
                    (QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE
                          - lockStat->prevThre)); /* new lock prio */
    QS_END_PRE_()

    /* restore the preemption threshold of the lock holder */
    Q_ALLEGE_ID(910, tx_thread_preemption_change(lockStat->lockHolder,
                     lockStat->prevThre,
                     &old_thre) == TX_SUCCESS);
}

