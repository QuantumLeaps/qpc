/**
* @file
* @brief QV/C (cooperative "Vanilla" kernel) platform-independent
* public interface
* @ingroup qv
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-08
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
#ifndef QV_H
#define QV_H

#include "qequeue.h"  /* QV kernel uses the native QP event queue  */
#include "qmpool.h"   /* QV kernel uses the native QP memory pool  */
#include "qpset.h"    /* QV kernel uses the native QP priority set */

/* QV event-queue used for AOs */
#define QF_EQUEUE_TYPE      QEQueue

/*! QV idle callback (customized in BSPs) */
/**
* @description
* QV_onIdle() is called by the cooperative QV kernel (from QF_run()) when
* the scheduler detects that no events are available for active objects
* (the idle condition). This callback gives the application an opportunity
* to enter a power-saving CPU mode, or perform some other idle processing
* (such as QS software tracing output).
*
* @note QV_onIdle() is invoked with interrupts DISABLED because the idle
* condition can be asynchronously changed at any time by an interrupt.
* QV_onIdle() MUST enable the interrupts internally, but not before
* putting the CPU into the low-power mode. (Ideally, enabling interrupts and
* low-power mode should happen atomically). At the very least, the function
* MUST enable interrupts, otherwise interrupts will remain disabled
* permanently.
*
* @note QV_onIdle() is only used by the cooperative QV kenrnel in the
* native (bare metal) QF ports, and is NOT used in any other QF ports. When
* QF is combined with the preemptive QK, the QK idle loop calls a different
* function QK_onIdle(), with different semantics than QV_onIdle(). When QF
* is combined with a 3rd-party RTOS or kernel, the idle processing mechanism
* of the RTOS or kernel is used instead of QV_onIdle().
*/
void QV_onIdle(void);


/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    /* QV-specific scheduler locking (not needed in QV) */
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy)   ((void)0)
    #define QF_SCHED_UNLOCK_(dummy) ((void)0)

    /* native QF event queue operations... */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0)

    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QV_readySet_, (uint_fast8_t)(me_)->prio)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
    #define QF_EPOOL_PUT_(p_, e_, qs_id_) \
        (QMPool_put(&(p_), (e_), (qs_id_)))

    extern QPSet QV_readySet_; /*!< QV ready-set of AOs */

#endif /* QP_IMPL */

#endif /* QV_H */
