/*****************************************************************************
* Product: Deferred Event state pattern example
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"

#include <stdio.h> /* this example uses printf() to report status */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum TServerSignals {
    NEW_REQUEST_SIG = Q_USER_SIG, /* the new request signal */
    RECEIVED_SIG,                 /* the request has been received */
    AUTHORIZED_SIG,               /* the request has been authorized */
    TERMINATE_SIG                 /* terminate the application */
};
/*..........................................................................*/
typedef struct {
    QEvt super;       /* inherit QEvt */
    uint8_t ref_num;  /* reference number of the request */
} RequestEvt;

/*..........................................................................*/
typedef struct TServerTag { /* Transaction Server active object */
    QActive super;    /* inherit QActive */

    QEQueue requestQueue; /* native QF queue for deferred request events */
    QEvt const *requestQSto[3]; /* storage for deferred queue buffer */
    RequestEvt const *activeRequest; /* request event being processed */

    QTimeEvt receivedEvt;   /* private time event generator */
    QTimeEvt authorizedEvt; /* private time event generator */
} TServer;

void TServer_ctor(TServer * const me); /* the default ctor */

/* hierarchical state machine ... */
static QState TServer_initial    (TServer * const me, QEvt const * const e);
static QState TServer_idle       (TServer * const me, QEvt const * const e);
static QState TServer_busy       (TServer * const me, QEvt const * const e);
static QState TServer_receiving  (TServer * const me, QEvt const * const e);
static QState TServer_authorizing(TServer * const me, QEvt const * const e);
static QState TServer_final      (TServer * const me, QEvt const * const e);

/*..........................................................................*/
void TServer_ctor(TServer * const me) { /* the default ctor */
    QActive_ctor(&me->super, Q_STATE_CAST(&TServer_initial));
    QEQueue_init(&me->requestQueue,
                 me->requestQSto, Q_DIM(me->requestQSto));
    QTimeEvt_ctorX(&me->receivedEvt,   &me->super, RECEIVED_SIG,   0U);
    QTimeEvt_ctorX(&me->authorizedEvt, &me->super, AUTHORIZED_SIG, 0U);
}
/* HSM definition ----------------------------------------------------------*/
QState TServer_initial(TServer * const me, QEvt const * const e) {
    (void)e; /* unused parameter */
    me->activeRequest = (RequestEvt const *)0; /* no active request yet */

    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&TServer_idle);
    QS_FUN_DICTIONARY(&TServer_busy);
    QS_FUN_DICTIONARY(&TServer_receiving);
    QS_FUN_DICTIONARY(&TServer_authorizing);
    QS_FUN_DICTIONARY(&TServer_final);

    QS_OBJ_DICTIONARY(&l_tserver);
    QS_OBJ_DICTIONARY(&l_tserver.requestQueue);
    QS_OBJ_DICTIONARY(&l_tserver.receivedEvt);
    QS_OBJ_DICTIONARY(&l_tserver.authorizedEvt);

    QS_SIG_DICTIONARY(NEW_REQUEST_SIG, (void *)0);
    QS_SIG_DICTIONARY(RECEIVED_SIG,    (void *)0);
    QS_SIG_DICTIONARY(AUTHORIZED_SIG,  (void *)0);
    QS_SIG_DICTIONARY(TERMINATE_SIG,   (void *)0);

    return Q_TRAN(&TServer_idle);
}
/*..........................................................................*/
QState TServer_idle(TServer * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("idle-ENTRY;\n");

            /* recall the oldest deferred request... */
            if (QActive_recall(&me->super, &me->requestQueue)) {
                printf("Request recalled\n");
            }
            else {
                printf("No deferred requests\n");
            }
            status = Q_HANDLED();
            break;
        }
        case NEW_REQUEST_SIG: {
            /* create and save a new reference to the request event so that
            * this event will be available beyond this RTC step and won't be
            * recycled.
            */
            Q_NEW_REF(me->activeRequest, RequestEvt);

            printf("Processing request #%d\n",
                   (int)me->activeRequest->ref_num);

            status = Q_TRAN(&TServer_receiving);
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&TServer_final);
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
QState TServer_busy(TServer * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_EXIT_SIG: {
            printf("busy-EXIT; done processing request #%d\n",
                   (int)me->activeRequest->ref_num);

            /* delete the reference to the active request, because
            * it is now processed.
            */
            Q_DELETE_REF(me->activeRequest);

            status = Q_HANDLED();
            break;
        }
        case NEW_REQUEST_SIG: {
            /* defer the new request event... */
            if (QActive_defer(&me->super, &me->requestQueue, e)) {
                printf("Request #%d deferred;\n",
                       (int)Q_EVT_CAST(RequestEvt)->ref_num);
            }
            else {
                /* notify the request sender that his request was denied... */
                printf("Request #%d IGNORED;\n",
                       (int)Q_EVT_CAST(RequestEvt)->ref_num);
            }
            status = Q_HANDLED();
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&TServer_final);
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
QState TServer_receiving(TServer * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            /* inform about the first stage of processing of the request... */
            printf("receiving-ENTRY; active request: #%d\n",
                   (int)me->activeRequest->ref_num);

            /* one-shot timeout in 1 second */
            QTimeEvt_armX(&me->receivedEvt, BSP_TICKS_PER_SEC, 0U);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->receivedEvt);
            status = Q_HANDLED();
            break;
        }
        case RECEIVED_SIG: {
            status = Q_TRAN(&TServer_authorizing);
            break;
        }
        default: {
            status = Q_SUPER(&TServer_busy);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState TServer_authorizing(TServer * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            /* inform about the second stage of processing of the request.. */
            printf("authorizing-ENTRY; active request: #%d\n",
                   (int)me->activeRequest->ref_num);

            /* one-shot timeout in 2 seconds */
            QTimeEvt_armX(&me->authorizedEvt, 2U*BSP_TICKS_PER_SEC, 0U);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->authorizedEvt);
            status = Q_HANDLED();
            break;
        }
        case AUTHORIZED_SIG: {
            status = Q_TRAN(&TServer_idle);
            break;
        }
        default: {
            status = Q_SUPER(&TServer_busy);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState TServer_final(TServer * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("final-ENTRY;\n");
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
// test harness ============================================================*/

// Local-scope objects -------------------------------------------------------
static TServer l_tserver;  /* the TServer active object */
static QEvt const *l_tserverQSto[10]; /* Event queue storage for TServer */
static QF_MPOOL_EL(RequestEvt) l_smlPoolSto[20]; /* storage for small pool */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    printf("Deferred Event state pattern\nQP version: %s\n"
           "Press 'n' to generate a new request\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    BSP_init(argc, argv); /* initialize the BSP */

    QF_init(); /* initialize the framework and the underlying RTOS/OS */


    /* publish-subscribe not used, no call to QF_psInit() */

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* start the active objects... */
    TServer_ctor(&l_tserver);
    QACTIVE_START((QMActive *)&l_tserver,
                  1U,
                  l_tserverQSto, Q_DIM(l_tserverQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run(); /* run the QF application */
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'n': {  /* 'n': new request? */
            static uint8_t reqCtr = 0; /* count the requests */
            RequestEvt *e = Q_NEW(RequestEvt, NEW_REQUEST_SIG);
            e->ref_num = (++reqCtr); /* set the reference number */
            /* post directly to TServer active object */
            QACTIVE_POST((QMActive *)&l_tserver, (QEvt *)e, (void *)0);
            break;
        }
        case '\33': { /* ESC pressed? */
            static QEvt const terminateEvt = { TERMINATE_SIG, 0U, 0U };
            QACTIVE_POST((QMActive *)&l_tserver, &terminateEvt, (void *)0);
            break;
        }
    }
}
