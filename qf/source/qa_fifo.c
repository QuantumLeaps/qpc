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

Q_DEFINE_THIS_MODULE("qa_fifo")

/**
* \file
* \ingroup qf
* \brief QActive_post() definition
*
* \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/*..........................................................................*/
#ifndef Q_SPY
uint8_t QActive_post(QActive * const me, QEvt const * const e,
                     uint16_t const margin)
#else
uint8_t QActive_post(QActive * const me, QEvt const * const e,
                     uint16_t const margin, void const * const sender)
#endif
{
    QEQueueCtr nFree;          /* temporary to avoid UB for volatile access */
    uint8_t status;
    QF_CRIT_STAT_

    Q_REQUIRE(e != (QEvt const *)0);                 /* event must be valid */

    QF_CRIT_ENTRY_();
    nFree = me->eQueue.nFree;            /* get volatile into the temporary */
    if (nFree > (QEQueueCtr)margin) {       /* margin available? */

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_priv_.aoObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(sender);                           /* the sender object */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                  /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_);         /* pool Id & ref Count */
            QS_EQC_(nFree);                       /* number of free entries */
            QS_EQC_(me->eQueue.nMin);         /* min number of free entries */
        QS_END_NOCRIT_()

        if (e->poolId_ != (uint8_t)0) {              /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e);      /* increment the reference counter */
        }

        --nFree;                             /* one free entry just used up */
        me->eQueue.nFree = nFree;                    /* update the volatile */
        if (me->eQueue.nMin > nFree) {
            me->eQueue.nMin = nFree;               /* update minimum so far */
        }

        if (me->eQueue.frontEvt == (QEvt const *)0) {       /* empty queue? */
            me->eQueue.frontEvt = e;              /* deliver event directly */
            QACTIVE_EQUEUE_SIGNAL_(me);           /* signal the event queue */
        }
        else {     /* queue is not empty, insert event into the ring-buffer */
                                /* insert event into the ring buffer (FIFO) */
            QF_PTR_AT_(me->eQueue.ring, me->eQueue.head) = e;
            if (me->eQueue.head == (QEQueueCtr)0) {   /* need to wrap head? */
                me->eQueue.head = me->eQueue.end;            /* wrap around */
            }
            --me->eQueue.head;      /* advance the head (counter clockwise) */
        }
        status = (uint8_t)1;                   /* event posted successfully */
    }
    else {
        Q_ASSERT(margin != (uint16_t)0);    /* can tollerate dropping evts? */

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_ATTEMPT, QS_priv_.aoObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(sender);                           /* the sender object */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                  /* this active object (recipient) */
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
