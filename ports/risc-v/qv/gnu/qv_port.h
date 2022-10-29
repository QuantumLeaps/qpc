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
* @date Last updated on: 2022-09-29
* @version Last updated for: @ref qpc_7_1_2
*
* @file
* @brief QV/C port to RISC-V, GNU compiler
*/
#ifndef RISCV_QV_PORT_H_
#define RISCV_QV_PORT_H_

/*! macro to put the CPU to sleep inside QV_onIdle() */
/* The following QV_CPU_SLEEP() executes the WFI instruction with
* interrupts still DISABLED. Interrupts are enabled only after
* the RISC-V CPU wakes up. See NOTE6.
*/
#define QV_CPU_SLEEP() do { \
    __asm volatile("wfi");  \
    QF_INT_ENABLE();        \
} while (false)

#include "qv.h" /* QV platform-independent public interface */

/*==========================================================================*/
/* NOTE6:
* The QV_CPU_SLEEP() macro is entered with interrupts still **disabled**.
* But, according to the "The RISC-V Instruction Set Manual",
* the `WFI` instruction *can* be executed when interrupts are disabled.
*/
#endif /* RISCV_QV_PORT_H_ */
