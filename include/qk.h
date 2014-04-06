/**
* \file
* \brief QK/C platform-independent public interface.
* \ingroup qk
* \cond
******************************************************************************
* Product: QK/C
* Last updated for version 5.3.0
* Last updated on  2014-03-01
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
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
* \note This is just an example of the macro definition. Typically, you need
* to define it in the specific QF port file (qf_port.h). In case of QK, which
* always depends on the native QF queue, this macro is defined at the level
* of the platform-independent interface qk.h.
*/
#define QF_EQUEUE_TYPE         QEQueue

#if defined(QK_TLS) || defined(QK_EXT_SAVE)
    #define QF_OS_OBJECT_TYPE  uint_fast8_t
    #define QF_THREAD_TYPE     void *
#endif /* QK_TLS || QK_EXT_SAVE */

#if (QF_MAX_ACTIVE <= 8)
    extern QPSet8  QK_readySet_; /*!< QK ready-set of AOs */
#else
    extern QPSet64 QK_readySet_; /*!< QK ready-set of AOs */
#endif

/****************************************************************************/
/*! QK scheduler */
void QK_sched_(uint_fast8_t p);

/*! QK extended scheduler */
void QK_schedExt_(uint_fast8_t p);

/*! Find the highest-priority task ready to run */
uint_fast8_t QK_schedPrio_(void);

/* public-scope objects */
extern uint_fast8_t volatile QK_currPrio_; /*!< current task priority */
extern uint_fast8_t volatile QK_intNest_;  /*!< interrupt nesting level */

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
* \note QK_onIdle() is invoked with interrupts enabled and must also
* return with interrupts enabled.
*
* \sa QF_onIdle()
*/
void QK_onIdle(void);

#ifndef QK_NO_MUTEX

    /*! QK Mutex type. */
    /**
    * QMutex represents the priority-ceiling mutex available in QK.
    * \sa QK_mutexLock()
    * \sa QK_mutexUnlock()
    */
    typedef uint_fast8_t QMutex;

    /*! QK priority-ceiling mutex lock */
    QMutex QK_mutexLock(uint_fast8_t const prioCeiling);

    /*! QK priority-ceiling mutex unlock */
    void QK_mutexUnlock(QMutex mutex);

#endif /* QK_MUTEX */

/****************************************************************************/
/*! get the current QK version number string */
/**
* version of QK as a constant 5-character string of the form X.Y.Z,
* where X is a 1-digit major version number, Y is a 1-digit minor
* version number, and Z is a 1-digit release number.
*/
#define QK_getVersion() (QP_VERSION_STR)

/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))

    #if (QF_MAX_ACTIVE <= 8)
        #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
            QPSet8_insert(&QK_readySet_, (me_)->prio); \
            if (QK_intNest_ == (uint_fast8_t)0) { \
                uint_fast8_t p = QK_schedPrio_(); \
                if (p != (uint_fast8_t)0) { \
                    QK_sched_(p); \
                } \
            } \
        } while (0)

        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            (QPSet8_remove(&QK_readySet_, (me_)->prio))
    #else
        #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
            QPSet64_insert(&QK_readySet_, (me_)->prio); \
            if (QK_intNest_ == (uint_fast8_t)0) { \
                uint_fast8_t p = QK_schedPrio_(); \
                if (p != (uint_fast8_t)0) { \
                    QK_sched_(p); \
                } \
            } \
        } while (0)

        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            QPSet64_remove(&QK_readySet_, (me_)->prio)
    #endif

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* QP_IMPL */

#endif /* qk_h */

