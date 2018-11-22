/**
* @file
* @brief QF/C "port" for QUTest unit test harness, Win32 with GNU or VisualC++
* @ingroup ports
* @cond
******************************************************************************
* Last Updated for Version: 6.3.7
* Date of the Last Update:  2018-11-17
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qf_port_h
#define qf_port_h

/* QUTest event queue and thread types */
#define QF_EQUEUE_TYPE QEQueue
/*#define QF_OS_OBJECT_TYPE */
/*#define QF_THREAD_TYPE */

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE        64

/* The number of system clock tick rates */
#define QF_MAX_TICK_RATE     2

/* QF interrupt disable/enable */
#define QF_INT_DISABLE()     (++QF_intNest)
#define QF_INT_ENABLE()      (--QF_intNest)

/* QUTest critical section */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy) QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)  QF_INT_ENABLE()

/* QF_LOG2 not defined -- use the internal LOG2() implementation */

#include "qep_port.h"  /* QEP port */
#include "qequeue.h"   /* QUTest port uses QEQueue event-queue */
#include "qmpool.h"    /* QUTest port uses QMPool memory-pool */
#include "qf.h"        /* QF platform-independent public interface */

/* interrupt nesting up-down counter */
extern uint8_t volatile QF_intNest;

/****************************************************************************/
/* Microsoft C++: portable "safe" facilities from <stdio.h> and <string.h> */
#ifdef _MSC_VER

#if (_MSC_VER < 1900) /* before Visual Studio 2015 */
#define snprintf _snprintf
#endif

#define SNPRINTF_S(buf_, len_, format_, ...) \
    _snprintf_s(buf_, len_, _TRUNCATE, format_, ##__VA_ARGS__)

#define STRNCPY_S(dest_, src_, len_) \
    strncpy_s(dest_, len_, src_, _TRUNCATE)

#define FOPEN_S(fp_, fName_, mode_) \
    if (fopen_s(&fp_, fName_, mode_) != 0) { \
        fp_ = (FILE *)0; \
    } else (void)0

#define CTIME_S(buf_, len_, time_) \
    ctime_s((char *)buf_, len_, time_)

#define SSCANF_S(buf_, format_, ...) \
    sscanf_s(buf_, format_, ##__VA_ARGS__)

#else /* other C/C++ compilers (GNU, etc.) */

#define SNPRINTF_S(buf_, len_, format_, ...) \
    snprintf(buf_, len_, format_, ##__VA_ARGS__)

#define STRNCPY_S(dest_, src_, len_) strncpy(dest_, src_, len_)

#define FOPEN_S(fp_, fName_, mode_) \
    (fp_ = fopen(fName_, mode_))

#define CTIME_S(buf_, len_, time_) \
    strncpy((char *)buf_, ctime(time_), len_)

#define SSCANF_S(buf_, format_, ...) \
    sscanf(buf_, format_, ##__VA_ARGS__)

#endif /* _MSC_VER */

/****************************************************************************/
/* interface used only inside QF implementation, but not in applications */
#ifdef QP_IMPL

    /* QUTest scheduler locking (not used) */
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) ((void)0)
    #define QF_SCHED_UNLOCK_()    ((void)0)

    /* native event queue operations */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        Q_ASSERT_ID(110, (me_)->eQueue.frontEvt != (QEvt *)0)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QS_rxPriv_.readySet, (uint_fast8_t)(me_)->prio)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_  QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_))

    #define QF_EPOOL_EVENT_SIZE_(p_)  ((p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), e_))

    #include "qf_pkg.h" /* internal QF interface */

#endif /* QP_IMPL */

#endif /* qf_port_h */
