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
* @date Last updated on: 2022-07-30
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QF/C port to MSP430, preemptive QK kernel
* @ingroup ports
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE        8U

#define QF_EVENT_SIZ_SIZE    1U
#define QF_EQUEUE_CTR_SIZE   1U
#define QF_MPOOL_SIZ_SIZE    1U
#define QF_MPOOL_CTR_SIZE    1U
#define QF_TIMEEVT_CTR_SIZE  2U

/* QF interrupt disable/enable... */
#define QF_INT_DISABLE()     __disable_interrupt()
#define QF_INT_ENABLE()      __enable_interrupt()

/* QF critical section entry/exit... */
#define QF_CRIT_STAT_TYPE    unsigned short
#define QF_CRIT_ENTRY(stat_) do { \
    (stat_) =  __get_interrupt_state(); \
    __disable_interrupt(); \
} while (false)
#define QF_CRIT_EXIT(stat_)  __set_interrupt_state(stat_)


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h> /* intrinsic functions */
#elif defined(__GNUC__)
    #include <msp430.h>
    #include "in430.h"
#endif

#include "qep_port.h"   /* QEP port */
#include "qk_port.h"    /* QK preemptive kernel port */
#include "qf.h"         /* QF platform-independent public interface */

/*==========================================================================*/
/* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 64, if necessary. Here it is set to a lower level to save some RAM.
*/

#endif /* QF_PORT_H */
