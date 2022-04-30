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
* @date Last updated on: 2022-07-29
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QV/C port example for a generic C compiler.
*/
#ifndef QV_PORT_H
#define QV_PORT_H

/*! Macro to put the CPU to sleep safely in the cooperative
* QV kernel (inside QV_onIdle()).
*/
/**
* @description
* This macro is provided in some QP ports for the QV kernel and
* in general it depends on the interrupt disabling policy.
*
* @note The provided code is just an example (for ARM Cortex-M).
*/
#define QV_CPU_SLEEP() do { \
    __disable_interrupt(); \
    QF_INT_ENABLE(); \
    __WFI(); \
    __enable_interrupt(); \
} while (false)

#include "qv.h" /* QV platform-independent public interface */

#endif /* QV_PORT_H */

