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

Q_DEFINE_THIS_MODULE("qa_get_")

/**
* \file
* \ingroup qf
* \brief QActive_get_() and QF_getQueueMargin() definitions.
*
* \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QACTIVE_EQUEUE_WAIT_(me);          /* wait for event to arrive directly */

    e = me->eQueue.frontEvt;

    if (me->eQueue.nFree != me->eQueue.end) {  /* any events in the buffer? */
                                              /* remove event from the tail */
        me->eQueue.frontEvt = QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail);
        if (me->eQueue.tail == (QEQueueCtr)0) {   /* need to wrap the tail? */
            me->eQueue.tail = me->eQueue.end;                /* wrap around */
        }
        --me->eQueue.tail;

        ++me->eQueue.nFree;       /* one more free event in the ring buffer */

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET, QS_aoObj_, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                              /* this active object */
            QS_U8_(QF_EVT_POOL_ID_(e));         /* the pool Id of the event */
            QS_U8_(QF_EVT_REF_CTR_(e));       /* the ref count of the event */
            QS_EQC_(me->eQueue.nFree);            /* number of free entries */
        QS_END_NOCRIT_()
    }
    else {
        me->eQueue.frontEvt = (QEvt const *)0;     /* queue becomes empty */
        QACTIVE_EQUEUE_ONEMPTY_(me);

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET_LAST, QS_aoObj_, me)
            QS_TIME_();                                        /* timestamp */
            QS_SIG_(e->sig);                    /* the signal of this event */
            QS_OBJ_(me);                              /* this active object */
            QS_U8_(QF_EVT_POOL_ID_(e));         /* the pool Id of the event */
            QS_U8_(QF_EVT_REF_CTR_(e));       /* the ref count of the event */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();
    return e;
}
/*..........................................................................*/
uint32_t QF_getQueueMargin(uint8_t const prio) {
    uint32_t margin;
    QF_CRIT_STAT_

    Q_REQUIRE((prio <= (uint8_t)QF_MAX_ACTIVE)
              && (QF_active_[prio] != (QActive *)0));

    QF_CRIT_ENTRY_();
    margin = (uint32_t)QF_active_[prio]->eQueue.nMin;
    QF_CRIT_EXIT_();

    return margin;
}
