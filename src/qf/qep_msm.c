/**
* @file
* @brief ::QMsm implementation
* @ingroup qep
* @cond
******************************************************************************
* Last updated for version 6.3.0
* Last updated on  2018-05-03
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

Q_DEFINE_THIS_MODULE("qep_msm")

/****************************************************************************/
/*! internal QEP constants */
enum {
    /*! maximum depth of entry levels in a MSM for transition to history. */
    QMSM_MAX_ENTRY_DEPTH_ = 4
};

static QMState const l_msm_top_s = {
    (QMState const *)0,
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

/*! helper function to execute a transition-action table. */
static QState QMsm_execTatbl_(QMsm * const me, QMTranActTable const *tatbl);

/*! helper function to exit the current state up to the transition source */
static void QMsm_exitToTranSource_(QMsm * const me, QMState const *s,
                                   QMState const *ts);

/*! helper function to execute a transition to history */
static QState QMsm_enterHistory_(QMsm * const me, QMState const * const hist);


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
* in the QHsmVtbl, which in turn will pull in the code for QHsm_init_() and
* QHsm_dispatch_() implemetations. To avoid this code size penalty, in case
* ::QHsm is not used in a given project, the QMsm_ctor() performs direct
* intitialization of the Vtbl, which avoids pulling in the code for QMsm.
*
* @usage
* The following example illustrates how to invoke QMsm_ctor() in the
* "constructor" of a derived state machine:
* @include qep_qmsm_ctor.c
*/
void QMsm_ctor(QMsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = { /* QMsm virtual table */
        &QMsm_init_,
        &QMsm_dispatch_
    };
    /* do not call the QHsm_ctor() here */
    me->vptr = &vtbl;
    me->state.obj = &l_msm_top_s; /* the current state (top) */
    me->temp.fun  = initial;      /* the initial transition handler */
}

/****************************************************************************/
/**
* @description
* Executes the top-most initial transition in a MSM.
*
* @param[in,out] me pointer (see @ref oop)
* @param[in]     e  pointer to the initialization event (might be NULL)
*
* @note
* Must be called only ONCE after the QMsm_ctor().
*/
void QMsm_init_(QMsm * const me, QEvt const * const e) {
    QState r;
    QS_CRIT_STAT_

    /** @pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QMsmVtbl const *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (me->state.obj == &l_msm_top_s));

    r = (*me->temp.fun)(me, e); /* the action of the top-most initial tran. */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.locFilter[SM_OBJ], me)
        QS_OBJ_(me); /* this state machine object */
        QS_FUN_(me->state.obj->stateHandler);        /* source state handler*/
        QS_FUN_(me->temp.tatbl->target->stateHandler);/*target state handler*/
    QS_END_()

    /* set state to the last tran. target */
    me->state.obj = me->temp.tatbl->target;

    /* drill down into the state hierarchy with initial transitions... */
    do {
        r = QMsm_execTatbl_(me, me->temp.tatbl); /* execute the tran. table */
    } while (r >= (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.locFilter[SM_OBJ], me)
        QS_TIME_();                           /* time stamp */
        QS_OBJ_(me);                          /* this state machine object */
        QS_FUN_(me->state.obj->stateHandler); /* the new current state */
    QS_END_()
}

/****************************************************************************/
/**
* @description
* Dispatches an event for processing to a meta state machine (MSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* @param[in,out] me pointer (see @ref oop)
* @param[in]     e  pointer to the event to be dispatched to the MSM
*
* @note
* This function should be called only via the virtual table (see
* QHSM_DISPATCH()) and should NOT be called directly in the applications.
*/
void QMsm_dispatch_(QMsm * const me, QEvt const * const e) {
    QMState const *s = me->state.obj; /* store the current state */
    QMState const *t = s;
    QState r;
    QS_CRIT_STAT_

    /** @pre current state must be initialized */
    Q_REQUIRE_ID(300, s != (QMState const *)0);

    QS_BEGIN_(QS_QEP_DISPATCH, QS_priv_.locFilter[SM_OBJ], me)
        QS_TIME_();               /* time stamp */
        QS_SIG_(e->sig);          /* the signal of the event */
        QS_OBJ_(me);              /* this state machine object */
        QS_FUN_(s->stateHandler); /* the current state handler */
    QS_END_()

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

            QS_BEGIN_(QS_QEP_UNHANDLED, QS_priv_.locFilter[SM_OBJ], me)
                QS_SIG_(e->sig);  /* the signal of the event */
                QS_OBJ_(me);      /* this state machine object */
                QS_FUN_(t->stateHandler); /* the current state */
            QS_END_()

            t = t->superstate; /* advance to the superstate */
        }
        else {
            /* no other return value should be produced */
            Q_ERROR_ID(310);
        }
    } while (t != (QMState const *)0);


    /* any kind of transition taken? */
    if (r >= (QState)Q_RET_TRAN) {
#ifdef Q_SPY
        QMState const *ts = t; /* transition source for QS tracing */

        /* the transition source state must not be NULL */
        Q_ASSERT_ID(320, ts != (QMState const *)0);
#endif /* Q_SPY */

        do {
            /* save the transition-action table before it gets clobbered */
            QMTranActTable const *tatbl = me->temp.tatbl;
            union QHsmAttr tmp; /* temporary to save intermediate values */

            /* was TRAN, TRAN_INIT, or TRAN_EP taken? */
            if (r <= (QState)Q_RET_TRAN_EP) {
                QMsm_exitToTranSource_(me, s, t);
                r = QMsm_execTatbl_(me, tatbl);
                s = me->state.obj;
            }
            /* was a transition segment to history taken? */
            else if (r == (QState)Q_RET_TRAN_HIST) {
                tmp.obj = me->state.obj; /* save history */
                me->state.obj = s; /* restore the original state */
                QMsm_exitToTranSource_(me, s, t);
                (void)QMsm_execTatbl_(me, tatbl);
                r = QMsm_enterHistory_(me, tmp.obj);
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
#endif /* Q_SPY */
                    QMsm_exitToTranSource_(me, s, t);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl);
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
#endif /* Q_SPY */
                    QMsm_exitToTranSource_(me, me->state.obj, t);
                    /* take the tran-to-XP segment inside submachine */
                    (void)QMsm_execTatbl_(me, tatbl);
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

        QS_BEGIN_(QS_QEP_TRAN, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();                /* time stamp */
            QS_SIG_(e->sig);           /* the signal of the event */
            QS_OBJ_(me);               /* this state machine object */
            QS_FUN_(ts->stateHandler); /* the transition source */
            QS_FUN_(s->stateHandler);  /* the new active state */
        QS_END_()
    }

#ifdef Q_SPY
    /* was the event handled? */
    else if (r == (QState)Q_RET_HANDLED) {
        /* internal tran. source can't be NULL */
        Q_ASSERT_ID(340, t != (QMState const *)0);

        QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();                /* time stamp */
            QS_SIG_(e->sig);           /* the signal of the event */
            QS_OBJ_(me);               /* this state machine object */
            QS_FUN_(t->stateHandler);  /* the source state */
        QS_END_()

    }
    /* event bubbled to the 'top' state? */
    else if (t == (QMState const *)0) {

        QS_BEGIN_(QS_QEP_IGNORED, QS_priv_.locFilter[SM_OBJ], me)
            QS_TIME_();                /* time stamp */
            QS_SIG_(e->sig);           /* the signal of the event */
            QS_OBJ_(me);               /* this state machine object */
            QS_FUN_(s->stateHandler);  /* the current state */
        QS_END_()

    }
#endif /* Q_SPY */
    else {
        /* empty */
    }
}

/****************************************************************************/
/**
* @description
* Helper function to execute transition sequence in a transition-action table.
*
* @param[in,out] me    pointer (see @ref oop)
* @param[in]     tatbl pointer to the transition-action table
*
* @returns
* status of the last action from the transition-action table.
*
* @note
* This function is for internal use inside the QEP event processor and
* should __not__ be called directly from the applications.
*/
static QState QMsm_execTatbl_(QMsm * const me, QMTranActTable const *tatbl) {
    QActionHandler const *a;
    QState r = (QState)Q_RET_NULL;
    QS_CRIT_STAT_

    /** @pre the transition-action table pointer must not be NULL */
    Q_REQUIRE_ID(400, tatbl != (QMTranActTable const *)0);

    for (a = &tatbl->act[0]; *a != Q_ACTION_CAST(0); QEP_ACT_PTR_INC_(a)) {
        r = (*(*a))(me); /* call the action through the 'a' pointer */
#ifdef Q_SPY
        if (r == (QState)Q_RET_ENTRY) {

            QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(me->temp.obj->stateHandler);/*entered state handler */
            QS_END_()
        }
        else if (r == (QState)Q_RET_EXIT) {

            QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(me->temp.obj->stateHandler); /* exited state handler*/
            QS_END_()
        }
        else if (r == (QState)Q_RET_TRAN_INIT) {

            QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(tatbl->target->stateHandler);         /* source */
                QS_FUN_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_()
        }
        else if (r == (QState)Q_RET_TRAN_EP) {

            QS_BEGIN_(QS_QEP_TRAN_EP, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(tatbl->target->stateHandler);         /* source */
                QS_FUN_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_()
        }
        else if (r == (QState)Q_RET_TRAN_XP) {

            QS_BEGIN_(QS_QEP_TRAN_XP, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(tatbl->target->stateHandler);         /* source */
                QS_FUN_(me->temp.tatbl->target->stateHandler);/* target */
            QS_END_()
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
* @description
* Static helper function to exit the current state configuration to the
* transition source, which in a hierarchical state machine might be a
* superstate of the current state.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     s    pointer to the current state
* @param[in]     ts   pointer to the transition source state
*/
static void QMsm_exitToTranSource_(QMsm * const me, QMState const *s,
                                   QMState const *ts)
{
    /* exit states from the current state to the tran. source state */
    while (s != ts) {
        /* exit action provided in state 's'? */
        if (s->exitAction != Q_ACTION_CAST(0)) {
            QS_CRIT_STAT_

            (void)(*s->exitAction)(me); /* execute the exit action */

            QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.locFilter[SM_OBJ], me)
                QS_OBJ_(me);              /* this state machine object */
                QS_FUN_(s->stateHandler); /* the exited state handler */
            QS_END_()
        }

        s = s->superstate; /* advance to the superstate */

        if (s == (QMState const *)0) { /* reached the top of a submachine? */
            s = me->temp.obj; /* the superstate from QM_SM_EXIT() */
            Q_ASSERT_ID(510, s != (QMState const *)0); /* must be valid */
        }
    }
}

/****************************************************************************/
/**
* @description
* Static helper function to execute the segment of transition to history
* after entering the composite state and
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     hist pointer to the history substate
*
* @returns
* #Q_RET_INIT, if an initial transition has been executed in the last entered
* state or #Q_RET_NULL if no such transition was taken.
*/
static QState QMsm_enterHistory_(QMsm * const me, QMState const *const hist) {
    QMState const *s = hist;
    QMState const *ts = me->state.obj; /* transition source */
    QMState const *epath[QMSM_MAX_ENTRY_DEPTH_];
    QState r;
    uint_fast8_t i = (uint_fast8_t)0;  /* transition entry path index */
    QS_CRIT_STAT_

    QS_BEGIN_(QS_QEP_TRAN_HIST, QS_priv_.locFilter[SM_OBJ], me)
        QS_OBJ_(me);                 /* this state machine object */
        QS_FUN_(ts->stateHandler);   /* source state handler */
        QS_FUN_(hist->stateHandler); /* target state handler */
    QS_END_()

    while (s != ts) {
        if (s->entryAction != (QActionHandler)0) {
            epath[i] = s;
            ++i;
            Q_ASSERT_ID(620, i <= (uint_fast8_t)Q_DIM(epath));
        }
        s = s->superstate;
        if (s == 0) {
            ts = s; /* force exit from the for-loop */
        }
    }

    /* retrace the entry path in reverse (desired) order... */
    while (i > (uint_fast8_t)0) {
        --i;
        r = (*epath[i]->entryAction)(me); /* run entry action in epath[i] */

        QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.locFilter[SM_OBJ], me)
            QS_OBJ_(me);
            QS_FUN_(epath[i]->stateHandler); /* entered state handler */
        QS_END_()
    }

    me->state.obj = hist; /* set current state to the transition target */

    /* initial tran. present? */
    if (hist->initAction != (QActionHandler)0) {
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

    for (s = me->state.obj; s != (QMState const *)0; s = s->superstate) {
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
    QMState const *child = me->state.obj;
    bool isFound = false; /* start with the child not found */
    QMState const *s;

    for (s = me->state.obj;
         s != (QMState const *)0;
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

    return child; /* return the child */
}

