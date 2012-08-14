/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 02, 2012
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
#ifndef qmpool_h
#define qmpool_h

/**
* \file
* \ingroup qf
* \brief platform-independent memory pool interface.
*
* This header file must be included in all QF ports that use native QF
* memory pool implementation.
*/

/****************************************************************************/
#ifndef QF_MPOOL_SIZ_SIZE
    /** \brief macro to override the default ::QMPoolSize size.
    * Valid values 1, 2, or 4; default 2
    */
    #define QF_MPOOL_SIZ_SIZE 2
#endif
#if (QF_MPOOL_SIZ_SIZE == 1)

    /** \brief The data type to store the block-size based on the macro
    * #QF_MPOOL_SIZ_SIZE.
    *
    * The dynamic range of this data type determines the maximum size
    * of blocks that can be managed by the native QF event pool.
    */
    typedef uint8_t QMPoolSize;
#elif (QF_MPOOL_SIZ_SIZE == 2)

    typedef uint16_t QMPoolSize;
#elif (QF_MPOOL_SIZ_SIZE == 4)
    typedef uint32_t QMPoolSize;
#else
    #error "QF_MPOOL_SIZ_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
#ifndef QF_MPOOL_CTR_SIZE
    /** \brief macro to override the default ::QMPoolCtr size.
    * Valid values 1, 2, or 4; default 2
    */
    #define QF_MPOOL_CTR_SIZE 2
#endif
#if (QF_MPOOL_CTR_SIZE == 1)

    /** \brief The data type to store the block-counter based on the macro
    * #QF_MPOOL_CTR_SIZE.
    *
    * The dynamic range of this data type determines the maximum number
    * of blocks that can be stored in the pool.
    */
    typedef uint8_t QMPoolCtr;
#elif (QF_MPOOL_CTR_SIZE == 2)
    typedef uint16_t QMPoolCtr;
#elif (QF_MPOOL_CTR_SIZE == 4)
    typedef uint32_t QMPoolCtr;
#else
    #error "QF_MPOOL_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
/** \brief Native QF Memory Pool
*
* This structure describes the native QF memory pool, which can be used as
* the event pool for dynamic event allocation, or as a fast, deterministic
* fixed block-size heap for any other objects in your application.
*
* The ::QMPool structure contains only data members for managing a memory
* pool, but does not contain the pool storage, which must be provided
* externally during the pool initialization.
*
* The native QF event pool is configured by defining the macro
* #QF_EPOOL_TYPE_ as QMPool in the specific QF port header file.
*/
typedef struct QMPoolTag {
    /** The head of the linked list of free blocks
    */
    void *free_head;

    /** the original start this pool
    */
    void *start;

    /** the last memory block managed by this memory pool
    */
    void *end;

    /**  maximum block size (in bytes)
    */
    QMPoolSize blockSize;

    /** total number of blocks
    */
    QMPoolCtr nTot;

    /** number of free blocks remaining
    */
    QMPoolCtr nFree;

    /** minimum number of free blocks ever present in this pool
    *
    * \note this attribute remembers the low watermark of the pool,
    * which provides a valuable information for sizing event pools.
    * \sa QF_getPoolMargin().
    */
    QMPoolCtr nMin;
} QMPool;

/* public functions: */

/** \brief Initializes the native QF event pool
*
* The parameters are as follows: \a me points to the ::QMPool struct to be
* initialized, \a poolSto is the pool storage, \a poolSize is the size
* of the pool storage in bytes, and \a blockSize is the block size of this
* pool.
*
* The caller of this method must make sure that the \a poolSto pointer
* is properly aligned. In particular, it must be possible to efficiently
* store a pointer at the location pointed to by \a poolSto.
* Internally, the QMPool_init() function rounds up the block size
* \a blockSize so that it can fit an integer number of pointers.
* This is done to achieve proper alignment of the blocks within the pool.
*
* \note Due to the rounding of block size the actual capacity of the pool
* might be less than (\a poolSize / \a blockSize). You can check the capacity
* of the pool by calling the QF_getPoolMargin() function.
*/
void QMPool_init(QMPool * const me, void * const poolSto,
                 uint32_t poolSize, QMPoolSize blockSize);

/** \brief Obtains a memory block from a memory pool.
*
* The only parameter \a me is a pointer to the ::QMPool from which the
* block is requested. The function returns a pointer to the allocated
* memory block or NULL if no free blocks are available.
*
* A allocated block must be returned to the same pool from which it has
* been allocated.
*
* This function can be called from any task level or ISR level.
*
* \note The memory pool \a me must be initialized before any events can
* be requested from it. Also, the QMPool_get() function uses internally a
* QF critical section, so you should be careful not to call it from within
* a critical section when nesting of critical section is not supported.
*
* \sa QMPool_put()
*/
void *QMPool_get(QMPool * const me);

/** \brief Returns a memory block back to a memory pool.
*
* The first parameter \a me is a pointer to the ::QMPool to which the
* block is returned. The second parameter is the pointer to the returned
* memory block.
*
* This function can be called from any task level or ISR level.
*
* \note The block must be allocated from the same memory pool to which it
* is returned. The QMPool_put() function raises an assertion if the returned
* pointer to the block points outside of the original memory buffer managed
* by the memory pool. Also, the QMPool_put() function uses internally a
* QF critical section, so you should be careful not to call it from within
* a critical section when nesting of critical section is not supported.
*
* \sa QMPool_get()
*/
void QMPool_put(QMPool * const me, void *b);

/* friend class QF; */

/** \brief Memory pool element to allocate correctly aligned storage
* for QMPool class.
*/
#define QF_MPOOL_EL(type_) \
    struct { void *sto_[((sizeof(type_) - 1U)/sizeof(void*)) + 1U]; }


#endif                                                          /* qmpool_h */

