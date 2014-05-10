/*****************************************************************************
* Product: QF/C generic port to uC/OS-II
* Last updated for version 5.3.1
* Last updated on  2014-05-10
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
*****************************************************************************/
#ifndef qf_port_h
#define qf_port_h

/* uC/OS-II event queue and thread types */
#define QF_EQUEUE_TYPE         OS_EVENT *
#define QF_THREAD_TYPE         uint_fast8_t

/* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE              (OS_MAX_TASKS < 64 ? OS_MAX_TASKS : 63)

/* uC/OS-II critical section operations (critical section type 3), NOTE1 */
#define QF_CRIT_STAT_TYPE      OS_CPU_SR
#define QF_CRIT_ENTRY(stat_)   ((stat_) = OSCPUSaveSR())
#define QF_CRIT_EXIT(stat_)    OSCPURestoreSR(stat_)

#include "ucos_ii.h"  /* uC/OS-II include file */

#include "qep_port.h" /* QEP port, includes the master uC/OS-II include */
#include "qequeue.h"  /* native QF event queue for deferring events */
#include "qf.h"       /* QF platform-independent public interface */

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    /* uC/OS-II event pool operations */
    #define QF_EPOOL_TYPE_  OS_MEM *
    #define QF_EPOOL_INIT_(pool_, poolSto_, poolSize_, evtSize_) do { \
        INT8U err; \
        (pool_) = OSMemCreate((poolSto_), (INT32U)((poolSize_)/(evtSize_)), \
                              (INT32U)(evtSize_), &err); \
        Q_ASSERT(err == OS_NO_ERR); \
    } while (0)

    #define QF_EPOOL_EVENT_SIZE_(pool_) ((pool_)->OSMemBlkSize)
    #define QF_EPOOL_GET_(pool_, e_, m_) do { \
        QF_CRIT_STAT_ \
        QF_CRIT_ENTRY_(); \
        if ((pool_)->OSMemNFree > (m_)) { \
            INT8U err; \
            (e_) = (QEvt *)OSMemGet((pool_), &err); \
            Q_ASSERT(err == OS_NO_ERR); \
        } \
        else { \
            (e_) = (QEvt *)0; \
        } \
        QF_CRIT_EXIT_(); \
    } while (0)

    #define QF_EPOOL_PUT_(pool_, e_) \
        Q_ALLEGE(OSMemPut((pool_), (void *)(e_)) == OS_NO_ERR)

#endif /* ifdef QP_IMPL */

#endif /* qf_port_h */

/*****************************************************************************
* NOTE1:
* The uC/OS critical section must be able to nest.
*/
