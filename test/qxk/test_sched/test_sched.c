/*============================================================================
* Product: System test fixture for QXK kernel
* Last updated for version 7.1.2
* Last updated on  2022-10-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
* <www.state-machine.com/licensing>
* <info@state-machine.com>
============================================================================*/
#include "qpc.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/*==========================================================================*/
/* AO ObjB */
enum { NUM_B = 3 };

typedef struct {
    QActive super;
} ObjB;

QState ObjB_initial(ObjB * const me, void const * const par);
QState ObjB_active(ObjB * const me, QEvt const * const e);

void ObjB_ctor(ObjB * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&ObjB_initial));
}

/*..........................................................................*/
QState ObjB_initial(ObjB * const me, void const * const par) {
    Q_UNUSED_PAR(par);
    QActive_subscribe(&me->super, TEST1_SIG);
    QActive_subscribe(&me->super, TEST2_SIG);
    QS_FUN_DICTIONARY(&ObjB_active);
    return Q_TRAN(&ObjB_active);
}
/*..........................................................................*/
QState ObjB_active(ObjB * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case TEST0_SIG: {
            BSP_trace(&me->super, "TEST0 1of2");
            BSP_trigISR();
            BSP_trace(&me->super, "TEST0 2of2");
            status_ = Q_HANDLED();
            break;
        }
        case TEST1_SIG: {
            BSP_trace(&me->super, "TEST1 1of1");
            status_ = Q_HANDLED();
            break;
        }
        case TEST2_SIG: {
            BSP_trace(&me->super, "TEST2 1of1");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*..........................................................................*/
ObjB aoB[NUM_B];


/*==========================================================================*/
enum { NUM_X = 3 };

void ThrX_run(QXThread * const me) {
    QActive_subscribe(&me->super, TEST1_SIG);
    QActive_subscribe(&me->super, TEST2_SIG);

    for (;;) {
        QEvt const *e = QXThread_queueGet(QXTHREAD_NO_TIMEOUT);
        switch (e->sig) {
            case TEST0_SIG: {
                BSP_trace(&me->super, "TEST0 1of2");
                BSP_trigISR();
                BSP_trace(&me->super, "TEST0 2of2");
                break;
            }
            case TEST1_SIG: {
                static QEvt const t2 = { TEST2_SIG, 0U, 0U };
                BSP_trace(&me->super, "TEST1 1of2");
                QACTIVE_PUBLISH(&t2, &me->super);
                BSP_trace(&me->super, "TEST1 2of2");
                break;
            }
            case TEST2_SIG: {
                BSP_trace(&me->super, "TEST2 1of1");
                break;
            }
            default: {
                break;
            }
        }
    }
}

/*..........................................................................*/
QXThread thrX[NUM_X];

/*==========================================================================*/
int main() {
    QF_init();  /* initialize the framework and the underlying QXK kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* initialize publish-subscribe... */
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; /* small pool */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* dictionaries */
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_SIG_DICTIONARY(QXK_DELAY_SIG,   (void *)0);
    QS_SIG_DICTIONARY(QXK_TIMEOUT_SIG, (void *)0);
    QS_SIG_DICTIONARY(TEST0_SIG,       (void *)0);
    QS_SIG_DICTIONARY(TEST1_SIG,       (void *)0);
    QS_SIG_DICTIONARY(TEST2_SIG,       (void *)0);
    QS_SIG_DICTIONARY(TEST3_SIG,       (void *)0);

    /* priority specifications for ObjBs... */
    static QPrioSpec pspecB[NUM_B];
    QS_OBJ_DICTIONARY(pspecB);

    /* priority specifications for ThrXs... */
    static QPrioSpec pspecX[NUM_X];
    QS_OBJ_DICTIONARY(pspecX);

    uint8_t n;
    for (n = 0U; n < NUM_B; ++n) {
        QS_OBJ_ARR_DICTIONARY(&aoB[n], n);
        pspecB[n] = n + 1U;
    }
    for (n = 0U; n < NUM_X; ++n) {
        QS_OBJ_ARR_DICTIONARY(&thrX[n], n);
        pspecX[n] = NUM_B + 1U + n;
    }

    /* pause execution of the test and wait for the test script to continue
    * NOTE:
    * this pause gives the test-script a chance to poke pspecB and pspecX
    * variables to start the threads with the desired prio-specifications.
    */
    QS_TEST_PAUSE();

    static QEvt const *aoB_queueSto[NUM_B][5];
    for (n = 0U; n < NUM_B; ++n) {
        if (pspecB[n] != 0U) {
            ObjB_ctor(&aoB[n]);          /* instantiate the AO */
            QACTIVE_START(&aoB[n].super, /* AO to start */
                pspecB[n],               /* QF-prio/p-thre.*/
                aoB_queueSto[n],         /* event queue storage */
                Q_DIM(aoB_queueSto[n]),  /* event length [events] */
                (void *)0,               /* no stack storage */
                0U,                      /* zero stack size [bytes] */
                (void *)0);              /* initialization param */
        }
    }

    static QEvt const *thrX_queueSto[NUM_X][5];
    static uint64_t thrXStackSto[NUM_X][32];
    for (n = 0U; n < NUM_X; ++n) {
    QXThread_ctor(&thrX[n], &ThrX_run, 0U); /* instantiate the thread */
        if (pspecX[n] != 0U) {
            QXTHREAD_START(&thrX[n],     /* thread to start */
                pspecX[n],               /* QF-prio/p-thre.*/
                thrX_queueSto[n],        /* event queue storage */
                Q_DIM(thrX_queueSto[n]), /* event length [events] */
                thrXStackSto[n],         /* stack storage */
                sizeof(thrXStackSto[n]), /* stack size [bytes] */
                (void *)0);              /* initialization param */
        }
    }
    return QF_run(); /* run the QF application */
}

/*==========================================================================*/
void QS_onTestSetup(void) {
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
}
/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

/****************************************************************************/
/*! Host callback function to "massage" the event, if necessary */
void QS_onTestEvt(QEvt *e) {
    (void)e;
}
/*..........................................................................*/
/*! callback function to output the posted QP events (not used here) */
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(status);
}

