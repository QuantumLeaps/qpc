/*****************************************************************************
* Product: DPP example, QK version
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include <stdlib.h>                                   /* for rand()/srand() */

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct PhiloTag {
    QActive super;
    ThreadContext context;                                /* thread context */
    QTimeEvt timeEvt;                   /* for timing out thining or eating */
} Philo;

static void Philo_ctor(Philo *me);

static QState Philo_initial (Philo *me, QEvt const *e);
static QState Philo_thinking(Philo *me, QEvt const *e);
static QState Philo_hungry  (Philo *me, QEvt const *e);
static QState Philo_eating  (Philo *me, QEvt const *e);

/* Local objects -----------------------------------------------------------*/
static Philo l_philo[N_PHILO];                    /* storage for all Philos */

#define THINK_TIME  (BSP_TICKS_PER_SEC/2)
#define EAT_TIME    (BSP_TICKS_PER_SEC/5)

                           /* helper macro to provide the ID of Philo "me_" */
#define PHILO_ID(me_)    ((uint8_t)((me_) - l_philo))

enum InternalSignals {                                  /* internal signals */
    TIMEOUT_SIG = MAX_SIG
};

/* Global objects ----------------------------------------------------------*/
QActive * const AO_Philo[N_PHILO] = {      /* "opaque" pointers to Philo AO */
    (QActive *)&l_philo[0],
    (QActive *)&l_philo[1],
    (QActive *)&l_philo[2],
    (QActive *)&l_philo[3],
    (QActive *)&l_philo[4]
};

/*..........................................................................*/
void Philo_start(uint8_t n,
                 uint8_t p, QEvt const *qSto[], uint32_t qLen)
{
    Philo *me = &l_philo[n];
    Philo_ctor(me);                                          /* instantiate */

    impure_ptr1 = &me->context.lib1;       /* initialize reentrant library1 */
    lib1_reent_init(p);
    impure_ptr2 = &me->context.lib2;       /* initialize reentrant library2 */
    lib2_reent_init(p);

    QActive_start((QActive *)me, p, qSto, qLen,
                  &me->context,
                  (uint8_t)(QK_LIB1_THREAD | QK_LIB2_THREAD | QK_FPU_THREAD),
                  (QEvt *)0);
}
/*..........................................................................*/
void Philo_ctor(Philo *me) {                  /* instantiate a Philo object */
    QActive_ctor(&me->super, (QStateHandler)&Philo_initial);
    QTimeEvt_ctor(&me->timeEvt, TIMEOUT_SIG);
}
/*..........................................................................*/
QState Philo_initial(Philo *me, QEvt const *e) {
    static uint8_t registered;         /* starts off with 0, per C-standard */

    (void)e;        /* suppress the compiler warning about unused parameter */

    if (!registered) {
        QS_OBJ_DICTIONARY(&l_philo[0]);
        QS_OBJ_DICTIONARY(&l_philo[0].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[1]);
        QS_OBJ_DICTIONARY(&l_philo[1].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[2]);
        QS_OBJ_DICTIONARY(&l_philo[2].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[3]);
        QS_OBJ_DICTIONARY(&l_philo[3].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[4]);
        QS_OBJ_DICTIONARY(&l_philo[4].timeEvt);

        QS_FUN_DICTIONARY(&Philo_initial);
        QS_FUN_DICTIONARY(&Philo_thinking);
        QS_FUN_DICTIONARY(&Philo_hungry);
        QS_FUN_DICTIONARY(&Philo_eating);

        srand(1234);              /* initialize the random number generator */

        registered = (uint8_t)1;
    }
    QS_SIG_DICTIONARY(HUNGRY_SIG, me);            /* signal for each Philos */
    QS_SIG_DICTIONARY(TIMEOUT_SIG, me);           /* signal for each Philos */

    QActive_subscribe((QActive *)me, EAT_SIG);

    return Q_TRAN(&Philo_thinking);          /* top-most initial transition */
}
/*..........................................................................*/
QState Philo_thinking(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvtCtr think_time;
            QMutex mutex;

            mutex = QK_mutexLock(N_PHILO);
            think_time = (QTimeEvtCtr)(5.0*rand()/RAND_MAX + THINK_TIME);
            QK_mutexUnlock(mutex);

            QTimeEvt_postIn(&me->timeEvt, (QActive *)me, think_time);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            lib1_test();
            lib2_test();
            return Q_TRAN(&Philo_hungry);
        }
        case EAT_SIG:                         /* intentionally fall-through */
        case DONE_SIG: {
                      /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Philo_hungry(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philoNum = PHILO_ID(me);
            QACTIVE_POST(AO_Table, (QEvt *)pe, me);
            return Q_HANDLED();
        }
        case EAT_SIG: {
            if (((TableEvt const *)e)->philoNum == PHILO_ID(me)) {
                lib1_test();
                lib2_test();
                return Q_TRAN(&Philo_eating);
            }
            break;
        }
        case DONE_SIG: {
                             /* DONE must be for other Philos than this one */
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Philo_eating(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvtCtr eat_time;
            QMutex mutex;

            mutex = QK_mutexLock(N_PHILO);
            eat_time = (QTimeEvtCtr)(5.0*rand()/RAND_MAX + EAT_TIME);
            QK_mutexUnlock(mutex);

            QTimeEvt_postIn(&me->timeEvt, (QActive *)me, eat_time);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philoNum = PHILO_ID(me);
            QF_PUBLISH((QEvt *)pe, me);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            lib1_test();
            lib2_test();
            return Q_TRAN(&Philo_thinking);
        }
        case EAT_SIG:                         /* intentionally fall-through */
        case DONE_SIG: {
                      /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

