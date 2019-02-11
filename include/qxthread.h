/**
* @file
* @brief QXK/C eXtended (blocking) thread.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 6.4.0
* Last updated on  2019-02-08
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
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
* @sa
* - QXThread_ctor()
* - QXTHREAD_START()
* - QXTHREAD_POST_X()
* - Q_XTHREAD_CAST()
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
typedef struct {
    QActive super;    /*!< inherited ::QActive */
    QTimeEvt timeEvt; /*!< time event to handle blocking timeouts */
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
* @param[in]     par_     pointer to the additional port-specific parameter(s)
*                         (might be NULL).
* @usage
* @include qxk_start.c
*/
#define QXTHREAD_START(me_, prio_, qSto_, qLen_, stkSto_, stkLen_, par_) do {\
    Q_ASSERT((me_)->super.super.vptr);                                       \
    ((*((QActiveVtbl const *)((me_)->super.super.vptr))->start)(             \
        &(me_)->super, (prio_), (QEvt const **)(qSto_), (qLen_),             \
        (stkSto_), (stkLen_), (par_)));                                      \
} while (0)

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
*                must still be available after posting the event.
*                The special value #QF_NO_MARGIN causes asserting failure
*                in case event allocation fails.
* @param[in]     sender_ pointer to the sender object.
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

/*! delay (block) the current extended thread for a specified # ticks */
bool QXThread_delay(uint_fast16_t const nTicks);

/*! cancel the delay */
bool QXThread_delayCancel(QXThread * const me);

/*! obtain a message from the private message queue (block if no messages) */
QEvt const *QXThread_queueGet(uint_fast16_t const nTicks);

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
/**
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

/*! initialize the counting semaphore */
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count,
                      uint_fast16_t max_count);

/*! wait (block) on the semaphore */
bool QXSemaphore_wait(QXSemaphore * const me,
                      uint_fast16_t const nTicks);

/*! try wait on the semaphore (non-blocking) */
bool QXSemaphore_tryWait(QXSemaphore * const me);

/*! signal (unblock) the semaphore */
bool QXSemaphore_signal(QXSemaphore * const me);


/****************************************************************************/
/*! Blocking Mutex the QXK preemptive kernel */
/**
* @description
* ::QXMutex is a blocking mutual exclusion mechanism that can also apply
* the **priority ceiling protocol** to avoid unbounded priority inversion
* (if initialized with a non-zero ceiling priority, see QXMutex_init()).
* In that case, ::QXMutex requires its own uinque QP priority level, which
* cannot be used by any thread or any other ::QXMutex.
* If initialzied with zero ceiling priority, ::QXMutex does **not** use the
* priority ceiling protocol and does not require a unique QP priority
* (see QXMutex_init()).
* ::QXMutex is **recursive** (reentrant), which means that it can be locked
* mutiliple times (up to 255 levels) by the *same* thread without causing
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
* of @ref QXK_schedLock() "selective QXK scheduler locking" should be used
* instead. @ref QXK_schedLock() "Selective scheduler locking" is available
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

/*! initialize the QXK priority-ceiling mutex ::QXMutex */
void QXMutex_init(QXMutex * const me, uint_fast8_t ceiling);

/*! lock the QXK priority-ceiling mutex ::QXMutex */
bool QXMutex_lock(QXMutex * const me,
                  uint_fast16_t const nTicks);

/*! try to lock the QXK priority-ceiling mutex ::QXMutex */
bool QXMutex_tryLock(QXMutex * const me);

/*! unlock the QXK priority-ceiling mutex ::QXMutex */
void QXMutex_unlock(QXMutex * const me);

#endif /* qxthread_h */

