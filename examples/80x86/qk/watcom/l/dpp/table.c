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

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct TableTag {
    QActive super;
    ThreadContext context;                                /* thread context */
    uint8_t fork[N_PHILO];
    uint8_t isHungry[N_PHILO];
} Table;

static void Table_ctor(Table *me);

static QState Table_initial(Table *me, QEvt const *e);
static QState Table_serving(Table *me, QEvt const *e);

#define RIGHT(n_) ((uint8_t)(((n_) + (N_PHILO - 1)) % N_PHILO))
#define LEFT(n_)  ((uint8_t)(((n_) + 1) % N_PHILO))
enum ForkState { FREE, USED };

/* Local objects -----------------------------------------------------------*/
static Table l_table;     /* the single instance of the Table active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Table = (QActive *)&l_table;      /* "opaque" AO pointer */

/*..........................................................................*/
void Table_start(uint8_t p, QEvt const *qSto[], uint32_t qLen) {
    Table *me = &l_table;
    Table_ctor(me);                                          /* instantiate */

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
void Table_ctor(Table *me) {
    uint8_t n;

    QActive_ctor(&me->super, (QStateHandler)&Table_initial);

    for (n = 0; n < N_PHILO; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
}
/*..........................................................................*/
QState Table_initial(Table *me, QEvt const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QS_OBJ_DICTIONARY(&l_table);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&Table_initial);
    QS_FUN_DICTIONARY(&Table_serving);

    QS_SIG_DICTIONARY(DONE_SIG,      0);                  /* global signals */
    QS_SIG_DICTIONARY(EAT_SIG,       0);
    QS_SIG_DICTIONARY(TERMINATE_SIG, 0);

    QS_SIG_DICTIONARY(HUNGRY_SIG,    me);          /* signal just for Table */

    /* signal HUNGRY_SIG is posted directly */
    QActive_subscribe((QActive *)me, DONE_SIG);
    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    return Q_TRAN(&Table_serving);
}
/*..........................................................................*/
QState Table_serving(Table *me, QEvt const *e) {
    uint8_t n, m;
    TableEvt *pe;

    switch (e->sig) {
        case HUNGRY_SIG: {
            lib1_test();
            lib2_test();
            n = ((TableEvt const *)e)->philoNum;
                      /* phil ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displyPhilStat(n, "hungry  ");
            m = LEFT(n);
            if ((me->fork[m] == FREE) && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = n;
                QF_PUBLISH((QEvt *)pe, me);
                BSP_displyPhilStat(n, "eating  ");
            }
            else {
                me->isHungry[n] = 1;
            }
            return Q_HANDLED();
        }
        case DONE_SIG: {
            lib1_test();
            lib2_test();
            n = ((TableEvt const *)e)->philoNum;
                      /* phil ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displyPhilStat(n, "thinking");
            m = LEFT(n);
                                      /* both forks of Phil[n] must be used */
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = me->fork[n] = FREE;
            m = RIGHT(n);                       /* check the right neighbor */
            if (me->isHungry[m] && (me->fork[m] == FREE)) {
                me->fork[n] = me->fork[m] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                BSP_displyPhilStat(m, "eating  ");
            }
            m = LEFT(n);                         /* check the left neighbor */
            n = LEFT(m);                  /* left fork of the left neighbor */
            if (me->isHungry[m] && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                BSP_displyPhilStat(m, "eating  ");
            }
            return Q_HANDLED();
        }
        case TEST_SIG: {
            lib1_test();
            lib2_test();
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            QF_stop();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
