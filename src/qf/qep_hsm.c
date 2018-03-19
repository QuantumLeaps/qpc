/**
* @file
* @brief ::QHsm implementation
* @ingroup qep
* @cond
******************************************************************************
* Last updated for version 6.2.0
* Last updated on  2018-03-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qep_port.h"     /* QEP port */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qep_hsm")

/****************************************************************************/
char_t const QP_versionStr[6] = QP_VERSION_STR;

/****************************************************************************/
/*! internal QEP constants */
enum {
    QEP_EMPTY_SIG_ = 0, /*!< reserved empty signal for internal use only */

    /*! maximum depth of state nesting in a HSM (including the top level),
    * must be >= 3
    */
    QHSM_MAX_NEST_DEPTH_ = 6
};

/**
* @description
* Static, preallocated standard events that the QEP event processor sends
* to state handler functions of ::QHsm and ::QFsm subclasses to execute
* entry actions, exit actions, and initial transitions.
*/
static QEvt const QEP_reservedEvt_[] = {
    { (QSignal)QEP_EMPTY_SIG_, (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG,    (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG,     (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_INIT_SIG,     (uint8_t)0, (uint8_t)0 }
};

/*! helper macro to trigger reserved event in an HSM */
#define QEP_TRIG_(state_, sig_) \
    ((*(state_))(me, &QEP_reservedEvt_[(sig_)]))

/*! helper macro to trigger exit action in an HSM */
#define QEP_EXIT_(state_) do { \
    if (QEP_TRIG_((state_), Q_EXIT_SIG) == (QState)Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.locFilter[SM_OBJ], me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)

/*! helper macro to trigger entry action in an HSM */
#define QEP_ENTER_(state_) do { \
    if (QEP_TRIG_((state_), Q_ENTRY_SIG) == (QState)Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.locFilter[SM_OBJ], me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)

/*! helper function to execute a transition chain in HSM */
static int_fast8_t QHsm_tran_(QHsm * const me,
                              QStateHandler path[QHSM_MAX_NEST_DEPTH_]);


/****************************************************************************/
/**
* @description
* Performs the first step of HSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     initial pointer to the top-most initial state-handler
*                        function in the derived state machine
*
* @note Must be called only by the constructors of the derived state
* machines.
*
* @note Must be called only ONCE before QHSM_INIT().
*
* @usage
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* @include qep_qhsm_ctor.c
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial) {
    static struct QHsmVtbl const vtbl = { /* QHsm virtual table */
        &QHsm_init_,
        &QHsm_dispatch_
    };
    me->vptr      = &vtbl;
    me->state.fun = Q_STATE_CAST(&QHsm_top);
    me->temp.fun  = initial;
}

/****************************************************************************/
/**
* @description
* Executes the top-most initial transition in a HSM.
*
* @param[in,out] me pointer (see @ref oop)
* @param[in]     e  pointer to the initialization event (might be NULL)
*
* @note Must be called only ONCE after the QHsm_ctor().
*/
void QHsm_init_(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state.fun;
    QState r;
    QS_CRIT_STAT_

    /** @pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QMsmVtbl const *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (t == Q_STATE_CAST(&QHsm_top)));

    r = (*me->temp.fun)(me, e); /* execute the top-most initial transition */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN);

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.locFilter[SM_OBJ], me)
        QS_OBJ_(me);           /* this state machine object */
        QS_FUN_(t);            /* the source state */
        QS_FUN_(me->temp.fun); /* the target of the initial transition */
    QS_END_()

    /* drill down into the state hierarchy with initial transitions... */
    do {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_]; /* tran entry path array */
        int_fast8_t ip = (int_fast8_t)0;          /* tran entry path index */

        path[0] = me->temp.fun;
        (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        while (me->temp.fun != t) {
            ++ip;
            Q_ASSERT_ID(220, ip < (int_fast8_t)Q_DIM(path));
            path[ip] = me->temp.fun;
            (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
        me->temp.fun = path[0];

        /* retrace the entry path in reverse (desired) order... */
        do {
            QEP_ENTER_(path[ip]); /* enter path[ip] */
            --ip;
        } while (ip >= (int_fast8_t)0);

        t = path[0]; /* current state becomes the new source */

        r = QEP_TRIG_(t, Q_INIT_SIG); /* execute initial transition */

#ifdef Q_SPY
        if (r == (QState)Q_RET_TRAN) {
            QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me);           /* this state machine object */
                QS_FUN_(t);            /* the source state */
                QS_FUN_(me->temp.fun); /* the target of the initial tran. */
            QS_END_()
        }
#endif /* Q_SPY */

    } while (r == (QState)Q_RET_TRAN);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.locFilter[SM_OBJ], me)
        QS_TIME_();    /* time stamp */
        QS_OBJ_(me);   /* this state machine object */
        QS_FUN_(t);    /* the new active state */
    QS_END_()

    me->state.fun = t; /* change the current active state */
    me->temp.fun  = t; /* mark the configuration as stable */
}

/****************************************************************************/
/**
* @description
* QHsm_top() is the ultimate root of state hierarchy in all HSMs derived
* from ::QHsm.
*
* @param[in] me pointer (see @ref oop)
* @param[in] e  pointer to the event to be dispatched to the FSM
*
* @returns Always returns #Q_RET_IGNORED, which means that the top state
*          ignores all events.
*
* @note The parameters to this state handler are not used. They are provided
* for conformance with the state-handler function signature ::QStateHandler.
*/
QState QHsm_top(void const * const me, QEvt const * const e) {
    (void)me; /* suppress the "unused parameter" compiler warning */
    (void)e;  /* suppress the "unused parameter" compiler warning */
    return (QState)Q_RET_IGNORED; /* the top state ignores all events */
}

/****************************************************************************/
/**
* @description
* Dispatches an event for processing to a hierarchical state machine (HSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* @param[in,out] me pointer (see @ref oop)
* @param[in]     e  pointer to the event to be dispatched to the HSM
*
* @note
* This function should be called only via the virtual table (see
* QHSM_DISPATCH()) and should NOT be called directly in the applications.
*/
void QHsm_dispatch_(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state.fun;
    QStateHandler s;
    QState r;
    QS_CRIT_STAT_

    /** @pre the current state must be initialized and
    * the state configuration must be stable
    */
    Q_REQUIRE_ID(400, (t != Q_STATE_CAST(0))
                       && (t == me->temp.fun));

    QS_BEGIN_(QS_QEP_DISPATCH, QS_priv_.locFilter[SM_OBJ], me)
        QS_TIME_();         /* time stamp */
        QS_SIG_(e->sig);    /* the signal of the event */
        QS_OBJ_(me);        /* this state machine object */
        QS_FUN_(t);         /* the current state */
    QS_END_()

    /* process the event hierarchically... */
    do {
        s = me->temp.fun;
        r = (*s)(me, e); /* invoke state handler s */

        if (r == (QState)Q_RET_UNHANDLED) { /* unhandled due to a guard? */

            QS_BEGIN_(QS_QEP_UNHANDLED, QS_priv_.locFilter[SM_OBJ], me)
                QS_SIG_(e->sig); /* the signal of the event */
                QS_OBJ_(me);     /* this state machine object */
                QS_FUN_(s);      /* the current state */
            QS_END_()

            r = QEP_TRIG_(s, QEP_EMPTY_SIG_); /* find superstate of s */
        }
    } while (r == (QState)Q_RET_SUPER);

    /* transition taken? */
    if (r >= (QState)Q_RET_TRAN) {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_];
        int_fast8_t ip;

        path[0] = me->temp.fun; /* save the target of the transition */
        path[1] = t;
        path[2] = s;

        /* exit current state to transition source s... */
        for (; t != s; t = me->temp.fun) {
            if (QEP_TRIG_(t, Q_EXIT_SIG) == (QState)Q_RET_HANDLED) {
                QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.locFilter[SM_OBJ], me)
                    QS_OBJ_(me);  /* this state machine object */
                    QS_FUN_(t);   /* the exited state */
                QS_END_()

                (void)QEP_TRIG_(t, QEP_EMPTY_SIG_); /* find superstate of t */
            }
        }

        ip = QHsm_tran_(me, path);

#ifdef Q_SPY
        if (r == (QState)Q_RET_TRAN_HIST) {

            QS_BEGIN_(QS_QEP_TRAN_HIST, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me);      /* this state machine object */
                QS_FUN_(t);       /* the source of the transition */
                QS_FUN_(path[0]); /* the target of the tran. to history */
            QS_END_()

        }
#endif /* Q_SPY */

        /* retrace the entry path in reverse (desired) order... */
        for (; ip >= (int_fast8_t)0; --ip) {
            QEP_ENTER_(path[ip]);  /* enter path[ip] */
        }

        t = path[0]; /* stick the target into register */
        me->temp.fun = t; /* update the next state */

        /* drill into the target hierarchy... */
        while (QEP_TRIG_(t, Q_INIT_SIG) == (QState)Q_RET_TRAN) {

            QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me);           /* this state machine object */
                QS_FUN_(t);            /* the source (pseudo)state */
                QS_FUN_(me->temp.fun); /* the target of the transition */
            QS_END_()

            ip = (int_fast8_t)0;
            path[0] = me->temp.fun;

            (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);/*find superstate */

            while (me->temp.fun != t) {
                ++ip;
                path[ip] = me->temp.fun;
                (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);/* find super */
            }
            me->temp.fun = path[0];

            /* entry path must not overflow */
            Q_ASSERT_ID(410, ip < (int_fast8_t)QHSM_MAX_NEST_DEPTH_);

            /* retrace the entry path in reverse (correct) order... */
            do {
                QEP_ENTER_(path[ip]); /* enter path[ip] */
                --ip;
            } while (ip >= (int_fast8_t)0);

            t = path[0]; /* current state becomes the new source */
        }

        QS_BEGIN_(QS_QEP_TRAN, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();          /* time stamp */
            QS_SIG_(e->sig);     /* the signal of the event */
            QS_OBJ_(me);         /* this state machine object */
            QS_FUN_(s);          /* the source of the transition */
            QS_FUN_(t);          /* the new active state */
        QS_END_()
    }

#ifdef Q_SPY
    else if (r == (QState)Q_RET_HANDLED) {

        QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();          /* time stamp */
            QS_SIG_(e->sig);     /* the signal of the event */
            QS_OBJ_(me);         /* this state machine object */
            QS_FUN_(s);          /* the source state */
        QS_END_()

    }
    else {

        QS_BEGIN_(QS_QEP_IGNORED, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();          /* time stamp */
            QS_SIG_(e->sig);     /* the signal of the event */
            QS_OBJ_(me);         /* this state machine object */
            QS_FUN_(me->state.fun); /* the current state */
        QS_END_()

    }
#endif /* Q_SPY */

    me->state.fun = t; /* change the current active state */
    me->temp.fun  = t; /* mark the configuration as stable */
}

/****************************************************************************/
/**
* @description
* Static helper function to execute transition sequence in a hierarchical
* state machine (HSM).
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in,out] path array of pointers to state-handler functions
*                     to execute the entry actions
* @returns the depth of the entry path stored in the @p path parameter.
*/
static int_fast8_t QHsm_tran_(QHsm * const me,
                              QStateHandler path[QHSM_MAX_NEST_DEPTH_])
{
    int_fast8_t ip = (int_fast8_t)(-1);/* transition entry path index */
    int_fast8_t iq; /* helper transition entry path index */
    QStateHandler t = path[0];
    QStateHandler s = path[2];
    QState r;
    QS_CRIT_STAT_

    /* (a) check source==target (transition to self)... */
    if (s == t) {
        QEP_EXIT_(s);        /* exit the source */
        ip = (int_fast8_t)0; /* enter the target */
    }
    else {
        (void)QEP_TRIG_(t, QEP_EMPTY_SIG_); /* find superstate of target */

        t = me->temp.fun;

        /* (b) check source==target->super... */
        if (s == t) {
            ip = (int_fast8_t)0; /* enter the target */
        }
        else {
            (void)QEP_TRIG_(s, QEP_EMPTY_SIG_); /* find superstate of src */

            /* (c) check source->super==target->super... */
            if (me->temp.fun == t) {
                QEP_EXIT_(s);        /* exit the source */
                ip = (int_fast8_t)0; /* enter the target */
            }
            else {
                /* (d) check source->super==target... */
                if (me->temp.fun == path[0]) {
                    QEP_EXIT_(s); /* exit the source */
                }
                else {
                    /* (e) check rest of source==target->super->super..
                    * and store the entry path along the way
                    */
                    iq = (int_fast8_t)0; /* indicate that LCA not found */
                    ip = (int_fast8_t)1; /* enter target and its superstate */
                    path[1] = t;         /* save the superstate of target */
                    t = me->temp.fun;    /* save source->super */

                    /* find target->super->super... */
                    r = QEP_TRIG_(path[1], QEP_EMPTY_SIG_);
                    while (r == (QState)Q_RET_SUPER) {
                        ++ip;
                        path[ip] = me->temp.fun; /* store the entry path */
                        if (me->temp.fun == s) { /* is it the source? */
                            iq = (int_fast8_t)1; /* indicate that LCA found */

                            /* entry path must not overflow */
                            Q_ASSERT_ID(510,
                                ip < (int_fast8_t)QHSM_MAX_NEST_DEPTH_);
                            --ip; /* do not enter the source */
                            r = (QState)Q_RET_HANDLED; /* terminate loop */
                        }
                         /* it is not the source, keep going up */
                        else {
                            r = QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
                        }
                    }

                    /* the LCA not found yet? */
                    if (iq == (int_fast8_t)0) {

                        /* entry path must not overflow */
                        Q_ASSERT_ID(520,
                            ip < (int_fast8_t)QHSM_MAX_NEST_DEPTH_);

                        QEP_EXIT_(s); /* exit the source */

                        /* (f) check the rest of source->super
                        *                  == target->super->super...
                        */
                        iq = ip;
                        r = (QState)Q_RET_IGNORED; /* LCA NOT found */
                        do {
                            if (t == path[iq]) { /* is this the LCA? */
                                r = (QState)Q_RET_HANDLED; /* LCA found */

                                /* do not enter LCA */
                                ip = (int_fast8_t)(iq - (int_fast8_t)1);
                                iq = (int_fast8_t)(-1);/*terminate the loop */
                            }
                            else {
                                --iq; /* try lower superstate of target */
                            }
                        } while (iq >= (int_fast8_t)0);

                        /* LCA not found? */
                        if (r != (QState)Q_RET_HANDLED) {
                            /* (g) check each source->super->...
                            * for each target->super...
                            */
                            r = (QState)Q_RET_IGNORED; /* keep looping */
                            do {
                                /* exit t unhandled? */
                                if (QEP_TRIG_(t, Q_EXIT_SIG)
                                    == (QState)Q_RET_HANDLED)
                                {
                                    QS_BEGIN_(QS_QEP_STATE_EXIT,
                                              QS_priv_.locFilter[SM_OBJ], me)
                                        QS_OBJ_(me);
                                        QS_FUN_(t);
                                    QS_END_()

                                    (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);
                                }
                                t = me->temp.fun; /* set to super of t */
                                iq = ip;
                                do {
                                    /* is this LCA? */
                                    if (t == path[iq]) {
                                        /* do not enter LCA */
                                        ip = (int_fast8_t)(iq-(int_fast8_t)1);
                                        /* cause breaking out of inner loop */
                                        iq = (int_fast8_t)(-1);
                                        r = (QState)Q_RET_HANDLED; /* break */
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= (int_fast8_t)0);
                            } while (r != (QState)Q_RET_HANDLED);
                        }
                    }
                }
            }
        }
    }
    return ip;
}

/****************************************************************************/
/**
* @description
* Tests if a state machine derived from QHsm is-in a given state.
*
* @note For a HSM, to "be in a state" means also to be in a superstate of
* of the state.
*
* @param[in] me    pointer (see @ref oop)
* @param[in] state pointer to the state-handler function to be tested
*
* @returns
*'true' if the HSM "is in" the @p state and 'false' otherwise
*/
bool QHsm_isIn(QHsm * const me, QStateHandler const state) {
    bool inState = false; /* assume that this HSM is not in 'state' */
    QState r;

    /** @pre the state configuration must be stable */
    Q_REQUIRE_ID(600, me->temp.fun == me->state.fun);

    do {
        /* do the states match? */
        if (me->temp.fun == state) {
            inState = true;            /* match found, return 'true' */
            r = (QState)Q_RET_IGNORED; /* break out of the loop */
        }
        else {
            r = QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
    } while (r != (QState)Q_RET_IGNORED); /* QHsm_top() state not reached */
    me->temp.fun = me->state.fun; /* restore the stable state configuration */

    return inState; /* return the status */
}

/****************************************************************************/
/**
* @description
* Finds the child state of the given @c parent, such that this child state
* is an ancestor of the currently active state. The main purpose of this
* function is to support **shallow history** transitions in state machines
* derived from QHsm.
*
* @param[in] me     pointer (see @ref oop)
* @param[in] parent pointer to the state-handler function
*
* @returns
* the child of a given @c parent state, which is an ancestor of the current
* active state. For the corner case when the currently active state is the
* given @c parent state, function returns the @c parent state.
*
* @note
* this function is designed to be called during state transitions, so it
* does not necessarily start in a stable state configuration.
* However, the function establishes stable state configuration upon exit.
*
* @sa QHsm_childState()
*/
QStateHandler QHsm_childState_(QHsm * const me,
                               QStateHandler const parent)
{
    QStateHandler child = me->state.fun; /* start with the current state */
    bool isFound = false; /* start with the child not found */
    QState r;

    /* establish stable state configuration */
    me->temp.fun = me->state.fun;
    do {
        /* is this the parent of the current child? */
        if (me->temp.fun == parent) {
            isFound = true; /* child is found */
            r = (QState)Q_RET_IGNORED; /* break out of the loop */
        }
        else {
            child = me->temp.fun;
            r = QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
    } while (r != (QState)Q_RET_IGNORED); /* QHsm_top() state not reached */
    me->temp.fun = me->state.fun; /* establish stable state configuration */

    /** @post the child must be found */
    Q_ENSURE_ID(810, isFound != false);

    return child; /* return the child */
}

