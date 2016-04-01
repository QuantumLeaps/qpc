/**
* @file
* @brief QK/C (preemptive non-blocking kernel) platform-independent
* public interface.
* @ingroup qk
* @cond
******************************************************************************
* Last updated for version 5.6.2
* Last updated on  2016-03-31
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

#include "qequeue.h"  /* QK kernel uses the native QF event queue  */
#include "qmpool.h"   /* QK kernel uses the native QF memory pool  */
#include "qpset.h"    /* QK kernel uses the native QF priority set */

/****************************************************************************/
/* QF configuration for QK */

/*! This macro defines the type of the event queue used for the
* active objects. */
/**
* @note This is just an example of the macro definition. Typically, you need
* to define it in the specific QF port file (qf_port.h). In case of QK, which
* always depends on the native QF queue, this macro is defined at the level
* of the platform-independent interface qk.h.
*/
#define QF_EQUEUE_TYPE         QEQueue

/*! OS-dependent per-thread operating-system object */
/**
* @description
* The use of this member depends on the CPU. For example, in port to
* ARM Cortex-M with FPU this member is used to store the LR.
*/
#define QF_OS_OBJECT_TYPE      void*

/*! OS-dependent representation of the private thread */
/**
* @description
* QK uses this member to store the start priority of the AO,
* which is needed when the QK priority-ceiling mutex is used.
*/
#define QF_THREAD_TYPE         uint_fast8_t

#if (QF_MAX_ACTIVE <= 8)
    extern QPSet8  QK_readySet_; /*!< QK ready-set of AOs */
#else
    extern QPSet64 QK_readySet_; /*!< QK ready-set of AOs */
#endif

/****************************************************************************/
/*! QK scheduler */
void QK_sched_(uint_fast8_t p);

/*! Find the highest-priority task ready to run */
uint_fast8_t QK_schedPrio_(void);

/*! priority of the current task */
extern uint_fast8_t volatile QK_currPrio_;

#ifndef QF_ISR_CONTEXT_
    extern uint_fast8_t volatile QK_intNest_;  /*!< ISR nesting level */
#endif /* QF_ISR_CONTEXT_ */

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
/*! QK priority-ceiling mutex class */
typedef struct {
    uint_fast8_t lockPrio; /*!< lock prio (priority ceiling) */
    uint_fast8_t prevPrio; /*!< previoius lock prio */
} QMutex;

/*! The QK mutex initialization */
void QMutex_init(QMutex * const me, uint_fast8_t prio);

/*! QMutex lock */
void QMutex_lock(QMutex * const me);

/*! QMutex unlock */
void QMutex_unlock(QMutex * const me);

extern uint_fast8_t volatile QK_lockPrio_;   /*!< lock prio (0 == no-lock) */

/****************************************************************************/
/*! get the current QK version number string of the form "X.Y.Z" */
#define QK_getVersion() (QP_versionStr)

/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #ifndef QK_ISR_CONTEXT_
        /*! Internal port-specific macro that reports the execution context
        * (ISR vs. thread).
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QK_ISR_CONTEXT_() (QK_intNest_ != (uint_fast8_t)0)
    #endif /* QK_ISR_CONTEXT_ */

    /* QF-specific scheduler locking */
    /*! Internal port-specific macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_TYPE_ QMutex

    /*! Internal port-specific macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(pLockStat_, prio_) do { \
        if (QK_ISR_CONTEXT_()) { \
            (pLockStat_)->lockPrio = (uint_fast8_t)(QF_MAX_ACTIVE + 1); \
        } else { \
            QMutex_init((pLockStat_), (prio_)); \
            QMutex_lock((pLockStat_)); \
        } \
    } while (0)

    /*! Internal port-specific macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_(pLockStat_) QMutex_unlock((pLockStat_))

    #if (QF_MAX_ACTIVE <= 8)
        #define QK_prioNotEmpty(set_)    QPSet8_notEmpty((set_))
        #define QK_prioIsSet(set_, p_)   QPSet8_hasElement((set_), (p_))
        #define QK_prioFindMax(set_, p_) QPSet8_findMax((set_), (p_))
        #define QK_prioInsert(set_, p_)  QPSet8_insert((set_), (p_))
        #define QK_prioRemove(set_, p_)  QPSet8_remove((set_), (p_))
    #else
        #define QK_prioNotEmpty(set_)    QPSet64_notEmpty((set_))
        #define QK_prioIsSet(set_, p_)   QPSet64_hasElement((set_), (p_))
        #define QK_prioFindMax(set_, p_) QPSet64_findMax((set_), (p_))
        #define QK_prioInsert(set_, p_)  QPSet64_insert((set_), (p_))
        #define QK_prioRemove(set_, p_)  QPSet64_remove((set_), (p_))
    #endif

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
        QK_prioInsert(&QK_readySet_, (me_)->prio); \
        if (!QK_ISR_CONTEXT_()) { \
            uint_fast8_t p = QK_schedPrio_(); \
            if (p != (uint_fast8_t)0) { \
                QK_sched_(p); \
            } \
        } \
    } while (0)

    #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
        QK_prioRemove(&QK_readySet_, (me_)->prio)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* QP_IMPL */

#endif /* qk_h */
