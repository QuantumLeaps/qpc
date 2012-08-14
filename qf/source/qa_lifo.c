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
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qa_lifo")

/**
* \file
* \ingroup qf
* \brief QActive_postLIFO() definition.
*
* \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/*..........................................................................*/
void QActive_postLIFO(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
        QS_EQC_(me->eQueue.nFree);                /* number of free entries */
        QS_EQC_(me->eQueue.nMin);             /* min number of free entries */
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {          /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }

    if (me->eQueue.frontEvt == (QEvt *)0) {               /* empty queue? */
        me->eQueue.frontEvt = e;                  /* deliver event directly */
        QACTIVE_EQUEUE_SIGNAL_(me);               /* signal the event queue */
    }
    else {            /* queue is not empty, leave event in the ring-buffer */
            /* the queue must be able to accept the event (cannot overflow) */
        Q_ASSERT(me->eQueue.nFree != (QEQueueCtr)0);

        ++me->eQueue.tail;
        if (me->eQueue.tail == me->eQueue.end) {  /* need to wrap the tail? */
            me->eQueue.tail = (QEQueueCtr)0;                 /* wrap around */
        }

        QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail) = me->eQueue.frontEvt;
        me->eQueue.frontEvt = e;                      /* put event to front */

        --me->eQueue.nFree;                 /* update number of free events */
        if (me->eQueue.nMin > me->eQueue.nFree) {
            me->eQueue.nMin = me->eQueue.nFree;    /* update minimum so far */
        }
    }
    QF_CRIT_EXIT_();
}

