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
* @date Last updated on: 2022-06-13
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QK/C (preemptive non-blocking kernel) platform-independent
* public interface.
*/
#ifndef QK_H
#define QK_H

#include "qequeue.h"  /* QK kernel uses the native QP event queue  */
#include "qmpool.h"   /* QK kernel uses the native QP memory pool  */
#include "qpset.h"    /* QK kernel uses the native QP priority set */

/*==========================================================================*/
/* QF configuration for QK -- data members of the QActive class... */

/* QK event-queue used for AOs */
#define QF_EQUEUE_TYPE      QEQueue

/* QK thread type used for AOs
* QK uses this member to store the private Thread-Local Storage pointer.
*/
#define QF_THREAD_TYPE      void*

/*==========================================================================*/
/*! @brief QK preemptive non-blocking kernel
* @class QK
*/
typedef struct {
    uint8_t volatile actPrio;    /*!< prio of the active AO */
    uint8_t volatile nextPrio;   /*!< prio of the next AO to execute */
    uint8_t volatile lockPrio;   /*!< lock prio (0 == no-lock) */
    uint8_t volatile lockHolder; /*!< prio of the AO holding the lock */
    uint8_t volatile intNest;    /*!< ISR nesting level */
    QPSet readySet;              /*!< QK ready-set of AOs */
} QK;

/*! global private attributes of the QK kernel
* @static @private @memberof QK
*/
extern QK QK_attr_;

/*==========================================================================*/
#ifdef QK_ON_CONTEXT_SW

    struct QActive; /* forward declaration */

    /*! QK context switch callback (customized in BSPs for QK)
    * @static @public @memberof QK
    *
    * @description
    * This callback function provides a mechanism to perform additional
    * custom operations when QK switches context from one thread to
    * another.
    *
    * @param[in] prev   pointer to the previous thread (active object)
    *                   (prev==0 means that @p prev was the QK idle loop)
    * @param[in] next   pointer to the next thread (active object)
    *                   (next==0) means that @p next is the QK idle loop)
    * @attention
    * QK_onContextSw() is invoked with interrupts **disabled** and must also
    * return with interrupts **disabled**.
    *
    * @note
    * This callback is enabled by defining the macro #QK_ON_CONTEXT_SW.
    *
    * @include qk_oncontextsw.c
    */
    void QK_onContextSw(struct QActive *prev, struct QActive *next);

#endif /* QK_ON_CONTEXT_SW */

/*! QK idle callback (customized in BSPs for QK)
* @static @public @memberof QK
*
* @description
* QK_onIdle() is called continuously by the QK idle loop. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note
* QK_onIdle() is invoked with interrupts enabled and must also return with
* interrupts enabled.
*
* @sa QV_onIdle()
*/
void QK_onIdle(void);

/*==========================================================================*/
/*! QK scheduler finds the highest-priority thread ready to run
* @static @private @memberof QK
*
* @description
* The QK scheduler finds out the priority of the highest-priority AO
* that (1) has events to process and (2) has priority that is above the
* current priority.
*
* @returns
* the 1-based priority of the the active object, or zero if no eligible
* active object is ready to run.
*
* @attention
* QK_sched_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
uint_fast8_t QK_sched_(void);

/*! QK activator activates the next active object. The activated AO preempts
* the currently executing AOs.
* @static @private @memberof QK
*
* @description
* QK_activate_() activates ready-to run AOs that are above the initial
* active priority (QK_attr_.actPrio).
*
* @note
* The activator might enable interrupts internally, but always returns with
* interrupts **disabled**.
*/
void QK_activate_(void);

/*==========================================================================*/
/*! QK Scheduler locking */

/*! The scheduler lock status */
typedef uint_fast16_t QSchedStatus;

/*! QK Scheduler lock
* @static @public @memberof QK
*
* @description
* This function locks the QK scheduler to the specified ceiling.
*
* @param[in]   ceiling    priority ceiling to which the QK scheduler
*                         needs to be locked
*
* @returns
* The previous QK Scheduler lock status, which is to be used to unlock
* the scheduler by restoring its previous lock status in QK_schedUnlock().
*
* @note
* QK_schedLock() must be always followed by the corresponding
* QK_schedUnlock().
*
* @sa QK_schedUnlock()
*
* @usage
* The following example shows how to lock and unlock the QK scheduler:
* @include qk_lock.c
*/
QSchedStatus QK_schedLock(uint_fast8_t ceiling);

/*! QK Scheduler unlock
* @static @public @memberof QK
*
* @description
* This function unlocks the QK scheduler to the previous status.
*
* @param[in]   stat       previous QK Scheduler lock status returned from
*                         QK_schedLock()
* @note
* QK_schedUnlock() must always follow the corresponding QK_schedLock().
*
* @sa QK_schedLock()
*
* @usage
* The following example shows how to lock and unlock the QK scheduler:
* @include qk_lock.c
*/
void QK_schedUnlock(QSchedStatus stat);

/*==========================================================================*/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #ifndef QK_ISR_CONTEXT_
        /*! Internal macro that reports the execution context (ISR vs. thread)
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QK_ISR_CONTEXT_() (QK_attr_.intNest != 0U)
    #endif /* QK_ISR_CONTEXT_ */

    /* QK-specific scheduler locking */
    /*! Internal macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_ QSchedStatus lockStat_;

    /*! Internal macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(prio_) do {         \
        if (QK_ISR_CONTEXT_()) {               \
            lockStat_ = 0xFFU;                 \
        } else {                               \
            lockStat_ = QK_schedLock((prio_)); \
        }                                      \
    } while (false)

    /*! Internal macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_() do {    \
        if (lockStat_ != 0xFFU) {      \
            QK_schedUnlock(lockStat_); \
        }                              \
    } while (false)

    /* native event queue operations... */
    #define QACTIVE_EQUEUE_WAIT_(me_)  \
        (Q_ASSERT_ID(110, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do {                         \
        QPSet_insert(&QK_attr_.readySet, (uint_fast8_t)(me_)->prio); \
        if (!QK_ISR_CONTEXT_()) {                                    \
            if (QK_sched_() != 0U) {                                 \
                QK_activate_();                                      \
            }                                                        \
        }                                                            \
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

#endif /* QK_H */
