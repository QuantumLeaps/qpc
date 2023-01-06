/*============================================================================
* QP/C Real-Time Embedded Framework
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
* @date Last updated on: 2022-11-23
* @version Last updated for: @ref qpc_7_2_0
*
* @file
* @brief QF/C sample port with all configurable options
*/
#ifndef QF_PORT_H_
#define QF_PORT_H_

/*! This macro defines the type of the thread handle used for AOs */
#define QF_THREAD_TYPE         void*

/*! This macro defines the type of the event-queue used for AOs */
#define QF_EQUEUE_TYPE         QEQueue

/*! This macro defines the type of the OS-Object used for blocking
* the native ::QEQueue when the queue is empty
*
* @description
* This macro is used when ::QEQueue is used as the event-queue for AOs
* but also the AO queue must *block* when the queue is empty.
* In that case, #QF_OS_OBJECT_TYPE specifies the blocking mechanism.
* For example, in the POSIX port, the blocking mechanism is a condition
* variable.
*/
#define QF_OS_OBJECT_TYPE      pthread_cond_t

/*! This macro defines the type of the event pool used in the port.
*
* @description
* This is a specific implementation for the QK-port of QF.
* In other QF ports you need to define the macro appropriately for
* the underlying kernel/OS you're using.
*/
#define QF_EPOOL_TYPE_         QMPool

/*! The maximum number of active objects in the application.
*
* @description
* This macro *must* be defined in the `qf_port.h` header file and should be
* in range of 1U..64U, inclusive. The value of this macro determines the
* maximum priority level of an active object in the system. Not all priority
* levels must be used, but the maximum priority cannot exceed `QF_MAX_ACTIVE`.
* Defining the value below the maximum limit of 64 saves some memory (RAM).
*/
#define QF_MAX_ACTIVE          64U

/*! The maximum number of clock tick rates in the application.
*
* @description
* This macro can be defined in the `qf_port.h` header file and should be
* in range of 1U..15U, inclusive. The value of this macro determines the
* maximum number of clock tick rates for time events (::QTimeEvt).
*
* If the macro is not defined, the `qf.h` header file sets the default
* value to 1U.
*/
#define QF_MAX_TICK_RATE       1U

/*! The size (in bytes) of the time event-counter representation
* in the ::QTimeEvt struct. Valid values: 1U, 2U, or 4U; default 2U.
*
* @description
* This macro can be defined in the `qf_port.h` header file to configure the
* internal tick counters of Time Events. If the macro is not defined, the
* default of 4 bytes will be chosen in `qf.h`. The valid #QF_TIMEEVT_CTR_SIZE
* values of 1, 2, or 4, correspond to tick counters of uint8_t, uint16_t,
* and uint32_t, respectively. The tick counter representation determines the
* dynamic range of time delays that a Time Event can handle.
* @sa ::QTimeEvt
*/
#define QF_TIMEEVT_CTR_SIZE    4U

/*! The maximum number of event pools in the application.
*
* @description
* This macro can be defined in the `qf_port.h` header file and should be
* in range of 1U..255U, inclusive. The value of this macro determines the
* maximum number of event pools in the system. Not all event pools must be
* actually used, but the maximum number of pools cannot exceed `QF_MAX_EPOOL`.
*
* If the macro is not defined, the `qf.h` header file sets the default
* value to 3U. Defining `QF_MAX_EPOOL` below the maximum limit of 255
* saves some memory (RAM).
*/
#define QF_MAX_EPOOL           3U

/*! The size (in bytes) of the event-size representation in the QF.
* Valid values: 1U, 2U, or 4U; default 2U
*
* @description
* This macro can be defined in the QF ports to configure the size
* of the event-size.
*/
#define QF_EVENT_SIZ_SIZE      2U

/*! The size (in bytes) of the ring-buffer counters used in the native QF
* event queue implementation. Valid values: 1U, 2U, or 4U; default 1U
*
* @description
* This macro can be defined in the `qf_port.h` header file to configure
* the ::QEQueueCtr type. If the macro is not defined, the default of 1 byte
* will be chosen in `qequeue.h`. The valid #QF_EQUEUE_CTR_SIZE values of
* 1U, 2U, or 4U, correspond to ::QEQueueCtr of uint8_t, uint16_t, and
* uint32_t, respectively. The::QEQueueCtr data type determines the dynamic
* range of numerical values of ring-buffer counters inside event queues, or,
* in other words, the maximum number of events that the native QF event
* queue can manage.
* @sa ::QEQueue
*/
#define QF_EQUEUE_CTR_SIZE     1U

/*! The size (in bytes) of the block-size representation in the native QF
* event pool. Valid values: 1U, 2U, or 4U; default #QF_EVENT_SIZ_SIZE.
*
* @description
* This macro can be defined in the `qf_port.h` header file to configure the
* ::QMPoolSize type. If the macro is not defined, the default of
* #QF_EVENT_SIZ_SIZE will be chosen in `qmpool.h`, because the memory pool
* is primarily used for implementing event pools.
*
* The valid #QF_MPOOL_SIZ_SIZE values of 1U, 2U, or 4U, correspond to
* ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolSize data type determines the dynamic range of block-sizes that
* the native ::QMPool can handle.
* @sa #QF_EVENT_SIZ_SIZE, ::QMPool
*/
#define QF_MPOOL_SIZ_SIZE      2U

/*! The size (in bytes) of the block-counter representation in the
* native QF event pool. Valid values: 1U, 2U, or 4U; default 2U.
*
* @description
* This macro can be defined in the `qf_port.h` header file to configure the
* ::QMPoolCtr type. If the macro is not defined, the default of 2 bytes will
* be chosen in `qmpool.h`. The valid #QF_MPOOL_CTR_SIZE values of 1U, 2U,
* or 4U, correspond to ::QMPoolSize of uint8_t, uint16_t, and uint32_t,
* respectively. The ::QMPoolCtr data type determines the dynamic range of
* block-counters that the native ::QMPool can handle, or, in other words,
* the maximum number of blocks that the native QF event pool can manage.
* @sa ::QMPool
*/
#define QF_MPOOL_CTR_SIZE      2U

/*! Define the interrupt disabling policy.
*
* @description
* This macro encapsulates platform-specific way of disabling interrupts
* from "C" for a given CPU and compiler.
*
* @note
* the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_DISABLE()       intDisable()

/*! Define the interrupt enabling policy.
*
* @description
* This macro encapsulates platform-specific way of enabling interrupts
* from "C" for a given CPU and compiler.
*
* @note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_ENABLE()        intEnable()

/*! Define the type of the critical section status.
*
* @description
* Defining this macro configures the "saving and restoring critical section
* status" policy. Conversely, if this macro is not defined, the simple
* "unconditional critical section exit" is used.
*/
#define QF_CRIT_STAT_TYPE      crit_stat_t

/*! Define the critical section entry policy.
*
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro sets the @a status_ argument to the critical
* section status just before the entry. When the policy of "unconditional
* critical section exit" is used, the macro does not use the @a status_
* argument.
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_ENTRY(stat_)   ((stat_) = critEntry())

/*! Define the critical section exit policy.
*
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro restores the critical section status from the
* @a status_ argument. When the policy of "unconditional critical section
* exit" is used, the macro does not use the @a status argument and
* exits the critical section unconditionally (often by means of enabling
* interrupts).
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_EXIT(stat_)    critExit(stat_)

/*! Define No-Operation (NOP) implementation to prevent joining
* adjacent critical sections.
*/
#define QF_CRIT_EXIT_NOP()     __asm volatile ("isb")

/*! Define integer log-base-2 implementation strategy
*
* @description
* This macro can be defined in the `qf_port.h` header file to speed up
* the integer log-base-2 computation, which is used internally in QP to
* quickly find the highest-priority AO to execute or to find the
* highest-priority subscriber AO to a given event signal. The QF_LOG2()
* macro allows the port to specify a fast, hardware-supported computation
* (e.g., based on the count-leading-zeros instruction available in some CPUs).
*
* If this macros is NOT defined a default, reasonably fast algorithm is used.
*/
#define QF_LOG2(n_) ((uint_fast8_t)(32U - __builtin_clz((unsigned)(n_))))

#include "qk_port.h"  /* underlying real-time kernel port */
#include "qf.h"       /* QF platform-independent public interface */

#endif /* QF_PORT_H */
