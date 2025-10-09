//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qep_msm")

// maximum depth of state nesting in a QMsm (including the top level)
#define QMSM_MAX_NEST_DEPTH_  ((int_fast8_t)6)

//! @cond INTERNAL

// top-state object for QMsm-style state machines
static struct QMState const l_msm_top_s = {
    (struct QMState *)0,
    Q_STATE_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0)
};

#ifdef Q_SPY
// helper macro to trace state action (entry/exit)
#define QS_STATE_ACT_(rec_, state_) \
    QS_CRIT_ENTRY();           \
    QS_BEGIN_PRE((rec_), qsId) \
        QS_OBJ_PRE(me);        \
        QS_FUN_PRE(state_);    \
    QS_END_PRE()               \
    QS_CRIT_EXIT()

// internal helper macro to top-most init
#define QS_TOP_INIT_(rec_, trg_) \
    QS_CRIT_ENTRY();           \
    QS_BEGIN_PRE((rec_), qsId) \
        QS_TIME_PRE();         \
        QS_OBJ_PRE(me);        \
        QS_FUN_PRE(trg_);      \
    QS_END_PRE()               \
    QS_CRIT_EXIT()

// internal helper macro to trace transition segment
#define QS_TRAN_SEG_(rec_, src_, trg_) \
    QS_CRIT_ENTRY();           \
    QS_BEGIN_PRE((rec_), qsId) \
        QS_OBJ_PRE(me);        \
        QS_FUN_PRE(src_);      \
        QS_FUN_PRE(trg_);      \
    QS_END_PRE()               \
    QS_CRIT_EXIT()

// internal helper macro to trace transition begin/end
#define QS_TRAN0_(rec_, trg_)  \
    QS_CRIT_ENTRY();           \
    QS_BEGIN_PRE((rec_), qsId) \
        QS_TIME_PRE();         \
        QS_SIG_PRE(e->sig);    \
        QS_OBJ_PRE(me);        \
        QS_FUN_PRE(trg_);      \
    QS_END_PRE()               \
    QS_CRIT_EXIT()

// internal helper macro to trace regular transition
#define QS_TRAN_END_(rec_, src_, trg_) \
    QS_CRIT_ENTRY();           \
    QS_BEGIN_PRE((rec_), qsId) \
        QS_TIME_PRE();         \
        QS_SIG_PRE(e->sig);    \
        QS_OBJ_PRE(me);        \
        QS_FUN_PRE(src_);      \
        QS_FUN_PRE(trg_);      \
    QS_END_PRE()               \
    QS_CRIT_EXIT()

#else
#define QS_STATE_ACT_(rec_, state_)     ((void)0)
#define QS_TOP_INIT_(rec_, trg_)        ((void)0)
#define QS_TRAN_SEG_(rec_, src_, trg_)  ((void)0)
#define QS_TRAN0_(rec_, trg_)           ((void)0)
#define QS_TRAN_END_(rec_, src_, trg_)  ((void)0)
#endif

//! @endcond

//============================================================================
// static and private helper function prototypes...

static QState QMsm_execTatbl_(
    QAsm * const me,
    QMTranActTable const * const tatbl,
    uint_fast8_t const qsId);

static void QMsm_exitToTranSource_(
    QAsm * const me,
    QMState const * const curr_state,
    QMState const * const tran_source,
    uint_fast8_t const qsId);

static QState QMsm_enterHistory_(
    QAsm * const me,
    QMState const *const hist,
    uint_fast8_t const qsId);

//============================================================================
//! @protected @memberof QMsm
void QMsm_ctor(QMsm * const me,
    QStateHandler const initial)
{
    static struct QAsmVtable const vtable = { // QAsm virtual table
        &QMsm_init_,
        &QMsm_dispatch_,
        &QMsm_isIn_,
        &QMsm_getStateHandler_
    };
    // no need to call the superclass' constructor QAsm_ctor() here
    me->super.vptr      = &vtable;      // QMsm class' VTABLE
    me->super.state.obj = &l_msm_top_s; // the current state (top)
    me->super.temp.fun  = initial;      // the initial tran. handler
}

//............................................................................
//! @private @memberof QMsm
void QMsm_init_(
    QAsm * const me,
    void const * const e,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    // current state must be initialized to &l_msm_top_s in QMsm_ctor()
    Q_REQUIRE_LOCAL(200, me->state.obj == &l_msm_top_s);

    // temp contains the top-most initial tran. handler, which must be valid
    Q_REQUIRE_LOCAL(210, me->temp.fun != Q_STATE_CAST(0));

    // execute the top-most initial tran.
    QState r = (*me->temp.fun)(me, Q_EVT_CAST(QEvt));

    // the top-most initial tran. must be taken
    Q_ASSERT_LOCAL(240, r == Q_RET_TRAN_INIT);

    // the top-most initial tran. must set the tran-action table in temp
    Q_ASSERT_LOCAL(250, me->temp.tatbl != (struct QMTranActTable *)0);

    QS_CRIT_STAT
    QS_TRAN_SEG_(QS_QEP_STATE_INIT,
       me->state.obj->stateHandler, me->temp.tatbl->target->stateHandler);

    // set state to the last tran. target
    me->state.obj = me->temp.tatbl->target;

    // drill down into the state hierarchy with initial transitions...
    do {
        r = QMsm_execTatbl_(me, me->temp.tatbl, qsId);
    } while (r == Q_RET_TRAN_INIT);

    QS_TOP_INIT_(QS_QEP_INIT_TRAN, me->state.obj->stateHandler);

#ifndef Q_UNSAFE
    // establish stable state configuration at the end of RTC step
    me->temp.uint = QP_DIS_UPDATE(uintptr_t, me->state.uint);
#endif
}

//............................................................................
//! @private @memberof QMsm
void QMsm_dispatch_(
    QAsm * const me,
    QEvt const * const e,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    // this state machine must be in a stable state configuration
    // NOTE: stable state configuration is established after every RTC step.
    Q_INVARIANT_LOCAL(300,
        QP_DIS_VERIFY(uintptr_t, me->state.uint, me->temp.uint));

    // the event to be dispatched must be valid
    Q_REQUIRE_LOCAL(310, e != (QEvt *)0);

    QMState const *s = me->state.obj; // the current state
    QMState const * const t = s; // store the current state for later
    QS_CRIT_STAT
    QS_TRAN0_(QS_QEP_DISPATCH, s->stateHandler);

    // scan the state hierarchy up to the top state...
    QState r;
    do {
        r = (*s->stateHandler)(me, e); // call state handler function
        if (r >= Q_RET_HANDLED) { // event handled? (the most frequent case)
            break; // done scanning the state hierarchy
        }
#ifdef Q_SPY
        if (r == Q_RET_UNHANDLED) { // event unhandled due to a guard?
            QS_CRIT_ENTRY();
            QS_BEGIN_PRE(QS_QEP_UNHANDLED, qsId)
                QS_SIG_PRE(e->sig);
                QS_OBJ_PRE(me);
                QS_FUN_PRE(s->stateHandler);
            QS_END_PRE()
            QS_CRIT_EXIT();
        }
#endif
        s = s->superstate; // advance to the superstate

    } while (s != (QMState *)0);

    if (s == (QMState *)0) { // event bubbled to the 'top' state?
#ifdef Q_SPY
        QS_TRAN0_(QS_QEP_IGNORED, t->stateHandler);
#endif
    }
    else if (r == Q_RET_HANDLED) { // was the event e handled?
        QS_TRAN0_(QS_QEP_INTERN_TRAN, s->stateHandler); // output QS record
    }
    else if ((r == Q_RET_TRAN) || (r == Q_RET_TRAN_HIST)) { //any tran. taken?
#ifdef Q_SPY
        QMState const * const ts = s; // tran. source for QS tracing
#endif // Q_SPY

        if (r == Q_RET_TRAN) { // tran. taken?
            struct QMTranActTable const * const tatbl = me->temp.tatbl;
            QMsm_exitToTranSource_(me, t, s, qsId);
            r = QMsm_execTatbl_(me, tatbl, qsId);
        }
        else { // must be tran. to history
            QMState const * const hist = me->state.obj; // save history
            me->state.obj = t; // restore the original state

            QS_TRAN_SEG_(QS_QEP_TRAN_HIST,
                s->stateHandler, hist->stateHandler);

            // save the tran-action table before it gets clobbered
            struct QMTranActTable const * const tatbl = me->temp.tatbl;
            QMsm_exitToTranSource_(me, t, s, qsId);
            (void)QMsm_execTatbl_(me, tatbl, qsId);
            r = QMsm_enterHistory_(me, hist, qsId);
        }
#ifdef Q_SPY
        s = me->state.obj;
#endif

        while (r == Q_RET_TRAN_INIT) { // initial tran. in the target?

            r = QMsm_execTatbl_(me, me->temp.tatbl, qsId);
#ifdef Q_SPY
            s = me->state.obj;
#endif

        }

        QS_TRAN_END_(QS_QEP_TRAN, ts->stateHandler, s->stateHandler);
    }
    else {
        Q_ERROR_LOCAL(360); // last action handler returned impossible value
    }

#ifndef Q_UNSAFE
    // establish stable state configuration at the end of RTC step
    me->temp.uint = QP_DIS_UPDATE(uintptr_t, me->state.uint);
#endif
}

//............................................................................
//! @private @memberof QMsm
static QState QMsm_execTatbl_(
    QAsm * const me,
    QMTranActTable const * const tatbl,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    // the tran-action table parameter must be valid
    Q_REQUIRE_LOCAL(400, tatbl != (struct QMTranActTable *)0);

    QS_CRIT_STAT
    QState r = Q_RET_SUPER;
    QActionHandler const *a = &tatbl->act[0];
    while (*a != Q_ACTION_CAST(0)) { // not at the end of the table?
        r = (*(*a))(me); // call the action through the 'a' pointer
        ++a;

        if (r == Q_RET_ENTRY) {
            QS_STATE_ACT_(QS_QEP_STATE_ENTRY, me->temp.obj->stateHandler);
        }
        else if (r == Q_RET_EXIT) {
            QS_STATE_ACT_(QS_QEP_STATE_EXIT, me->temp.obj->stateHandler);
        }
        else if (r == Q_RET_TRAN_INIT) {
            QS_TRAN_SEG_(QS_QEP_STATE_INIT,
                tatbl->target->stateHandler,
                me->temp.tatbl->target->stateHandler);
        }
        else {
            Q_ERROR_LOCAL(460); //last action handler returned impossible value
        }

    }

    me->state.obj = tatbl->target; // set new current state
    return r;
}

//............................................................................
//! @private @memberof QMsm
static void QMsm_exitToTranSource_(
    QAsm * const me,
    QMState const * const curr_state,
    QMState const * const tran_source,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif
    QS_CRIT_STAT

    // exit states from the current state to the tran. source state
    // NOTE: the following loop does not need the fixed loop bound check
    // because the path from the current state to the tran.source has
    // been already checked in the invariant 340.
    QMState const *s = curr_state;
    while (s != tran_source) {
        if (s->exitAction != Q_ACTION_CAST(0)) { // exit action provided?
            // exit state s, ignore the result
            (void)(*s->exitAction)(me);
            QS_STATE_ACT_(QS_QEP_STATE_EXIT, me->temp.obj->stateHandler);
        }
        s = s->superstate; // advance to the superstate
    }
}

//............................................................................
//! @private @memberof QMsm
static QState QMsm_enterHistory_(
    QAsm * const me,
    QMState const *const hist,
    uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    // record the entry path from current state to history
    QMState const *path[QMSM_MAX_NEST_DEPTH_];
    QMState const *s = hist;
    int_fast8_t i = -1; // entry path index (one below [0])
    while (s != me->state.obj) {
        if (s->entryAction != Q_ACTION_CAST(0)) { // does s have an entry action?
            ++i;
            Q_INVARIANT_LOCAL(610, i < QMSM_MAX_NEST_DEPTH_);
            path[i] = s;
        }
        s = s->superstate;
    }

    QS_CRIT_STAT
    // retrace the entry path in reverse (desired) order...
    // NOTE: i is the fixed loop bound already checked in invariant 610
    for (; i >= 0; --i) {
        // enter the state in path[i], ignore the result
        (void)(*path[i]->entryAction)(me);
        QS_STATE_ACT_(QS_QEP_STATE_ENTRY, path[i]->stateHandler);
    }

    me->state.obj = hist; // set current state to the tran. target

    // initial tran. present?
    QState r = Q_RET_SUPER;
    if (hist->initAction != Q_ACTION_CAST(0)) { // init. action provided?
        r = (*hist->initAction)(me); // execute the init. action
        QS_TRAN_SEG_(QS_QEP_STATE_INIT,
            hist->stateHandler, me->temp.tatbl->target->stateHandler);
    }
    return r; // inform the caller if the init action was taken
}

//............................................................................
//! @public @memberof QMsm
QMState const * QMsm_topQMState(void) {
    // return the top state (object pointer)
    return &l_msm_top_s;
}
//............................................................................
//! @private @memberof QMsm
bool QMsm_isIn_(QAsm * const me, QStateHandler const stateHndl) {
    bool inState = false; // assume that this SM is not in 'state'
    QMState const *s = me->state.obj;
    while (s != (QMState *)0) {
        if (s->stateHandler == stateHndl) { // match found?
            inState = true;
            break;
        }
        s = s->superstate; // advance to the superstate
    }

    return inState;
}

//............................................................................
//! @public @memberof QMsm
QMState const * QMsm_childStateObj(
    QMsm const * const me,
    QMState const * const parentHndl)
{
    QMState const *s = me->super.state.obj; // start with current state
    QMState const *child = s;
    bool isFound = false; // assume the child NOT found
    while (s != (QMState *)0) { // top of state hierarchy not reached yet?
        if (s == parentHndl) {
            isFound = true; // child is found
            break;
        }
        child = s;
        s = s->superstate;
    }
    // the child state must be found, or the state machine is corrupt
    Q_ENSURE_LOCAL(890, isFound);

#ifdef Q_UNSAFE
    Q_UNUSED_PAR(isFound);
#endif

    return child;
}
//............................................................................
//! @public @memberof QMsm
QMState const* QMsm_stateObj(QMsm const* const me) {
    // return the current state (object pointer)
    return me->super.state.obj;
}
//............................................................................
//! @public @memberof QMsm
QStateHandler QMsm_getStateHandler_(QAsm const * const me) {
    // return the current state handler (function pointer)
    return me->state.obj->stateHandler;
}
