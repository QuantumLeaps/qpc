/**
* @file
* @brief QP natvie, platform-independent, thread-safe event queue interface
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-01-18
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
#ifndef QEQUEUE_H
#define QEQUEUE_H

/**
* @description
* This header file must be included in all QF ports that use native QF
* event queue for active objects. Also, this file needs to be included
* in the QP/C library when the application uses QActive_defer()/
* QActive_recall(). Finally, this file is also needed when the "raw"
* thread-safe queues are used for communication between active objects
* and non-framework entities, such as ISRs, device drivers, or legacy
* code.
*/

#ifndef QF_EQUEUE_CTR_SIZE

    /*! The size [bytes] of the ring-buffer counters used in the
    * native QF event queue implementation. Valid values: 1U, 2U, or 4U;
    * default 1U. */
    /**
    * @description
    * This macro can be defined in the QF port file (qf_port.h) to
    * configure the ::QEQueueCtr type. Here the macro is not defined so the
    * default of 1 byte is chosen.
    */
    #define QF_EQUEUE_CTR_SIZE 1U
#endif
#if (QF_EQUEUE_CTR_SIZE == 1U)

    /*! The data type to store the ring-buffer counters based on
    * the macro #QF_EQUEUE_CTR_SIZE. */
    /**
    * @description
    * The dynamic range of this data type determines the maximum length
    * of the ring buffer managed by the native QF event queue.
    */
    typedef uint8_t QEQueueCtr;
#elif (QF_EQUEUE_CTR_SIZE == 2U)
    typedef uint16_t QEQueueCtr;
#elif (QF_EQUEUE_CTR_SIZE == 4U)
    typedef uint32_t QEQueueCtr;
#else
    #error "QF_EQUEUE_CTR_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif

/****************************************************************************/
/*! Native QF Event Queue */
/**
* @description
* This class describes the native QF event queue, which can be used as
* the event queue for active objects, or as a simple "raw" event queue for
* thread-safe event passing among non-framework entities, such as ISRs,
* device drivers, or other third-party components.@n
* @n
* The native QF event queue is configured by defining the macro
* #QF_EQUEUE_TYPE as ::QEQueue in the specific QF port header file.@n
* @n
* The ::QEQueue structure contains only data members for managing an event
* queue, but does not contain the storage for the queue buffer, which must
* be provided externally during the queue initialization.@n
* @n
* The event queue can store only event pointers, not the whole events. The
* internal implementation uses the standard ring-buffer plus one external
* location that optimizes the queue operation for the most frequent case
* of empty queue.@n
* @n
* The ::QEQueue structure is used with two sets of functions. One set is for
* the active object event queue, which might need to block the active object
* task when the event queue is empty and might need to unblock it when
* events are posted to the queue. The interface for the native active object
* event queue consists of the following functions: QActive_post(),
* QActive_postLIFO(), and QActive_get_(). Additionally the function
* QEQueue_init() is used to initialize the queue.@n
* @n
* The other set of functions, uses ::QEQueue as a simple "raw" event
* queue to pass events between entities other than active objects, such as
* ISRs. The "raw" event queue is not capable of blocking on the get()
* operation, but is still thread-safe because it uses QF critical section
* to protect its integrity. The interface for the "raw" thread-safe queue
* consists of the following functions: QEQueue_post(),
* QEQueue_postLIFO(), and QEQueue_get(). Additionally the function
* QEQueue_init() is used to initialize the queue.
*
* @note Most event queue operations (both the active object queues and
* the "raw" queues) internally use  the QF critical section. You should be
* careful not to invoke those operations from other critical sections when
* nesting of critical sections is not supported.
*
* @sa ::QEQueue for the description of the data members
*/
typedef struct QEQueue {
    /*! pointer to event at the front of the queue. */
    /**
    * @description
    * All incoming and outgoing events pass through the frontEvt location.
    * When the queue is empty (which is most of the time), the extra
    * frontEvt location allows to bypass the ring buffer altogether,
    * greatly optimizing the performance of the queue. Only bursts of events
    * engage the ring buffer.
    *
    * @note The additional role of this attribute is to indicate the empty
    * status of the queue. The queue is empty when frontEvt is NULL.
    */
    QEvt const * volatile frontEvt;

    /*! pointer to the start of the ring buffer. */
    QEvt const **ring;

    /*! offset of the end of the ring buffer from the start of the buffer. */
    QEQueueCtr end;

    /*! offset to where next event will be inserted into the buffer. */
    QEQueueCtr volatile head;

    /*! offset of where next event will be extracted from the buffer. */
    QEQueueCtr volatile tail;

    /*! number of free events in the ring buffer. */
    QEQueueCtr volatile nFree;

    /*! minimum number of free events ever in the ring buffer. */
    /**
    * @description
    * this attribute remembers the low-watermark of the ring buffer,
    * which provides a valuable information for sizing event queues.
    * @sa QF_getQueueMargin().
    */
    QEQueueCtr nMin;
} QEQueue;

/* public class operations */

/*! Initialize the native QF event queue */
void QEQueue_init(QEQueue * const me,
                  QEvt const * * const qSto, uint_fast16_t const qLen);

/*! Post an event to the "raw" thread-safe event queue (FIFO). */
bool QEQueue_post(QEQueue * const me, QEvt const * const e,
                  uint_fast16_t const margin, uint_fast8_t const qs_id);

/*! Post an event to the "raw" thread-safe event queue (LIFO). */
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e,
                      uint_fast8_t const qs_id);

/*! Obtain an event from the "raw" thread-safe queue. */
QEvt const *QEQueue_get(QEQueue * const me, uint_fast8_t const qs_id);

/*! "raw" thread-safe QF event queue operation for obtaining the number
* of free entries still available in the queue. */
/**
* @description
* This operation needs to be used with caution because the number of free
* entries can change unexpectedly. The main intent for using this operation
* is in conjunction with event deferral. In this case the queue is accessed
* only from a single thread (by a single AO),  so the number of free
* entries cannot change unexpectedly.
*
* @param[in] me_ pointer (see @ref oop)
*
* @returns the current number of free slots in the queue.
*/
#define QEQueue_getNFree(me_) ((me_)->nFree)

/*! "raw" thread-safe QF event queue operation for obtaining the minimum
* number of free entries ever in the queue (a.k.a. "low-watermark"). */
/**
* @description
* This operation needs to be used with caution because the "low-watermark"
* can change unexpectedly. The main intent for using this operation is to
* get an idea of queue usage to size the queue adequately.
*
* @param[in] me_  pointer (see @ref oop)
*
* @returns the minimum number of free entries ever in the queue since init.
*/
#define QEQueue_getNMin(me_) ((me_)->nMin)

/*! "raw" thread-safe QF event queue operation to find out if the queue
* is empty. */
/**
* @description
* This operation needs to be used with caution because the queue status
* can change unexpectedly. The main intent for using this operation is in
* conjunction with event deferral. In this case the queue is accessed only
* from a single thread (by a single AO), so no other entity can post
* events to the queue.
*
* @param[in] me_  pointer (see @ref oop)
*
* @returns 'true' if the queue is current empty and 'false' otherwise.
*/
#define QEQueue_isEmpty(me_) ((me_)->frontEvt == (QEvt *)0)

#endif /* QEQUEUE_H */

