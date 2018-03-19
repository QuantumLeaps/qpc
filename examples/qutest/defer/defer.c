/*****************************************************************************
* Product: Deferred Event state pattern example
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-03-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
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
#include "defer.h"
//#include <stdio.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
typedef struct { /* Transaction Server active object */
    QActive super;    /* inherit QActive */

    QEQueue requestQueue; /* native QF queue for deferred request events */
    QEvt const *requestQSto[3]; /* storage for deferred queue buffer */
    RequestEvt const *activeRequest; /* request event being processed */

    QTimeEvt receivedEvt;   /* private time event generator */
    QTimeEvt authorizedEvt; /* private time event generator */
} TServer;

/* hierarchical state machine ... */
static QState TServer_initial    (TServer * const me, QEvt const * const e);
static QState TServer_idle       (TServer * const me, QEvt const * const e);
static QState TServer_busy       (TServer * const me, QEvt const * const e);
static QState TServer_receiving  (TServer * const me, QEvt const * const e);
static QState TServer_authorizing(TServer * const me, QEvt const * const e);
static QState TServer_final      (TServer * const me, QEvt const * const e);

/*..........................................................................*/
static TServer l_tserver;
QActive * const AO_TServer = &l_tserver.super;

/*..........................................................................*/
void TServer_ctor() {
    TServer * const me = &l_tserver;
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
            //printf("idle-ENTRY;\n");

            /* recall the oldest deferred request... */
            if (QActive_recall(&me->super, &me->requestQueue)) {
                //printf("Request recalled\n");
            }
            else {
                //printf("No deferred requests\n");
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

            //printf("Processing request #%d\n",
            //       (int)me->activeRequest->ref_num);

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
            //printf("busy-EXIT; done processing request #%d\n",
            //       (int)me->activeRequest->ref_num);

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
                //printf("Request #%d deferred;\n",
                //       (int)Q_EVT_CAST(RequestEvt)->ref_num);
            }
            else {
                /* notify the request sender that his request was denied... */
                //printf("Request #%d IGNORED;\n",
                //       (int)Q_EVT_CAST(RequestEvt)->ref_num);
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
            //printf("receiving-ENTRY; active request: #%d\n",
            //       (int)me->activeRequest->ref_num);

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
            //printf("authorizing-ENTRY; active request: #%d\n",
            //       (int)me->activeRequest->ref_num);

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
            //printf("final-ENTRY;\n");
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
