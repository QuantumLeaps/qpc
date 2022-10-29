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
* @date Last updated on: 2022-08-25
* @version Last updated for: Zephyr 3.1.99 and @ref qpc_7_1_0
*
* @file
* @brief QF/C port to Zephyr RTOS (v 3.1.99)
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
struct k_spinlock QF_spinlock;

/*..........................................................................*/
void QF_init(void) {
    QF_spinlock = (struct k_spinlock){};
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();
#ifdef Q_SPY

#if CONFIG_NUM_PREEMPT_PRIORITIES > 0
    /* lower the priority of the main thread to the level of idle thread */
    k_thread_priority_set(k_current_get(),
                          CONFIG_NUM_PREEMPT_PRIORITIES - 1);
#endif

    /* perform QS work... */
    while (true) {
        QS_rxParse();   /* parse any QS-RX bytes */
        QS_doOutput();  /* perform the QS-TX output */
    }
#else
    return 0; /* return from the main Zephyr thread */
#endif
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();  /* cleanup callback */
}

/*..........................................................................*/
static void thread_entry(void *p1, void *p2, void *p3) {
    QActive *act = (QActive *)p1;
    (void)p2; /* unused parameter */
    (void)p3; /* unused parameter */

    /* event-loop */
    for (;;) {  /* for-ever */
        QEvt const *e = QActive_get_(act);
        QHSM_DISPATCH(&act->super, e, act->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
/*
* In the Zephyr port the generic function QActive_setAttr() is used to
* set the options for the Zephyr thread (attr1) and thread name (attr2).
* QActive_setAttr() needs to be called *before* QACTIVE_START() for the
* given active object.
*
* In this Zephyr port the attributes will be used as follows (see also
* Active_start_()):
* - attr1 - will be used for thread options in k_thread_create()
* - attr2 - will be used for thread name in k_thread_name_set()
*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    me->thread.base.order_key = attr1; /* will be used for thread options */
    me->thread.init_data = (void *)attr2; /* will be used for thread name */
}
/*..........................................................................*/
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    me->prio  = (uint8_t)(prioSpec & 0xFFU); /* QF-priority of the AO */
    me->pthre = (uint8_t)(prioSpec >> 8U);   /* preemption-threshold */
    QActive_register_(me); /* register this AO */

    /* initialize the Zephyr message queue */
    k_msgq_init(&me->eQueue, (char *)qSto, sizeof(QEvt *), (uint32_t)qLen);

    QHSM_INIT(&me->super, par, me->prio); /* the top-most initial tran. */
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* Zephyr uses the reverse priority numbering than QP */
    int zprio = (int)QF_MAX_ACTIVE - (int)me->prio;

    /* extract data temporarily saved in me->thread by QActive_setAttr() */
    uint32_t opt = me->thread.base.order_key;
#ifdef CONFIG_THREAD_NAME
    char const *name = (char const *)me->thread.init_data;
#endif

    /* clear the Zephyr thread structure before creating the thread */
    me->thread = (struct k_thread){};

    /* create a Zephyr thread for the AO... */
    k_thread_create(&me->thread,
                    (k_thread_stack_t *)stkSto,
                    (size_t)stkSize,
                    &thread_entry,
                    (void *)me, /* p1 */
                    (void *)0,  /* p2 */
                    (void *)0,  /* p3 */
                    zprio,      /* Zephyr priority */
                    opt,        /* thread options */
                    K_NO_WAIT); /* start immediately */

#ifdef CONFIG_THREAD_NAME
    /* set the Zephyr thread name, if initialized, or the default name "AO" */
    k_thread_name_set(&me->thread, (name != (char *)0) ? name : "AO");
#endif
}
/*..........................................................................*/
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT_
    QF_CRIT_E_();
    uint_fast16_t nFree = (uint_fast16_t)k_msgq_num_free_get(&me->eQueue);

    bool status;
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

        /* posting to the Zephyr message queue must succeed, see NOTE1 */
        Q_ALLEGE_ID(520,
                    k_msgq_put(&me->eQueue, (void const *)&e, K_NO_WAIT)
                     == 0);
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
        QS_EQC_PRE_(k_msgq_num_free_get(&me->eQueue)); /* # free */
        QS_EQC_PRE_(0U);      /* min # free entries (unknown) */
    QS_END_NOCRIT_PRE_()

    if (e->poolId_ != 0U) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    QF_CRIT_X_();

    /* NOTE: Zephyr message queue does not currently support LIFO posting
    * so normal FIFO posting is used instead.
    */
    Q_ALLEGE_ID(610, k_msgq_put(&me->eQueue, (void *)&e, K_NO_WAIT) == 0);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QS_CRIT_STAT_

    /* wait for an event (forever), which must succeed */
    Q_ALLEGE_ID(710, k_msgq_get(&me->eQueue, (void *)&e, K_FOREVER) == 0);

    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();       /* timestamp */
        QS_SIG_PRE_(e->sig);  /* the signal of this event */
        QS_OBJ_PRE_(me);      /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(k_msgq_num_free_get(&me->eQueue));/* # free */
    QS_END_PRE_()

    return e;
}
