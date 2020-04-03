/*****************************************************************************
* Product: Reminder2 state pattern example
* Last updated for version 6.4.0
* Last updated on  2019-02-08
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <stdlib.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum ReminderSignals {
    CRUNCH_SIG = Q_USER_SIG, /* the invented reminder signal */
    ECHO_SIG,                /* check the responsiveness of the system */
    TERMINATE_SIG            /* terminate the application */
};

typedef struct ReminderEvtTag {
    QEvt super;
    uint32_t iter; /* the next iteration to perform */
} ReminderEvt;

/*..........................................................................*/
typedef struct {            /* the Cruncher active object */
    QActive super;          /* inherit QActive */
    double sum;             /* internal variable */
} Cruncher;

void Cruncher_ctor(Cruncher * const me);

/* hierarchical state machine ... */
static QState Cruncher_initial   (Cruncher * const me, QEvt const * const e);
static QState Cruncher_processing(Cruncher * const me, QEvt const * const e);
static QState Cruncher_final     (Cruncher * const me, QEvt const * const e);

/*--------------------------------------------------------------------------*/
void Cruncher_ctor(Cruncher * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&Cruncher_initial));
}
/*..........................................................................*/
QState Cruncher_initial(Cruncher * const me, QEvt const * const e) {
    (void)e; /* unused parameter */
    return Q_TRAN(&Cruncher_processing);
}
/*..........................................................................*/
QState Cruncher_processing(Cruncher * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG);
            reminder->iter = 0U;
            QACTIVE_POST((QActive *)me, (QEvt const *)reminder, me);
            me->sum = 0.0;
            status = Q_HANDLED();
            break;
        }
        case CRUNCH_SIG: {
            uint32_t i = ((ReminderEvt const *)e)->iter;
            uint32_t n = i;
            i += 100U;
            for (; n < i; ++n) {
                if ((n & 1U) == 0U) {
                    me->sum += 1.0/(2*n + 1);
                }
                else {
                    me->sum -= 1.0/(2*n + 1);
                }
            }
            if (i < 0x07000000U) {
                ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG);
                reminder->iter = i;
                QACTIVE_POST((QActive *)me, (QEvt const *)reminder, me);
                status = Q_HANDLED();
            }
            else {
                PRINTF_S("pi=%16.14f\n", 4.0*me->sum);
                fflush(stdout);
                status = Q_TRAN(&Cruncher_processing);
            }
            break;
        }
        case ECHO_SIG: {
            PRINTF_S("Echo! pi=%16.14f\n", 4.0*me->sum);
            fflush(stdout);
            status = Q_HANDLED();
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&Cruncher_final);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Cruncher_final(Cruncher * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "final-ENTRY;");
            fflush(stdout);
            QF_stop(); /* terminate the application */
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static Cruncher l_cruncher; /* the Cruncher active object */
QEvt const *l_cruncherQSto[10]; /* Event queue storage for Cruncher AO */
static QF_MPOOL_EL(ReminderEvt) l_smlPoolSto[20]; /* storage for small pool */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    PRINTF_S("Reminder state pattern\nQP version: %s\n"
           "Press 'e' to echo the current value...\n"
           "Press ESC to quit...\n",
           QP_versionStr);
    fflush(stdout);

    Cruncher_ctor(&l_cruncher);

    BSP_init(argc, argv); /* initialize the BSP */

    QF_init();/* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */

    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* instantiate and start the active objects... */
    QACTIVE_START(&l_cruncher.super,
                  1U,
                  l_cruncherQSto, Q_DIM(l_cruncherQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run();                              /* run the QF application */
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'e': {
            static QEvt const echoEvt = { ECHO_SIG, 0U, 0U };
            QACTIVE_POST((QActive *)&l_cruncher, &echoEvt, (void *)0);
            break;
        }
        case '\033': {                                      /* ESC pressed? */
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static QEvt const terminateEvt = { TERMINATE_SIG, 0U, 0U };
            QACTIVE_POST((QActive *)&l_cruncher, &terminateEvt, (void *)0);
            break;
        }
    }
}
