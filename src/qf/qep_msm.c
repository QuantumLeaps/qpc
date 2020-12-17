/**
* @file
* @brief ::QMsm implementation
* @ingroup qep
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2020-12-16
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
#include "qep_port.h"     /* QEP port */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qep_msm")

/****************************************************************************/
/*! internal QEP constants */
enum {
    /*! maximum depth of entry levels in a MSM for transition to history. */
    QMSM_MAX_ENTRY_DEPTH_ = 4
};

static struct QMState const l_msm_top_s = {
    (struct QMState *)0,
    Q_STATE_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0)
};

/*! Internal QEP macro to increment the given action table @p act_ */
/**
* @note
* Incrementing a pointer violates the MISRA-C 2004 Rule 17.4(req),
* pointer arithmetic other than array indexing. Encapsulating this violation
* in a macro allows to selectively suppress this specific deviation.
*/
#define QEP_ACT_PTR_INC_(act_) (++(act_))

/*! helper function to execute a transition-action table.
* @private @memberof QMsm
*/
#ifdef Q_SPY
static QState QMsm_execTatbl_(QHsm * const me,
                              struct QMTranActTable const *tatbl,
                              uint_fast8_t const qs_id);
#else
static QState QMsm_execTatbl_(QHsm * const me,
                              struct QMTranActTable const *tatbl);
#endif

/*! helper function to exit the current state up to the transition source
* @private @memberof QMsm
*/
#ifdef Q_SPY
static void QMsm_exitToTranSource_(QHsm * const me, QMState const *cs,
                                   QMState const *ts,
                                   uint_fast8_t const qs_id);
#else
static void QMsm_exitToTranSource_(QHsm * const me, QMState const *cs,
                                   QMState const *ts);
#endif

/*! helper function to execute a transition to history
* @private @memberof QMsm
*/
#ifdef Q_SPY
static QState QMsm_enterHistory_(QHsm * const me, QMState const * const hist,
                                 uint_fast8_t const qs_id);
#else
static QState QMsm_enterHistory_(QHsm * const me, QMState const * const hist);
#endif

/****************************************************************************/
/**
* @description
* Performs the first step of QMsm initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     initial  the top-most initial transition for the MSM.
*
* @note
* Must be called only ONCE before QHSM_INIT().
*
* @note
* QMsm inherits QHsm, so by the @ref oop convention it should call the
* constructor of the superclass, i.e., QHsm_ctor(). However, this would pull
* in the QHsmVtable, which in turn will pull in the code for QHsm_init_() and
* QHsm_dispatch_() implemetations. To avoid this code size penalty, in case
* ::QHsm is not used in a given project, the QMsm_ctor() performs direct
* intitialization of the Vtable, which avoids pulling in the code for QMsm.
*
* @usage
* The following example illustrates how to invoke QMsm_ctor() in the
* "constructor" of a derived state machine:
* @include qep_qmsm_ctor.c
*/
void QMsm_ctor(QMsm * const me, QStateHandler initial) {
    static struct QHsmVtable const vtable = { /* QHsm virtual table */
        &QMsm_init_,
        &QMsm_dispatch_
#ifdef Q_SPY
        ,&QMsm_getStateHandler_
#endif
    };
    /* do not call the QHsm_ctor() here */
    me->super.vptr = &vtable;
    me->super.state.obj = &l_msm_top_s; /* the current state (top) */
    me->super.temp.fun  = initial;      /* the initial transition handler */
}

/****************************************************************************/
/**
* @public @memberof QMsm
* @description
* Executes the top-most initial transition in a MSM.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     e   pointer to an extra parameter (might be NULL)
* @param[in]     qs_id QS-id of this state machine (for QS local filter)
*
* @note
* Must be called only ONCE after the QMsm_ctor().
*/
#ifdef Q_SPY
void QMsm_init_(QHsm * const me, void const * const e,
                uint_fast8_t const qs_id)
#else
void QMsm_init_(QHsm * const me, void const * const e)
#endif
{
    QState r;
    QS_CRIT_STAT_

    /** @pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (struct QHsmVtable *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (me->state.obj == &l_msm_top_s));

    /* execute the top-most initial tran. */
    r = (*me->temp.fun)(me, Q_EVT_CAST(QEvt));

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
        QS_OBJ_PRE_(me); /* this state machine object */
        QS_FUN_PRE_(me->state.obj->stateHandler);        /* source state */
        QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);/*target state */
    QS_END_PRE_()

    /* set state to the last tran. target */
    me->state.obj = me->temp.tatbl->target;

    /* drill down into the state hierarchy with initial transitions... */
    /* execute the tran. table */
    do {
#ifdef Q_SPY
        r = QMsm_execTatbl_(me, me->temp.tatbl, qs_id);
#else
        r = QMsm_execTatbl_(me, me->temp.tatbl);
#endif
    } while (r >= (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_PRE_(QS_QEP_INIT_TRAN, qs_id)
        QS_TIME_PRE_();   /* time stamp */
        QS_OBJ_PRE_(me);  /* this state machine object */
        QS_FUN_PRE_(me->state.obj->stateHandler); /* the new current state */
    QS_END_PRE_()
}

/****************************************************************************/
/**
* @private @memberof QMsm
* @description
* Dispatches an event for processing to a meta state machine (MSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* @param[in,out] me pointer (see @ref oop)
* @param[in]     e  pointer to the event to be dispatched to the MSM
* @param[in]     qs_id QS-id of this state machine (for QS local filter)
*
* @note
* This function should be called only via the virtual table (see
* QHSM_DISPATCH()) and should NOT be called directly in the applications.
*/
#ifdef Q_SPY
void QMsm_dispatch_(QHsm * const me, QEvt const * const e,
                    uint_fast8_t const qs_id)
#else
void QMsm_dispatch_(QHsm * const me, QEvt const * const e)
#endif
{
    QMState const *s = me->state.obj; /* store the current state */
    QMState const *t = s;
    QState r;
    QS_CRIT_STAT_

    /** @pre current state must be initialized */
    Q_REQUIRE_ID(300, s != (QMState *)0);

    QS_BEGIN_PRE_(QS_QEP_DISPATCH, qs_id)
        QS_TIME_PRE_();               /* time stamp */
        QS_SIG_PRE_(e->sig);          /* the signal of the event */
        QS_OBJ_PRE_(me);              /* this state machine object */
        QS_FUN_PRE_(s->stateHandler); /* the current state handler */
    QS_END_PRE_()

    /* scan the state hierarchy up to the top state... */
    do {
        r = (*t->stateHandler)(me, e);  /* call state handler function */

        /* event handled? (the most frequent case) */
        if (r >= (QState)Q_RET_HANDLED) {
            break; /* done scanning the state hierarchy */
        }
        /* event unhandled and passed to the superstate? */
        else if (r == (QState)Q_RET_SUPER) {
            t = t->superstate; /* advance to the superstate */
        }
        /* event unhandled and passed to a submachine superstate? */
        else if (r == (QState)Q_RET_SUPER_SUB) {
            t = me->temp.obj; /* current host state of the submachie */
        }
        /* event unhandled due to a guard? */
        else if (r == (QState)Q_RET_UNHANDLED) {

            QS_BEGIN_PRE_(QS_QEP_UNHANDLED, qs_id)
                QS_SIG_PRE_(e->sig);  /* the signal of the event */
                QS_OBJ_PRE_(me);      /* this state machine object */
                QS_FUN_PRE_(t->stateHandler); /* the current state */
            QS_END_PRE_()

            t = t->superstate; /* advance to the superstate */
        }
        else {
            /* no other return value should be produced */
            Q_ERROR_ID(310);
        }
    } while (t != (QMState *)0);


    /* any kind of transition taken? */
    if (r >= (QState)Q_RET_TRAN) {
#ifdef Q_SPY
        QMState const *ts = t; /* transition source for QS tracing */

        /* the transition source state must not be NULL */
        Q_ASSERT_ID(320, ts != (QMState *)0);
#endif /* Q_SPY */

        do {
            /* save the transition-action table before it gets clobbered */
            struct QMTranActTable const *tatbl = me->temp.tatbl;
            union QHsmAttr tmp; /* temporary to save intermediate values */

            /* was TRAN, TRAN_INIT, or TRAN_EP taken? */
            if (r <= (QState)Q_RET_TRAN_EP) {
#ifdef Q_SPY
                QMsm_exitToTranSource_(me, s, t, qs_id);
                r = QMsm_execTatbl_(me, tatbl, qs_id);
#else
                QMsm_exitToTranSource_(me, s, t);
                r = QMsm_execTatbl_(me, tatbl);
#endif
                s = me->state.obj;
            }
            /* was a transition segment to history taken? */
            else if (r == (QState)Q_RET_TRAN_HIST) {
                tmp.obj = me->state.obj; /* save history */
                me->state.obj = s; /* restore the original state */
#ifdef Q_SPY
                QMsm_exitToTranSource_(me, s, t, qs_id);
                (void)QMsm_execTatbl_(me, tatbl, qs_id);
                r = QMsm_enterHistory_(me, tmp.obj, qs_id);
#else
                QMsm_exitToTranSource_(me, s, t);
                (void)QMsm_execTatbl_(me, tatbl);
                r = QMsm_enterHistory_(me, tmp.obj);
#endif
                s = me->state.obj;
            }
            /* was a transition segment to an exit point taken? */
            else if (r == (QState)Q_RET_TRAN_XP) {
                tmp.act = me->state.act; /* save XP action */
                me->state.obj = s; /* restore the original state */
                r = (*tmp.act)(me); /* execute the XP action */
                if (r == (QState)Q_RET_TRAN) { /* XP -> TRAN ? */
#ifdef Q_SPY
                    tmp.tatbl = me->temp.tatbl; /* save me->temp */
                    QMsm_exitToTranSource_(me, s, t, qs_id);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl, qs_id);
#else
                    QMsm_exitToTranSource_(me, s, t);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl);
#endif /* Q_SPY */
                    s = me->state.obj;
#ifdef Q_SPY
                    me->temp.tatbl = tmp.tatbl; /* restore me->temp */
#endif /* Q_SPY */
                }
                else if (r == (QState)Q_RET_TRAN_HIST) { /* XP -> HIST ? */
                    tmp.obj = me->state.obj; /* save the history */
                    me->state.obj = s; /* restore the original state */
#ifdef Q_SPY
                    s = me->temp.obj; /* save me->temp */
                    QMsm_exitToTranSource_(me, me->state.obj, t, qs_id);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl, qs_id);
#else
                    QMsm_exitToTranSource_(me, me->state.obj, t);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl);
#endif /* Q_SPY */
#ifdef Q_SPY
                    me->temp.obj = s; /* restore me->temp */
#endif /* Q_SPY */
                    s = me->state.obj;
                    me->state.obj = tmp.obj; /* restore the history */
                }
                else {
                    /* TRAN_XP must NOT be followed by any other tran type */
                    Q_ASSERT_ID(330, r < (QState)Q_RET_TRAN);
                }
            }
            else {
                /* no other return value should be produced */
                Q_ERROR_ID(340);
            }

            t = s; /* set target to the current state */

        } while (r >= (QState)Q_RET_TRAN);

        QS_BEGIN_PRE_(QS_QEP_TRAN, qs_id)
            QS_TIME_PRE_();                /* time stamp */
            QS_SIG_PRE_(e->sig);           /* the signal of the event */
            QS_OBJ_PRE_(me);               /* this state machine object */
            QS_FUN_PRE_(ts->stateHandler); /* the transition source */
            QS_FUN_PRE_(s->stateHandler);  /* the new active state */
        QS_END_PRE_()
    }

#ifdef Q_SPY
    /* was the event handled? */
    else if (r == (QState)Q_RET_HANDLED) {
        /* internal tran. source can't be NULL */
        Q_ASSERT_ID(340, t != (QMState *)0);

        QS_BEGIN_PRE_(QS_QEP_INTERN_TRAN, qs_id)
            QS_TIME_PRE_();                /* time stamp */
            QS_SIG_PRE_(e->sig);           /* the signal of the event */
            QS_OBJ_PRE_(me);               /* this state machine object */
            QS_FUN_PRE_(t->stateHandler);  /* the source state */
        QS_END_PRE_()

    }
    /* event bubbled to the 'top' state? */
    else if (t == (QMState *)0) {

        QS_BEGIN_PRE_(QS_QEP_IGNORED, qs_id)
            QS_TIME_PRE_();                /* time stamp */
            QS_SIG_PRE_(e->sig);           /* the signal of the event */
            QS_OBJ_PRE_(me);               /* this state machine object */
            QS_FUN_PRE_(s->stateHandler);  /* the current state */
        QS_END_PRE_()

    }
#endif /* Q_SPY */
    else {
        /* empty */
    }
}

/****************************************************************************/
#ifdef Q_SPY
QStateHandler QMsm_getStateHandler_(QHsm * const me) {
    return me->state.obj->stateHandler;
}
#endif

/****************************************************************************/
/**
* @private @memberof QMsm
* @description
* Helper function to execute transition sequence in a transition-action table.
*
* @param[in,out] me    pointer (see @ref oop)
* @param[in]     tatbl pointer to the transition-action table
* @param[in]     qs_id QS-id of this state machine (for QS local filter)
*
* @returns
* status of the last action from the transition-action table.
*
* @note
* This function is for internal use inside the QEP event processor and
* should __not__ be called directly from the applications.
*/
#ifdef Q_SPY
static QState QMsm_execTatbl_(QHsm * const me,
                              struct QMTranActTable const *tatbl,
                              uint_fast8_t const qs_id)
#else
static QState QMsm_execTatbl_(QHsm * const me,
                              struct QMTranActTable const *tatbl)
#endif
{
    QActionHandler const *a;
    QState r = (QState)Q_RET_NULL;
    QS_CRIT_STAT_

    /** @pre the transition-action table pointer must not be NULL */
    Q_REQUIRE_ID(400, tatbl != (struct QMTranActTable *)0);

    for (a = &tatbl->act[0]; *a != Q_ACTION_CAST(0); QEP_ACT_PTR_INC_(a)) {
        r = (*(*a))(me); /* call the action through the 'a' pointer */
#ifdef Q_SPY
        if (r == (QState)Q_RET_ENTRY) {

            QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, qs_id)
                QS_OBJ_PRE_(me); /* this state machine object */
                QS_FUN_PRE_(me->temp.obj->stateHandler);/*entered state handler */
            QS_END_PRE_()
        }
        else if (r == (QState)Q_RET_EXIT) {

            QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, qs_id)
                QS_OBJ_PRE_(me); /* this state machine object */
                QS_FUN_PRE_(me->temp.obj->stateHandler); /* exited state handler*/
            QS_END_PRE_()
        }
        else if (r == (QState)Q_RET_TRAN_INIT) {

            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(me); /* this state machine object */
                QS_FUN_PRE_(tatbl->target->stateHandler);         /* source */
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_PRE_()
        }
        else if (r == (QState)Q_RET_TRAN_EP) {

            QS_BEGIN_PRE_(QS_QEP_TRAN_EP, qs_id)
                QS_OBJ_PRE_(me); /* this state machine object */
                QS_FUN_PRE_(tatbl->target->stateHandler);         /* source */
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_PRE_()
        }
        else if (r == (QState)Q_RET_TRAN_XP) {

            QS_BEGIN_PRE_(QS_QEP_TRAN_XP, qs_id)
                QS_OBJ_PRE_(me); /* this state machine object */
                QS_FUN_PRE_(tatbl->target->stateHandler);         /* source */
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_PRE_()
        }
        else {
            /* empty */
        }
#endif /* Q_SPY */
    }

    me->state.obj = (r >= (QState)Q_RET_TRAN)
        ? me->temp.tatbl->target
        : tatbl->target;
    return r;
}

/****************************************************************************/
/**
* @private @memberof QMsm
* @description
* Static helper function to exit the current state configuration to the
* transition source, which in a hierarchical state machine might be a
* superstate of the current state.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     cs   pointer to the current state
* @param[in]     ts   pointer to the transition source state
* @param[in]     qs_id QS-id of this state machine (for QS local filter)
*/
#ifdef Q_SPY
static void QMsm_exitToTranSource_(QHsm * const me, QMState const *cs,
                                   QMState const *ts,
                                   uint_fast8_t const qs_id)
#else
static void QMsm_exitToTranSource_(QHsm * const me, QMState const *cs,
                                   QMState const *ts)
#endif
{
    QMState const *s = cs;

    /* exit states from the current state to the tran. source state */
    while (s != ts) {
        /* exit action provided in state 's'? */
        if (s->exitAction != Q_ACTION_CAST(0)) {
            QS_CRIT_STAT_

            (void)(*s->exitAction)(me); /* execute the exit action */

            QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, qs_id)
                QS_OBJ_PRE_(me);              /* this state machine object */
                QS_FUN_PRE_(s->stateHandler); /* the exited state handler */
            QS_END_PRE_()
        }

        s = s->superstate; /* advance to the superstate */

        if (s == (QMState *)0) { /* reached the top of a submachine? */
            s = me->temp.obj; /* the superstate from QM_SM_EXIT() */
            Q_ASSERT_ID(510, s != (QMState *)0); /* must be valid */
        }
    }
}

/****************************************************************************/
/**
* @private @memberof QMsm
* @description
* Static helper function to execute the segment of transition to history
* after entering the composite state and
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     hist pointer to the history substate
* @param[in]     qs_id QS-id of this state machine (for QS local filter)
*
* @returns
* #Q_RET_TRAN_INIT, if an initial transition has been executed in the last
* entered state or #Q_RET_NULL if no such transition was taken.
*/
#ifdef Q_SPY
static QState QMsm_enterHistory_(QHsm * const me, QMState const *const hist,
                                 uint_fast8_t const qs_id)
#else
static QState QMsm_enterHistory_(QHsm * const me, QMState const *const hist)
#endif
{
    QMState const *s = hist;
    QMState const *ts = me->state.obj; /* transition source */
    QMState const *epath[QMSM_MAX_ENTRY_DEPTH_];
    QState r;
    uint_fast8_t i = 0U;  /* transition entry path index */
    QS_CRIT_STAT_

    QS_BEGIN_PRE_(QS_QEP_TRAN_HIST, qs_id)
        QS_OBJ_PRE_(me);                 /* this state machine object */
        QS_FUN_PRE_(ts->stateHandler);   /* source state handler */
        QS_FUN_PRE_(hist->stateHandler); /* target state handler */
    QS_END_PRE_()

    while (s != ts) {
        if (s->entryAction != Q_ACTION_CAST(0)) {
            epath[i] = s;
            ++i;
            Q_ASSERT_ID(620, i <= Q_DIM(epath));
        }
        s = s->superstate;
        if (s == (QMState *)0) {
            ts = s; /* force exit from the for-loop */
        }
    }

    /* retrace the entry path in reverse (desired) order... */
    while (i > 0U) {
        --i;
        (void)(*epath[i]->entryAction)(me); /* run entry action in epath[i] */

        QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, qs_id)
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(epath[i]->stateHandler); /* entered state handler */
        QS_END_PRE_()
    }

    me->state.obj = hist; /* set current state to the transition target */

    /* initial tran. present? */
    if (hist->initAction != Q_ACTION_CAST(0)) {
        r = (*hist->initAction)(me); /* execute the transition action */
    }
    else {
        r = (QState)Q_RET_NULL;
    }
    return r;
}

/****************************************************************************/
/**
* @description
* Tests if a state machine derived from QMsm is-in a given state.
*
* @note
* For a MSM, to "be-in" a state means also to "be-in" a superstate of
* of the state.
*
* @param[in] me    pointer (see @ref oop)
* @param[in] state pointer to the QMState object that corresponds to the
*                  tested state.
* @returns
* 'true' if the MSM "is in" the @p state and 'false' otherwise
*/
bool QMsm_isInState(QMsm const * const me, QMState const * const state) {
    bool inState = false; /* assume that this MSM is not in 'state' */
    QMState const *s;

    for (s = me->super.state.obj; s != (QMState *)0; s = s->superstate) {
        if (s == state) {
            inState = true; /* match found, return 'true' */
            break;
        }
    }
    return inState;
}

/****************************************************************************/
/**
* @description
* Finds the child state of the given @c parent, such that this child state
* is an ancestor of the currently active state. The main purpose of this
* function is to support **shallow history** transitions in state machines
* derived from QMsm.
*
* @param[in] me     pointer (see @ref oop)
* @param[in] parent pointer to the state-handler object
*
* @returns
* the child of a given @c parent state, which is an ancestor of
* the currently active state. For the corner case when the currently active
* state is the given @c parent state, function returns the @c parent state.
*
* @sa QMsm_childStateObj()
*/
QMState const *QMsm_childStateObj_(QMsm const * const me,
                                   QMState const * const parent)
{
    QMState const *child = me->super.state.obj;
    bool isFound = false; /* start with the child not found */
    QMState const *s;

    for (s = me->super.state.obj;
         s != (QMState *)0;
         s = s->superstate)
    {
        if (s == parent) {
            isFound = true; /* child is found */
            break;
        }
        else {
            child = s;
        }
    }

    /** @post the child must be found */
    Q_ENSURE_ID(810, isFound != false);
#ifdef Q_NASSERT
    (void)isFound; /* avoid compiler warning about unused variable */
#endif

    return child; /* return the child */
}

