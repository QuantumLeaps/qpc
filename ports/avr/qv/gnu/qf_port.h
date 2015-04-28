/**
* @file
* @brief QF/C port to AVRmega, cooperative QV kernel, GNU-AVR toolset
* @ingroup ports
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

/* various QF object sizes configuration for this port... */
#define QF_MAX_ACTIVE           8
#define QF_MAX_EPOOL            3
#define QF_EVENT_SIZ_SIZE       1
#define QF_EQUEUE_CTR_SIZE      1
#define QF_MPOOL_SIZ_SIZE       1
#define QF_MPOOL_CTR_SIZE       1
#define QF_TIMEEVT_CTR_SIZE     2

/* QF interrupt disable/enable... */
#define QF_INT_DISABLE()        __asm__ __volatile__ ("cli" ::)
#define QF_INT_ENABLE()         __asm__ __volatile__ ("sei" ::)

/* QF critical section entry/exit... */
#define QF_CRIT_STAT_TYPE       uint8_t
#define QF_CRIT_ENTRY(stat_)    do { \
    (stat_) = SREG; \
    QF_INT_DISABLE(); \
} while (0)
#define QF_CRIT_EXIT(stat_)     (SREG = (stat_))

/* QF reset... */
#define QF_RESET()              __asm__ __volatile__ ("jmp 0x0000" ::)


#include <avr/io.h>   /* for SREG/SMCR definitions */

#include "qep_port.h" /* QEP port */
#include "qv_port.h"  /* QV coopeartive kernel port */
#include "qf.h"       /* QF platform-independent public interface */

#endif /* qf_port_h */
