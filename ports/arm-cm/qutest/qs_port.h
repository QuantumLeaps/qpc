/*============================================================================
* QS/C port to ARM Cortex-M, QUTest, generic compiler
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
* @brief QS/C port to a 32-bit CPU and a generic C99 compiler.
* @ingroup qs
*/
#ifndef QS_PORT_H
#define QS_PORT_H

/* QS time-stamp size in bytes */
#define QS_TIME_SIZE     4

/* object pointer size in bytes */
#define QS_OBJ_PTR_SIZE  4

/* function pointer size in bytes */
#define QS_FUN_PTR_SIZE  4

/* flush the QS output buffer after each QS record */
#define QS_REC_DONE()  QS_onFlush()

//*==========================================================================*/
/* NOTE: QS might be used with or without other QP components, in which case
* the separate definitions of the macros QF_CRIT_STAT_TYPE, QF_CRIT_ENTRY,
* and QF_CRIT_EXIT are needed. In this port QS is configured to be used with
* the other QP component, by simply including "qf_port.h" *before* "qs.h".
*/
#include "qf_port.h" /* use QS with QF */
#include "qs.h"      /* QS platform-independent public interface */

#endif /* QS_PORT_H */

