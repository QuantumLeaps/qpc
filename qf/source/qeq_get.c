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

Q_DEFINE_THIS_MODULE("qeq_get")

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
    e = me->frontEvt;        /* always remove event from the front location */

    if (e != (QEvt const *)0) {                 /* was the queue not empty? */
        QEQueueCtr nFree = me->nFree + (QEQueueCtr)1;/*get volatile into tmp*/
        me->nFree = nFree;                      /* upate the number of free */

        if (nFree <= me->end) {           /* any events in the ring buffer? */
            me->frontEvt = QF_PTR_AT_(me->ring, me->tail); /* get from tail */
            if (me->tail == (QEQueueCtr)0) {      /* need to wrap the tail? */
                me->tail = me->end;                          /* wrap around */
            }
            --me->tail;

            QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_GET, QS_priv_.eqObjFilter, me)
                QS_TIME_();                                    /* timestamp */
                QS_SIG_(e->sig);                /* the signal of this event */
                QS_OBJ_(me);                           /* this queue object */
                QS_2U8_(e->poolId_, e->refCtr_);     /* pool Id & ref Count */
                QS_EQC_(nFree);                   /* number of free entries */
            QS_END_NOCRIT_()
        }
        else {
            me->frontEvt = (QEvt const *)0;          /* queue becomes empty */

                  /* all entries in the queue must be free (+1 for fronEvt) */
            Q_ASSERT(nFree == (me->end + (QEQueueCtr)1));

            QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_GET_LAST, QS_priv_.eqObjFilter, me)
                QS_TIME_();                                    /* timestamp */
                QS_SIG_(e->sig);                /* the signal of this event */
                QS_OBJ_(me);                           /* this queue object */
                QS_2U8_(e->poolId_, e->refCtr_);     /* pool Id & ref Count */
            QS_END_NOCRIT_()
        }
    }
    QF_CRIT_EXIT_();
    return e;
}
