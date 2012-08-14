/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 4.5.01
* Date of the Last Update:  Jun 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_dis")

/**
* \file
* \ingroup qep
* \brief QHsm_dispatch() implementation.
*/

/*..........................................................................*/
void QHsm_dispatch(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state;
    QStateHandler s;
    QState r;
    QS_CRIT_STAT_

    Q_REQUIRE(t == me->temp);     /* the state configuration must be stable */

    QS_BEGIN_(QS_QEP_DISPATCH, QS_smObj_, me)
        QS_TIME_();                                           /* time stamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(t);                                    /* the current state */
    QS_END_()

    do {                             /* process the event hierarchically... */
        s = me->temp;
        r = (*s)(me, e);                          /* invoke state handler s */

        if (r == Q_RET_UNHANDLED) {            /* unhandled due to a guard? */

            QS_BEGIN_(QS_QEP_UNHANDLED, QS_smObj_, me)
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(s);                            /* the current state */
            QS_END_()

            r = QEP_TRIG_(s, QEP_EMPTY_SIG_);       /* find superstate of s */
        }
    } while (r == Q_RET_SUPER);

    if (r == Q_RET_TRAN) {                             /* transition taken? */
        QStateHandler path[QEP_MAX_NEST_DEPTH_];
        int8_t ip = (int8_t)(-1);            /* transition entry path index */
        int8_t iq;                    /* helper transition entry path index */
#ifdef Q_SPY
        QStateHandler src = s;    /* save the transition source for tracing */
#endif

        path[0] = me->temp;            /* save the target of the transition */
        path[1] = t;

        while (t != s) {    /* exit current state to transition source s... */
            if (QEP_TRIG_(t, Q_EXIT_SIG) == Q_RET_HANDLED) {/*exit handled? */
                QS_BEGIN_(QS_QEP_STATE_EXIT, QS_smObj_, me)
                    QS_OBJ_(me);               /* this state machine object */
                    QS_FUN_(t);                         /* the exited state */
                QS_END_()

                (void)QEP_TRIG_(t, QEP_EMPTY_SIG_); /* find superstate of t */
            }
            t = me->temp;                  /* me->temp holds the superstate */
        }

        t = path[0];                            /* target of the transition */

        if (s == t) {      /* (a) check source==target (transition to self) */
            QEP_EXIT_(s)                                 /* exit the source */
            ip = (int8_t)0;                             /* enter the target */
        }
        else {
            (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);     /* superstate of target */

            t = me->temp;
            if (s == t) {                /* (b) check source==target->super */
                ip = (int8_t)0;                         /* enter the target */
            }
            else {
                (void)QEP_TRIG_(s, QEP_EMPTY_SIG_);    /* superstate of src */

                                  /* (c) check source->super==target->super */
                if (me->temp == t) {
                    QEP_EXIT_(s)                         /* exit the source */
                    ip = (int8_t)0;                     /* enter the target */
                }
                else {
                                         /* (d) check source->super==target */
                    if (me->temp == path[0]) {
                        QEP_EXIT_(s)                     /* exit the source */
                    }
                    else { /* (e) check rest of source==target->super->super..
                            * and store the entry path along the way
                            */
                        iq = (int8_t)0;      /* indicate that LCA not found */
                        ip = (int8_t)1;  /* enter target and its superstate */
                        path[1] = t;       /* save the superstate of target */
                        t = me->temp;                 /* save source->super */
                                               /* find target->super->super */
                        r = QEP_TRIG_(path[1], QEP_EMPTY_SIG_);
                        while (r == Q_RET_SUPER) {
                            ++ip;
                            path[ip] = me->temp;    /* store the entry path */
                            if (me->temp == s) {       /* is it the source? */
                                iq = (int8_t)1;  /* indicate that LCA found */
                                            /* entry path must not overflow */
                                Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);
                                --ip;            /* do not enter the source */
                                r = Q_RET_HANDLED;    /* terminate the loop */
                            }
                            else {   /* it is not the source, keep going up */
                                r = QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
                            }
                        }
                        if (iq == (int8_t)0) {    /* the LCA not found yet? */

                                            /* entry path must not overflow */
                            Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);

                            QEP_EXIT_(s)                /* exit the source */

                                /* (f) check the rest of source->super
                                 *                  == target->super->super...
                                 */
                            iq = ip;
                            r = Q_RET_IGNORED;    /* indicate LCA NOT found */
                            do {
                                if (t == path[iq]) {    /* is this the LCA? */
                                    r = Q_RET_HANDLED;/* indicate LCA found */
                                                          /*do not enter LCA*/
                                    ip = (int8_t)(iq - (int8_t)1);
                                    iq = (int8_t)(-1);/* terminate the loop */
                                }
                                else {
                                    --iq; /* try lower superstate of target */
                                }
                            } while (iq >= (int8_t)0);

                            if (r != Q_RET_HANDLED) { /* LCA not found yet? */
                                    /* (g) check each source->super->...
                                     * for each target->super...
                                     */
                                r = Q_RET_IGNORED;          /* keep looping */
                                do {
                                                       /* exit t unhandled? */
                                    if (QEP_TRIG_(t, Q_EXIT_SIG)
                                        == Q_RET_HANDLED)
                                    {
                                        QS_BEGIN_(QS_QEP_STATE_EXIT,
                                                  QS_smObj_, me)
                                            QS_OBJ_(me);
                                            QS_FUN_(t);
                                        QS_END_()

                                        (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);
                                    }
                                    t = me->temp;     /*  set to super of t */
                                    iq = ip;
                                    do {
                                        if (t == path[iq]) {/* is this LCA? */
                                                        /* do not enter LCA */
                                            ip = (int8_t)(iq - (int8_t)1);
                                            iq = (int8_t)(-1);/*break inner */
                                            r = Q_RET_HANDLED;/*break outer */
                                        }
                                        else {
                                            --iq;
                                        }
                                    } while (iq >= (int8_t)0);
                                } while (r != Q_RET_HANDLED);
                            }
                        }
                    }
                }
            }
        }
                    /* retrace the entry path in reverse (desired) order... */
        for (; ip >= (int8_t)0; --ip) {
            QEP_ENTER_(path[ip])                          /* enter path[ip] */
        }
        t = path[0];                      /* stick the target into register */
        me->temp = t;                              /* update the next state */

                                      /* drill into the target hierarchy... */
        while (QEP_TRIG_(t, Q_INIT_SIG) == Q_RET_TRAN) {

            QS_BEGIN_(QS_QEP_STATE_INIT, QS_smObj_, me)
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(t);                     /* the source (pseudo)state */
                QS_FUN_(me->temp);          /* the target of the transition */
            QS_END_()

            ip = (int8_t)0;
            path[0] = me->temp;

            (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);   /* find superstate */

            while (me->temp != t) {
                ++ip;
                path[ip] = me->temp;
                (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);/*find superstate */
            }
            me->temp = path[0];
                                            /* entry path must not overflow */
            Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);

            do {    /* retrace the entry path in reverse (correct) order... */
                QEP_ENTER_(path[ip])                      /* enter path[ip] */
                --ip;
            } while (ip >= (int8_t)0);

            t = path[0];
        }

        QS_BEGIN_(QS_QEP_TRAN, QS_smObj_, me)
            QS_TIME_();                                       /* time stamp */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(src);                   /* the source of the transition */
            QS_FUN_(t);                             /* the new active state */
        QS_END_()

    }
    else {                                          /* transition not taken */
#ifdef Q_SPY
        if (r == Q_RET_HANDLED) {

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
        else {

            QS_BEGIN_(QS_QEP_IGNORED, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
#endif
    }

    me->state = t;                       /* change the current active state */
    me->temp  = t;                      /* mark the configuration as stable */
}
