/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 19, 2013
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

Q_DEFINE_THIS_MODULE("qmp_get")

/**
* \file
* \ingroup qf
* \brief QMPool_get() and QF_getPoolMargin() implementation.
*/

/*..........................................................................*/
void *QMPool_get(QMPool * const me, uint16_t const margin) {
    QFreeBlock *fb;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    if (me->nFree > (QMPoolCtr)margin) {     /* have more free than margin? */
        fb = (QFreeBlock *)me->free_head;               /* get a free block */
        Q_ASSERT(fb != (QFreeBlock *)0);    /* free block must be available */
        me->free_head = fb->next;/* adjust list head to the next free block */
        --me->nFree;                                 /* one less free block */

        if (me->nMin > me->nFree) {               /* is it the new minimum? */
            me->nMin = me->nFree;               /* remember the new minimum */
        }

        QS_BEGIN_NOCRIT_(QS_QF_MPOOL_GET, QS_priv_.mpObjFilter, me->start)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(me->start);          /* the memory managed by this pool */
            QS_MPC_(me->nFree);    /* the number of free blocks in the pool */
            QS_MPC_(me->nMin);/* min number of free blocks ever in the pool */
        QS_END_NOCRIT_()
    }
    else {                     /* don't have above the margin at this point */
        fb = (QFreeBlock *)0;

        QS_BEGIN_NOCRIT_(QS_QF_MPOOL_GET_ATTEMPT,
                         QS_priv_.mpObjFilter, me->start)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(me->start);          /* the memory managed by this pool */
            QS_MPC_(me->nFree);    /* the number of free blocks in the pool */
            QS_MPC_((QMPoolCtr)margin);             /* the requested margin */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();

    return fb;  /* return the pointer to memory block or NULL to the caller */
}
/*..........................................................................*/
uint16_t QF_getPoolMin(uint_t const poolId) {
    uint16_t min;
    QF_CRIT_STAT_

    Q_REQUIRE(((uint_t)1 <= poolId) && (poolId <= QF_maxPool_));

    QF_CRIT_ENTRY_();
    min = (uint16_t)QF_pool_[poolId - (uint_t)1].nMin;
    QF_CRIT_EXIT_();

    return min;
}
