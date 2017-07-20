/**
* @file
* @brief QXK/C eXtended (blocking) thread.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.9.5
* Last updated on  2017-07-19
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qxthread_h
#define qxthread_h

/****************************************************************************/
/*! eXtended (blocking) thread of the QXK preemptive kernel */
/**
* @description
* QXThread represents the eXtended (blocking) thread of the QXK preemptive
* kernel. Each extended thread in the application must be represented by
* the corresponding ::QXThread instance
*
* @note
* Typically, ::QXThread is instantiated directly in the application code.
* The customization of the thread occurs in the QXThread_ctor(), where you
* provide the thred-handler function as the parameter.
*
* @sa ::QActive
*
* @usage
* The following example illustrates how to instantiate an extended thread
* in your application.
* @include qf_qxthread.c
*/
typedef struct {
    QActive super;    /* inherit QActive */
    QTimeEvt timeEvt; /* time event to handle timeouts */
} QXThread;

/*! Virtual Table for the ::QXThread class (inherited from ::QActiveVtbl) */
/**
* @note
* ::QXThread inherits ::QActive without adding any new virtual
* functions and therefore, ::QXThreadVtbl is typedef'ed as ::QActiveVtbl.
*/
typedef QActiveVtbl QXThreadVtbl;

/*! Polymorphically start an extended thread. */
/**
* @description
* Starts execution of the thread and registers the thread with the framework.
*
* @param[in,out] me_      pointer (see @ref oop)
* @param[in]     prio_    priority of the extended-thread
* @param[in]     qSto_    pointer to the storage for the ring buffer of the
*                         message queue (possibly NULL)
* @param[in]     qLen_    length of the message queue [events] (possibly 0)
* @param[in]     stkSto_  pointer to the stack storage (required)
* @param[in]     stkSize_ stack size [bytes]
* @param[in]     param_   pointer to the additional port-specific parameter(s)
*                         (might be NULL).
* @usage
* @include qxk_start.c
*/
#define QXTHREAD_START(me_, prio_, qSto_, qLen_, stkSto_, stkLen_, param_) \
    ((*((QActiveVtbl const *)((me_)->super.super.vptr))->start)( \
        &(me_)->super, (prio_), (QEvt const **)(qSto_), (qLen_), \
        (stkSto_), (stkLen_), (param_)))

/*! Thread handler pointer-to-function */
typedef void (*QXThreadHandler)(QXThread * const me);

/*! constructor of an extended-thread */
void QXThread_ctor(QXThread * const me, QXThreadHandler handler,
                   uint_fast8_t tickRate);

/**
* @description
* This macro does not assert if the queue overflows and cannot accept
* the event with the specified margin of free slots remaining.
*
* @param[in,out] me_   pointer (see @ref oop)
* @param[in]     e_    pointer to the event to post
* @param[in]     margin_ the minimum free slots in the queue, which
*                      must still be available after posting the event
* @param[in]     sender_ pointer to the sender object.
*
* @returns 'true' if the posting succeeded, and 'false' if the posting
* failed due to insufficient margin of free slots available in the queue.
*
* @note The @p sender_ parameter is actually only used when QS tracing
* is enabled (macro #Q_SPY is defined). When QS software tracing is
* disabled, the QACTIVE_POST() macro does not pass the @p sender_
* argument, so the overhead of passing this extra argument is entirely
* avoided.
*
* @note the pointer to the sender object is not necessarily a pointer
* to an active object. In fact, if QACTIVE_POST() is called from an
* interrupt or other context, you can create a unique object just to
* unambiguously identify the sender of the event.
*
* @usage
* @include qf_postx.c
*/
#define QXTHREAD_POST_X(me_, e_, margin_, sender_) \
    QACTIVE_POST_X(&(me_)->super, (e_), (margin_), (sender_))

/*! delay (block) the current extended thread for a specified # ticks */
bool QXThread_delay(uint_fast16_t const nTicks, uint_fast8_t const tickRate);

/*! cancel the delay */
bool QXThread_delayCancel(QXThread * const me);

/*! obtain a message from the private message queue (block if no messages) */
QEvt const *QXThread_queueGet(uint_fast16_t const nTicks,
                              uint_fast8_t const tickRate);

/*! no-timeout special timeout value when blocking on queues or semaphores */
#define QXTHREAD_NO_TIMEOUT  ((uint_fast16_t)0)

/*! Perform cast to QXThreadHandler. */
/**
* @description
* This macro encapsulates the cast of a specific thread handler function
* pointer to ::QXThreadHandler, which violates MISRA-C 2004 rule 11.4(adv).
* This macro helps to localize this deviation.
*/
#define Q_XTHREAD_CAST(handler_)  ((QXThreadHandler)(handler_))

/****************************************************************************/
/*! Counting Semaphore of the QXK preemptive kernel */
typedef struct {
    uint_fast16_t count;
    QPSet waitSet; /*!< set of extended-threads waiting on this semaphore */
} QXSemaphore;

/*! initialize the counting semaphore */
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count);

/*! signal (unblock) the semaphore */
void QXSemaphore_signal(QXSemaphore * const me);

/*! wait (block) on the semaphore */
bool QXSemaphore_wait(QXSemaphore * const me,
                      uint_fast16_t const nTicks,
                      uint_fast8_t const tickRate);

#endif /* qxthread_h */

