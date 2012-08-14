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

Q_DEFINE_THIS_MODULE("qa_defer")

/**
* \file
* \ingroup qf
* \brief QActive_defer() and QActive_recall() implementation.
*/

/*..........................................................................*/
void QActive_defer(QActive * const me, QEQueue * const eq,
                   QEvt const * const e)
{
    (void)me;                 /* avoid compiler warning about 'me' not used */
    QEQueue_postFIFO(eq, e);
}
/*..........................................................................*/
uint8_t QActive_recall(QActive * const me, QEQueue * const eq) {
    QEvt const *e = QEQueue_get(eq);  /* get an event from deferred queue */
    uint8_t recalled;
    if (e != (QEvt const *)0) {                       /* event available? */
        QF_CRIT_STAT_

        QActive_postLIFO(me, e);  /* post it to the front of the AO's queue */

        QF_CRIT_ENTRY_();

        if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {   /* is it a dynamic event? */

            /* after posting to the AO's queue the event must be referenced
            * at least twice: once in the deferred event queue (eq->get()
            * did NOT decrement the reference counter) and once in the
            * AO's event queue.
            */
            Q_ASSERT(QF_EVT_REF_CTR_(e) > (uint8_t)1);

            /* we need to decrement the reference counter once, to account
            * for removing the event from the deferred event queue.
            */
            QF_EVT_REF_CTR_DEC_(e);      /* decrement the reference counter */
        }

        QF_CRIT_EXIT_();
        recalled = (uint8_t)1;
    }
    else {
        recalled = (uint8_t)0;
    }
    return recalled;
}
