/**
* @file
* @brief QF/C port to MSP430, preemptive QK kernel, IAR-430 compiler
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
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

/* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE               8

#define QF_EVENT_SIZ_SIZE           1
#define QF_EQUEUE_CTR_SIZE          1
#define QF_MPOOL_SIZ_SIZE           1
#define QF_MPOOL_CTR_SIZE           1
#define QF_TIMEEVT_CTR_SIZE         2

/* QF interrupt disable/enable... */
#define QF_INT_DISABLE()            __disable_interrupt()
#define QF_INT_ENABLE()             __enable_interrupt()

/* QF critical section entry/exit... */
#define QF_CRIT_STAT_TYPE           istate_t
#define QF_CRIT_ENTRY(stat_) do { \
    (stat_) =  __get_interrupt_state(); \
    __disable_interrupt(); \
} while (0)
#define QF_CRIT_EXIT(stat_)         __set_interrupt_state(stat_)


#include <intrinsics.h> /* IAR intrinsic functions */

#include "qep_port.h"   /* QEP port */
#include "qk_port.h"    /* QK preemptive kernel port */
#include "qf.h"         /* QF platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*/

#endif  /* qf_port_h */
