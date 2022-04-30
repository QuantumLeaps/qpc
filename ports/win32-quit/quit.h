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
* @date Last updated on: 2022-11-14
* @version Last updated for: @ref qpc_7_1_3
*
* @file
* @brief "QUIT" QP Unit Internal Test
*/
#ifndef QUIT_H
#define QUIT_H

/* macro to verify an expectation */
#define VERIFY(cond_) \
    ((cond_) ? (void)0 : QUIT_fail_(#cond_, &Q_this_module_[0], __LINE__))

#ifdef __cplusplus
extern "C" {
#endif

void TEST(char const *title);
void QUIT_fail_(char const *cond, char const *module, int line);
void onRunTests(void); /* user-defined callback to run the tests */

#include "qf_port.h"   /* QF/C port from the port directory */
#include "qassert.h"   /* QP embedded systems-friendly assertions */

#ifdef Q_SPY /* software tracing enabled? */
#include "qs_port.h"   /* QS/C port from the port directory */
#else
#include "qs_dummy.h"  /* QS/C dummy (inactive) interface */
#endif

#ifdef __cplusplus
}
#endif

#endif /* QUIT_H */
