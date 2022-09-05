/*============================================================================
* Product: System test fixture for QK kernel on the EFM32 target
* Last updated for version 7.1.1
* Last updated on  2022-09-05
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
enum { NumB = 3 };

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
    QS_FUN_DICTIONARY(&ObjB_active);
    return Q_TRAN(&ObjB_active);
}
/*..........................................................................*/
QState ObjB_active(ObjB * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case TRIG_SIG: {
            BSP_trigISR();
            status_ = Q_HANDLED();
            break;
        }
        case TEST_SIG: {
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
ObjB aoB[NumB];


/*==========================================================================*/
/* AO ObjA */
typedef struct {
    QActive super;
} ObjA;

QState ObjA_initial(ObjA * const me, void const * const par);
QState ObjA_active(ObjA * const me, QEvt const * const e);

void ObjA_ctor(ObjA * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&ObjA_initial));
}

/*..........................................................................*/
QState ObjA_initial(ObjA * const me, void const * const par) {
    Q_UNUSED_PAR(par);

    QActive_subscribe(&me->super, TEST_SIG);

    QS_FUN_DICTIONARY(&ObjA_initial);
    QS_FUN_DICTIONARY(&ObjA_active);

    return Q_TRAN(&ObjA_active);
}
/*..........................................................................*/
QState ObjA_active(ObjA * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case TRIG_SIG: {
            BSP_trigISR();
            status_ = Q_HANDLED();
            break;
        }
        case TEST_SIG: {
            static QEvt const te = { TEST_SIG, 0U, 0U };
            BSP_ledOn();
            QACTIVE_POST(&aoB[2].super, &te, me);
            QACTIVE_POST(&aoB[1].super, &te, me);
            QACTIVE_POST(&aoB[0].super, &te, me);
            BSP_ledOff();
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
ObjA aoA;

/*==========================================================================*/
int main() {
    QF_init();  /* initialize the framework and the underlying QXK kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* dictionaries */
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_SIG_DICTIONARY(TEST_SIG, (void *)0);
    QS_SIG_DICTIONARY(TRIG_SIG, (void *)0);
    QS_OBJ_DICTIONARY(&aoA);
    for (uint8_t n = 0U; n < NumB; ++n) {
        QS_OBJ_ARR_DICTIONARY(&aoB[n], n);
    }
    static uint16_t pspec[NumB + 1] = {
        Q_PRIO(1U, 0U),
        Q_PRIO(2U, 0U),
        Q_PRIO(3U, 0U),
        Q_PRIO(4U, 0U)
    };
    QS_OBJ_DICTIONARY(pspec);

    /* initialize publish-subscribe... */
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; /* small pool */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* pause execution of the test and wait for the test script to continue */
    QS_TEST_PAUSE();

    static QEvt const *aoB_queueSto[NumB][5];
    for (uint8_t n = 0U; n < NumB; ++n) {
        ObjB_ctor(&aoB[n]);           /* instantiate the AO */
        QACTIVE_START(&aoB[n].super,  /* AO to start */
            pspec[n],                 /* QF-prio/p-thre.*/
            aoB_queueSto[n],          /* event queue storage */
            Q_DIM(aoB_queueSto[n]),   /* event length [events] */
            (void *)0,                /* no stack storage */
            0U,                       /* zero stack size [bytes] */
            (void *)0);               /* initialization param */
    }

    static QEvt const *aoA_queueSto[5];
    ObjA_ctor(&aoA); /* instantiate the AO */
    QACTIVE_START(&aoA.super,    /* AO to start */
        pspec[NumB],             /* QF-prio/p-thre.*/
        aoA_queueSto,            /* event queue storage */
        Q_DIM(aoA_queueSto),     /* event length [events] */
        (void *)0,               /* no stack storage */
        0U,                      /* zero stack size [bytes] */
        (void *)0);              /* initialization param */

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

