/*****************************************************************************
* Product: QF/C port to Qt-1T (single thread)
* Last Updated for Version: 4.5.00
* Date of the Last Update:  Jun 11, 2012
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

                        /* undefine the conflicitng macro between Qt and QP */
#undef Q_ASSERT

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE               63
                    /* The maximum number of event pools in the application */
#define QF_MAX_EPOOL                8
                     /* various QF object sizes configuration for this port */
#define QF_EVENT_SIZ_SIZE           4
#define QF_EQUEUE_CTR_SIZE          4
#define QF_MPOOL_SIZ_SIZE           4
#define QF_MPOOL_CTR_SIZE           4
#define QF_TIMEEVT_CTR_SIZE         4

                                      /* Qt-1T critical section, see NOTE01 */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy)        ((void)0)
#define QF_CRIT_EXIT(dummy)         ((void)0)

/* native event pool operations */
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    QMPool_init(&(p_), (poolSto_), (poolSize_), (QMPoolSize)(evtSize_))
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).blockSize)
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvt *)QMPool_get(&(p_)))
#define QF_EPOOL_PUT_(p_, e_)       (QMPool_put(&(p_), (e_)))

#include "qep_port.h"                                           /* QEP port */
#include "qequeue.h"                      /* event-queue for event deferral */
#include "qmpool.h"                             /* Qt port uses memory-pool */
#include "qf.h"                 /* QF platform-independent public interface */

void QF_setTickRate(uint32_t ticksPerSec);           /* set clock tick rate */

#ifdef Q_SPY
void QS_onEvent(void);
#endif

/*****************************************************************************
* NOTE01:
* The Qt-1T port has no need for a critical section, because everything
* runs in the event loop of a single thread of the Qt application.
*/

#endif                                                         /* qf_port_h */
