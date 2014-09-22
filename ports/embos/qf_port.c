/*****************************************************************************
* Product: QF/C port to embOS
* Last updated for version 5.3.0
* Last updated on  2014-06-25
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
*****************************************************************************/
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
    OS_InitKern();  /* initialize embOS */
    OS_InitHW();    /* initialize the hardware used by embOS */
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();
    OS_Start(); /* start multitasking; NOTE: QS_start() does not return */
    return (int_t)0; /* dummy return to make the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
static void thread_function(void *pVoid) { /* embOS signature */
    QActive *act = (QActive *)pVoid;

#if defined (__ARM7EM__) && (__CORE__ == __ARM7EM__) && defined (__ARMVFP__)
    /* does the task use the FPU? see NOTE1 */
    if ((act->osObject & QF_TASK_USES_FPU) != (uint8_t)0) {
        OS_ExtendTaskContext_VFP();
    }
#endif

    act->osObject = (uint8_t)1;/* enable thread-loop */
    while (act->osObject) {
        QEvt const *e = QActive_get_(act);
        QMSM_DISPATCH(&act->super, e);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }

    QF_remove_(act); /* remove this object from QF */
    OS_DeleteMB(&act->eQueue);
    OS_TerminateTask(&act->thread);
}
/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    /* create the embOS message box for the AO */
    OS_CreateMB(&me->eQueue,
                (OS_U16)sizeof(QEvt *),
                (OS_UINT)qLen,
                (void *)&qSto[0]);

    me->prio = prio;  /* save the QF priority */
    QF_add_(me);      /* make QF aware of this active object */
    QMSM_INIT(&me->super, ie); /* execute initial transition */

    QS_FLUSH(); /* flush the trace buffer to the host */


    OS_CreateTaskEx(&me->thread,
                    "AO",
                    (OS_PRIO)prio, /* embOS uses the same numbering as QP */
                    &thread_function,
                    (void OS_STACKPTR *)stkSto,
                    (OS_UINT)stkSize,
                    (OS_UINT)0,    /* no AOs at the same prio */
                    (void *)me);
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
    nFree = (uint_fast16_t)(me->eQueue.maxMsg - me->eQueue.nofMsg);

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
        /* posting to the embOS mailbox must succeed, see NOTE2 */
        Q_ALLEGE(OS_PutMailCond(&me->eQueue, (OS_CONST_PTR void *)&e)
                 == (char)0);

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
        QS_EQC_((QEQueueCtr)(me->eQueue.maxMsg - me->eQueue.nofMsg));
        QS_EQC_((QEQueueCtr)0); /* min # free entries (unknown) */
    QS_END_NOCRIT_()

    if (e->poolId_ != (uint8_t)0) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    /* posting to the embOS mailbox must succeed, see NOTE2 */
    Q_ALLEGE(OS_PutMailFrontCond(&me->eQueue, (OS_CONST_PTR void *)&e)
             == (char)0);

    QF_CRIT_EXIT_();
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QS_CRIT_STAT_

    OS_GetMail(&me->eQueue, (void *)&e);

    QS_BEGIN_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, me)
        QS_TIME_();             /* timestamp */
        QS_SIG_(e->sig);        /* the signal of this event */
        QS_OBJ_(me);            /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        /* # free entries */
        QS_EQC_((QEQueueCtr)(me->eQueue.maxMsg - me->eQueue.nofMsg));
    QS_END_()

    return e;
}

/*****************************************************************************
* NOTE1:
* In case of hardware-supported floating point unit (FPU), a task must
* preserve the FPU registers accross the context switch. However, this
* additional overhead is necessary only for tasks that actually use the
* FPU. In this QP-embOS port, an active object task that uses the FPU is
* designated by initially setting the bit QF_TASK_USES_FPU in the
* QActive.osObject attribute. Later on the QActive.osObject attribute is
* reused as the event-loop flag to gracefully terminate the AOs event
* loop, in case QActive_stop() is called.
*
* NOTE2:
* The event posting to embOS mailbox occurs inside a critical section,
* but this is OK, because the QF/embOS critical sections are designed
* to nest.
*/

