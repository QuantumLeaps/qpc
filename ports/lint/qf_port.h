/**
* \file
* \brief QF/C port example for a "generic" C compiler.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-03-01
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
******************************************************************************
* \endcond
*/
#ifndef qf_port_h
#define qf_port_h

/*! The maximum number of active objects in the application. */
/**
* \description
* This macro *must* be defined in the QF port and should be in range
* of 1..63, inclusive. The value of this macro determines the maximum
* priority level of an active object in the system. Not all priority
* levels must be used, but the maximum priority cannot exceed
* #QF_MAX_ACTIVE.
*
* \note Once you choose a certain value of #QF_MAX_ACTIVE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_ACTIVE               63

/*! The maximum number of event pools in the application. */
/**
* \description
* This macro should be defined in the QF ports and should be in range
* of 1..255, inclusive. The value of this macro determines the maximum
* event pools in the system. Not all event pools must be actually used,
* but the maximum number of pools cannot exceed #QF_MAX_EPOOL.
*
* If the macro is not defined, the default value is 3. Defining the value
* below the maximum saves some memory, mostly for the subscriber-lists.
* \sa ::QSubscrList.
*
* \note Once you choose a certain value of #QF_MAX_EPOOL, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_EPOOL               3

/*! The size (in bytes) of the event-size representation in the QF.
* Valid values: 1, 2, or 4; default 2
*/
/**
* \description
* This macro can be defined in the QF ports to configure the ::QEvtSize
* type. If the macro is not defined, the default of 2 byte will be chosen in
* qf.h. The valid #QF_EVENT_SIZ_SIZE values of 1, 2, or 4, correspond
* to ::QEvtSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QEvtSize data type determines the dynamic range of event-sizes in
* your application.
* \sa QF_poolInit(), QF_new_()
*
* \note Once you choose a certain value of #QF_EVENT_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EVENT_SIZ_SIZE           2

/*! The size (in bytes) of the ring-buffer counters used in the native QF
* event queue implementation. Valid values: 1, 2, or 4; default 1
*/
/**
* \description
* This macro can be defined in the QF ports to configure the ::QEQueueCtr
* type. If the macro is not defined, the default of 1 byte will be chosen in
* qequeue.h. The valid #QF_EQUEUE_CTR_SIZE values of 1, 2, or 4, correspond
* to ::QEQueueCtr of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QEQueueCtr data type determines the dynamic range of numerical values of
* ring-buffer counters inside event queues, or, in other words, the maximum
* number of events that the native QF event queue can manage.
* \sa ::QEQueue
*
* \note Once you choose a certain value of #QF_EQUEUE_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EQUEUE_CTR_SIZE          1

/*! The size (in bytes) of the block-size representation in the native QF
* event pool. Valid values: 1, 2, or 4; default #QF_EVENT_SIZ_SIZE.
*/
/**
* \description
* This macro can be defined in the QF ports to configure the ::QMPoolSize
* type. If the macro is not defined, the default of #QF_EVENT_SIZ_SIZE
* will be chosen in qmpool.h, because the memory pool is primarily used for
* implementing event pools.
*
* The valid #QF_MPOOL_SIZ_SIZE values of 1, 2, or 4, correspond to
* ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolSize data type determines the dynamic range of block-sizes that
* the native ::QMPool can handle.
* \sa #QF_EVENT_SIZ_SIZE, ::QMPool
*
* \note Once you choose a certain value of #QF_MPOOL_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_SIZ_SIZE           2

/*! The size (in bytes) of the block-counter representation in the
* native QF event pool. Valid values: 1, 2, or 4; default 2.
*/
/**
* \description
* This macro can be defined in the QF ports to configure the ::QMPoolCtr
* type. If the macro is not defined, the default of 2 bytes will be chosen
* in qmpool.h. The valid #QF_MPOOL_CTR_SIZE values of 1, 2, or 4, correspond
* to ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolCtr data type determines the dynamic range of block-counters that
* the native ::QMPool can handle, or, in other words, the maximum number
* of blocks that the native QF event pool can manage.
* \sa ::QMPool
*
* \note Once you choose a certain value of #QF_MPOOL_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_CTR_SIZE           2

/*! The size (in bytes) of the time event-counter representation
* in the ::QTimeEvt struct. Valid values: 1, 2, or 4; default 2.
*/
/**
* \description
* This macro can be defined in the QF ports to configure the internal tick
* counters of Time Events. If the macro is not defined, the default of 2
* bytes will be chosen in qf.h. The valid #QF_TIMEEVT_CTR_SIZE values of 1,
* 2, or 4, correspond to tick counters of uint8_t, uint16_t, and uint32_t,
* respectively. The tick counter representation determines the dynamic range
* of time delays that a Time Event can handle.
* \sa ::QTimeEvt
*
* \note Once you choose a certain value of #QF_TIMEEVT_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_TIMEEVT_CTR_SIZE         2

/*! Define the interrupt disabling policy.
*
* This macro encapsulates platform-specific way of disabling interrupts
* from "C" for a given CPU and compiler.
*
* \note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_DISABLE()            intDisable()

/*! Define the interrupt enabling policy. */
/**
* \description
* This macro encapsulates platform-specific way of enabling interrupts
* from "C" for a given CPU and compiler.
*
* \note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_ENABLE()             intEnable()

void intDisable(void);
void intEnable(void);

/*! Define the type of the critical section status. */
/**
* \description
* Defining this macro configures the "saving and restoring critical section
* status" policy. Coversely, if this macro is not defined, the simple
* "unconditional critical section exit" is used.
*/
#define QF_CRIT_STAT_TYPE           crit_stat_t

/*! Define the critical section entry policy. */
/**
* \description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro sets the \a status_ argument to the critical
* section status just before the entry. When the policy of "unconditional
* critical section exit" is used, the macro does not use the \a status_
* argument.
*
* \note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_ENTRY(stat_)        ((stat_) = critEntry())

/*! Define the critical section exit policy. */
/**
* \description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro restores the critical section status from the
* \a status_ argument. When the policy of "unconditional critical section
* exit" is used, the macro does not use the \a status argument and
* exits the critical section unconditionally (often by means of enabling
* interrupts).
*
* \note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_EXIT(stat_)         critExit(stat_)

/*! Macro to put the CPU to sleep safely in the cooperative
* Vanilla kernel (inside QF_idle()).
*/
/**
* \description
* This macro is provided in some QP ports for the Vanilla kernel and
* in general it depends on the interrupt disabling policy.
*
* \note The provided code is just an example (for ARM Cortex-M).
*/
#define QF_CPU_SLEEP() do { \
    __disable_interrupt(); \
    QF_INT_ENABLE(); \
    __WFI(); \
    __enable_interrupt(); \
} while (0)

typedef unsigned int crit_stat_t;
QF_CRIT_STAT_TYPE critEntry(void);
void critExit(QF_CRIT_STAT_TYPE stat);
void QF_onIdle(void);

#include "qep_port.h"  /* QEP port */
#include "qk_port.h"   /* QK port */
#include "qf.h"        /* QF platform-independent public interface */

#endif /* qf_port_h */

