/**
* @file
* @brief QK/C (preemptive non-blocking kernel) platform-independent
* public interface.
* @ingroup qk
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
#ifndef qk_h
#define qk_h

#include "qequeue.h"  /* QK kernel uses the native QP event queue  */
#include "qmpool.h"   /* QK kernel uses the native QP memory pool  */
#include "qpset.h"    /* QK kernel uses the native QP priority set */

/****************************************************************************/
/* QF configuration for QK */

/*! This macro defines the type of the event queue used for the
* active objects. */
/**
* @description
* QK uses the native QF event queue QEQueue.
*/
#define QF_EQUEUE_TYPE      QEQueue

/****************************************************************************/
/*! attributes of the QK kernel */
typedef struct {
    uint_fast8_t volatile actPrio;  /*!< prio of the active AO */
    uint_fast8_t volatile nextPrio; /*!< prio of the next AO to execute */
    uint_fast8_t volatile lockPrio;   /*!< lock prio (0 == no-lock) */
    uint_fast8_t volatile lockHolder; /*!< prio of the AO holding the lock */
#ifndef QK_ISR_CONTEXT_
    uint_fast8_t volatile intNest;    /*!< ISR nesting level */
#endif /* QK_ISR_CONTEXT_ */
    QPSet readySet; /*!< QK ready-set of AOs */
} QK_Attr;

/*! global attributes of the QK kernel */
extern QK_Attr QK_attr_;

/****************************************************************************/
/*! QK initialization */
/**
* QK_init() is called from QF_init() in qk.c. This function is
* defined in the QK ports.
*/
void QK_init(void);

/*! QK idle callback (customized in BSPs for QK) */
/**
* QK_onIdle() is called continuously by the QK idle loop. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note QK_onIdle() is invoked with interrupts enabled and must also
* return with interrupts enabled.
*
* @sa QV_onIdle()
*/
void QK_onIdle(void);

/****************************************************************************/
/*! QK scheduler finds the highest-priority thread ready to run */
uint_fast8_t QK_sched_(void);

/*! QK activator activates the next active object. The activated AO preempts
* the currently executing AOs.
*/
void QK_activate_(void);

/****************************************************************************/
/*! QK priority-ceiling mutex class */
typedef struct {
    uint_fast8_t lockPrio;   /*!< lock prio (priority ceiling) */
    uint_fast8_t prevPrio;   /*!< previoius lock prio */
} QMutex;

/*! The QMutex initialization */
void QMutex_init(QMutex * const me, uint_fast8_t prio);

/*! QMutex lock */
void QMutex_lock(QMutex * const me);

/*! QMutex unlock */
void QMutex_unlock(QMutex * const me);

/****************************************************************************/
/*! get the current QK version number string of the form "X.Y.Z" */
#define QK_getVersion() (QP_versionStr)

/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #ifndef QK_ISR_CONTEXT_
        /*! Internal macro that reports the execution context (ISR vs. thread)
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QK_ISR_CONTEXT_() (QK_attr_.intNest != (uint_fast8_t)0)
    #endif /* QK_ISR_CONTEXT_ */

    /* QK-specific scheduler locking */
    /*! Internal macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_ QMutex schedLock_;

    /*! Internal macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(prio_) do { \
        if (QK_ISR_CONTEXT_()) { \
            schedLock_.lockPrio = (uint_fast8_t)0; \
        } else { \
            QMutex_init(&schedLock_, (prio_)); \
            QMutex_lock(&schedLock_); \
        } \
    } while (0)

    /*! Internal macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_() do { \
        if (schedLock_.lockPrio != (uint_fast8_t)0) { \
            QMutex_unlock(&schedLock_); \
        } \
    } while (0)

    /* native event queue operations... */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
        QPSet_insert(&QK_attr_.readySet, (me_)->prio); \
        if (!QK_ISR_CONTEXT_()) { \
            if (QK_sched_() != (uint_fast8_t)0) { \
                QK_activate_(); \
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

#endif /* qk_h */
