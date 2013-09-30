/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 28, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qk_h
#define qk_h

/**
* \file
* \ingroup qk qf
* \brief QK/C platform-independent public interface.
*
* This header file must be included directly or indirectly
* in all modules (*.c files) that use QK/C.
*/

#include "qequeue.h"       /* The QK kernel uses the native QF event queue  */
#include "qmpool.h"        /* The QK kernel uses the native QF memory pool  */
#include "qpset.h"         /* The QK kernel uses the native QF priority set */

/****************************************************************************/
/* QF configuration for QK */

/** \brief This macro defines the type of the event queue used for the
* active objects.
*
* \note This is just an example of the macro definition. Typically, you need
* to define it in the specific QF port file (qf_port.h). In case of QK, which
* always depends on the native QF queue, this macro is defined at the level
* of the platform-independent interface qk.h.
*/
#define QF_EQUEUE_TYPE         QEQueue

#if defined(QK_TLS) || defined(QK_EXT_SAVE)
    #define QF_OS_OBJECT_TYPE      uint8_t
    #define QF_THREAD_TYPE         void *
#endif                                             /* QK_TLS || QK_EXT_SAVE */

#if (QF_MAX_ACTIVE <= 8)
    extern QPSet8  QK_readySet_;
#else
    extern QPSet64 QK_readySet_;                   /**< QK ready-set of AOs */
#endif


/****************************************************************************/
/** \brief QK scheduler
*
* \note QK_sched_() must be always called with interrupts disabled.
* The scheduler might enable interrupts internally, but always
* returns with interrupts disabled.
*/
void QK_sched_(uint8_t p);

/** \brief QK extended scheduler
*
* \note QK_schedExt_() must be always called with interrupts disabled.
* The scheduler might enable the interrupts internally, but always
* returns with interrupts disabled.
*/
void QK_schedExt_(uint8_t p);

/** \brief Find the highest-priority task ready to run
*
* \note QK_schedPrio_() must be always called with interrupts disabled
* and returns with interrupts disabled.
*/
uint8_t QK_schedPrio_(void);

/* public-scope objects */
extern uint8_t volatile QK_currPrio_;  /**< current task/interrupt priority */
extern uint8_t volatile QK_intNest_;           /**< interrupt nesting level */

/****************************************************************************/
/** \brief QK initialization
*
* QK_init() is called from QF_init() in qk.c. This function is
* defined in the QK ports.
*/
void QK_init(void);

/** \brief QK idle callback (customized in BSPs for QK)
*
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

    /** \brief QK Mutex type.
    *
    * QMutex represents the priority-ceiling mutex available in QK.
    * \sa QK_mutexLock()
    * \sa QK_mutexUnlock()
    */
    typedef uint8_t QMutex;

    /** \brief QK priority-ceiling mutex lock
    *
    * Lock the QK scheduler up to the priority level \a prioCeiling.
    *
    * \note This function should be always paired with QK_mutexUnlock(). The
    * code between QK_mutexLock() and QK_mutexUnlock() should be kept to the
    * minimum.
    *
    * \include qk_mux.c
    */
    QMutex QK_mutexLock(uint8_t const prioCeiling);

    /** \brief QK priority-ceiling mutex unlock
    *
    * \note This function should be always paired with QK_mutexLock(). The
    * code between QK_mutexLock() and QK_mutexUnlock() should be kept to the
    * minimum.
    *
    * \include qk_mux.c
    */
    void QK_mutexUnlock(QMutex mutex);

#endif                                                          /* QK_MUTEX */

/****************************************************************************/
/** \brief get the current QK version number string
*
* version of QK as a constant 5-character string of the form X.Y.Z,
* where X is a 1-digit major version number, Y is a 1-digit minor
* version number, and Z is a 1-digit release number.
*/
#define QK_getVersion() (QP_VERSION_STR)

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef qf_pkg_h

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT((me_)->eQueue.frontEvt != (QEvt *)0))

    #if (QF_MAX_ACTIVE <= 8)
        #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
            QPSet8_insert(&QK_readySet_, (me_)->prio); \
            if (QK_intNest_ == (uint8_t)0) { \
                uint8_t p = QK_schedPrio_(); \
                if (p != (uint8_t)0) { \
                    QK_sched_(p); \
                } \
            } \
        } while (0)

        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            (QPSet8_remove(&QK_readySet_, (me_)->prio))
    #else
        #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
            QPSet64_insert(&QK_readySet_, (me_)->prio); \
            if (QK_intNest_ == (uint8_t)0) { \
                uint8_t p = QK_schedPrio_(); \
                if (p != (uint8_t)0) { \
                    QK_sched_(p); \
                } \
            } \
        } while (0)

        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            QPSet64_remove(&QK_readySet_, (me_)->prio)
    #endif

    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (QMPoolSize)(evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((QEvtSize)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif                                                    /* ifdef qf_pkg_h */

#endif                                                              /* qk_h */

