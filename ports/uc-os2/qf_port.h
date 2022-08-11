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
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @ingroup ports
* @brief QF/C port to uC-OS2, generic C99 compiler
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* uC-OS2 event queue and thread types */
#define QF_EQUEUE_TYPE       OS_EVENT *
#define QF_THREAD_TYPE       uint32_t

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE ((OS_LOWEST_PRIO - 2 < 64) ? (OS_LOWEST_PRIO - 2) : 64U)

#include "ucos_ii.h"  /* uC-OS2 API, port and compile-time configuration */

/* uC-OS2 crtitical section, NOTE1 */
#if (OS_CRITICAL_METHOD == 1u)
    /* QF_CRIT_STAT_TYPE  not defined */
    #define QF_CRIT_ENTRY(dummy) OS_ENTER_CRITICAL()
    #define QF_CRIT_EXIT(dummy)  OS_EXIT_CRITICAL()
#elif (OS_CRITICAL_METHOD == 3u)
    #define QF_CRIT_STAT_TYPE    OS_CPU_SR
    #define QF_CRIT_ENTRY(dummy) OS_ENTER_CRITICAL()
    #define QF_CRIT_EXIT(dummy)  OS_EXIT_CRITICAL()
#else
    #error Unsupported uC-OS2 critical section type
#endif /* OS_CRITICAL_METHOD */

enum UCOS2_TaskAttrs {
    TASK_NAME_ATTR
};

#include "qep_port.h" /* QEP port */
#include "qequeue.h"  /* native QF event queue for deferring events */
#include "qmpool.h"   /* native QF event pool */
#include "qf.h"       /* QF platform-independent public interface */

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    /* uC-OS2 crtitical section, NOTE1 */
#if (OS_CRITICAL_METHOD == 3u)
    /* internal uC-OS2 critical section operations, NOTE1 */
    #define QF_CRIT_STAT_       OS_CPU_SR cpu_sr;
    #define QF_CRIT_E_()    OS_ENTER_CRITICAL()
    #define QF_CRIT_X_()     OS_EXIT_CRITICAL()
#endif /* OS_CRITICAL_METHOD */

    /* uC-OS2-specific scheduler locking, see NOTE2 */
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

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
    #define QF_EPOOL_PUT_(p_, e_, qs_id_) \
        (QMPool_put(&(p_), (e_), (qs_id_)))

#endif /* ifdef QP_IMPL */

/*****************************************************************************
* NOTE1:
* This QP port to uC-OS2 re-uses the exact same crtical section mechanism
* as uC-OS2. The goal is to make this port independent on the CPU or the
* toolchain by employing only the official uC-OS2 API. That way, all CPU
* and toolchain dependencies are handled internally by uC-OS2.
*
* NOTE2:
* uC-OS2 provides only global scheduler locking for all thread priorities
* by means of OSSchedLock() and OSSchedUnlock(). Therefore, locking the
* scheduler only up to the specified lock priority is not supported.
*/

#endif /* QF_PORT_H */

