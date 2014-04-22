/**
* \file
* \ingroup qf
* \brief QMPool_init() implementation.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-17
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qmp_init")

/****************************************************************************/
/**
* \description
* Initialize a fixed block-size memory pool by providing it with the pool
* memory to manage, size of this memory, and the block size.
*
* \arguments
* \arg[in,out] \c me       pointer (see \ref derivation)
* \arg[in]     \c poolSto  pointer to the memory buffer for pool storage
* \arg[in]     \c poolSize size of the storage buffer in bytes
* \arg[in]     \c blockSize fixed-size of the memory blocks in bytes
*
* \attention
* The caller of QMPool::init() must make sure that the \c poolSto
* pointer is properly __aligned__. In particular, it must be possible to
* efficiently store a pointer at the location pointed to by \c poolSto.
* Internally, the QMPool_init() function rounds up the block size
* \c blockSize so that it can fit an integer number of pointers.
* This is done to achieve proper alignment of the blocks within the pool.
*
* \note Due to the rounding of block size the actual capacity of the pool
* might be less than (\c poolSize / \c blockSize). You can check the capacity
* of the pool by calling the QF_getPoolMin() function.
*
* \note This function is __not__ protected by a critical section, because
* it is intended to be called only during the initialization of the system,
* when interrupts are not allowed yet.
*
* \note Many QF ports use memory pools to implement the event pools.
*/
void QMPool_init(QMPool * const me, void * const poolSto,
                 uint_fast16_t poolSize, uint_fast16_t blockSize)
{
    QFreeBlock *fb;
    uint_fast16_t nblocks;
    QS_CRIT_STAT_

    /** \pre The memory block must be valid
    * and the poolSize must fit at least one free block
    * and the blockSize must not be too close to the top of the dynamic range
    */
    Q_REQUIRE_ID(100, (poolSto != (void *)0)
              && (poolSize >= (uint_fast16_t)sizeof(QFreeBlock))
              && ((uint_fast16_t)(blockSize
                   + (uint_fast16_t)sizeof(QFreeBlock)) > blockSize));

    me->free_head = poolSto;

    /* round up the blockSize to fit an integer # free blocks, no division */
    me->blockSize = (QMPoolSize)sizeof(QFreeBlock);  /* start with just one */
    nblocks = (uint_fast16_t)1;/* #free blocks that fit in one memory block */
    while (me->blockSize < (QMPoolSize)blockSize) {
        me->blockSize += (QMPoolSize)sizeof(QFreeBlock);
        ++nblocks;
    }
    blockSize = (uint_fast16_t)me->blockSize; /* round-up to nearest block */

    /* the pool buffer must fit at least one rounded-up block */
    Q_ASSERT_ID(110, poolSize >= blockSize);

    /* chain all blocks together in a free-list... */
    poolSize -= blockSize;       /* don't count the last block */
    me->nTot  = (QMPoolCtr)1;    /* the last block already in the pool */
    fb = (QFreeBlock *)me->free_head; /* start at the head of the free list */

    /* chain all blocks together in a free-list... */
    while (poolSize >= blockSize) {
        fb->next = &QF_PTR_AT_(fb, nblocks);/*point next link to next block */
        fb = fb->next;           /* advance to the next block */
        poolSize -= blockSize;   /* reduce the available pool size */
        ++me->nTot;              /* increment the number of blocks so far */
    }

    fb->next  = (QFreeBlock *)0; /* the last link points to NULL */
    me->nFree = me->nTot;        /* all blocks are free */
    me->nMin  = me->nTot;        /* the minimum number of free blocks */
    me->start = poolSto;         /* the original start this pool buffer */
    me->end   = fb;              /* the last block in this pool */

    QS_BEGIN_(QS_QF_MPOOL_INIT, QS_priv_.mpObjFilter, me->start)
        QS_OBJ_(me->start);      /* the memory managed by this pool */
        QS_MPC_(me->nTot);       /* the total number of blocks */
    QS_END_()
}

