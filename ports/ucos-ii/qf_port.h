/**
* @file
* @brief QF/C generic port to uC/OS-II
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

/* uC/OS-II event queue and thread types */
#define QF_EQUEUE_TYPE       OS_EVENT *
#define QF_THREAD_TYPE       uint32_t

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE ((OS_LOWEST_PRIO - 2 < 64) ? (OS_LOWEST_PRIO - 2) : 64U)

#include "ucos_ii.h"  /* uC/OS-II API, port and compile-time configuration */

/* uC/OS-II crtitical section, NOTE1 */
#if (OS_CRITICAL_METHOD == 1u)
    /* QF_CRIT_STAT_TYPE  not defined */
    #define QF_CRIT_ENTRY(dummy) OS_ENTER_CRITICAL()
    #define QF_CRIT_EXIT(dummy)  OS_EXIT_CRITICAL()
#elif (OS_CRITICAL_METHOD == 3u)
    #define QF_CRIT_STAT_TYPE    OS_CPU_SR
    #define QF_CRIT_ENTRY(dummy) OS_ENTER_CRITICAL()
    #define QF_CRIT_EXIT(dummy)  OS_EXIT_CRITICAL()
#else
    #error Unsupported uC/OS-II critical section type
#endif /* OS_CRITICAL_METHOD */

#include "qep_port.h" /* QEP port */
#include "qequeue.h"  /* native QF event queue for deferring events */
#include "qmpool.h"   /* native QF event pool */
#include "qf.h"       /* QF platform-independent public interface */


/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    /* uC/OS-II crtitical section, NOTE1 */
#if (OS_CRITICAL_METHOD == 3u)
    /* internal uC/OS-II critical section operations, NOTE1 */
    #define QF_CRIT_STAT_       OS_CPU_SR cpu_sr;
    #define QF_CRIT_E_()    OS_ENTER_CRITICAL()
    #define QF_CRIT_X_()     OS_EXIT_CRITICAL()
#endif /* OS_CRITICAL_METHOD */

    /* uC/OS-II-specific scheduler locking, see NOTE2 */
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) do { \
        if (OSIntNesting == 0) {       \
            OSSchedLock();             \
        }                              \
    } while (false)

    #define QF_SCHED_UNLOCK_() do { \
        if (OSIntNesting == 0) {    \
            OSSchedUnlock();        \
        } \
    } while (false)

    /* uC/OS-II event pool operations... */
    #define QF_EPOOL_TYPE_ OS_MEM*
    #define QF_EPOOL_INIT_(pool_, poolSto_, poolSize_, evtSize_) do {       \
        INT8U err;                                                          \
        (pool_) = OSMemCreate((poolSto_), (INT32U)((poolSize_)/(evtSize_)), \
                              (INT32U)(evtSize_), &err);                    \
        Q_ASSERT_ID(105, err == OS_ERR_NONE);                               \
    } while (false)

    #define QF_EPOOL_EVENT_SIZE_(pool_) ((pool_)->OSMemBlkSize)
    #define QF_EPOOL_GET_(pool_, e_, m_, qs_id_) do { \
        QF_CRIT_STAT_                                 \
        QF_CRIT_E_();                                 \
        if ((pool_)->OSMemNFree > (m_)) {             \
            INT8U err;                                \
            (e_) = (QEvt *)OSMemGet((pool_), &err);   \
            Q_ASSERT_ID(205, err == OS_ERR_NONE);     \
        }                                             \
        else {                                        \
            (e_) = (QEvt *)0;                         \
        }                                             \
        QF_CRIT_X_();                                 \
    } while (false)

    #define QF_EPOOL_PUT_(pool_, e_, qs_id_) \
        Q_ALLEGE_ID(305, OSMemPut((pool_), (void *)(e_)) == OS_ERR_NONE)

#endif /* ifdef QP_IMPL */

/*****************************************************************************
* NOTE1:
* This QP port to uC/OS-II re-uses the exact same crtical section mechanism
* as uC/OS-II. The goal is to make this port independent on the CPU or the
* toolchain by employing only the official uC/OS-II API. That way, all CPU
* and toolchain dependencies are handled internally by uC/OS-II.
*
* NOTE2:
* uC/OS-II provides only global scheduler locking for all thread priorities
* by means of OSSchedLock() and OSSchedUnlock(). Therefore, locking the
* scheduler only up to the specified lock priority is not supported.
*/

#endif /* QF_PORT_H */
