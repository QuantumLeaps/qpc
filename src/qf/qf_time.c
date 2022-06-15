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
* @brief QF/C time events and time management services
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

Q_DEFINE_THIS_MODULE("qf_time")

/* Package-scope objects ****************************************************/
/*! @static @private @memberof QF */
QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE]; /* heads of time event lists */

#ifdef Q_SPY
    /*! intertnal macro to encapsulate a MISRA deviation
    * @description
    * This internal macro encapsulates the violation of MISRA-C 2012
    * Rule 11.5(A) "A conversion should not be performed from pointer to void
    * into pointer to object".
    */
    #define QACTIVE_CAST_(ptr_) ((QActive *)(ptr_))
#endif

/*..........................................................................*/
#ifdef Q_SPY
void QF_tickX_(uint_fast8_t const tickRate, void const * const sender)
#else
void QF_tickX_(uint_fast8_t const tickRate)
#endif
{
    QTimeEvt *prev = &QF_timeEvtHead_[tickRate];

    QF_CRIT_STAT_
    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_TICK, 0U)
        ++prev->ctr;
        QS_TEC_PRE_(prev->ctr); /* tick ctr */
        QS_U8_PRE_(tickRate);   /* tick rate */
    QS_END_NOCRIT_PRE_()

    /* scan the linked-list of time events at this rate... */
    for (;;) {
        QTimeEvt *t = prev->next;  /* advance down the time evt. list */

        /* end of the list? */
        if (t == (QTimeEvt *)0) {

            /* any new time events armed since the last run of QF_tickX_()? */
            if (QF_timeEvtHead_[tickRate].act != (void *)0) {

                /* sanity check */
                Q_ASSERT_CRIT_(110, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
                QF_timeEvtHead_[tickRate].act = (void *)0;
                t = prev->next;  /* switch to the new list */
            }
            else {
                break; /* all currently armed time evts. processed */
            }
        }

        /* time event scheduled for removal? */
        if (t->ctr == 0U) {
            prev->next = t->next;
            /* mark time event 't' as NOT linked */
            t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED & 0xFFU);
            /* do NOT advance the prev pointer */
            QF_CRIT_X_(); /* exit crit. section to reduce latency */

            /* prevent merging critical sections, see NOTE1 below  */
            QF_CRIT_EXIT_NOP();
        }
        else {
            --t->ctr;

            /* is time event about to expire? */
            if (t->ctr == 0U) {
                /* temporary for volatile */
                QActive * const act = (QActive *)t->act;

                /* periodic time evt? */
                if (t->interval != 0U) {
                    t->ctr = t->interval; /* rearm the time event */
                    prev = t; /* advance to this time event */
                }
                /* one-shot time event: automatically disarm */
                else {
                    prev->next = t->next;
                    /* mark time event 't' as NOT linked */
                    t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED & 0xFFU);
                    /* do NOT advance the prev pointer */

                    QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_AUTO_DISARM, act->prio)
                        QS_OBJ_PRE_(t);        /* this time event object */
                        QS_OBJ_PRE_(act);      /* the target AO */
                        QS_U8_PRE_(tickRate);  /* tick rate */
                    QS_END_NOCRIT_PRE_()
                }

                QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_POST, act->prio)
                    QS_TIME_PRE_();            /* timestamp */
                    QS_OBJ_PRE_(t);            /* the time event object */
                    QS_SIG_PRE_(t->super.sig); /* signal of this time event */
                    QS_OBJ_PRE_(act);          /* the target AO */
                    QS_U8_PRE_(tickRate);      /* tick rate */
                QS_END_NOCRIT_PRE_()

                QF_CRIT_X_(); /* exit critical section before posting */

                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(act, &t->super, sender);
            }
            else {
                prev = t;         /* advance to this time event */
                QF_CRIT_X_();  /* exit crit. section to reduce latency */

                /* prevent merging critical sections, see NOTE1 below  */
                QF_CRIT_EXIT_NOP();
            }
        }
        QF_CRIT_E_(); /* re-enter crit. section to continue */
    }
    QF_CRIT_X_();
}

/*..........................................................................*/
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate) {

    /*! @pre the tick rate must be in range */
    Q_REQUIRE_ID(200, tickRate < QF_MAX_TICK_RATE);

    bool inactive;
    if (QF_timeEvtHead_[tickRate].next != (QTimeEvt *)0) {
        inactive = false;
    }
    else if ((QF_timeEvtHead_[tickRate].act != (void *)0)) {
        inactive = false;
    }
    else {
        inactive = true;
    }
    return inactive;
}

/*..........................................................................*/
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate)
{
    /*! @pre The signal must be valid and the tick rate in range */
    Q_REQUIRE_ID(300, (sig >= (enum_t)Q_USER_SIG)
        && (tickRate < QF_MAX_TICK_RATE));

    me->next      = (QTimeEvt *)0;
    me->ctr       = 0U;
    me->interval  = 0U;
    me->super.sig = (QSignal)sig;

    /* For backwards compatibility with QTimeEvt_ctor(), the active object
    * pointer can be uninitialized (NULL) and is NOT validated in the
    * precondition. The active object pointer is validated in preconditions
    * to QTimeEvt_arm_() and QTimeEvt_rearm().
    */
    me->act       = act;

    /* Setting the POOL_ID event attribute to zero is correct only for
    * events not allocated from event pools, which must be the case
    * for Time Events.
    */
    me->super.poolId_ = 0U;

    /* The refCtr_ attribute is not used in time events, so it is
    * reused to hold the tickRate as well as other information
    */
    me->super.refCtr_ = (uint8_t)tickRate;
}

/*..........................................................................*/
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval)
{
    uint_fast8_t const tickRate
                       = ((uint_fast8_t)me->super.refCtr_ & QTE_TICK_RATE);
    QTimeEvtCtr const ctr = me->ctr;
#ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
#endif

    /*! @pre the host AO must be valid, time evnet must be disarmed,
    * number of clock ticks cannot be zero, and the signal must be valid.
    */
    Q_REQUIRE_ID(400, (me->act != (void *)0)
                 && (ctr == 0U)
                 && (nTicks != 0U)
                 && (tickRate < (uint_fast8_t)QF_MAX_TICK_RATE)
                 && (me->super.sig >= (QSignal)Q_USER_SIG));
#ifdef Q_NASSERT
    (void)ctr; /* avoid compiler warning about unused variable */
#endif

    QF_CRIT_STAT_
    QF_CRIT_E_();
    me->ctr = nTicks;
    me->interval = interval;

    /* is the time event unlinked?
    * NOTE: For the duration of a single clock tick of the specified tick
    * rate a time event can be disarmed and yet still linked into the list,
    * because un-linking is performed exclusively in the QF_tickX() function.
    */
    if ((me->super.refCtr_ & QTE_IS_LINKED) == 0U) {
        me->super.refCtr_ |= QTE_IS_LINKED; /* mark as linked */

        /* The time event is initially inserted into the separate
        * "freshly armed" link list based on QF_timeEvtHead_[tickRate].act.
        * Only later, inside the QF_tickX() function, the "freshly armed"
        * list is appended to the main list of armed time events based on
        * QF_timeEvtHead_[tickRate].next. Again, this is to keep any
        * changes to the main list exclusively inside the QF_tickX()
        * function.
        */
        me->next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
        QF_timeEvtHead_[tickRate].act = me;
    }

    QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_ARM, qs_id)
        QS_TIME_PRE_();        /* timestamp */
        QS_OBJ_PRE_(me);       /* this time event object */
        QS_OBJ_PRE_(me->act);  /* the active object */
        QS_TEC_PRE_(nTicks);   /* the number of ticks */
        QS_TEC_PRE_(interval); /* the interval */
        QS_U8_PRE_(tickRate);  /* tick rate */
    QS_END_NOCRIT_PRE_()

    QF_CRIT_X_();
}

/*..........................................................................*/
bool QTimeEvt_disarm(QTimeEvt * const me) {
#ifdef Q_SPY
    uint_fast8_t const qs_id = QACTIVE_CAST_(me->act)->prio;
#endif

    QF_CRIT_STAT_
    QF_CRIT_E_();

    /* is the time event actually armed? */
    bool wasArmed;
    if (me->ctr != 0U) {
        wasArmed = true;
        me->super.refCtr_ |= QTE_WAS_DISARMED;

        QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_DISARM, qs_id)
            QS_TIME_PRE_();            /* timestamp */
            QS_OBJ_PRE_(me);           /* this time event object */
            QS_OBJ_PRE_(me->act);      /* the target AO */
            QS_TEC_PRE_(me->ctr);      /* the number of ticks */
            QS_TEC_PRE_(me->interval); /* the interval */
            QS_U8_PRE_(me->super.refCtr_ & QTE_TICK_RATE);
        QS_END_NOCRIT_PRE_()

        me->ctr = 0U;  /* schedule removal from the list */
    }
    else { /* the time event was already disarmed automatically */
        wasArmed = false;
        me->super.refCtr_ &= (uint8_t)(~QTE_WAS_DISARMED & 0xFFU);

        QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_DISARM_ATTEMPT, qs_id)
            QS_TIME_PRE_();            /* timestamp */
            QS_OBJ_PRE_(me);           /* this time event object */
            QS_OBJ_PRE_(me->act);      /* the target AO */
            QS_U8_PRE_(me->super.refCtr_ & QTE_TICK_RATE);
        QS_END_NOCRIT_PRE_()

    }
    QF_CRIT_X_();

    return wasArmed;
}

/*..........................................................................*/
bool QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks) {
    uint_fast8_t const tickRate
                       = (uint_fast8_t)me->super.refCtr_ & QTE_TICK_RATE;
#ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
#endif

    /*! @pre AO must be valid, tick rate must be in range, nTicks must not
    * be zero, and the signal of this time event must be valid
    */
    Q_REQUIRE_ID(600, (me->act != (void *)0)
                      && (tickRate < QF_MAX_TICK_RATE)
                      && (nTicks != 0U)
                      && (me->super.sig >= (QSignal)Q_USER_SIG));

    QF_CRIT_STAT_
    QF_CRIT_E_();

    /* is the time evt not running? */
    bool wasArmed;
    if (me->ctr == 0U) {
        wasArmed = false;

        /* NOTE: For the duration of a single clock tick of the specified
        * tick rate a time event can be disarmed and yet still linked into
        * the list, because unlinking is performed exclusively in the
        * QF_tickX() function.
        */
        /* is the time event linked yet? */
        if ((me->super.refCtr_ & QTE_IS_LINKED) == 0U) {
            me->super.refCtr_ |= QTE_IS_LINKED; /* mark as linked */

            /* The time event is initially inserted into the separate
            * "freshly armed" list based on QF_timeEvtHead_[tickRate].act.
            * Only later, inside the QF_tickX() function, the "freshly armed"
            * list is appended to the main list of armed time events based on
            * QF_timeEvtHead_[tickRate].next. Again, this is to keep any
            * changes to the main list exclusively inside the QF_tickX()
            * function.
            */
            me->next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
            QF_timeEvtHead_[tickRate].act = me;
        }
    }
    else { /* the time event was armed */
        wasArmed = true;
    }
    me->ctr = nTicks; /* re-load the tick counter (shift the phasing) */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_REARM, qs_id)
        QS_TIME_PRE_();            /* timestamp */
        QS_OBJ_PRE_(me);           /* this time event object */
        QS_OBJ_PRE_(me->act);      /* the target AO */
        QS_TEC_PRE_(me->ctr);      /* the number of ticks */
        QS_TEC_PRE_(me->interval); /* the interval */
        QS_2U8_PRE_(tickRate, (wasArmed ? 1U : 0U));
    QS_END_NOCRIT_PRE_()

    QF_CRIT_X_();

    return wasArmed;
}

/*..........................................................................*/
bool QTimeEvt_wasDisarmed(QTimeEvt * const me) {
    uint8_t const wasDisarmed = (me->super.refCtr_ & QTE_WAS_DISARMED);
    me->super.refCtr_ |= QTE_WAS_DISARMED; /* mark as disarmed */
    return wasDisarmed != 0U;
}

/*..........................................................................*/
QTimeEvtCtr QTimeEvt_currCtr(QTimeEvt const * const me) {
    QF_CRIT_STAT_
    QF_CRIT_E_();
    QTimeEvtCtr const ret = me->ctr;
    QF_CRIT_X_();

    return ret;
}

/*============================================================================
* NOTE1:
* In some QF ports the critical section exit takes effect only on the next
* machine instruction. If this case, the next instruction is another entry
* to a critical section, the critical section won't be really exited, but
* rather the two adjacent critical sections would be merged.
*
* The QF_CRIT_EXIT_NOP() macro contains minimal code required
* to prevent such merging of critical sections in QF ports,
* in which it can occur.
*/
