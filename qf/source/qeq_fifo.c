/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 02, 2013
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

Q_DEFINE_THIS_MODULE("qeq_fifo")

/**
* \file
* \ingroup qf
* \brief QEQueue_post() definition.
*
* \note this function is used for the "raw" thread-safe queues and NOT
* for the queues of active objects.
*/

/*..........................................................................*/
uint8_t QEQueue_post(QEQueue * const me, QEvt const * const e,
                     uint_t const margin)
{
    QEQueueCtr nFree;          /* temporary to avoid UB for volatile access */
    uint8_t status;
    QF_CRIT_STAT_

    Q_REQUIRE(e != (QEvt const *)0);                 /* event must be valid */

    QF_CRIT_ENTRY_();
    nFree = me->nFree;                   /* get volatile into the temporary */

    if (nFree > (QEQueueCtr)margin) {         /* required margin available? */

        QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_FIFO, QS_priv_.eqObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                               /* this queue object */
            QS_2U8_(e->poolId_, e->refCtr_);         /* pool Id & ref Count */
            QS_EQC_(nFree);                       /* number of free entries */
            QS_EQC_(me->nMin);                /* min number of free entries */
        QS_END_NOCRIT_()

        if (e->poolId_ != (uint8_t)0) {              /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e);      /* increment the reference counter */
        }

        --nFree;                             /* one free entry just used up */
        me->nFree = nFree;                           /* update the volatile */
        if (me->nMin > nFree) {
            me->nMin = nFree;                      /* update minimum so far */
        }

        if (me->frontEvt == (QEvt const *)0) {      /* was the queue empty? */
            me->frontEvt = e;                     /* deliver event directly */
        }
        else {    /* queue was not empty, insert event into the ring-buffer */
                                /* insert event into the ring buffer (FIFO) */
            QF_PTR_AT_(me->ring, me->head) = e;     /* insert e into buffer */
            if (me->head == (QEQueueCtr)0) {      /* need to wrap the head? */
                me->head = me->end;                          /* wrap around */
            }
            --me->head;
        }
        status = (uint8_t)1;                   /* event posted successfully */
    }
    else {
        Q_ASSERT(margin != (uint_t)0);      /* can tollerate dropping evts? */

        QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_ATTEMPT, QS_priv_.eqObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                               /* this queue object */
            QS_2U8_(e->poolId_, e->refCtr_);         /* pool Id & ref Count */
            QS_EQC_(nFree);                       /* number of free entries */
            QS_EQC_((QEQueueCtr)margin);                /* margin requested */
        QS_END_NOCRIT_()

        status = (uint8_t)0;
    }
    QF_CRIT_EXIT_();

    return status;
}

/*****************************************************************************
* NOTE01:
* The zero value of the 'margin' argument is special and denotes situation
* when the post() operation is assumed to succeed (event delivery guarantee).
* An assertion fires, when the event cannot be delivered in this case.
*/
