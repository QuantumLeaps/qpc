/**
* @file
* @brief QF/C generic port to TI-RTOS kernel (SYS/BIOS)
* @cond
******************************************************************************
* Last Updated for Version: 5.8.1
* Date of the Last Update:  2016-12-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qf_port_h
#define qf_port_h

/* TI-RTOS-specific event queue and thread types, see NOTE1 */
#define QF_EQUEUE_TYPE       QEQueue
#define QF_OS_OBJECT_TYPE    Swi_Struct
#define QF_THREAD_TYPE       Swi_Handle

/* The maximum number of active objects in the application, see NOTE2 */
#define QF_MAX_ACTIVE        32

/* TI-RTOS-specific critical section operations, NOTE3 */
#define QF_CRIT_STAT_TYPE    UInt
#define QF_CRIT_ENTRY(key_)  ((key_) = Hwi_disable())
#define QF_CRIT_EXIT(key_)   (Hwi_restore((key_)))

#include <ti/sysbios/hal/Hwi.h> /* TI-RTOS Hwi package */
#include <ti/sysbios/knl/Swi.h> /* TI-RTOS Swi package */

#include "qep_port.h" /* QEP port */
#include "qequeue.h"  /* this QF port uses the native QF event queue */
#include "qmpool.h"   /* this QF port uses the native QF memory pool */
#include "qf.h"       /* QF platform-independent public interface */


/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    /* TI-RTOS-specific scheduler locking, see NOTE4 */
    #define QF_SCHED_STAT_        UInt key_;
    #define QF_SCHED_LOCK_(dummy) (key_ = Swi_disable())
    #define QF_SCHED_UNLOCK_()    (Swi_restore(key_))

    /* TI-RTOS-specific native event queue operations... */
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))
    #define QACTIVE_EQUEUE_SIGNAL_(me_) Swi_post((me_)->thread)

    /* native QF event pool operations... */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* ifdef QP_IMPL */

#endif /* qf_port_h */

/*****************************************************************************
*
* NOTE1:
* This QP port uses the TI-RTOS Swis ("Software Interrupts") to provide the
* thread context to run QP active objects. The TI-RTOS Swis are a perfect
* match to execute the run-to-completion (RTC) event-processing in active
* objects and requre far less resources (no private stacks) than regular
* blocking threads.
*
* NOTE2:
* The maximum number of active objects in the application cannot exceed the
* number of Swis available in TI-RTOS. On 32-bit CPUs, such as ARM Cortex-M,
* the maximum number of Swis is 32.
*
* NOTE3:
* The QF critical section is implemented with the TI-RTOS Hwi disable/enable.
* The type of critical section preserves the interrupt status and allows
* critical sections to nest.
*
* NOTE4:
* TI-RTOS provides only global disabling of Swis. Therefore, locking the
* scheduler only up to the specified lock priority is not supported.
*/
