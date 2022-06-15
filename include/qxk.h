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
* @brief QXK/C (preemptive dual-mode kernel) platform-independent
* public interface.
*/
#ifndef QXK_H
#define QXK_H

#include "qequeue.h"  /* QXK kernel uses the native QP event queue  */
#include "qmpool.h"   /* QXK kernel uses the native QP memory pool  */
#include "qpset.h"    /* QXK kernel uses the native QP priority set */

/*==========================================================================*/
/* QF configuration for QXK -- data members of the QActive class... */

/* QXK event-queue used for AOs */
#define QF_EQUEUE_TYPE      QEQueue

/* QXK OS-object used to store the private stack poiner for extended threads.
* (The private stack pointer is NULL for basic-threads).
*/
#define QF_OS_OBJECT_TYPE  void*

/* QXK thread type used to store the private Thread-Local Storage pointer. */
#define QF_THREAD_TYPE     void*

/*! Access Thread-Local Storage (TLS) and cast it on the given @p type_ */
#define QXK_TLS(type_) ((type_)QXK_current()->thread)

/*==========================================================================*/
struct QActive; /* forward declaration */

/*==========================================================================*/
/*! @brief The QXK kernel class
* @class QXK
*/
typedef struct {
    struct QActive * volatile curr; /*!< current thread pointer (NULL=basic) */
    struct QActive * volatile next; /*!< next thread pointer to execute */
    uint8_t volatile actPrio;       /*!< prio of the active AO */
    uint8_t volatile lockPrio;      /*!< lock prio (0 == no-lock) */
    uint8_t volatile lockHolder;    /*!< prio of the lock holder */
    uint8_t volatile intNest;       /*!< ISR nesting level */
    struct QActive * idleThread;    /*!< pointer to the idle thread */
    QPSet readySet;                 /*!< ready-set of all threads */
} QXK;

/*! global attributes of the QXK kernel
* @static @private @memberof QXK
*/
extern QXK QXK_attr_;

/*==========================================================================*/
#ifdef QXK_ON_CONTEXT_SW

    /*! QXK context switch callback (customized in BSPs for QXK)
    * @static @public @memberof QXK
    *
    * @description
    * This callback function provides a mechanism to perform additional
    * custom operations when QXK switches context from one thread to
    * another.
    *
    * @param[in] prev   pointer to the previous thread (active object)
    *                   (prev==0 means that @p prev was the QXK idle thread)
    * @param[in] next   pointer to the next thread (active object)
    *                   (next==0) means that @p next is the QXK idle thread)
    * @attention
    * QXK_onContextSw() is invoked with interrupts **disabled** and must also
    * return with interrupts **disabled**.
    *
    * @note
    * This callback is enabled by defining the macro #QXK_ON_CONTEXT_SW.
    *
    * @include qxk_oncontextsw.c
    */
    void QXK_onContextSw(struct QActive *prev, struct QActive *next);

#endif /* QXK_ON_CONTEXT_SW */

/*! QXK idle callback (customized in BSPs for QXK)
* @static @public @memberof QXK
*
* @description
* QXK_onIdle() is called continuously by the QXK idle thread. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note
* QXK_onIdle() is invoked with interrupts enabled and must also return with
* interrupts enabled.
*/
void QXK_onIdle(void);

/*==========================================================================*/
/*! QXK scheduler finds the highest-priority thread ready to run
* @static @private @memberof QXK
*
* @description
* The QXK scheduler finds the priority of the highest-priority thread
* that is ready to run.
*
* @returns
* the 1-based priority of the the thread (basic or extended) run next,
* or zero if no eligible thread is found.
*
* @attention
* QXK_sched_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
uint_fast8_t QXK_sched_(void);

/*! QXK activator activates the next active object. The activated AO preempts
* the currently executing AOs.
* @static @private @memberof QXK
*
* @attention
* QXK_activate_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*
* @note
* The activate function might enable interrupts internally, but it always
* returns with interrupts **disabled**.
*/
void QXK_activate_(void);

/*==========================================================================*/
/*! obtain the currently executing active-object/thread
* @static @public @memberof QXK
*
* @returns
* pointer to the currently executing active-object/thread
*/
struct QActive *QXK_current(void);

/*==========================================================================*/
/*! The scheduler lock status */
typedef uint_fast16_t QSchedStatus;

/*! QXK Scheduler lock
* @static @public @memberof QXK
*
* @description
* This function locks the QXK scheduler to the specified ceiling.
*
* @param[in]   ceiling    priority ceiling to which the QXK scheduler
*                         needs to be locked
*
* @returns
* The previous QXK Scheduler lock status, which is to be used to unlock
* the scheduler by restoring its previous lock status in QXK_schedUnlock().
*
* @note
* A QXK scheduler can be locked from both basic threads (AOs) and
* extended threads and the scheduler locks can nest.
*
* @note
* QXK_schedLock() must be always followed by the corresponding
* QXK_schedUnlock().
*
* @attention
* QXK will fire an assertion if a thread holding the lock attempts
* to block.
*
* @sa QXK_schedUnlock()
*
* @usage
* The following example shows how to lock and unlock the QXK scheduler:
* @include qxk_lock.c
*/
QSchedStatus QXK_schedLock(uint_fast8_t ceiling);

/*! QXK Scheduler unlock
* @static @public @memberof QXK
*
* @description
* This function unlocks the QXK scheduler to the previous status.
*
* @param[in]   stat       previous QXK Scheduler lock status returned from
*                         QXK_schedLock()
*
* @note
* A QXK scheduler can be locked from both basic threads (AOs) and
* extended threads and the scheduler locks can nest.
*
* @note
* QXK_schedUnlock() must always follow the corresponding QXK_schedLock().
*
* @sa QXK_schedLock()
*
* @usage
* The following example shows how to lock and unlock the QXK scheduler:
* @include qxk_lock.c
*/
void QXK_schedUnlock(QSchedStatus stat);

/*==========================================================================*/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #ifndef QXK_ISR_CONTEXT_
        /*! Internal macro that reports the execution context (ISR vs. thread)
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QXK_ISR_CONTEXT_() (QXK_attr_.intNest != 0U)
    #endif /* QXK_ISR_CONTEXT_ */

    /* QF-specific scheduler locking */
    /*! Internal macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_ QSchedStatus lockStat_;

    /*! Internal macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(prio_) do {          \
        if (QXK_ISR_CONTEXT_()) {               \
            lockStat_ = 0xFFU;                  \
        } else {                                \
            lockStat_ = QXK_schedLock((prio_)); \
        }                                       \
    } while (false)

    /*! Internal macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_() do {     \
        if (lockStat_ != 0xFFU) {       \
            QXK_schedUnlock(lockStat_); \
        }                               \
    } while (false)

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(110, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do {                             \
        QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)(me_)->dynPrio); \
        if (!QXK_ISR_CONTEXT_()) {                                       \
            if (QXK_sched_() != 0U) {                                    \
                QXK_activate_();                                         \
            }                                                            \
        }                                                                \
    } while (false)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
    #define QF_EPOOL_PUT_(p_, e_, qs_id_) \
        (QMPool_put(&(p_), (e_), (qs_id_)))

#endif /* QP_IMPL */

#endif /* QXK_H */
