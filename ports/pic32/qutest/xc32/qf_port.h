/**
* @file
* @brief QF/C port to PIC32, QUTEST unit test harness, generic C99 compiler
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
#ifndef QF_PORT_H
#define QF_PORT_H

/* QUTEST event queue and thread types */
#define QF_EQUEUE_TYPE QEQueue
/*#define QF_OS_OBJECT_TYPE */
/*#define QF_THREAD_TYPE */

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE        32U

/* The number of system clock tick rates */
#define QF_MAX_TICK_RATE     2U

/* QF interrupt disable/enable */
#define QF_INT_DISABLE()     (++QF_intNest)
#define QF_INT_ENABLE()      (--QF_intNest)

/* QF critical section */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy) QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)  QF_INT_ENABLE()

/* QF_LOG2 not defined -- use the internal LOG2() implementation */

#include "qep_port.h"  /* QEP port */
#include "qequeue.h"   /* QUTEST port uses QEQueue event-queue */
#include "qmpool.h"    /* QUTEST port uses QMPool memory-pool */
#include "qf.h"        /* QF platform-independent public interface */

/* interrupt nesting up-down counter */
extern uint8_t volatile QF_intNest;

/****************************************************************************/
/* interface used only inside QF implementation, but not in applications */
#ifdef QP_IMPL

    /* QUTEST scheduler locking (not used) */
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) ((void)0)
    #define QF_SCHED_UNLOCK_()    ((void)0)

    /* native event queue operations */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QS_rxPriv_.readySet, (uint_fast8_t)(me_)->prio)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
    #define QF_EPOOL_PUT_(p_, e_, qs_id_) \
        (QMPool_put(&(p_), (e_), (qs_id_)))

    #include "qf_pkg.h" /* internal QF interface */

#endif /* QP_IMPL */

#endif /* QF_PORT_H */
