/*****************************************************************************
* Product:  QF/C, port to ThreadX
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

                                    /* ThreadX event queue and thread types */
#define QF_EQUEUE_TYPE      TX_QUEUE
#define QF_THREAD_TYPE      TX_THREAD

     /* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE       8

 /* QF priority offset within ThreadX priority numbering scheme, see NOTE02 */
#define QF_TX_PRIO_OFFSET   8

                                         /* QF critical section for ThreadX */
#define QF_CRIT_STAT_TYPE     UINT
#define QF_CRIT_ENTRY(stat_)  ((stat_) = tx_interrupt_control(TX_INT_DISABLE))
#define QF_CRIT_EXIT(stat_)   ((void)tx_interrupt_control(stat_))

#include "qep_port.h"                                           /* QEP port */
#include "qequeue.h"                             /* used for event deferral */
#include "qf.h"                 /* QF platform-independent public interface */


/*****************************************************************************
* interface used only inside QF, but not in applications
*/

typedef struct ThreadXBlockPoolTag {    /* TreadX block pool and block-size */
    TX_BLOCK_POOL pool;                               /* ThreadX block pool */
    QEvtSize block_size;                    /* the block size of the pool */
} ThreadXBlockPool;

                                            /* TreadX block pool operations */
#define QF_EPOOL_TYPE_              ThreadXBlockPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) do { \
    (p_).block_size = (evtSize_); \
    Q_ALLEGE(tx_block_pool_create(&(p_).pool, "P", (evtSize_), \
                 (poolSto_), (poolSize_)) == TX_SUCCESS); \
} while (0)

#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).block_size)

#define QF_EPOOL_GET_(p_, e_)       \
    if (tx_block_allocate(&(p_).pool, (void **)&(e_), TX_NO_WAIT) \
        != TX_SUCCESS) \
    { \
        (e_) = (QEvt *)0; \
    } else ((void)0)

#define QF_EPOOL_PUT_(p_, e_)       \
    Q_ALLEGE(tx_block_release((e_)) == TX_SUCCESS)


/*****************************************************************************
* NOTE01:
* The number of active objects is related to the number of TreadX thread
* priorities TX_MAX_PRIORITIES, because each active object requires a unique
* priority level. The maximum number of active objects in QF can be increased
* to 63, inclusive.
*
* NOTE02:
* The ThreadX priority for an active object thread is calculated as follows:
*     tx_prio = QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - qf_prio
*/

#endif                                                         /* qf_port_h */
