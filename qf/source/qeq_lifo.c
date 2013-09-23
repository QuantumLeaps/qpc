/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 18, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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

Q_DEFINE_THIS_MODULE("qeq_lifo")

/**
* \file
* \ingroup qf
* \brief QEQueue_postLIFO() definition. NOTE: this function is used for
* the "raw" thread-safe queues and NOT for the queues of active objects.
*/

/*..........................................................................*/
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e) {
    QEvt const *frontEvt;      /* temporary to avoid UB for volatile access */
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_LIFO, QS_priv_.eqObjFilter, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                   /* this queue object */
        QS_2U8_(e->poolId_, e->refCtr_);/* pool Id & ref Count of the event */
        QS_EQC_(me->nFree);                       /* number of free entries */
        QS_EQC_(me->nMin);                    /* min number of free entries */
    QS_END_NOCRIT_()

    if (e->poolId_ != (uint8_t)0) {                  /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }

    frontEvt = me->frontEvt;            /* read volatile into the temporary */
    if (frontEvt == (QEvt const *)0) {               /* is the queue empty? */
        me->frontEvt = e;                         /* deliver event directly */
    }
    else {            /* queue is not empty, leave event in the ring-buffer */
            /* the queue must be able to accept the event (cannot overflow) */
        Q_ASSERT(me->nFree != (QEQueueCtr)0);

        ++me->tail;
        if (me->tail == me->end) {                /* need to wrap the tail? */
            me->tail = (QEQueueCtr)0;                        /* wrap around */
        }

        QF_PTR_AT_(me->ring, me->tail) = frontEvt;    /* save old front evt */
        me->frontEvt = e;                             /* put event to front */

        --me->nFree;                        /* update number of free events */
        if (me->nMin > me->nFree) {
            me->nMin = me->nFree;                  /* update minimum so far */
        }
    }
    QF_CRIT_EXIT_();
}
