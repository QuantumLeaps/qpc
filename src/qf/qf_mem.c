/**
* @file
* @ingroup qf
* @brief ::QMPool implementatin (Memory Pool)
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_mem")

/****************************************************************************/
/**
* @description
* Initialize a fixed block-size memory pool by providing it with the pool
* memory to manage, size of this memory, and the block size.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     poolSto  pointer to the memory buffer for pool storage
* @param[in]     poolSize size of the storage buffer in bytes
* @param[in]     blockSize fixed-size of the memory blocks in bytes
*
* @attention
* The caller of QMPool::init() must make sure that the @p poolSto
* pointer is properly **aligned**. In particular, it must be possible to
* efficiently store a pointer at the location pointed to by @p poolSto.
* Internally, the QMPool_init() function rounds up the block size
* @p blockSize so that it can fit an integer number of pointers.
* This is done to achieve proper alignment of the blocks within the pool.
*
* @note
* Due to the rounding of block size the actual capacity of the pool might
* be less than (@p poolSize / @p blockSize). You can check the capacity
* of the pool by calling the QF_getPoolMin() function.
*
* @note
* This function is **not** protected by a critical section, because
* it is intended to be called only during the initialization of the system,
* when interrupts are not allowed yet.
*
* @note
* Many QF ports use memory pools to implement the event pools.
*
* @usage
* The following example illustrates how to invoke QMPool_init():
* @include qmp_init.c
*/
void QMPool_init(QMPool * const me, void * const poolSto,
                 uint_fast32_t poolSize, uint_fast16_t blockSize)
{
    QFreeBlock *fb;
    uint_fast16_t nblocks;

    /** @pre The memory block must be valid
    * and the poolSize must fit at least one free block
    * and the blockSize must not be too close to the top of the dynamic range
    */
    Q_REQUIRE_ID(100, (poolSto != (void *)0)
              && (poolSize >= sizeof(QFreeBlock))
              && ((blockSize + sizeof(QFreeBlock)) > blockSize));

    me->free_head = poolSto;

    /* round up the blockSize to fit an integer # free blocks, no division */
    me->blockSize = (QMPoolSize)sizeof(QFreeBlock);  /* start with just one */
    nblocks = 1U;/* #free blocks that fit in one memory block */
    while (me->blockSize < (QMPoolSize)blockSize) {
        me->blockSize += (QMPoolSize)sizeof(QFreeBlock);
        ++nblocks;
    }
    blockSize = (uint_fast16_t)me->blockSize; /* round-up to nearest block */

    /* the pool buffer must fit at least one rounded-up block */
    Q_ASSERT_ID(110, poolSize >= blockSize);

    /* chain all blocks together in a free-list... */
    poolSize -= (uint_fast32_t)blockSize; /* don't count the last block */
    me->nTot  = 1U; /* the last block already in the pool */
    fb = (QFreeBlock *)me->free_head; /* start at the head of the free list */

    /* chain all blocks together in a free-list... */
    while (poolSize >= (uint_fast32_t)blockSize) {
        fb->next = &QF_PTR_AT_(fb, nblocks);/*point next link to next block */
        fb = fb->next;           /* advance to the next block */
        poolSize -= (uint_fast32_t)blockSize; /* reduce available pool size */
        ++me->nTot;              /* increment the number of blocks so far */
    }

    fb->next  = (QFreeBlock *)0; /* the last link points to NULL */
    me->nFree = me->nTot;        /* all blocks are free */
    me->nMin  = me->nTot;        /* the minimum number of free blocks */
    me->start = poolSto;         /* the original start this pool buffer */
    me->end   = fb;              /* the last block in this pool */
}

/****************************************************************************/
/**
* @description
* Recycle a memory block to the fixed block-size memory pool.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     b    pointer to the memory block that is being recycled
*
* @attention
* The recycled block must be allocated from the **same** memory pool
* to which it is returned.
*
* @note
* This function can be called from any task level or ISR level.
*
* @sa
* QMPool_get()
*
* @usage
* The following example illustrates how to use QMPool_put():
* @include qmp_use.c
*/
void QMPool_put(QMPool * const me, void *b, uint_fast8_t const qs_id) {
    QF_CRIT_STAT_

    /** @pre # free blocks cannot exceed the total # blocks and
    * the block pointer must be from this pool.
    */
    Q_REQUIRE_ID(200, (me->nFree < me->nTot)
                      && QF_PTR_RANGE_(b, me->start, me->end));

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_E_();
    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free_head;/* link into list */
    me->free_head = b;      /* set as new head of the free list */
    ++me->nFree;            /* one more free block in this pool */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_PUT, qs_id)
        QS_TIME_PRE_();         /* timestamp */
        QS_OBJ_PRE_(me);        /* this memory pool */
        QS_MPC_PRE_(me->nFree); /* the number of free blocks in the pool */
    QS_END_NOCRIT_PRE_()

    QF_CRIT_X_();
}

/****************************************************************************/
/**
* @description
* The function allocates a memory block from the pool and returns a pointer
* to the block back to the caller.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     margin  the minimum number of unused blocks still available
*                        in the pool after the allocation.
*
* @note
* This function can be called from any task level or ISR level.
*
* @note
* The memory pool @p me must be initialized before any events can
* be requested from it. Also, the QMPool_get() function uses internally a
* QF critical section, so you should be careful not to call it from within
* a critical section when nesting of critical section is not supported.
*
* @attention
* An allocated block must be later returned back to the same pool
* from which it has been allocated.
*
* @sa QMPool_put()
*
* @usage
* The following example illustrates how to use QMPool_get():
* @include qmp_use.c
*/
void *QMPool_get(QMPool * const me, uint_fast16_t const margin,
                 uint_fast8_t const qs_id)
{
    QFreeBlock *fb;
    QF_CRIT_STAT_

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_E_();

    /* have more free blocks than the requested margin? */
    if (me->nFree > (QMPoolCtr)margin) {
        void *fb_next;
        fb = (QFreeBlock *)me->free_head; /* get a free block */

        /* the pool has some free blocks, so a free block must be available */
        Q_ASSERT_CRIT_(310, fb != (QFreeBlock *)0);

        fb_next = fb->next; /* put volatile to a temporary to avoid UB */

        /* is the pool becoming empty? */
        --me->nFree; /* one less free block */
        if (me->nFree == 0U) {
            /* pool is becoming empty, so the next free block must be NULL */
            Q_ASSERT_CRIT_(320, fb_next == (QFreeBlock *)0);

            me->nMin = 0U; /* remember that the pool got empty */
        }
        else {
            /* pool is not empty, so the next free block must be in range
            *
            * NOTE: the next free block pointer can fall out of range
            * when the client code writes past the memory block, thus
            * corrupting the next block.
            */
            Q_ASSERT_CRIT_(330, QF_PTR_RANGE_(fb_next, me->start, me->end));

            /* is the number of free blocks the new minimum so far? */
            if (me->nMin > me->nFree) {
                me->nMin = me->nFree; /* remember the new minimum */
            }
        }

        me->free_head = fb_next; /* set the head to the next free block */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_GET, qs_id)
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(me);        /* this memory pool */
            QS_MPC_PRE_(me->nFree); /* # of free blocks in the pool */
            QS_MPC_PRE_(me->nMin);  /* min # free blocks ever in the pool */
        QS_END_NOCRIT_PRE_()
    }
    /* don't have enough free blocks at this point */
    else {
        fb = (QFreeBlock *)0;

        QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_GET_ATTEMPT, qs_id)
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(me);        /* this memory pool */
            QS_MPC_PRE_(me->nFree); /* the number of free blocks in the pool */
            QS_MPC_PRE_(margin);    /* the requested margin */
        QS_END_NOCRIT_PRE_()
    }
    QF_CRIT_X_();

    return fb;  /* return the block or NULL pointer to the caller */
}

/****************************************************************************/
/**
* @description
* This function obtains the minimum number of free blocks in the given
* event pool since this pool has been initialized by a call to QF_poolInit().
*
* @param[in] poolId  event pool ID in the range 1..QF_maxPool_, where
*                    QF_maxPool_ is the number of event pools initialized
*                    with the function QF_poolInit().
*
* @returns
* the minimum number of unused blocks in the given event pool.
*/
uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId) {
    uint_fast16_t min;
    QF_CRIT_STAT_

    /** @pre the poolId must be in range */
    Q_REQUIRE_ID(400, (0U < poolId) && (poolId <= QF_maxPool_)
                      && (poolId <= QF_MAX_EPOOL));

    QF_CRIT_E_();
    min = (uint_fast16_t)QF_pool_[poolId - 1U].nMin;
    QF_CRIT_X_();

    return min;
}

