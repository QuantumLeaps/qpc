/**
* @file
* @brief QXK/C (preemptive extended (blocking) kernel) platform-independent
* public interface.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.7.0
* Last updated on  2016-07-14
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

#include "qequeue.h"  /* QXK kernel uses the native QF event queue  */
#include "qmpool.h"   /* QXK kernel uses the native QF memory pool  */
#include "qpset.h"    /* QXK kernel uses the native QF priority set */

/****************************************************************************/
/* QXK-specific data members of the ::QMActive class... */

/*! This macro defines the type of the event queue used for the AOs */
/**
* @description
* QXK uses the native QF event queue QEQueue.
*/
#define QF_EQUEUE_TYPE      QEQueue

/*! OS-dependent per-thread operating-system object */
/**
* @description
* The use of this member depends on the CPU. For example, in port to
* ARM Cortex-M with FPU this member is used to store the LR.
*/
#define QF_OS_OBJECT_TYPE   void*

/*! OS-dependent representation of the private thread */
/**
* @description
* QXK uses this member to store thread attributes.
*/
#define QF_THREAD_TYPE      QXK_ThreadType

/****************************************************************************/
/*! Type of the QMActive.thread member for the QXK kernel */
typedef struct {
    void *stack;            /*!< top of the per-thread stack */
    /* ... possibly other thread attributes in the future */
} QXK_ThreadType;

/*! attributes of the QXK kernel */
typedef struct {
    void volatile *curr;  /*!< currently executing thread */
    void volatile *next;  /*!< next thread to execute */
    uint_fast8_t volatile lockPrio;   /*!< lock prio (0 == no-lock) */
    uint_fast8_t volatile lockHolder; /*!< prio of the lock holder */
#ifndef QXK_ISR_CONTEXT_
    uint_fast8_t volatile intNest; /*!< ISR nesting level */
#endif /* QXK_ISR_CONTEXT_ */
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  readySet;     /*!< QXK ready-set of AOs and "naked" threads */
#else
    QPSet64 readySet;     /*!< QXK ready-set of AOs and "naked" threads */
#endif
} QXK_Attr;

/*! global attributes of the QXK kernel */
extern QXK_Attr QXK_attr_;

/****************************************************************************/
/*! QXK initialization */
/**
* QXK_init() must be called from the application before QF_run()
* to initialize the QXK idle thread.
*/
void QXK_init(void *idleStkSto, uint_fast16_t idleStkSize);

/*! QXK idle callback (customized in BSPs for QXK) */
/**
* QXK_onIdle() is called continuously by the QXK idle loop. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note QXK_onIdle() is invoked with interrupts enabled and must also
* return with interrupts enabled.
*
* @sa QK_onIdle() QV_onIdle()
*/
void QXK_onIdle(void);

/****************************************************************************/
/*! QXK scheduler */
void QXK_sched_(void);

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
        /*! Internal port-specific macro that reports the execution context
        * (ISR vs. thread).
        */
        /*! @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QXK_ISR_CONTEXT_() (QXK_attr_.intNest != (uint_fast8_t)0)
    #endif /* QXK_ISR_CONTEXT_ */

    /* QF-specific scheduler locking */
    /*! Internal port-specific macro to represent the scheduler lock status
    * that needs to be preserved to allow nesting of locks.
    */
    #define QF_SCHED_STAT_TYPE_ QXMutex

    /*! Internal port-specific macro for selective scheduler locking. */
    #define QF_SCHED_LOCK_(pLockStat_, prio_) do { \
        if (QXK_ISR_CONTEXT_()) { \
            (pLockStat_)->lockPrio = (uint_fast8_t)(QF_MAX_ACTIVE + 1); \
        } else { \
            QXMutex_init((pLockStat_), (prio_)); \
            QXMutex_lock((pLockStat_)); \
        } \
    } while (0)

    /*! Internal port-specific macro for selective scheduler unlocking. */
    #define QF_SCHED_UNLOCK_(pLockStat_) QXMutex_unlock((pLockStat_))

    #if (QF_MAX_ACTIVE <= 8)
        #define QXK_prioNotEmpty(set_)    QPSet8_notEmpty((set_))
        #define QXK_prioFindMax(set_, p_) QPSet8_findMax((set_), (p_))
        #define QXK_prioInsert(set_, p_)  QPSet8_insert((set_), (p_))
        #define QXK_prioRemove(set_, p_)  QPSet8_remove((set_), (p_))
    #else
        #define QXK_prioNotEmpty(set_)    QPSet64_notEmpty((set_))
        #define QXK_prioFindMax(set_, p_) QPSet64_findMax((set_), (p_))
        #define QXK_prioInsert(set_, p_)  QPSet64_insert((set_), (p_))
        #define QXK_prioRemove(set_, p_)  QPSet64_remove((set_), (p_))
    #endif

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        if ((me_)->eQueue.frontEvt == (QEvt *)0) { \
            QXK_prioRemove(&QXK_attr_.readySet, (me_)->prio); \
            QXK_sched_(); \
            QF_CRIT_EXIT_(); \
            QF_CRIT_EXIT_NOP(); \
            QF_CRIT_ENTRY_(); \
        }

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
        QXK_prioInsert(&QXK_attr_.readySet, (me_)->prio); \
        if (!QXK_ISR_CONTEXT_()) { \
            QXK_sched_(); \
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
