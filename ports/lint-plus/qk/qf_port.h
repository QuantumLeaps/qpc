/*============================================================================
* QEP/C port to ARM Cortex-M, GNU-ARM
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
* @brief QF/C port example for QK, generic C compiler.
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* interrupt disabling mechanism */
#define QF_INT_DISABLE()            intDisable()
#define QF_INT_ENABLE()             intEnable()

void intDisable(void);
void intEnable(void);

/* QF critical section mechanism */
#define QF_CRIT_STAT_TYPE           crit_stat_t
#define QF_CRIT_ENTRY(stat_)        ((stat_) = critEntry())
#define QF_CRIT_EXIT(stat_)         critExit(stat_)

typedef unsigned int crit_stat_t;
crit_stat_t critEntry(void);
void critExit(crit_stat_t stat);

#include "qep_port.h"  /* QEP port */
#include "qk_port.h"   /* QK port */

#endif /* QF_PORT_H */
