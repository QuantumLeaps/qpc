/*============================================================================
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
* @date Last updated on: 2022-04-09
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QEP/C port, ARM-Clang/LLVM compiler
*/
#ifndef QEP_PORT_H
#define QEP_PORT_H

/*! no-return function specifier (ARM-Clang/LLVM compiler) */
#define Q_NORETURN   __attribute__ ((noreturn)) void

#include <stdint.h>  /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h> /* Boolean type.      WG14/N843 C99 Standard */

#include "qep.h"     /* QEP platform-independent public interface */

#endif /* QEP_PORT_H */

