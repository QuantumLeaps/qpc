/*****************************************************************************
* Product:  QF/C, generic port to FreeRTOS.org
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
                               /* FreeRTOS.org event queue and thread types */
#define QF_EQUEUE_TYPE              xQueueHandle
#define QF_THREAD_TYPE              xTaskHandle

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE               63

                                /* FreeRTOS.org critical section operations */
/* #define QF_CRIT_STAT_TYPE */
#define QF_CRTI_ENTRY(dummy)        portENTER_CRITICAL()
#define QF_CRIT_EXIT(dummy)         portEXIT_CRITICAL()


#include "FreeRTOS.h"                   /* FreeRTOS.org master include file */
#include "task.h"                          /* FreeRTOS.org task  management */
#include "queue.h"                         /* FreeRTOS.org queue management */

#include "qep_port.h"                                           /* QEP port */
#include "qmpool.h"                  /* FreeRTOS uses native QF memory-pool */
#include "qequeue.h"          /* native QF event queue for deferring events */
#include "qf.h"                 /* QF platform-independent public interface */

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
                                         /* native QF event pool operations */
#define QF_EPOOL_TYPE_              QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    QMPool_init(&(p_), poolSto_, poolSize_, evtSize_)
#define QF_EPOOL_EVENT_SIZE_(p_)    ((p_).blockSize)
#define QF_EPOOL_GET_(p_, e_)       ((e_) = (QEvt *)QMPool_get(&(p_)))
#define QF_EPOOL_PUT_(p_, e_)       (QMPool_put(&(p_), e_))

#endif                                                         /* qf_port_h */
