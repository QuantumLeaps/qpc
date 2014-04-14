/**
* \file
* \brief platform-independent memory pool ::QMPool interface.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-10
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
#ifndef qmpool_h
#define qmpool_h

/****************************************************************************/
#ifndef QF_MPOOL_SIZ_SIZE
    /*! macro to override the default ::QMPoolSize size.
    * Valid values 1, 2, or 4; default 2
    */
    #define QF_MPOOL_SIZ_SIZE 2
#endif
#if (QF_MPOOL_SIZ_SIZE == 1)

    /*! The data type to store the block-size based on the macro
    * #QF_MPOOL_SIZ_SIZE.
    */
    /**
    * \description
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
    /*! macro to override the default ::QMPoolCtr size.
    * Valid values 1, 2, or 4; default 2
    */
    #define QF_MPOOL_CTR_SIZE 2
#endif
#if (QF_MPOOL_CTR_SIZE == 1)

    /*! The data type to store the block-counter based on the macro
    * #QF_MPOOL_CTR_SIZE.
    */
    /**
    * \description
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
/*! Native QF Memory Pool */
/**
* \description
* A fixed block-size memory pool is a very fast and efficient data
* structure for dynamic allocation of fixed block-size chunks of memory.
* A memory pool offers fast and deterministic allocation and recycling of
* memory blocks and is not subject to fragmenation.\n
* \n
* The ::QMPool class describes the native QF memory pool, which can be used as
* the event pool for dynamic event allocation, or as a fast, deterministic
* fixed block-size heap for any other objects in your application.
*
* \note
* ::QMPool contains only data members for managing a memory pool, but
* does not contain the pool storage, which must be provided externally
* during the pool initialization.
*
* \note
* The native QF event pool is configured by defining the macro
* #QF_EPOOL_TYPE_ as ::QMPool in the specific QF port header file.
*/
typedef struct {
    /*! The head of the linked list of free blocks */
    void * volatile free_head;

    /*! the original start this pool */
    void *start;

    /*! the last memory block managed by this memory pool */
    void *end;

    /*!  maximum block size (in bytes) */
    QMPoolSize blockSize;

    /*! total number of blocks */
    QMPoolCtr nTot;

    /*! number of free blocks remaining */
    QMPoolCtr volatile nFree;

    /*! minimum number of free blocks ever present in this pool */
    /**
    * \description
    * this attribute remembers the low watermark of the pool, which
    * provides a valuable information for sizing event pools.
    * \sa QF_getPoolMin().
    */
    QMPoolCtr nMin;
} QMPool;

/* public functions: */

/*! Initializes the native QF memory pool */
void QMPool_init(QMPool * const me, void * const poolSto,
                 uint_fast16_t poolSize, uint_fast16_t blockSize);

/*! Obtains a memory block from a memory pool. */
void *QMPool_get(QMPool * const me, uint_fast16_t const margin);

/*! Recycles a memory block back to a memory pool. */
void QMPool_put(QMPool * const me, void *b);

/*! Memory pool element to allocate correctly aligned storage
* for QMPool class.
*/
/**
* \arguments
* \arg[in] \c evType_ event type (name of the subclass of QEvt)
*/
#define QF_MPOOL_EL(evType_) \
    struct { void *sto_[((sizeof(evType_) - 1U)/sizeof(void*)) + 1U]; }

#endif /* qmpool_h */

