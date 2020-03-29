/*****************************************************************************
* Product: DPP example
* Last updated for version 5.4.0
* Last updated on  2015-03-25
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"     /* QP/C header file */
#include "dpp.h"     /* application events and active objects */
#include "bsp.h"     /* Board Support Package header file */

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct PhiloTag {
    QActive super;
    QTimeEvt timeEvt;                   /* for timing out thining or eating */
} Philo;

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
QActive * const AO_Philo[N_PHILO] = {     /* "opaque" pointers to Philo AO */
    (QActive *)&l_philo[0],
    (QActive *)&l_philo[1],
    (QActive *)&l_philo[2],
    (QActive *)&l_philo[3],
    (QActive *)&l_philo[4]
};

/*..........................................................................*/
void Philo_ctor(void) {                    /* instantiate all Philo objects */
    uint8_t n;
    Philo *me;
    for (n = 0; n < N_PHILO; ++n) {
        me = &l_philo[n];
        QActive_ctor(&me->super, (QStateHandler)&Philo_initial);
        QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
    }
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
            QTimeEvt_armX(&me->timeEvt, THINK_TIME, 0U);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
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
            QTimeEvt_armX(&me->timeEvt, EAT_TIME, 0U);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philoNum = PHILO_ID(me);
            QF_PUBLISH((QEvt *)pe, me);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
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

