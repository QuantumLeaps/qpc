/*****************************************************************************
* Product: QP/C
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
#ifndef qequeue_h
#define qequeue_h

/**
* \file
* \ingroup qf qk
* \brief platform-independent event queue interface.
*
* This header file must be included in all QF ports that use native QF
* event queue implementation. Also, this file is needed when the "raw"
* thread-safe queues are used for communication between active objects
* and non-framework entities, such as ISRs, device drivers, or legacy
* code.
*/

#ifndef QF_EQUEUE_CTR_SIZE

    /** \brief The size (in bytes) of the ring-buffer counters used in the
    * native QF event queue implementation. Valid values: 1, 2, or 4;
    * default 1.
    *
    * This macro can be defined in the QF port file (qf_port.h) to
    * configure the ::QEQueueCtr type. Here the macro is not defined so the
    * default of 1 byte is chosen.
    */
    #define QF_EQUEUE_CTR_SIZE 1
#endif
#if (QF_EQUEUE_CTR_SIZE == 1)

    /** \brief The data type to store the ring-buffer counters based on
    * the macro #QF_EQUEUE_CTR_SIZE.
    *
    * The dynamic range of this data type determines the maximum length
    * of the ring buffer managed by the native QF event queue.
    */
    typedef uint8_t QEQueueCtr;
#elif (QF_EQUEUE_CTR_SIZE == 2)
    typedef uint16_t QEQueueCtr;
#elif (QF_EQUEUE_CTR_SIZE == 4)
    typedef uint32_t QEQueueCtr;
#else
    #error "QF_EQUEUE_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
/** \brief Native QF Event Queue
*
* This structure describes the native QF event queue, which can be used as
* the event queue for active objects, or as a simple "raw" event queue for
* thread-safe event passing among non-framework entities, such as ISRs,
* device drivers, or other third-party components.
*
* The native QF event queue is configured by defining the macro
* #QF_EQUEUE_TYPE as ::QEQueue in the specific QF port header file.
*
* The ::QEQueue structure contains only data members for managing an event
* queue, but does not contain the storage for the queue buffer, which must
* be provided externally during the queue initialization.
*
* The event queue can store only event pointers, not the whole events. The
* internal implementation uses the standard ring-buffer plus one external
* location that optimizes the queue operation for the most frequent case
* of empty queue.
*
* The ::QEQueue structure is used with two sets of functions. One set is for
* the active object event queue, which needs to block the active object
* task when the event queue is empty and unblock it when events are posted
* to the queue. The interface for the native active object event queue
* consists of the following functions: QActive_postFIFO(),
* QActive_postLIFO(), and QActive_get(). Additionally the function
* QEQueue_init() is used to initialize the queue.
*
* The other set of functions, uses this structure as a simple "raw" event
* queue to pass events between entities other than active objects, such as
* ISRs. The "raw" event queue is not capable of blocking on the get()
* operation, but is still thread-safe because it uses QF critical section
* to protect its integrity. The interface for the "raw" thread-safe queue
* consists of the following functions: QEQueue_postFIFO(),
* QEQueue_postLIFO(), and QEQueue_get(). Additionally the function
* QEQueue_init() is used to initialize the queue.
*
* \note Most event queue operations (both the active object queues and
* the "raw" queues) internally use  the QF critical section. You should be
* careful not to invoke those operations from other critical sections when
* nesting of critical sections is not supported.
*
* \sa ::QEQueueTag for the description of the data members
*/
typedef struct QEQueueTag {
    /** \brief pointer to event at the front of the queue
    *
    * All incoming and outgoing events pass through the frontEvt location.
    * When the queue is empty (which is most of the time), the extra
    * frontEvt location allows to bypass the ring buffer altogether,
    * greatly optimizing the performance of the queue. Only bursts of events
    * engage the ring buffer.
    *
    * The additional role of this attribute is to indicate the empty status
    * of the queue. The queue is empty if the frontEvt location is NULL.
    */
    QEvt const *frontEvt;

    /** \brief pointer to the start of the ring buffer
    */
    QEvt const **ring;

    /** \brief offset of the end of the ring buffer from the start
    * of the buffer
    */
    QEQueueCtr end;

    /** \brief offset to where next event will be inserted into the buffer
    */
    QEQueueCtr head;

    /** \brief offset of where next event will be extracted from the buffer
    */
    QEQueueCtr tail;

    /** \brief number of free events in the ring buffer
    */
    QEQueueCtr nFree;

    /** \brief minimum number of free events ever in the ring buffer.
    *
    * \note this attribute remembers the low-watermark of the ring buffer,
    * which provides a valuable information for sizing event queues.
    * \sa QF_getQueueMargin().
    */
    QEQueueCtr nMin;
} QEQueue;

/* public methods */

/** \brief Initializes the native QF event queue
*
* The parameters are as follows: \a me points to the ::QEvt struct to be
* initialized, \a qSto[] is the ring buffer storage, \a qLen is the length
* of the ring buffer in the units of event-pointers.
*
* \note The actual capacity of the queue is qLen + 1, because of the extra
* location fornEvt.
*/
void QEQueue_init(QEQueue * const me,
                  QEvt const *qSto[], QEQueueCtr const qLen);

/** \brief "raw" thread-safe QF event queue implementation for the
* First-In-First-Out (FIFO) event posting. You can call this function from
* any task context or ISR context. Please note that this function uses
* internally a critical section.
*
* \note The function raises an assertion if the native QF queue becomes
* full and cannot accept the event.
*
* \sa QEQueue_postLIFO(), QEQueue_get()
*/
void QEQueue_postFIFO(QEQueue * const me, QEvt const * const e);

/** \brief "raw" thread-safe QF event queue implementation for the
* Last-In-First-Out (LIFO) event posting.
*
* \note The LIFO policy should be used only with great caution because it
* alters order of events in the queue.
* \note The function raises an assertion if the native QF queue becomes
* full and cannot accept the event. You can call this function from
* any task context or ISR context. Please note that this function uses
* internally a critical section.
*
* \sa QEQueue_postFIFO(), QEQueue_get()
*/
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e);

/** \brief "raw" thread-safe QF event queue implementation for the
* event get operation.
*
* If the queue has one or more events, the function returns the event
* at the front of the queue, otherwise it returns NULL. You can call this
* function from any task context or ISR context. Please note that this
* function uses internally a critical section.
*
* \sa QEQueue_postFIFO(), QEQueue_postLIFO()
*/
QEvt const *QEQueue_get(QEQueue * const me);

/** \brief "raw" thread-safe QF event queue operation for
* obtaining the number of free entries still available in the queue.
*
* \note This operation needs to be used with caution because the
* number of free entries can change unexpectedly. The main intent for
* using this operation is in conjunction with event deferral. In this
* case the queue is accessed only from a single thread (by a single AO),
* so the number of free entries cannot change unexpectedly.
*
* \sa QActive_defer(), QActive_recall()
*/
#define QEQueue_getNFree(me_) ((me_)->nFree)

/* friend class QF */
/* friend class QActive */

#endif                                                         /* qequeue_h */

