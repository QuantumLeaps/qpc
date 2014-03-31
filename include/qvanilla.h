/**
* \file
* \brief platform-independent interface to the cooperative "vanilla" kernel.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-24
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
#ifndef qvanilla_h
#define qvanilla_h

#include "qequeue.h" /* "Vanilla" kernel uses the native QF event queue  */
#include "qmpool.h"  /* "Vanilla" kernel uses the native QF memory pool  */
#include "qpset.h"   /* "Vanilla" kernel uses the native QF priority set */

/*! This macro defines the type of the event queue used for the
* active objects. For the built-in Vanilla kernel, this is ::QEqueue.
*/
#define QF_EQUEUE_TYPE  QEQueue

/*! QF idle callback (customized in BSPs for QF) */
/**
* \description
* QF_onIdle() is called by the non-preemptive "Vanilla" scheduler
* (from QF_run()) when the scheduler detects that no events are available
* for active objects (the idle condition). This callback gives the
* application an opportunity to enter a power-saving CPU mode, or perform
* some other idle processing (such as Q-Spy output).
*
* \note QF_onIdle() is invoked with interrupts DISABLED because the idle
* condition can be asynchronously changed at any time by an interrupt.
* QF_onIdle() MUST enable the interrupts internally, but not before
* putting the CPU into the low-power mode. (Ideally, enabling interrupts and
* low-power mode should happen atomically). At the very least, the function
* MUST enable interrupts, otherwise interrupts will remain disabled
* permanently.
*
* \note QF_onIdle() is only used by the non-preemptive "Vanilla" scheduler
* in the "bare metal" QF port, and is NOT used in any other QF ports. When
* QF is combined with QK, the QK idle loop calls a different function
* QK_onIdle(), with different semantics than QF_onIdle(). When QF is
* combined with a 3rd-party RTOS or kernel, the idle processing mechanism
* of the RTOS or kernel is used instead of QF_onIdle().
*/
void QF_onIdle(void);

/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    /* native QF event queue operations */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0)

    #if (QF_MAX_ACTIVE <= 8)
        #define QACTIVE_EQUEUE_SIGNAL_(me_) \
            QPSet8_insert(&QF_readySet_, (me_)->prio)
        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            QPSet8_remove(&QF_readySet_, (me_)->prio)
    #else
        #define QACTIVE_EQUEUE_SIGNAL_(me_) \
            QPSet64_insert(&QF_readySet_, (me_)->prio)
        #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
            QPSet64_remove(&QF_readySet_, (me_)->prio)
    #endif

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

    #if (QF_MAX_ACTIVE <= 8)
        extern QPSet8 QF_readySet_;  /*!< QF-ready set of AOs */
    #else
        extern QPSet64 QF_readySet_; /*!< QF-ready set of AOs */
    #endif

#endif /* QP_IMPL */

#endif /* qvanilla_h */
