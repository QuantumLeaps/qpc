/**
* \file
* \brief QHsm_dispatch_() and QHsm_tran_() definitions
* \ingroup qep
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
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
******************************************************************************
* \endcond
*/
#include "qep_port.h"     /* QEP port */
#include "qep_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_dis")

/*! helper function to execute a transition chain in HSM */
static int_fast8_t QHsm_tran_(QHsm * const me,
                              QStateHandler path[QHSM_MAX_NEST_DEPTH_]);

/****************************************************************************/
/**
* \description
* Dispatches an event for processing to a hierarchical state machine (HSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* \arguments
* \arg[in,out] \c me pointer (see \ref derivation)
* \arg[in]     \c e  pointer to the event to be dispatched to the HSM
*
* \note
* This function should be called only via the virtual table (see
* QMSM_DISPATCH()) and should NOT be called directly in the applications.
*/
void QHsm_dispatch_(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state.fun;
    QStateHandler s;
    QState r;
    QS_CRIT_STAT_

    /** \pre the state configuration must be stable */
    Q_REQUIRE_ID(100, t == me->temp.fun);

    QS_BEGIN_(QS_QEP_DISPATCH, QS_priv_.smObjFilter, me)
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

            QS_BEGIN_(QS_QEP_UNHANDLED, QS_priv_.smObjFilter, me)
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
                QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.smObjFilter, me)
                    QS_OBJ_(me); /* this state machine object */
                    QS_FUN_(t);  /* the exited state */
                QS_END_()

                (void)QEP_TRIG_(t, QEP_EMPTY_SIG_); /* find superstate of t */
            }
        }

        ip = QHsm_tran_(me, path);

#ifdef Q_SPY
        if (r == (QState)Q_RET_TRAN_HIST) {

            QS_BEGIN_(QS_QEP_TRAN_HIST, QS_priv_.smObjFilter, me)
                QS_OBJ_(me);     /* this state machine object */
                QS_FUN_(t);      /* the source of the transition */
                QS_FUN_(path[0]);/* the target of the tran. to history */
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

            QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
                QS_OBJ_(me); /* this state machine object */
                QS_FUN_(t);  /* the source (pseudo)state */
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
            Q_ASSERT_ID(110, ip < (int_fast8_t)QHSM_MAX_NEST_DEPTH_);

            /* retrace the entry path in reverse (correct) order... */
            do {
                QEP_ENTER_(path[ip]); /* enter path[ip] */
                --ip;
            } while (ip >= (int_fast8_t)0);

            t = path[0]; /* current state becomes the new source */
        }

        QS_BEGIN_(QS_QEP_TRAN, QS_priv_.smObjFilter, me)
            QS_TIME_();          /* time stamp */
            QS_SIG_(e->sig);     /* the signal of the event */
            QS_OBJ_(me);         /* this state machine object */
            QS_FUN_(s);          /* the source of the transition */
            QS_FUN_(t);          /* the new active state */
        QS_END_()
    }

#ifdef Q_SPY
    else if (r == (QState)Q_RET_HANDLED) {

        QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_priv_.smObjFilter, me)
            QS_TIME_();          /* time stamp */
            QS_SIG_(e->sig);     /* the signal of the event */
            QS_OBJ_(me);         /* this state machine object */
            QS_FUN_(s);          /* the source state */
        QS_END_()

    }
    else {

        QS_BEGIN_(QS_QEP_IGNORED, QS_priv_.smObjFilter, me)
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
* \description
* Static helper function to execute transition sequence in a hierarchical
* state machine (HSM).
*
* \arguments
* \arg[in,out] \c me   pointer (see \ref derivation)
* \arg[in,out] \c path array of pointers to state-handler functions
*                      to execute the entry actions
* \returns the depth of the entry path stored in the \c path argument.
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
                            Q_ASSERT_ID(210,
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
                        Q_ASSERT_ID(220,
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

                        /* not found? */
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
                                              QS_priv_.smObjFilter, me)
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
                                        r = (QState)Q_RET_HANDLED;
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
