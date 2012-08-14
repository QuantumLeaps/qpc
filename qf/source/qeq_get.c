/*****************************************************************************
* Product: QF/C
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
#include "qf_pkg.h"
/* #include "qassert.h" */

/* Q_DEFINE_THIS_MODULE("qeq_get") */

/**
* \file
* \ingroup qf
* \brief QEQueue_get() definition. NOTE: this function is used for
* the "raw" thread-safe queues and NOT for the queues of active objects.
*/

/*..........................................................................*/
QEvt const *QEQueue_get(QEQueue * const me) {
    QEvt const *e;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    if (me->frontEvt == (QEvt const *)0) {         /* is the queue empty? */
        e = (QEvt const *)0;           /* no event available at this time */
    }
    else {                                        /* the queue is not empty */
        e = me->frontEvt;

        if (me->nFree != me->end) {       /* any events in the ring buffer? */
            me->frontEvt = QF_PTR_AT_(me->ring, me->tail); /* get from tail */
            if (me->tail == (QEQueueCtr)0) {      /* need to wrap the tail? */
                me->tail = me->end;                          /* wrap around */
            }
            --me->tail;

            ++me->nFree;          /* one more free event in the ring buffer */

            QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_GET, QS_eqObj_, me)
                QS_TIME_();                                    /* timestamp */
                QS_SIG_(e->sig);                /* the signal of this event */
                QS_OBJ_(me);                           /* this queue object */
                QS_U8_(QF_EVT_POOL_ID_(e));     /* the pool Id of the event */
                QS_U8_(QF_EVT_REF_CTR_(e));   /* the ref count of the event */
                QS_EQC_(me->nFree);               /* number of free entries */
            QS_END_NOCRIT_()
        }
        else {
            me->frontEvt = (QEvt const *)0;        /* queue becomes empty */

            QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_GET_LAST, QS_eqObj_, me)
                QS_TIME_();                                    /* timestamp */
                QS_SIG_(e->sig);                /* the signal of this event */
                QS_OBJ_(me);                           /* this queue object */
                QS_U8_(QF_EVT_POOL_ID_(e));     /* the pool Id of the event */
                QS_U8_(QF_EVT_REF_CTR_(e));   /* the ref count of the event */
            QS_END_NOCRIT_()
        }
    }
    QF_CRIT_EXIT_();
    return e;
}
