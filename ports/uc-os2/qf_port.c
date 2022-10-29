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
* @date Last updated on: 2022-08-29
* @version Last updated for: @ref qpc_7_1_0
*
* @file
* @ingroup ports
* @brief QF/C port to uC-OS2, generic C99 compiler
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

/* Local objects -----------------------------------------------------------*/
static void task_function(void *pdata); /* uC-OS2 task signature */

/*..........................................................................*/
void QF_init(void) {
    OSInit();  /* initialize uC-OS2 */
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();  /* QF callback to configure and start interrupts */

    /* produce the QS_QF_RUN trace record */
    QS_CRIT_STAT_
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    OSStart();       /* start uC-OS2 multitasking */
    Q_ERROR_ID(100); /* OSStart() should never return */
    return 0; /* this unreachable return keeps the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();  /* cleanup callback */
}

/*..........................................................................*/
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
     /* task name to be passed to OSTaskCreateExt() */
    void * const task_name = (void *)me->eQueue;

    me->eQueue = OSQCreate((void **)qSto, qLen);  /* create uC-OS2 queue */
    /* the uC-OS2 queue must be created correctly */
    Q_ASSERT_ID(210, me->eQueue != (OS_EVENT *)0);

    me->prio  = (uint8_t)(prioSpec & 0xFFU); /* QF-priority of the AO */
    me->pthre = (uint8_t)(prioSpec >> 8U);   /* preemption-threshold */
    QActive_register_(me); /* register this AO */

    QHSM_INIT(&me->super, par, me->prio); /* initial tran. (virtual) */
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* map from QP to uC/OS priority */
    INT8U const p_ucos = (INT8U)(QF_MAX_ACTIVE - me->prio);

    /* create AO's task... */
    /*
    * NOTE: The call to uC-OS2 API OSTaskCreateExt() assumes that the
    * pointer to the top-of-stack (ptos) is at the end of the provided
    * stack memory. This is correct only for CPUs with downward-growing
    * stack, but must be changed for CPUs with upward-growing stack
    */
    INT8U const err = OSTaskCreateExt(&task_function, /* the task function */
             (void *)me,     /* the 'pdata' parameter */
#if OS_STK_GROWTH
             &((OS_STK *)stkSto)[(stkSize/sizeof(OS_STK)) - 1], /* ptos */
#else
             (OS_STK *)stkSto, /* ptos */
#endif
             p_ucos,           /* uC-OS2 task priority */
             (INT16U)me->prio, /* the unique AO priority as task ID */
#if OS_STK_GROWTH
             (OS_STK *)stkSto, /* pbos */
#else
             &((OS_STK *)stkSto)[(stkSize/sizeof(OS_STK)) - 1], /* pbos */
#endif
             (INT32U)(stkSize/sizeof(OS_STK)),/* stack size in OS_STK units */
             task_name,        /* pext */
             (INT16U)me->thread); /* task options, see NOTE1 */

    /* uC-OS2 task must be created correctly */
    Q_ENSURE_ID(220, err == OS_ERR_NONE);
}
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    switch (attr1) {
        case TASK_NAME_ATTR:
           /* this function must be called before QACTIVE_START(),
           * which implies that me->eQueue must not be used yet;
           */
           Q_ASSERT_ID(300, me->eQueue == (OS_EVENT *)0);
           /* temporarily store the name, cast 'const' away */
            me->eQueue = (OS_EVENT *)attr2;
            break;
        /* ... */
        default:
            me->thread = attr1;
            break;
    }
}

/*..........................................................................*/
static void task_function(void *pdata) { /* uC-OS2 task signature */
    /* event-loop */
    for (;;) { /* for-ever */
        QEvt const *e = QActive_get_((QActive *)pdata);
        /* dispatch to the AO's SM */
        QHSM_DISPATCH((QHsm *)pdata, e, ((QActive *)pdata)->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT_
    QF_CRIT_E_();

    uint_fast16_t const nFree =
        (uint_fast16_t)(((OS_Q_DATA *)me->eQueue)->OSQSize
         - ((OS_Q_DATA *)me->eQueue)->OSNMsgs);
    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post */
            Q_ERROR_ID(710); /* must be able to post the event */
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
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(sender);    /* the sender object */
            QS_SIG_PRE_(e->sig);    /* the signal of the event */
            QS_OBJ_PRE_(me);        /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);     /* # free entries */
            QS_EQC_PRE_(0U);        /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        QF_CRIT_X_();

        /* posting the event to uC-OS2 message queue must succeed */
        Q_ALLEGE_ID(720,
            OSQPost(me->eQueue, (void *)e) == OS_ERR_NONE);
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(sender);    /* the sender object */
            QS_SIG_PRE_(e->sig);    /* the signal of the event */
            QS_OBJ_PRE_(me);        /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);     /* # free entries available */
            QS_EQC_PRE_(margin);    /* margin requested */
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
        QS_TIME_PRE_();             /* timestamp */
        QS_SIG_PRE_(e->sig);        /* the signal of this event */
        QS_OBJ_PRE_(me);            /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(((OS_Q *)me->eQueue)->OSQSize
                     - ((OS_Q *)me->eQueue)->OSQEntries); /* # free entries */
        QS_EQC_PRE_(0U);            /* min # free entries (unknown) */
    QS_END_NOCRIT_PRE_()

    if (e->poolId_ != 0U) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    QF_CRIT_X_();

    /* posting the event to uC/OS message queue must succeed */
    Q_ALLEGE_ID(810,
        OSQPostFront((OS_EVENT *)me->eQueue, (void *)e) == OS_ERR_NONE);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    INT8U err;
    QEvt const *e = (QEvt *)OSQPend((OS_EVENT *)me->eQueue, 0U, &err);

    Q_ASSERT_ID(910, err == OS_ERR_NONE);

    QS_CRIT_STAT_
    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();             /* timestamp */
        QS_SIG_PRE_(e->sig);        /* the signal of this event */
        QS_OBJ_PRE_(me);            /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(((OS_Q *)me->eQueue)->OSQSize
                    - ((OS_Q *)me->eQueue)->OSQEntries); /* # free entries */
    QS_END_PRE_()

    return e;
}

/*****************************************************************************
* NOTE0:
* The QF_onStartup() should enter the critical section before configuring
* and starting interrupts and it should NOT exit the critical section.
* Thus the interrupts cannot fire until uC-OS2 starts multitasking
* in OSStart(). This is to prevent a (narrow) time window in which interrupts
* could make some tasks ready to run, but the OS would not be ready yet
* to perform context switch.
*
* NOTE1:
* The member QActive.thread is set to the uC-OS2 task options in the
* function QF_setUCosTaskAttr(), which must be called **before**
* QACTIVE_START().
*/
