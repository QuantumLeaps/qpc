/**
* @file
* @brief QXK/C (preemptive dual-mode kernel) platform-independent
* public interface.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-14
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
#ifndef QXK_H
#define QXK_H

#include "qequeue.h"  /* QXK kernel uses the native QP event queue  */
#include "qmpool.h"   /* QXK kernel uses the native QP memory pool  */
#include "qpset.h"    /* QXK kernel uses the native QP priority set */

/****************************************************************************/
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

/****************************************************************************/
struct QActive; /* forward declaration */

/****************************************************************************/
/*! attributes of the QXK kernel */
typedef struct {
    struct QActive * volatile curr; /*!< current thread pointer (NULL=basic) */
    struct QActive * volatile next; /*!< next thread pointer to execute */
    uint8_t volatile actPrio;       /*!< prio of the active AO */
    uint8_t volatile lockPrio;      /*!< lock prio (0 == no-lock) */
    uint8_t volatile lockHolder;    /*!< prio of the lock holder */
    uint8_t volatile intNest;       /*!< ISR nesting level */
    struct QActive * idleThread;    /*!< pointer to the idle thread */
    QPSet readySet;                 /*!< ready-set of all threads */
} QXK_PrivAttr;

/*! global attributes of the QXK kernel */
extern QXK_PrivAttr QXK_attr_;

/****************************************************************************/
#ifdef QXK_ON_CONTEXT_SW

    /*! QXK context switch callback (customized in BSPs for QXK) */
    /**
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

/*! QXK idle callback (customized in BSPs for QXK) */
/**
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

/****************************************************************************/
/*! QXK scheduler finds the highest-priority thread ready to run */
uint_fast8_t QXK_sched_(void);

/*! QXK activator activates the next active object. The activated AO preempts
* the currently executing AOs.
*/
void QXK_activate_(void);

/****************************************************************************/
/*! return the currently executing active-object/thread */
struct QActive *QXK_current(void);

/****************************************************************************/
/*! QXK Scheduler locking */

/*! The scheduler lock status */
typedef uint_fast16_t QSchedStatus;

/*! QXK Scheduler lock */
QSchedStatus QXK_schedLock(uint_fast8_t ceiling);

/*! QXK Scheduler unlock */
void QXK_schedUnlock(QSchedStatus stat);

/****************************************************************************/
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
