/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QXK/C eXtended (blocking) thread.
*/
#ifndef QXTHREAD_H
#define QXTHREAD_H

/*==========================================================================*/
/*! @brief eXtended (blocking) thread of the QXK preemptive kernel
* @class QXThread
* @extends QActive
*
* @description
* QXThread represents the eXtended (blocking) thread of the QXK preemptive
* kernel. Each extended thread in the application must be represented by
* the corresponding ::QXThread instance
*
* @note
* Typically, ::QXThread is instantiated directly in the application code.
* The customization of the thread occurs in the QXThread_ctor(), where you
* provide the thread-handler function as the parameter.
*
* @sa
* - ::QXThread
* - QXThread_ctor()
* - QXTHREAD_START()
* - QXTHREAD_POST_X()
* - QXThread_delay()
* - QXThread_delayCancel()
* - QXThread_queueGet()
* - ::QActive
*
* @usage
* The following example illustrates how to instantiate and use an extended
* thread in your application.
* @include qxk_thread.c
*/
struct QXThread {
    QActive super;    /*!< inherited ::QActive */
    QTimeEvt timeEvt; /*!< time event to handle blocking timeouts */
};

/*! @brief Virtual Table for the ::QXThread class
* (inherited from ::QActiveVtable)
*
* @note
* ::QXThread inherits ::QActive without adding any new virtual
* functions and therefore, ::QXThreadVtable is typedef'ed as ::QActiveVtable.
*/
typedef QActiveVtable QXThreadVtable;

/*! Polymorphically start an extended thread.
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
* @param[in]     par_     pointer to the additional port-specific parameter(s)
*                         (might be NULL).
* @usage
* @include qxk_start.c
*/
#define QXTHREAD_START(me_, prio_, qSto_, qLen_, stkSto_, stkSize_, par_) \
do {                                                                      \
    Q_ASSERT((me_)->super.super.vptr);                                    \
    ((*((QActiveVtable const *)((me_)->super.super.vptr))->start)(        \
        &(me_)->super, (prio_), (QEvt const **)(qSto_), (qLen_),          \
        (stkSto_), (stkSize_), (par_)));                                  \
} while (false)

/*! constructor of an extended-thread
* @public @memberof QXThread
*
* @description
* Performs the first step of QXThread initialization by assigning the
* thread-handler function and the tick rate at which it will handle
* the timeouts.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     handler  the thread-handler function
* @param[in]     tickRate the ticking rate for timeouts in this thread
*                (see QXThread_delay() and QF_TICK_X())
*
* @note
* Must be called only ONCE before QXTHREAD_START().
*
* @usage
* The following example illustrates how to invoke QXThread_ctor() in the
* main() function
*
* @include
* qxk_thread_ctor.c
*/
void QXThread_ctor(QXThread * const me, QXThreadHandler handler,
                   uint_fast8_t tickRate);

/*! Asynchronous posting events to the event queue of an eXtended thread
* @description
* This macro does not assert if the queue overflows and cannot accept
* the event with the specified margin of free slots remaining.
*
* @param[in,out] me_   pointer (see @ref oop)
* @param[in]     e_    pointer to the event to post
* @param[in]     margin_ the minimum free slots in the queue, which
*                      must still be available after posting the event.
*                      The special value #QF_NO_MARGIN causes asserting
*                      failure in case event allocation fails.
* @param[in]     sender_ pointer to the sender object (used in QS tracing)
*
* @returns
* 'true' if the posting succeeded, and 'false' if the posting failed due
* to insufficient margin of free slots available in the queue.
*
* @note
* The @p sender_ parameter is actually only used when QS tracing is enabled
* (macro #Q_SPY is defined). When QS software tracing is disabled, the
* QXTHREAD_POST_X() macro does not pass the @p sender_ argument, so the
* overhead of passing this extra argument is entirely avoided.
*
* @note
* The pointer to the sender object is not necessarily a pointer to an
* active object. In fact, if QXTHREAD_POST_X() is called from an interrupt
* or other context, you can create a unique object just to unambiguously
* identify the sender of the event.
*
* @usage
* @include qf_postx.c
*/
#define QXTHREAD_POST_X(me_, e_, margin_, sender_) \
    QACTIVE_POST_X(&(me_)->super, (e_), (margin_), (sender_))

/*! delay (block) the current extended thread for a specified # ticks
* @static @public @memberof QXThread
*
* @description
* Blocking delay for the number of clock tick at the associated tick rate.
*
* @param[in]  nTicks    number of clock ticks (at the associated rate)
*                       to wait for the event to arrive.
* @note
* For the delay to work, the QF_TICK_X() macro needs to be called
* periodically at the associated clock tick rate.
*
* @sa QXThread_ctor()
* @sa QF_TICK_X()
*/
bool QXThread_delay(uint_fast16_t const nTicks);

/*! cancel the delay
* @public @memberof QXThread
*
* @description
* Cancel the blocking delay and cause return from the QXThread_delay()
* function.
*
* @returns
* "true" if the thread was actually blocked on QXThread_delay() and
* "false" otherwise.
*/
bool QXThread_delayCancel(QXThread * const me);

/*! obtain a message from the private message queue (block if no messages)
* @static @public @memberof QXThread
*
* @description
* The QXThread_queueGet() operation allows the calling extended thread to
* receive QP events directly into its own built-in event queue from an ISR,
* basic thread (AO), or another extended thread.
*
* If QXThread_queueGet() is called when no events are present in the
* thread's private event queue, the operation blocks the current extended
* thread until either an event is received, or a user-specified timeout
* expires.
*
* @param[in]  nTicks    number of clock ticks (at the associated rate)
*                       to wait for the event to arrive. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the queue will block indefinitely.
* @returns
* A pointer to the event. If the pointer is not NULL, the event was delivered.
* Otherwise the event pointer of NULL indicates that the queue has timed out.
*/
QEvt const *QXThread_queueGet(uint_fast16_t const nTicks);

/*! no-timeout special timeout value when blocking on queues or semaphores */
#define QXTHREAD_NO_TIMEOUT  ((uint_fast16_t)0)

/*==========================================================================*/
/*! @brief Counting Semaphore of the QXK preemptive kernel
* @class QXSemaphore
*
* @description
* ::QXSemaphore is a blocking mechanism intended primarily for signaling
* @ref ::QXThread "extended threads". The semaphore is initialized with
* the maximum count (see QXSemaphore_init()), which allows you to create
* a binary semaphore (when the maximum count is 1) and
* counting semaphore when the maximum count is > 1.
*
* @sa
* - QXSemaphore_init()
* - QXSemaphore_signal()
* - QXSemaphore_wait()
* - QXSemaphore_tryWait()
*
* @usage
* The following example illustrates how to instantiate and use the semaphore
* in your application.
* @include qxk_sema.c
*/
typedef struct {
    QPSet waitSet; /*!< set of extended-threads waiting on this semaphore */
    uint16_t volatile count; /*!< semaphore up-down counter */
    uint16_t max_count; /*!< maximum value of the semaphore counter */
} QXSemaphore;

/*! initialize the counting semaphore
* @public @memberof QXSemaphore
*
* @description
* Initializes a semaphore with the specified count and maximum count.
* If the semaphore is used for resource sharing, both the initial count
* and maximum count should be set to the number of identical resources
* guarded by the semaphore. If the semaphore is used as a signaling
* mechanism, the initial count should set to 0 and maximum count to 1
* (binary semaphore).
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     count  initial value of the semaphore counter
* @param[in]     max_count  maximum value of the semaphore counter.
*                The purpose of the max_count is to limit the counter
*                so that the semaphore cannot unblock more times than
*                the maximum.
*
* @note
* QXSemaphore_init() must be called **before** the semaphore can be used
* (signaled or waited on).
*/
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count,
                      uint_fast16_t max_count);

/*! wait (block) on the semaphore
* @public @memberof QXSemaphore
*
* @description
* When an extended thread calls QXSemaphore_wait() and the value of the
* semaphore counter is greater than 0, QXSemaphore_wait() decrements the
* semaphore counter and returns (true) to its caller. However, if the value
* of the semaphore counter is 0, the function places the calling thread in
* the waiting list for the semaphore. The thread waits until the semaphore
* is signaled by calling QXSemaphore_signal(), or the specified timeout
* expires. If the semaphore is signaled before the timeout expires, QXK
* resumes the highest-priority extended thread waiting for the semaphore.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to wait for the semaphore. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the semaphore will wait indefinitely.
*
* @returns
* 'true' if the semaphore has been signaled and 'false' if a timeout occurred.
*
* @note
* Multiple extended threads can wait for a given semaphore.
*/
bool QXSemaphore_wait(QXSemaphore * const me,
                      uint_fast16_t const nTicks);

/*! try wait on the semaphore (non-blocking)
* @public @memberof QXSemaphore
*
* @description
* This function checks if the semaphore counter is greater than 0,
* in which case the counter is decremented.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' if the semaphore has count available and 'false' NOT available.
*
* @note
* This function can be called from any context, including ISRs and basic
* threads (active objects).
*/
bool QXSemaphore_tryWait(QXSemaphore * const me);

/*! signal (unblock) the semaphore
* @public @memberof QXSemaphore
*
* @description
* If the semaphore counter value is 0 or more, it is incremented, and
* this function returns to its caller. If the extended threads are waiting
* for the semaphore to be signaled, QXSemaphore_signal() removes the highest-
* priority thread waiting for the semaphore from the waiting list and makes
* this thread ready-to-run. The QXK scheduler is then called to determine if
* the awakened thread is now the highest-priority thread that is ready-to-run.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' when the semaphore signaled and 'false' when the semaphore count
* exceeded the maximum.
*
* @note
* A semaphore can be signaled from many places, including from ISRs, basic
* threads (AOs), and extended threads.
*/
bool QXSemaphore_signal(QXSemaphore * const me);

/*==========================================================================*/
/*! @brief Blocking Mutex the QXK preemptive kernel
* @class QXMutex
*
* @description
* ::QXMutex is a blocking mutual exclusion mechanism that can also apply
* the **priority ceiling protocol** to avoid unbounded priority inversion
* (if initialized with a non-zero ceiling priority, see QXMutex_init()).
* In that case, ::QXMutex requires its own uinque QP priority level, which
* cannot be used by any thread or any other ::QXMutex.
* If initialized with zero ceiling priority, ::QXMutex does **not** use the
* priority ceiling protocol and does not require a unique QP priority
* (see QXMutex_init()).
* ::QXMutex is **recursive** (re-entrant), which means that it can be locked
* multiple times (up to 255 levels) by the *same* thread without causing
* deadlock.
* ::QXMutex is primarily intended for the @ref ::QXThread
* "extened (blocking) threads", but can also be used by the @ref ::QActive
* "basic threads" through the non-blocking QXMutex_tryLock() API.
*
* @note
* ::QXMutex should be used in situations when at least one of the extended
* threads contending for the mutex blocks while holding the mutex (between
* the QXMutex_lock() and QXMutex_unlock() operations). If no blocking is
* needed while holding the mutex, the more efficient non-blocking mechanism
* of @ref srs_qxk_schedLock() "selective QXK scheduler locking" should be used
* instead. @ref srs_qxk_schedLock() "Selective scheduler locking" is available
* for both @ref ::QActive "basic threads" and @ref ::QXThread "extended
* threads", so it is applicable to situations where resources are shared
* among all these threads.
*
* @sa
* - QXMutex_init()
* - QXMutex_lock()
* - QXMutex_tryLock()
* - QXMutex_unlock()
*
* @usage
* The following example illustrates how to instantiate and use the mutex
* in your application.
* @include qxk_mutex.c
*/
typedef struct {
    QPSet waitSet; /*!< set of extended-threads waiting on this mutex */
    uint8_t volatile lockNest; /*!< lock-nesting up-down counter */
    uint8_t volatile holderPrio; /*!< priority of the lock holder thread */
    uint8_t ceiling; /*!< prioirty ceiling of this mutex */
} QXMutex;

/*! initialize the QXK priority-ceiling mutex ::QXMutex
* @public @memberof QXMutex
*
* @description
* Initialize the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     ceiling the ceiling-priority of this mutex or zero
*
* @note
* `ceiling == 0` means that the priority-ceiling protocol shall __not__
* be used by this mutex. Such mutex will __not__ change (boost) the
* priority of the holding thread.
*
* @note
* `ceiling > 0` means that the priority-ceiling protocol shall be used
* by this mutex. Such mutex __will__ boost the priority of the holding
* thread to the `ceiling` level for as long as the thread holds this mutex.
*
* @attention
* When the priority-ceiling protocol is used (`ceiling > 0`), the
* `ceiling` priority must be unused by any other thread or mutex.
* Also, the `ceiling` priority must be higher than priority of any thread
* that uses this mutex.
*
* @usage
* @include qxk_mutex.c
*/
void QXMutex_init(QXMutex * const me, uint_fast8_t ceiling);

/*! lock the QXK priority-ceiling mutex ::QXMutex
* @public @memberof QXMutex
*
* @description
* Lock the QXK priority ceiling mutex ::QXMutex.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]  nTicks    number of clock ticks (at the associated rate)
*                       to wait for the semaphore. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the semaphore will wait indefinitely.
* @returns
* 'true' if the mutex has been acquired and 'false' if a timeout occurred.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (< 255). However, each call to
* QXMutex_lock() must be balanced by the matching call to QXMutex_unlock().
*
* @usage
* @include qxk_mutex.c
*/
bool QXMutex_lock(QXMutex * const me,
                  uint_fast16_t const nTicks);

/*! try to lock the QXK priority-ceiling mutex ::QXMutex
* @public @memberof QXMutex
*
* @description
* Try to lock the QXK priority ceiling mutex ::QXMutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @returns
* 'true' if the mutex was successfully locked and 'false' if the mutex was
* unavailable and was NOT locked.
*
* @note
* This function **can** be called from both basic threads (active objects)
* and extended threads.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (<= 225). However, each successful
* call to QXMutex_tryLock() must be balanced by the matching call to
* QXMutex_unlock().
*/
bool QXMutex_tryLock(QXMutex * const me);

/*! unlock the QXK priority-ceiling mutex ::QXMutex
* @public @memberof QXMutex
*!
* @description
* Unlock the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note
* This function **can** be called from both basic threads (active objects)
* and extended threads.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (<= 225). However, each call to
* QXMutex_lock() or a *successful* call to QXMutex_tryLock() must be
* balanced by the matching call to QXMutex_unlock().
*
* @usage
* @include qxk_mutex.c
*/
void QXMutex_unlock(QXMutex * const me);

#endif /* QXTHREAD_H */
