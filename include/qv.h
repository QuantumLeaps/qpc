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
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QV/C (cooperative "Vanilla" kernel) platform-independent
* public interface
*/
#ifndef QV_H
#define QV_H

#include "qequeue.h"  /* QV kernel uses the native QP event queue  */
#include "qmpool.h"   /* QV kernel uses the native QP memory pool  */
#include "qpset.h"    /* QV kernel uses the native QP priority set */

/* QV event-queue used for AOs */
#define QF_EQUEUE_TYPE  QEQueue

/*! @brief The QV kernel class
* @class QV
*/
typedef struct {
    QPSet readySet; /*!< QV ready-set of AOs */
} QV;

/*! global private attributes of the QK kernel
* @static @private @memberof QV
*/
extern QV QV_attr_;

/*! QV idle callback (customized in BSPs)
* @static @public @memberof QV
*
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

/*==========================================================================*/
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
        QPSet_insert(&QV_attr_.readySet, (uint_fast8_t)(me_)->prio)

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

#endif /* QV_H */
