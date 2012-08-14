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

Q_DEFINE_THIS_MODULE("qmp_get")

/**
* \file
* \ingroup qf
* \brief QMPool_get() and QF_getPoolMargin() implementation.
*/

/*..........................................................................*/
void *QMPool_get(QMPool * const me) {
    QFreeBlock *fb;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    fb = (QFreeBlock *)me->free_head;           /* get a free block or NULL */
    if (fb != (QFreeBlock *)0) {                   /* free block available? */
        me->free_head = fb->next;/* adjust list head to the next free block */

        Q_ASSERT(me->nFree > (QMPoolCtr)0);      /* at least one free block */
        --me->nFree;                                 /* one less free block */
        if (me->nMin > me->nFree) {
            me->nMin = me->nFree;            /* remember the minimum so far */
        }
    }

    QS_BEGIN_NOCRIT_(QS_QF_MPOOL_GET, QS_mpObj_, me->start)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me->start);              /* the memory managed by this pool */
        QS_MPC_(me->nFree);        /* the number of free blocks in the pool */
        QS_MPC_(me->nMin);    /* min number of free blocks ever in the pool */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return fb;            /* return the block or NULL pointer to the caller */
}
/*..........................................................................*/
uint32_t QF_getPoolMargin(uint8_t const poolId) {
    uint32_t margin;
    QF_CRIT_STAT_

    Q_REQUIRE(((uint8_t)1 <= poolId) && (poolId <= QF_maxPool_));

    QF_CRIT_ENTRY_();
    margin = (uint32_t)QF_pool_[poolId - (uint8_t)1].nMin;
    QF_CRIT_EXIT_();

    return margin;
}
