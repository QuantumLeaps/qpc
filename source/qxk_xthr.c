/**
* @file
* @brief QXK preemptive kernel extended (blocking) thread functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.6.2
* Last updated on  2016-03-28
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef qxk_h
    #error "Source file included in a project NOT based on the QXK kernel"
#endif /* qxk_h */

Q_DEFINE_THIS_MODULE("qxk_xthr")

/****************************************************************************/
static void QXThread_init_(QMsm * const me, QEvt const * const e);
static void QXThread_dispatch_(QMsm * const me, QEvt const * const e);
static void QXThread_start_(QMActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie);
#ifndef Q_SPY
static bool QXThread_post_(QMActive * const me, QEvt const * const e,
                    uint_fast16_t const margin);
#else
static bool QXThread_post_(QMActive * const me, QEvt const * const e,
                    uint_fast16_t const margin, void const * const sender);
#endif
static void QXThread_postLIFO_(QMActive * const me, QEvt const * const e);


/****************************************************************************/
/* QXThread virtual function implementations... */
static void QXThread_init_(QMsm * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    Q_ERROR_ID(100);
}

/****************************************************************************/
static void QXThread_dispatch_(QMsm * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    Q_ERROR_ID(200);
}

/****************************************************************************/
static void QXThread_start_(QMActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    QF_CRIT_STAT_

    Q_REQUIRE_ID(300, (!QXK_ISR_CONTEXT_()) /* don't start AO's in an ISR! */
                      && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                      && (stkSto != (void *)0)
                      && (stkSize != (uint_fast16_t)0)
                      && (me->super.state.act == (QActionHandler)0));

    (void)ie; /* parameter not referenced */

    /* is storage for the queue buffer provided? */
    if (qSto != (QEvt const **)0) {
        QEQueue_init(&me->eQueue, qSto, qLen);
    }

    /* "naked" threads provide their thread function in place of
    * the top-most initial transition 'me->super.temp.act'
    */
    QXK_stackInit_(me, (QXThreadHandler)me->super.temp.act, stkSto, stkSize);

    me->prio = prio;
    QF_add_(me); /* make QF aware of this naked thread */

    QF_CRIT_ENTRY_();
    QXK_prioInsert(&QXK_attr_.readySet, me->prio);

    /* Is QXK running? */
    if (QXK_attr_.curr != (void *)0) {
        QXK_sched_();
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/* NOTE:
* For compatibility with the V-table from the superclass QMActive, the
* me-pointer is typed as pointing to QMActive. However, the me pointer
* here actually points to the QXThread subclass. Therefore the downcast
* (QXThread *)me is always correct.
*/
#ifndef Q_SPY
static bool QXThread_post_(QMActive * const me, QEvt const * const e,
                           uint_fast16_t const margin)
#else
static bool QXThread_post_(QMActive * const me, QEvt const * const e,
                        uint_fast16_t const margin, void const * const sender)
#endif
{
    bool stat;
    QF_CRIT_STAT_

    /* is it the private time event? */
    if (e == &((QXThread *)me)->timeEvt.super) {
        QF_CRIT_ENTRY_();
        stat = true;
        /* the private time event is disarmed and not in any queue,
        * so it is safe to change its signal. The signal of 0 means
        * that the time event __has__ expired.
        */
        ((QXThread *)me)->timeEvt.super.sig = (QSignal)0;

        QXThread_unblock_((QXThread *)me);
        QF_CRIT_EXIT_();
    }
    /* is the event queue provided? */
    else if (me->eQueue.end != (QEQueueCtr)0) {
        QF_CRIT_ENTRY_();
        (void)QXThread_teDisarm_((QXThread *)me);
        QF_CRIT_EXIT_();

#ifndef Q_SPY
        stat = QActive_post_(me, e, margin);
#else
        stat = QActive_post_(me, e, margin, sender);
#endif
    }
    else { /* the queue is not available */
         QF_gc(e); /* make sure the event is not leaked */
         stat = false;
         Q_ERROR_ID(410);
    }

    return stat;
}

/****************************************************************************/
static void QXThread_postLIFO_(QMActive * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    Q_ERROR_ID(510);
}

/****************************************************************************/
/*! constructor of a "naked" thread */
void QXThread_ctor(QXThread * const me,
                   QXThreadHandler handler, uint_fast8_t tickRate)
{
    static QXThreadVtbl const vtbl = { /* QXThread virtual table */
        { &QXThread_init_,       /* not used in QXThread */
          &QXThread_dispatch_ }, /* not used in QXThread */
        &QXThread_start_,
        &QXThread_post_,
        &QXThread_postLIFO_
    };

    QMActive_ctor(&me->super, Q_STATE_CAST(handler)); /* superclass' ctor */
    me->super.super.vptr = &vtbl.super; /* set the vptr to QXThread v-table */
    me->super.super.state.act = (QActionHandler)0; /* mark as naked thread */

    /* construct the time event added in the QXThread class */
    QTimeEvt_ctorX(&me->timeEvt, &me->super, (enum_t)QXK_DELAY_SIG, tickRate);
}


/****************************************************************************/
/* must be called from within a critical section */
void QXThread_block_(QXThread * const me) {
    /* the thread holding the lock cannot block! */
    Q_REQUIRE_ID(100,  me->super.prio != QXK_attr_.lockPrio);
    QXK_prioRemove(&QXK_attr_.readySet, me->super.prio);
    QXK_sched_();
}

/****************************************************************************/
/* must be called from within a critical section */
void QXThread_unblock_(QXThread * const me) {
    QXK_prioInsert(&QXK_attr_.readySet, me->super.prio);
    if ((!QXK_ISR_CONTEXT_()) /* not inside ISR? */
        && (QXK_attr_.curr != (void *)0))  /* multitasking started? */
    {
        QXK_sched_();
    }
}

/****************************************************************************/
/* must be called from within a critical section */
void QXThread_teArm_(QXThread * const me,
                     QSignal sig,
                     uint_fast16_t const nTicks, uint_fast8_t const tickRate)
{
    me->timeEvt.super.sig = sig;

    if (nTicks != QXTHREAD_NO_TIMEOUT) {
        me->timeEvt.ctr = (QTimeEvtCtr)nTicks;
        me->timeEvt.interval = (QTimeEvtCtr)0;

        /* is the time event unlinked?
        * NOTE: For the duration of a single clock tick of the specified tick
        * rate a time event can be disarmed and yet still linked in the list,
        * because un-linking is performed exclusively in QF_tickX().
        */
        if ((me->timeEvt.super.refCtr_ & (uint8_t)0x80) == (uint8_t)0) {
            me->timeEvt.super.refCtr_ |= (uint8_t)0x80; /* mark as linked */

            /* The time event is initially inserted into the separate
            * "freshly armed" list based on QF_timeEvtHead_[tickRate].act.
            * Only later, inside the QF_tickX() function, the "freshly armed"
            * list is appended to the main list of armed time events based on
            * QF_timeEvtHead_[tickRate].next. Again, this is to keep any
            * changes to the main list exclusively inside QF_tickX().
            */
            me->timeEvt.next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
            QF_timeEvtHead_[tickRate].act = &me->timeEvt;
        }
    }
}

/****************************************************************************/
/* must be called from within a critical section */
bool QXThread_teDisarm_(QXThread * const me) {
    bool wasArmed;
    /* is the time evt running? */
    if (me->timeEvt.ctr != (QTimeEvtCtr)0) {
        wasArmed = true;
        me->timeEvt.ctr = (QTimeEvtCtr)0;  /* schedule removal from list */
    }
    /* the time event was already automatically disarmed */
    else {
        wasArmed = false;
    }
    return wasArmed;
}


/****************************************************************************/
/*! block (suspend) the current "naked" thread */
void QXThread_block(void) {
    QXThread *thr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    thr = (QXThread *)QXK_attr_.curr;
    Q_REQUIRE_ID(700, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        /* this must be a "naked" thread (no state) */
        && (thr->super.super.state.act == (QActionHandler)0));
    QXThread_block_(thr);
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/*! unblock (resume) a given "naked" thread */
void QXThread_unblock(QXThread * const me) {
    QF_CRIT_STAT_

    /* the unblocked thread must be a "naked" thread (no state) */
    Q_REQUIRE_ID(800, me->super.super.state.act == (QActionHandler)0);

    QF_CRIT_ENTRY_();
    QXThread_unblock_(me);
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/*! delay (timed block) the current "naked" thread */
bool QXThread_delay(uint_fast16_t const nTicks, uint_fast8_t const tickRate) {
    QXThread *thr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    thr = (QXThread *)QXK_attr_.curr;
    /* remember the blocking object */
    thr->super.super.temp.obj = (QMState const *)&thr->timeEvt;
    QXThread_teArm_(thr, (QSignal)QXK_DELAY_SIG, nTicks, tickRate);
    QXThread_block_(thr);
    QF_CRIT_EXIT_();

    /* signal of zero means that the time event was posted without
    * being canceled.
    */
    return (bool)(thr->timeEvt.super.sig == (QSignal)0);
}

/****************************************************************************/
/*! cancel the delay */
bool QXThread_delayCancel(QXThread * const me) {
    bool wasArmed;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    wasArmed = QXThread_teDisarm_(me);
    QXThread_unblock_(me);
    QF_CRIT_EXIT_();

    return wasArmed;
}

/****************************************************************************/
/*! obtain a message from the private message queue (block if no messages) */
void const *QXThread_queueGet(uint_fast16_t const nTicks,
                              uint_fast8_t const tickRate)
{
    QXThread *thr;
    QEQueueCtr nFree;
    QEvt const *e;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    thr = (QXThread *)QXK_attr_.curr;

    Q_REQUIRE_ID(900, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        /* this must be a "naked" thread (no state) */
        && (thr->super.super.state.act == (QActionHandler)0));

    /* is the queue empty? -- block and wait for event(s) */
    if (thr->super.eQueue.frontEvt == (QEvt *)0) {
        thr->super.super.temp.obj = (QMState const *)&thr->super.eQueue;
        QXThread_teArm_(thr, (QSignal)QXK_QUEUE_SIG, nTicks, tickRate);
        QXK_prioRemove(&QXK_attr_.readySet, thr->super.prio);
        QXK_sched_();
        QF_CRIT_EXIT_();
        QF_CRIT_EXIT_NOP();
        QF_CRIT_ENTRY_();
    }

    /* is the queue not empty? */
    if (thr->super.eQueue.frontEvt != (QEvt *)0) {
        e = thr->super.eQueue.frontEvt; /* always remove from the front */
        nFree= thr->super.eQueue.nFree +(QEQueueCtr)1; /* volatile into tmp */
        thr->super.eQueue.nFree = nFree; /* update the number of free */

        /* any events in the ring buffer? */
        if (nFree <= thr->super.eQueue.end) {

            /* remove event from the tail */
            thr->super.eQueue.frontEvt =
                QF_PTR_AT_(thr->super.eQueue.ring, thr->super.eQueue.tail);
            if (thr->super.eQueue.tail == (QEQueueCtr)0) { /* need to wrap? */
                thr->super.eQueue.tail = thr->super.eQueue.end;  /* wrap */
            }
            --thr->super.eQueue.tail;

            QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, thr)
                QS_TIME_();                   /* timestamp */
                QS_SIG_(e->sig);              /* the signal of this event */
                QS_OBJ_(&thr->super);         /* this active object */
                QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
                QS_EQC_(nFree);               /* number of free entries */
            QS_END_NOCRIT_()
        }
        else {
            thr->super.eQueue.frontEvt = (QEvt const *)0; /* empty queue */

            /* all entries in the queue must be free (+1 for fronEvt) */
            Q_ASSERT_ID(910, nFree == (thr->super.eQueue.end +(QEQueueCtr)1));

            QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET_LAST, QS_priv_.aoObjFilter, thr)
                QS_TIME_();                   /* timestamp */
                QS_SIG_(e->sig);              /* the signal of this event */
                QS_OBJ_(&thr->super);         /* this active object */
                QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_()
        }
    }
    else { /* the queue is still empty -- the timeout must have fired */
         e = (QEvt const *)0;
    }
    QF_CRIT_EXIT_();

    return e;
}
