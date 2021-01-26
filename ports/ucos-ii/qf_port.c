/**
* @file
* @brief QF/C generic port to uC/OS-II
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.9.2a
* Last updated on  2020-01-26
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
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
static void task_function(void *pdata); /* uC/OS-II task signature */

/*..........................................................................*/
void QF_init(void) {
    OSInit();        /* initialize uC/OS-II */
}
/*..........................................................................*/
int_t QF_run(void) {
     QS_CRIT_STAT_

    QF_onStartup();  /* QF callback to configure and start interrupts */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    OSStart();       /* start uC/OS-II multitasking */
    Q_ERROR_ID(100); /* OSStart() should never return */
    return 0; /* this unreachable return keeps the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();  /* cleanup callback */
}
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    (void)attr2; /* unused parameter */
    me->thread = attr1;
}

/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    INT8U p_ucos;
    INT8U err;
    char_t task_name[4]; /* task name to be passed to OSTaskCreateExt() */

    me->eQueue = OSQCreate((void **)qSto, qLen);  /* create uC/OS-II queue */
    /* the uC/OS-II queue must be created correctly */
    Q_ASSERT_ID(210, me->eQueue != (OS_EVENT *)0);

    me->prio = prio; /* save the QF priority */
    QF_add_(me); /* make QF aware of this active object */

    QHSM_INIT(&me->super, par, me->prio); /* initial tran. (virtual) */
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* map from QP to uC/OS priority */
    p_ucos = (INT8U)(QF_MAX_ACTIVE - me->prio);

    /* prepare the unique task name of the form "Axx",
    * where xx is a 2-digit QP priority of the Active Object
    */
    task_name[0] = 'A';
    task_name[1] = '0' + (prio / 10U);
    task_name[2] = '0' + (prio % 10U);
    task_name[3] = '\0'; /* zero-terminate */

    /* create AO's task... */
    /*
    * NOTE: The call to uC/OS-II API OSTaskCreateExt() assumes that the
    * pointer to the top-of-stack (ptos) is at the end of the provided
    * stack memory. This is correct only for CPUs with downward-growing
    * stack, but must be changed for CPUs with upward-growing stack
    */
    err = OSTaskCreateExt(&task_function, /* the task function */
             (void *)me,     /* the 'pdata' parameter */
#if OS_STK_GROWTH
             &((OS_STK *)stkSto)[(stkSize/sizeof(OS_STK)) - 1], /* ptos */
#else
             (OS_STK *)stkSto, /* ptos */
#endif
             p_ucos,           /* uC/OS-II task priority */
             (INT16U)me->prio, /* the unique AO priority as task ID */
             (OS_STK *)stkSto, /* pbos */
             (INT32U)(stkSize/sizeof(OS_STK)),/* stack size in OS_STK units */
             task_name,        /* pext */
             (INT16U)me->thread); /* task options, see NOTE1 */

    /* uC/OS-II task must be created correctly */
    Q_ENSURE_ID(220, err == OS_ERR_NONE);
}

/*..........................................................................*/
static void task_function(void *pdata) { /* uC/OS-II task signature */
    /* event-loop */
    for (;;) { /* for-ever */
        QEvt const *e = QActive_get_((QActive *)pdata);
        /* dispatch to the AO's SM */
        QHSM_DISPATCH((QHsm *)pdata, e, ((QActive *)pdata)->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
#ifndef Q_SPY
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin)
#else
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
#endif
{
    bool status;
    uint_fast16_t nFree;
    QF_CRIT_STAT_

    QF_CRIT_E_();

    nFree = (uint_fast16_t)(((OS_Q_DATA *)me->eQueue)->OSQSize
                            - ((OS_Q_DATA *)me->eQueue)->OSNMsgs);
    if (margin == QF_NO_MARGIN) {
        if (nFree > (QEQueueCtr)0) {
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
            QS_U8_PRE_(e->poolId_); /* the pool Id of the event */
            QS_U8_PRE_(e->refCtr_); /* the ref count of the event */
            QS_EQC_PRE_(nFree);     /* # free entries */
            QS_EQC_PRE_(0U);        /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        QF_CRIT_X_();

        /* posting the event to uC/OS-II message queue must succeed */
        Q_ALLEGE_ID(720,
            OSQPost((OS_EVENT *)me->eQueue, (void *)e) == OS_ERR_NONE);
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
    QS_CRIT_STAT_
    QEvt const *e = (QEvt *)OSQPend((OS_EVENT *)me->eQueue, 0U, &err);

    Q_ASSERT_ID(910, err == OS_ERR_NONE);

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
* Thus the interrupts cannot fire until uC/OS-II starts multitasking
* in OSStart(). This is to prevent a (narrow) time window in which interrupts
* could make some tasks ready to run, but the OS would not be ready yet
* to perform context switch.
*
* NOTE1:
* The member QActive.thread is set to the uC/OS-II task options in the
* function QF_setUCosTaskAttr(), which must be called **before**
* QACTIVE_START().
*/
