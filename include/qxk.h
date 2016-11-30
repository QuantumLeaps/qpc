/**
* @file
* @brief QXK/C (preemptive dual-mode kernel) platform-independent
* public interface.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.8.0
* Last updated on  2016-11-19
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qxk_h
#define qxk_h

#include "qequeue.h"  /* QXK kernel uses the native QP event queue  */
#include "qmpool.h"   /* QXK kernel uses the native QP memory pool  */
#include "qpset.h"    /* QXK kernel uses the native QP priority set */

/****************************************************************************/
/* QF configuration for QXK */

/*! This macro defines the type of the event queue used for the
* active objects. */
/**
* @description
* QXK uses the native QF event queue QEQueue.
*/
#define QF_EQUEUE_TYPE      QEQueue

/*! OS-dependent representation of the private thread */
/**
* @description
* QXK uses this member to store the private stack poiner for the thread.
* (The private stack pointer is NULL for AO-threads).
*/
#define QF_THREAD_TYPE      void*

/****************************************************************************/
/*! attributes of the QXK kernel */
typedef struct {
    void *curr;  /*!< pointer to the current thread (0 for basic) */
    void *next;  /*!< pointer to the next thread to execute */
    uint_fast8_t volatile actPrio;    /*!< prio of the active basic thread */
    uint_fast8_t volatile lockPrio;   /*!< lock prio (0 == no-lock) */
    uint_fast8_t volatile lockHolder; /*!< prio of the lock holder */
#ifndef QXK_ISR_CONTEXT_
    uint_fast8_t intNest;    /*!< ISR nesting level */
#endif /* QXK_ISR_CONTEXT_ */
    QPSet readySet;          /*!< ready-set of basuc- and extended-threads */
} QXK_Attr;

/*! global attributes of the QXK kernel */
extern QXK_Attr QXK_attr_;

/****************************************************************************/
/*! QXK initialization */
/**
* QXK_init() is called from QF_init() in qxk.c. This function is
* defined in the QXK ports.
*/
void QXK_init(void);

/*! QXK idle callback (customized in BSPs for QXK) */
/**
* QXK_onIdle() is called continuously by the QXK idle loop. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note QXK_onIdle() is invoked with interrupts enabled and must also
* return with interrupts enabled.
*
* @sa QK_onIdle()
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
/*! QXK priority-ceiling mutex class */
typedef struct {
    uint_fast8_t lockPrio;   /*!< lock prio (priority ceiling) */
    uint_fast8_t prevPrio;   /*!< previoius lock prio */
    uint_fast8_t prevHolder; /*!< priority of the thread holding the lock */
} QXMutex;

/*! The QXMutex initialization */
void QXMutex_init(QXMutex * const me, uint_fast8_t prio);

/*! QXMutex lock */
void QXMutex_lock(QXMutex * const me);

/*! QXMutex unlock */
void QXMutex_unlock(QXMutex * const me);

/****************************************************************************/
/*! get the current QXK version number string of the form "X.Y.Z" */
#define QXK_getVersion() (QP_versionStr)

/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #ifndef QXK_ISR_CONTEXT_
        /*! Internal macro that reports the execution context (ISR vs. thread)
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QXK_ISR_CONTEXT_() (QXK_attr_.intNest != (uint_fast8_t)0)
    #endif /* QXK_ISR_CONTEXT_ */

    /* QF-specific scheduler locking */
    /*! Internal macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_ QXMutex schedLock_;

    /*! Internal macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(prio_) do { \
        if (QXK_ISR_CONTEXT_()) { \
            schedLock_.lockPrio = (uint_fast8_t)0; \
        } else { \
            QXMutex_init(&schedLock_, (prio_)); \
            QXMutex_lock(&schedLock_); \
        } \
    } while (0)

    /*! Internal macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_() do { \
        if (schedLock_.lockPrio != (uint_fast8_t)0) { \
            QXMutex_unlock(&schedLock_); \
        } \
    } while (0)

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
        QPSet_insert(&QXK_attr_.readySet, (me_)->prio); \
        if (!QXK_ISR_CONTEXT_()) { \
            if (QXK_sched_() != (uint_fast8_t)0) { \
                QXK_activate_(); \
            } \
        } \
    } while (0)

    #define QACTIVE_EQUEUE_ONEMPTY_(me_) ((void)0)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* QP_IMPL */

#endif /* qxk_h */
