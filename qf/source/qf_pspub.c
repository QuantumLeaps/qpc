/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 4.5.04
* Date of the Last Update:  Feb 04, 2013
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

Q_DEFINE_THIS_MODULE("qf_pspub")

/**
* \file
* \ingroup qf
* \brief QF_publish() implementation.
*/

/*..........................................................................*/
#ifndef Q_SPY
void QF_publish(QEvt const * const e)
#else
void QF_publish(QEvt const * const e, void const * const sender)
#endif
{
    QF_CRIT_STAT_

      /* make sure that the published signal is within the configured range */
    Q_REQUIRE(e->sig < (QSignal)QF_maxSignal_);

    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_PUBLISH, (void *)0, (void *)0)
        QS_TIME_();                                        /* the timestamp */
        QS_OBJ_(sender);                               /* the sender object */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {       /* is it a dynamic event? */
        QF_EVT_REF_CTR_INC_(e);      /* increment reference counter, NOTE01 */
    }
    QF_CRIT_EXIT_();

#if (QF_MAX_ACTIVE <= 8)
    {
        uint8_t tmp = QF_subscrList_[e->sig].bits[0];
        while (tmp != (uint8_t)0) {
            uint8_t p = QF_LOG2(tmp);
            tmp &= Q_ROM_BYTE(QF_invPwr2Lkup[p]);   /* clear subscriber bit */
            Q_ASSERT(QF_active_[p] != (QActive *)0);  /* must be registered */

                /* QACTIVE_POST() asserts internally if the queue overflows */
            QACTIVE_POST(QF_active_[p], e, sender);
        }
    }
#else
    {
        uint8_t i = (uint8_t)Q_DIM(QF_subscrList_[0].bits);
        do {               /* go through all bytes in the subscription list */
            uint8_t tmp;
            --i;
            tmp = QF_PTR_AT_(QF_subscrList_, e->sig).bits[i];
            while (tmp != (uint8_t)0) {
                uint8_t p = QF_LOG2(tmp);
                tmp &= Q_ROM_BYTE(QF_invPwr2Lkup[p]);/*clear subscriber bit */
                p = (uint8_t)(p + (uint8_t)(i << 3));/* adjust the priority */
                Q_ASSERT(QF_active_[p] != (QActive *)0);/*must be registered*/

                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(QF_active_[p], e, sender);
            }
        } while (i != (uint8_t)0);
    }
#endif

    QF_gc(e);                      /* run the garbage collector, see NOTE01 */
}

/*****************************************************************************
* NOTE01:
* QF_publish() increments the reference counter to prevent premature
* recycling of the event while the multicasting is still in progress.
* At the end of the function, the garbage collector step decrements the
* reference counter and recycles the event if the counter drops to zero.
* This covers the case when the event was published without any subscribers.
*/
