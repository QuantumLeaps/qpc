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

Q_DEFINE_THIS_MODULE("qeq_fifo")

/**
* \file
* \ingroup qf
* \brief QEQueue_postFIFO() definition. NOTE: this function is used for
* the "raw" thread-safe queues and NOT for the queues of active objects.
*/

/*..........................................................................*/
void QEQueue_postFIFO(QEQueue * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_FIFO, QS_eqObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                   /* this queue object */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
        QS_EQC_(me->nFree);                       /* number of free entries */
        QS_EQC_(me->nMin);                    /* min number of free entries */
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {          /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }

    if (me->frontEvt == (QEvt const *)0) {         /* is the queue empty? */
        me->frontEvt = e;                         /* deliver event directly */
    }
    else {            /* queue is not empty, leave event in the ring-buffer */
            /* the queue must be able to accept the event (cannot overflow) */
        Q_ASSERT(me->nFree != (QEQueueCtr)0);

        QF_PTR_AT_(me->ring, me->head) = e;  /* insert e into buffer (FIFO) */
        if (me->head == (QEQueueCtr)0) {          /* need to wrap the head? */
            me->head = me->end;                              /* wrap around */
        }
        --me->head;

        --me->nFree;                        /* update number of free events */
        if (me->nMin > me->nFree) {
            me->nMin = me->nFree;                  /* update minimum so far */
        }
    }
    QF_CRIT_EXIT_();
}
