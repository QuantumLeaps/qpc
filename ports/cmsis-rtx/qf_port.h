/**
* @file
* @brief QF/C, Cortex-M, CMIS-RTOS RTX port, all supported compilers
* @ingroup ports
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-05-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
******************************************************************************
* @endcond
*/
#ifndef qf_port_h
#define qf_port_h

/* RTX message mailbox and thread types */
#define QF_EQUEUE_TYPE      QEQueue
#define QF_THREAD_TYPE      osThreadId
#define QF_OS_OBJECT_TYPE   uint32_t

/* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE       32

/* QF critical section entry/exit... */
/* QF_CRIT_STAT_TYPE not defined: unconditional interrupt unlocking" policy */
#define QF_CRIT_ENTRY(dummy) QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)  QF_INT_ENABLE()

#if defined (__CC_ARM)    /* ARM-KEIL? */

    #define QF_INT_DISABLE()   __disable_irq()
    #define QF_INT_ENABLE()    __enable_irq()
    #define QF_CRIT_EXIT_NOP() __nop()

    #if (__TARGET_ARCH_THUMB > 3) /* Not Cortex-M0/M0+/M1(v6-M, v6S-M)?*/
        /* Cortex-M3/M4/M4F provide the CLZ instruction for fast LOG2 */
        #define QF_LOG2(n_) ((uint8_t)(32U - __clz(n_)))
    #endif /* Not Cortex-M0/M0+/M1? */

#elif defined (__GNUC__)  /* GCC-ARM? */

    #define QF_INT_DISABLE()   __asm volatile ("cpsid i")
    #define QF_INT_ENABLE()    __asm volatile ("cpsie i")
    #define QF_CRIT_EXIT_NOP() __asm volatile ("nop")

    #ifndef ARM_ARCH_V6M /* Not Cortex-M0/M0+/M1(v6-M, v6S-M)?*/
        /* Cortex-M3/M4/M4F provide the CLZ instruction for fast LOG2 */
        #define QF_LOG2(n_) ((uint8_t)(32U - __builtin_clz(n_)))
    #endif /* Not Cortex-M0/M0+/M1? */

#elif defined (__ICCARM__) /* IAR ARM? */

    /* QF interrupt disable/enable and log2()... */
    #define QF_INT_DISABLE()   __disable_interrupt()
    #define QF_INT_ENABLE()    __enable_interrupt()
    #define QF_CRIT_EXIT_NOP() __no_operation()

    /* log2()... */
    #if !(__CORE__ == __ARM6M__)  /* Not Cortex-M0/M0+/M1? */
        /* Cortex-M3/M4/M4F/M7 provide the CLZ instruction for fast LOG2 */
        #define QF_LOG2(n_) ((uint8_t)(32U - __CLZ(n_)))
    #endif /* Not Cortex-M0/M0+/M1? */

    #include <intrinsics.h>  /* IAR intrinsic functions */

#else

    #error "Compiler not supported"

#endif /* ARM compiler type */

#include "cmsis_os.h"    /* CMSIS-RTOS API */
#include "qep_port.h"    /* QEP port */
#include "qequeue.h"     /* used for event deferral */
#include "qmpool.h"      /* this QP port uses the native QF memory pool */
#include "qf.h"          /* QF platform-independent public interface */

/* Facilities specific to the RTX port... */

/* set RTX thread priority;
* can be called either before or after QACTIVE_START().
*/
void QF_setRtxPrio(QActive *act, osPriority prio);

/* set the ticker thread period [millisec] and RTX priority (0=default) */
void QF_setRtxTicker(uint32_t period, osPriority prio);

/* callback to the application to process one tick in the ticker thread */
void QF_onRtxTicker(void);


/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        while ((me_)->eQueue.frontEvt == (QEvt *)0) { \
            QF_CRIT_EXIT_(); \
            Q_ALLEGE_ID(105, osSignalWait(0x01U, osWaitForever).status \
                             == osEventSignal); \
            QF_CRIT_ENTRY_(); \
        }

    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QF_CRIT_EXIT_(); \
        (void)osSignalSet((me_)->thread, 0x01U); \
        QF_CRIT_ENTRY_()

    #define QACTIVE_EQUEUE_ONEMPTY_(me_) ((void)0)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* ifdef QP_IMPL */

/*****************************************************************************
* NOTE1:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*/

#endif /* qf_port_h */
