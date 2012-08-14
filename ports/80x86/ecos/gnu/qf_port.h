/*****************************************************************************
* Product:  QF/C port to 80x86, eCos, gnu
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#ifndef qf_port_h
#define qf_port_h

/**
*  event queue and thread types
*/
#define QF_EQUEUE_TYPE          QF_cyg_mbox
#define QF_THREAD_TYPE          QF_cyg_thread

/**
* various QF object sizes configuration for this port
*/
#define QF_MAX_ACTIVE           31
#define QF_EVENT_SIZ_SIZE       2
#define QF_EQUEUE_CTR_SIZE      1
#define QF_MPOOL_SIZ_SIZE       2
#define QF_MPOOL_CTR_SIZE       2
#define QF_TIMEEVT_CTR_SIZE     4


/**
*  eCos critical section operations
*/
#define QF_CRIT_STAT_TYPE       CYG_INTERRUPT_STATE
#define QF_CRIT_ENTRY(stat_)    HAL_DISABLE_INTERRUPTS(stat_)
#define QF_CRIT_EXIT(stat_)     HAL_RESTORE_INTERRUPTS(stat_)


#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/kernel/kapi.h>

typedef struct QF_cyg_mbox_Tag {          /* event queue structure for eCos */
    cyg_handle_t handle;                          /* handle to eCos mailbox */
    cyg_mbox     mbox;                            /* eCos mailbox structure */
} QF_cyg_mbox;

typedef struct QF_cyg_thread_Tag {             /* thread structure for eCos */
    cyg_handle_t handle;                       /* handle to the eCos thread */
    cyg_thread   thread;                           /* eCos thread structure */
} QF_cyg_thread;


#include "qep_port.h"     /* QEP port, includes the master uC/OS-II include */
#include "qequeue.h"          /* native QF event queue for deferring events */
#include "qf.h"                 /* QF platform-independent public interface */

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
typedef struct QF_cyg_mempool_fix_Tag {              /* event pool for eCos */
    cyg_handle_t    handle;                   /* handle to eCos memory pool */
    cyg_mempool_fix fix;                    /* eCos fixed mempool structure */
    QEvtSize      evtSize;                 /* the event size of this pool */
} QF_cyg_mempool_fix;

#define QF_EPOOL_TYPE_          QF_cyg_mempool_fix

#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) do { \
    Q_ASSERT((poolSto_) != (void *)0); \
    (p_).evtSize = (evtSize_); \
    cyg_mempool_fix_create((poolSto_),  (poolSize_), (evtSize_), \
                           &(p_).handle, &&(p_).fix); \
    Q_ENSURE((p_).handle != (cyg_handle_t)0); \
} while (0)

#define QF_EPOOL_EVENT_SIZE_(p_) ((p_).evtSize)

#define QF_EPOOL_GET_(p_, e_) do { \
    (e_) = (QEvt *)cyg_mempool_fix_try_alloc((p_).handle); \
    Q_ENSURE((e_) != (QEvt *)0); \
} while (0)

#define QF_EPOOL_PUT_(p_, e_) do { \
    Q_REQUIRE((e_) != (QEvt *)0); \
    cyg_mempool_fix_free((p_).handle, (void *)(e_)); \
} while (0)


#endif                                                         /* qf_port_h */
