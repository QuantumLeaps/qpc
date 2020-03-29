/**
* @file
* @brief QF/C port, PIC24/dsPIC, QK kernel, MPLABX-XC16 compiler
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-01-19
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE           8U

#define QF_EVENT_SIZ_SIZE       1U
#define QF_EQUEUE_CTR_SIZE      1U
#define QF_MPOOL_SIZ_SIZE       1U
#define QF_MPOOL_CTR_SIZE       1U
#define QF_TIMEEVT_CTR_SIZE     2U

/* QF interrupt disable/enable, see NOTE02 */
#define QF_INT_DISABLE()        __builtin_disi(0x3FFFU)
#define QF_INT_ENABLE()         __builtin_disi(0x0000U)

/* QF critical section entry/exit, see NOTE02 */
/* QF_CRIT_STAT_TYPE not defined: unconditional interrupt unlocking" policy */
#define QF_CRIT_ENTRY(dummy)    __builtin_disi(0x3FFFU)
#define QF_CRIT_EXIT(dummy)     __builtin_disi(0x0000U)

/* fast log-base-2 with FBCL instruction, NOTE03 */
#define QF_LOG2(n_) ((uint8_t)(15U + __builtin_fbcl(n_)))

#include "qep_port.h"   /* QEP port */
#include "qk_port.h"    /* QK preemptive kernel port */
#include "qf.h"         /* QF platform-independent public interface */


/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased up to
* 64, inclusive. The lower limit of 8 is used here to save RAM and also
* because the internal scheduler algorithms perform slighlty better for this
* lower limit.
*
* NOTE02:
* The DISI (disable interrupts) instruction is used for very fast,
* unconditional locking and unlocking of interrupts. The DISI #0x3FFF
* instruction disables interrupts for 16383 instruction cycles, which is
* much longer than any critical section in QP. The DISI #0 instruction
* is then used to unconditionally unlock the interrupts at the end of the
* critical section. The DISI instruction only disables interrupts with
* priority levels 1-6. Priority level 7 interrupts and all trap events still
* have the ability to interrupt the CPU when the DISI instruction is active.
* This means that from the perspective of QP, the level 7 interrupts are
* treated as non-maskable interrupts (NMIs). Such non-maskable interrupts
* _cannot_ call any QP services. In particular, they cannot post events.
*
* CAUTION: This QP port assumes that interrupt nesting is _enabled_,
* which is the default in the PIC24/dsPIC processors. Interrupt nesting should
* never be disabled by setting the NSTDIS control bit (INTCON1<15>). If you
* don't want interrupts to nest, you can always prioritize them at the same
* level. For example, the default priority level for all interrupts is 4 out
* of reset. If you don't change this level for any interrupt the nesting of
* interrupt will not occur.
*
* NOTE03:
* The FBCL instruction (Find First Bit Change Left) determines the exponent
* of a value by detecting the first bit change starting from the value’s sign
* bit and working towards the LSB. Since the PIC24/dsPIC’s barrel shifter
* uses negative values to specify a left shift, the FBCL instruction returns
* the negated exponent of a value. This value added to 15 gives the log-2.
*/

#endif /* QF_PORT_H */

