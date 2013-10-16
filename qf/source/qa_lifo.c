/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Oct 08, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
    QEvt const *frontEvt;      /* temporary to avoid UB for volatile access */
    QEQueueCtr nFree;          /* temporary to avoid UB for volatile access */
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    nFree = me->eQueue.nFree;            /* get volatile into the temporary */

            /* the queue must be able to accept the event (cannot overflow) */
    Q_ASSERT(nFree != (QEQueueCtr)0);

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_priv_.aoObjFilter, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_);/* pool Id & ref Count of the event */
        QS_EQC_(nFree);                           /* number of free entries */
        QS_EQC_(me->eQueue.nMin);             /* min number of free entries */
    QS_END_NOCRIT_()

    if (e->poolId_ != (uint8_t)0) {                  /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }

    --nFree;                                 /* one free entry just used up */
    me->eQueue.nFree = nFree;                        /* update the volatile */
    if (me->eQueue.nMin > nFree) {
        me->eQueue.nMin = nFree;                   /* update minimum so far */
    }

    frontEvt = me->eQueue.frontEvt;     /* read volatile into the temporary */
    me->eQueue.frontEvt = e;     /* deliver the event directly to the front */
    if (frontEvt == (QEvt const *)0) {              /* was the queue empty? */
        QACTIVE_EQUEUE_SIGNAL_(me);               /* signal the event queue */
    }
    else {       /* queue was not empty, leave the event in the ring-buffer */
        ++me->eQueue.tail;
        if (me->eQueue.tail == me->eQueue.end) {  /* need to wrap the tail? */
            me->eQueue.tail = (QEQueueCtr)0;                 /* wrap around */
        }

        QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail) = frontEvt;
    }
    QF_CRIT_EXIT_();
}

