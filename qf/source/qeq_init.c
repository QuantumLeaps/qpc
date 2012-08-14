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

/**
* \file
* \ingroup qf
* \brief QEQueue_init() definition. This function is used both for Active
* Object queues and for the "raw" thread-safe queues.
*/

/*..........................................................................*/
void QEQueue_init(QEQueue * const me, QEvt const *qSto[],
                  QEQueueCtr const qLen)
{
    QS_CRIT_STAT_

    me->frontEvt = (QEvt const *)0;             /* no events in the queue */
    me->ring     = &qSto[0];            /* the beginning of the ring buffer */
    me->end      = qLen;
    me->head     = (QEQueueCtr)0;
    me->tail     = (QEQueueCtr)0;
    me->nFree    = qLen;
    me->nMin     = qLen;

    QS_BEGIN_(QS_QF_EQUEUE_INIT, QS_eqObj_, me)
        QS_OBJ_(qSto);                               /* this QEQueue object */
        QS_EQC_(qLen);                           /* the length of the queue */
    QS_END_()
}
