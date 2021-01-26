/**
* @file
* @brief QF/C port to embOS
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.9.2a
* Last updated on  2021-01-26
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

/*..........................................................................*/
/* define __TARGET_FPU_VFP symbol depending on the compiler...  */
#if defined (__CC_ARM)          /* ARM Compiler */
    /* in ARM Compiler __TARGET_FPU_VFP is a pre-defined symbol*/
#elif defined (__ICCARM__)      /* IAR Compiler */
    #if defined __ARMVFP__
        #define __TARGET_FPU_VFP 1
    #endif
#elif defined (__GNUC__)        /* GNU Compiler */
    #if defined (__VFP_FP__) && !defined(__SOFTFP__)
        #define __TARGET_FPU_VFP 1
    #endif
#endif

/*..........................................................................*/
void QF_init(void) {
    OS_InitKern();  /* initialize embOS */
    OS_InitHW();    /* initialize the hardware used by embOS */
}
/*..........................................................................*/
int_t QF_run(void) {
    QS_CRIT_STAT_

    QF_onStartup();  /* QF callback to configure and start interrupts */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    OS_Start();      /* start embOS multitasking */
    Q_ERROR_ID(100); /* OS_Start() should never return */
    return 0; /* dummy return to make the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();  /* cleanup callback */
}

/*..........................................................................*/
static void thread_function(void *pVoid) { /* embOS signature */
    QActive *act = (QActive *)pVoid;

#ifdef __TARGET_FPU_VFP
    /* does the task use the FPU? see NOTE1 */
    if ((act->osObject & QF_TASK_USES_FPU) != 0U) {
        OS_ExtendTaskContext_VFP();
    }
#endif  /* __TARGET_FPU_VFP */

    /* event-loop */
    for (;;) {  /* for-ever */
        QEvt const *e = QActive_get_(act);
        QHSM_DISPATCH(&act->super, e, act->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    char task_name[4]; /* task name to be passed to OS_CreateTaskEx() */

    /* create the embOS message box for the AO */
    OS_CreateMB(&me->eQueue,
                (OS_U16)sizeof(QEvt *),
                (OS_UINT)qLen,
                (void *)&qSto[0]);

    me->prio = prio;  /* save the QF priority */
    QF_add_(me);      /* make QF aware of this active object */
    QHSM_INIT(&me->super, par, me->prio); /* the top-most initial tran. */
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* prepare the unique task name of the form "Axx",
    * where xx is a 2-digit QP priority of the Active Object
    */
    task_name[0] = 'A';
    task_name[1] = '0' + (prio / 10U);
    task_name[2] = '0' + (prio % 10U);
    task_name[3] = '\0'; /* zero-terminate */

    /* create an embOS task for the AO */
    OS_CreateTaskEx(&me->thread,
                    task_name,     /* the unique task name */
                    (OS_PRIO)prio, /* embOS uses the same numbering as QP */
                    &thread_function,
                    (void OS_STACKPTR *)stkSto,
                    (OS_UINT)stkSize,
                    (OS_UINT)0,    /* no AOs at the same prio */
                    (void *)me);
}
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    (void)attr2; /* unused parameter */
    me->osObject = attr1;
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

    QF_CRIT_E_();
    nFree = (uint_fast16_t)(me->eQueue.maxMsg - me->eQueue.nofMsg);

    if (margin == QF_NO_MARGIN) {
        if (nFree > (QEQueueCtr)0) {
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
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* # free entries available */
            QS_EQC_PRE_(0U);     /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        QF_CRIT_X_();

        /* posting to the embOS mailbox must succeed, see NOTE3 */
        Q_ALLEGE_ID(520,
            OS_PutMailCond(&me->eQueue, (OS_CONST_PTR void *)&e)
            == (char)0);
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* # free entries available */
            QS_EQC_PRE_(margin); /* margin requested */
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
        QS_TIME_PRE_();          /* timestamp */
        QS_SIG_PRE_(e->sig);     /* the signal of this event */
        QS_OBJ_PRE_(me);         /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(me->eQueue.maxMsg - me->eQueue.nofMsg); /* # free */
        QS_EQC_PRE_(0U);         /* min # free entries (unknown) */
    QS_END_NOCRIT_PRE_()

    if (e->poolId_ != 0U) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    QF_CRIT_X_();

    /* posting to the embOS mailbox must succeed, see NOTE3 */
    Q_ALLEGE_ID(810,
        OS_PutMailFrontCond(&me->eQueue, (OS_CONST_PTR void *)&e)
        == (char)0);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QS_CRIT_STAT_

    OS_GetMail(&me->eQueue, (void *)&e);

    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();          /* timestamp */
        QS_SIG_PRE_(e->sig);     /* the signal of this event */
        QS_OBJ_PRE_(me);         /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_(me->eQueue.maxMsg - me->eQueue.nofMsg);/* # free */
    QS_END_PRE_()

    return e;
}

/*****************************************************************************
* NOTE1:
* In case of hardware-supported floating point unit (FPU), a task must
* preserve the FPU registers accross the context switch. However, this
* additional overhead is necessary only for tasks that actually use the
* FPU. In this QP-embOS port, an active object task that uses the FPU is
* designated by the QF_TASK_USES_FPU attribute, which can be set wiht the
* QF_setEmbOsTaskAttr() function. The task attributes must be set *before*
* calling QACTIVE_START(). The task attributes are saved in QActive.osObject
* member.
*
* NOTE3:
* The event posting to embOS mailbox occurs inside a critical section,
* but this is OK, because the QF/embOS critical sections are designed
* to nest.
*/
