/**
* @file
* @brief QF/C port to POSIX API (multi-threaded)
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-02-26
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* POSIX event queue and thread types */
#define QF_EQUEUE_TYPE       QEQueue
#define QF_OS_OBJECT_TYPE    pthread_cond_t
#define QF_THREAD_TYPE       uint8_t

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE        64U

/* The number of system clock tick rates */
#define QF_MAX_TICK_RATE     2U

/* various QF object sizes configuration for this port */
#define QF_EVENT_SIZ_SIZE    4U
#define QF_EQUEUE_CTR_SIZE   4U
#define QF_MPOOL_SIZ_SIZE    4U
#define QF_MPOOL_CTR_SIZE    4U
#define QF_TIMEEVT_CTR_SIZE  4U

/* QF critical section entry/exit for POSIX, see NOTE1 */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy) QF_enterCriticalSection_()
#define QF_CRIT_EXIT(dummy)  QF_leaveCriticalSection_()

#include <pthread.h>   /* POSIX-thread API */
#include "qep_port.h"  /* QEP port */
#include "qequeue.h"   /* POSIX needs event-queue */
#include "qmpool.h"    /* POSIX needs memory-pool */
#include "qf.h"        /* QF platform-independent public interface */

void QF_enterCriticalSection_(void);
void QF_leaveCriticalSection_(void);

/* set clock tick rate and p-thread priority */
void QF_setTickRate(uint32_t ticksPerSec, int_t tickPrio);

/* clock tick callback (NOTE not called when "ticker thread" is not running) */
void QF_onClockTick(void); /* clock tick callback (provided in the app) */

/* abstractions for console access... */
void QF_consoleSetup(void);
void QF_consoleCleanup(void);
int QF_consoleGetKey(void);
int QF_consoleWaitForKey(void);

/****************************************************************************/
/* interface used only inside QF implementation, but not in applications */
#ifdef QP_IMPL

    /* QF-specific scheduler locking (not used at this point) */
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) ((void)0)
    #define QF_SCHED_UNLOCK_()    ((void)0)

    /* POSIX active object event queue customization... */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        while ((me_)->eQueue.frontEvt == (QEvt *)0) \
            pthread_cond_wait(&(me_)->osObject, &QF_pThreadMutex_)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        Q_ASSERT_ID(410, QF_active_[(me_)->prio] != (QActive *)0); \
        pthread_cond_signal(&(me_)->osObject)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_  QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        QMPool_init(&(p_), poolSto_, poolSize_, evtSize_)
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), e_))

    /* mutex for QF critical section */
    extern pthread_mutex_t QF_pThreadMutex_;

#endif /* QP_IMPL */

/****************************************************************************/
/*
* NOTE1:
* QF, like all real-time frameworks, needs to execute certain sections of
* code exclusively, meaning that only one thread can execute the code at
* the time. Such sections of code are called "critical sections"
*
* This port uses a pair of functions QF_enterCriticalSection_() /
* QF_leaveCriticalSection_() to enter/leave the cirtical section,
* respectively.
*
* These functions are implemented in the qf_port.c module, where they
* manipulate the file-scope POSIX mutex object QF_pThreadMutex_
* to protect all critical sections. Using the single mutex for all crtical
* section guarantees that only one thread at a time can execute inside a
* critical section. This prevents race conditions and data corruption.
*
* Please note, however, that the POSIX mutex implementation behaves
* differently than interrupt disabling. A common POSIX mutex ensures
* that only one thread at a time can execute a critical section, but it
* does not guarantee that a context switch cannot occur within the
* critical section. In fact, such context switches probably will happen,
* but they should not cause concurrency hazards because the critical
* section eliminates all race conditionis.
*
* Unlinke simply disabling and enabling interrupts, the mutex approach is
* also subject to priority inversions. However, the p-thread mutex
* implementation, such as POSIX threads, should support the priority-
* inheritance protocol.
*/

#endif /* QF_PORT_H */

