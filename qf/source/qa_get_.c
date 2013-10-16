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

Q_DEFINE_THIS_MODULE("qa_get_")

/**
* \file
* \ingroup qf
* \brief QActive_get_() and QF_getQueueMin() definitions.
*
* \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEQueueCtr nFree;
    QEvt const *e;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QACTIVE_EQUEUE_WAIT_(me);          /* wait for event to arrive directly */

    e = me->eQueue.frontEvt; /* always remove event from the front location */
    nFree= me->eQueue.nFree + (QEQueueCtr)1;       /* get volatile into tmp */
    me->eQueue.nFree = nFree;                   /* upate the number of free */

    if (nFree <= me->eQueue.end) {        /* any events in the ring buffer? */
                                              /* remove event from the tail */
        me->eQueue.frontEvt = QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail);
        if (me->eQueue.tail == (QEQueueCtr)0) {   /* need to wrap the tail? */
            me->eQueue.tail = me->eQueue.end;                /* wrap around */
        }
        --me->eQueue.tail;

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                              /* this active object */
            QS_2U8_(e->poolId_, e->refCtr_);         /* pool Id & ref Count */
            QS_EQC_(nFree);                       /* number of free entries */
        QS_END_NOCRIT_()
    }
    else {
        me->eQueue.frontEvt = (QEvt const *)0;       /* queue becomes empty */

                  /* all entries in the queue must be free (+1 for fronEvt) */
        Q_ASSERT(nFree == (me->eQueue.end + (QEQueueCtr)1));

        QACTIVE_EQUEUE_ONEMPTY_(me);

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET_LAST, QS_priv_.aoObjFilter, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                              /* this active object */
            QS_2U8_(e->poolId_, e->refCtr_);         /* pool Id & ref Count */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();
    return e;
}
/*..........................................................................*/
uint16_t QF_getQueueMin(uint8_t const prio) {
    uint16_t min;
    QF_CRIT_STAT_

    Q_REQUIRE((prio <= (uint8_t)QF_MAX_ACTIVE)
              && (QF_active_[prio] != (QActive *)0));

    QF_CRIT_ENTRY_();
    min = (uint16_t)QF_active_[prio]->eQueue.nMin;
    QF_CRIT_EXIT_();

    return min;
}
