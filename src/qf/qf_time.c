/**
* @file
* @brief ::QTimeEvt implementation and QF system clock tick QF_tickX_())
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
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
QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE]; /* heads of time event lists */

/****************************************************************************/
#ifdef Q_SPY
/**
* @description
* This function must be called periodically from a time-tick ISR or from
* a task so that QF can manage the timeout events assigned to the given
* system clock tick rate.
*
* @param[in] tickRate  system clock tick rate serviced in this call [1..15].
* @param[in] sender    pointer to a sender object (used only for QS tracing)
*
* @note
* this function should be called only via the macro QF_TICK_X()
*
* @note
* the calls to QF_tickX_() with different @p tickRate parameter can preempt
* each other. For example, higher clock tick rates might be serviced from
* interrupts while others from tasks (active objects).
*
* @sa
* ::QTimeEvt.
*/
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
            t->super.refCtr_ &= (uint8_t)(~TE_IS_LINKED & 0xFFU);
            /* do NOT advance the prev pointer */
            QF_CRIT_X_(); /* exit crit. section to reduce latency */

            /* prevent merging critical sections, see NOTE1 below  */
            QF_CRIT_EXIT_NOP();
        }
        else {
            --t->ctr;

            /* is time event about to expire? */
            if (t->ctr == 0U) {
                QActive *act = (QActive *)t->act; /* temp. for volatile */

                /* periodic time evt? */
                if (t->interval != 0U) {
                    t->ctr = t->interval; /* rearm the time event */
                    prev = t; /* advance to this time event */
                }
                /* one-shot time event: automatically disarm */
                else {
                    prev->next = t->next;
                    /* mark time event 't' as NOT linked */
                    t->super.refCtr_ &= (uint8_t)(~TE_IS_LINKED & 0xFFU);
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

/*****************************************************************************
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


/****************************************************************************/
/**
* @description
* Find out if any time events are armed at the given clock tick rate.
*
* @param[in]  tickRate  system clock tick rate to find out about.
*
* @returns
* 'true' if no time events are armed at the given tick rate and
* 'false' otherwise.
*
* @note
* This function should be called in critical section.
*/
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate) {
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

/****************************************************************************/
/**
* @description
* When creating a time event, you must commit it to a specific active object
* @p act, tick rate @p tickRate and event signal @p sig. You cannot change
* these attributes later.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     act  pointer to the active object associated with this
*                     time event. The time event will post itself to this AO.
* @param[in]     sig  signal to associate with this time event.
* @param[in]     tickRate systemclock tick rate to associate with this
*                     time event in the range [0..15].
*
* @note You should call the constructor exactly once for every Time Event
* object **before** arming the Time Event. The ideal place for initializing
* the time event(s) associated with a given AO is the AO's constructor.
*/
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate)
{
    /** @pre The signal must be valid and the tick rate in range */
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

/****************************************************************************/
/**
* @description
* Arms a time event to fire in a specified number of clock ticks and with
* a specified interval. If the interval is zero, the time event is armed for
* one shot ('one-shot' time event). The time event gets directly posted
* (using the FIFO policy) into the event queue of the host active object.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to rearm the time event with.
* @param[in]     interval interval (in clock ticks) for periodic time event.
*
* @note
* After posting, a one-shot time event gets automatically disarmed
* while a periodic time event (interval != 0) is automatically re-armed.
*
* @note
* A time event can be disarmed at any time by calling the
* QTimeEvt_disarm() function. Also, a time event can be re-armed to fire
* in a different number of clock ticks by calling the QTimeEvt_rearm()
* function.
*
* @usage
* The following example shows how to arm a one-shot time event from a state
* machine of an active object:
* @include qf_state.c
*/
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval)
{
    uint_fast8_t tickRate = ((uint_fast8_t)me->super.refCtr_ & TE_TICK_RATE);
    QTimeEvtCtr ctr = me->ctr;
#ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
#endif
    QF_CRIT_STAT_

    /** @pre the host AO must be valid, time evnet must be disarmed,
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

    QF_CRIT_E_();
    me->ctr = nTicks;
    me->interval = interval;

    /* is the time event unlinked?
    * NOTE: For the duration of a single clock tick of the specified tick
    * rate a time event can be disarmed and yet still linked into the list,
    * because un-linking is performed exclusively in the QF_tickX() function.
    */
    if ((me->super.refCtr_ & TE_IS_LINKED) == 0U) {
        me->super.refCtr_ |= TE_IS_LINKED; /* mark as linked */

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

/****************************************************************************/
/**
* @description
* Disarm the time event so it can be safely reused.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' if the time event was truly disarmed, that is, it was running.
* The return of 'false' means that the time event was not truly disarmed,
* because it was not running. The 'false' return is only possible for one-
* shot time events that have been automatically disarmed upon expiration.
* In this case the 'false' return means that the time event has already
* been posted or published and should be expected in the active object's
* state machine.
*
* @note
* there is no harm in disarming an already disarmed time event
*/
bool QTimeEvt_disarm(QTimeEvt * const me) {
    bool wasArmed;
#ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
#endif
    QF_CRIT_STAT_

    QF_CRIT_E_();

    /* is the time event actually armed? */
    if (me->ctr != 0U) {
        wasArmed = true;
        me->super.refCtr_ |= TE_WAS_DISARMED;

        QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_DISARM, qs_id)
            QS_TIME_PRE_();            /* timestamp */
            QS_OBJ_PRE_(me);           /* this time event object */
            QS_OBJ_PRE_(me->act);      /* the target AO */
            QS_TEC_PRE_(me->ctr);      /* the number of ticks */
            QS_TEC_PRE_(me->interval); /* the interval */
            QS_U8_PRE_(me->super.refCtr_ & TE_TICK_RATE);
        QS_END_NOCRIT_PRE_()

        me->ctr = 0U;  /* schedule removal from the list */
    }
    else { /* the time event was already disarmed automatically */
        wasArmed = false;
        me->super.refCtr_ &= (uint8_t)(~TE_WAS_DISARMED & 0xFFU);

        QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_DISARM_ATTEMPT, qs_id)
            QS_TIME_PRE_();            /* timestamp */
            QS_OBJ_PRE_(me);           /* this time event object */
            QS_OBJ_PRE_(me->act);      /* the target AO */
            QS_U8_PRE_(me->super.refCtr_ & TE_TICK_RATE);
        QS_END_NOCRIT_PRE_()

    }
    QF_CRIT_X_();
    return wasArmed;
}

/****************************************************************************/
/**
* @description
* Rearms a time event with a new number of clock ticks. This function can
* be used to adjust the current period of a periodic time event or to
* prevent a one-shot time event from expiring (e.g., a watchdog time event).
* Rearming a periodic timer leaves the interval unchanged and is a convenient
* method to adjust the phasing of a periodic time event.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to rearm the time event with.
*
* @returns
* 'true' if the time event was running as it was re-armed. The 'false'
* return means that the time event was not truly rearmed because it was
* not running. The 'false' return is only possible for one-shot time events
* that have been automatically disarmed upon expiration. In this case the
* 'false' return means that the time event has already been posted or
* published and should be expected in the active object's state machine.
*/
bool QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks) {
    uint_fast8_t tickRate = (uint_fast8_t)me->super.refCtr_ & TE_TICK_RATE;
    bool wasArmed;
#ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
#endif
    QF_CRIT_STAT_

    /** @pre AO must be valid, tick rate must be in range, nTicks must not
    * be zero, and the signal of this time event must be valid
    */
    Q_REQUIRE_ID(600, (me->act != (void *)0)
                      && (tickRate < QF_MAX_TICK_RATE)
                      && (nTicks != 0U)
                      && (me->super.sig >= (QSignal)Q_USER_SIG));

    QF_CRIT_E_();

    /* is the time evt not running? */
    if (me->ctr == 0U) {
        wasArmed = false;

        /* NOTE: For the duration of a single clock tick of the specified
        * tick rate a time event can be disarmed and yet still linked into
        * the list, because unlinking is performed exclusively in the
        * QF_tickX() function.
        */
        /* is the time event linked yet? */
        if ((me->super.refCtr_ & TE_IS_LINKED) == 0U) {
            me->super.refCtr_ |= TE_IS_LINKED; /* mark as linked */

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

/****************************************************************************/
/**
* @description
* Useful for checking whether a one-shot time event was disarmed in the
* QTimeEvt_disarm() operation.
*
* @param[in,out] me   pointer (see @ref oop)
*
* @returns
* 'true' if the time event was truly disarmed in the last QTimeEvt_disarm()
* operation. The 'false' return means that the time event was not truly
* disarmed, because it was not running at that time. The 'false' return is
* only possible for one-shot time events that have been automatically disarmed
* upon expiration. In this case the 'false' return means that the time event
* has already been posted or published and should be expected in the active
* object's event queue.
*
* @note
* This function has a **side effect** of setting the "was disarmed" status,
* which means that the second and subsequent times this function is called
* the function will return 'true'.
*/
bool QTimeEvt_wasDisarmed(QTimeEvt * const me) {
    uint8_t wasDisarmed = (me->super.refCtr_ & TE_WAS_DISARMED);
    me->super.refCtr_ |= TE_WAS_DISARMED; /* set the flag */
    return (wasDisarmed != 0U) ? true : false;
}

/****************************************************************************/
/**
* @description
* Useful for checking how many clock ticks (at the tick rate associated
* with the time event) remain until the time event expires.
*
* @param[in,out] me   pointer (see @ref oop)
*
* @returns
* For an armed time event, the function returns the current value of the
* down-counter of the given time event. If the time event is not armed,
* the function returns 0.
*
* @note
* The function is thread-safe.
*/
QTimeEvtCtr QTimeEvt_currCtr(QTimeEvt const * const me) {
    QTimeEvtCtr ret;
    QF_CRIT_STAT_

    QF_CRIT_E_();
    ret = me->ctr;
    QF_CRIT_X_();

    return ret;
}

