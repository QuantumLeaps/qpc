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
* @date Last updated on: 2022-09-30
* @version Last updated for: @ref qpc_7_1_2
*
* @file
* @brief QF/C port to RISC-V, cooperative QV kernel, GNU compiler
*/
#ifndef RISCV_QF_PORT_H_
#define RISCV_QF_PORT_H_

/* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE          32U

/* interrupt disabling policy, see NOTE2 */
#define QF_INT_DISABLE()       __asm__ volatile("csrc mstatus,8")
#define QF_INT_ENABLE()        __asm__ volatile("csrs mstatus,8")

/* QF critical section entry/exit, see NOTE3 */
#define QF_CRIT_STAT_TYPE      unsigned int
#define QF_CRIT_ENTRY(status_) do { \
    __asm__ volatile("csrr %0, mstatus" : "=r"(status_)); \
    QF_INT_DISABLE(); \
} while (false)
#define QF_CRIT_EXIT(status_)  \
    __asm__ volatile ("csrw mstatus, %0" :: "rK"(status_))

/* support for fast LOG2, see NOTE4 */
#define QF_LOG2(n_) ((uint_fast8_t)(32U - __builtin_clz((n_))))

/* NOP instruction for RISC-V, NOTE5 */
#define QF_CRIT_EXIT_NOP()     __asm__ volatile ("nop")

#include "qep_port.h" /* QEP port */
#include "qv_port.h"  /* QV cooperative kernel port */
#include "qf.h"       /* QF platform-independent public interface */

/*==========================================================================*/
/* NOTE1:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 64U, if necessary. Here it is set to a lower level of 32 to
* slightly improve performance and save some RAM.
*
* NOTE2:
* Interrupts are disabled by clearing the MIE (bit-3) in the mstatus register.
* Interrupts are enabled by setting the MIE (bit-3) in the mstatus register.
*
* NOTE3:
* QP critical section uses the "save and restore interrupt status" policy.
* This policy is appropriate for CPUs that disable interrupts inside ISRs,
* which is the case with RISC-V. Also, this policy allows *nesting* of
* critical sections.
*
* NOTE4:
* The __builtin_clz() function is used for the integer log-base-2
* implementation, which is used in the QV scheduler.

* NOTE5:
* The NOP instruction is used to prevent merging of back-to-back critical
* sections.
*/

#endif /* RISCV_QF_PORT_H_ */
