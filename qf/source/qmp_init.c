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

Q_DEFINE_THIS_MODULE("qmp_init")

/**
* \file
* \ingroup qf
* \brief QMPool_init() implementation.
*/

/*..........................................................................*/
void QMPool_init(QMPool * const me, void * const poolSto,
                 uint32_t poolSize, QMPoolSize blockSize)
{
    QFreeBlock *fb;
    uint32_t nblocks;
    QS_CRIT_STAT_

    /* The memory block must be valid
    * and the poolSize must fit at least one free block
    * and the blockSize must not be too close to the top of the dynamic range
    */
    Q_REQUIRE((poolSto != (void *)0)
              && (poolSize >= (uint32_t)sizeof(QFreeBlock))
              && ((QMPoolSize)(blockSize + (QMPoolSize)sizeof(QFreeBlock))
                    > blockSize));

    me->free_head = poolSto;

     /* round up the blockSize to fit an integer # free blocks, no division */
    me->blockSize = (QMPoolSize)sizeof(QFreeBlock);  /* start with just one */
    nblocks = (uint32_t)1;    /* # free blocks that fit in one memory block */
    while (me->blockSize < blockSize) {
        me->blockSize += (QMPoolSize)sizeof(QFreeBlock);
        ++nblocks;
    }
    blockSize = me->blockSize;      /* use the rounded-up value from now on */

                  /* the pool buffer must fit at least one rounded-up block */
    Q_ASSERT(poolSize >= (uint32_t)blockSize);

                             /* chain all blocks together in a free-list... */
    poolSize -= (uint32_t)blockSize;          /* don't count the last block */
    me->nTot  = (QMPoolCtr)1;         /* the last block already in the pool */
    fb = (QFreeBlock *)me->free_head; /* start at the head of the free list */
    while (poolSize >= (uint32_t)blockSize) {
        fb->next = &QF_PTR_AT_(fb, nblocks);/*point next link to next block */
        fb = fb->next;                         /* advance to the next block */
        poolSize -= (uint32_t)blockSize;  /* reduce the available pool size */
        ++me->nTot;                /* increment the number of blocks so far */
    }

    fb->next  = (QFreeBlock *)0;            /* the last link points to NULL */
    me->nFree = me->nTot;                            /* all blocks are free */
    me->nMin  = me->nTot;              /* the minimum number of free blocks */
    me->start = poolSto;             /* the original start this pool buffer */
    me->end   = fb;                          /* the last block in this pool */

    QS_BEGIN_(QS_QF_MPOOL_INIT, QS_mpObj_, me->start)
        QS_OBJ_(me->start);              /* the memory managed by this pool */
        QS_MPC_(me->nTot);                    /* the total number of blocks */
    QS_END_()
}

