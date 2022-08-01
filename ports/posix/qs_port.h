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
* @date Last updated on: 2022-06-12
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QS/C port to POSIX with GNU compiler
* @ingroup ports
*/
#ifndef QS_PORT_H
#define QS_PORT_H

#define QS_TIME_SIZE        4U

#if defined(__LP64__) || defined(_LP64) /* 64-bit architecture? */
    #define QS_OBJ_PTR_SIZE 8U
    #define QS_FUN_PTR_SIZE 8U
#else                                   /* 32-bit architecture */
    #define QS_OBJ_PTR_SIZE 4U
    #define QS_FUN_PTR_SIZE 4U
#endif

void QS_output(void);    /* handle the QS output */
void QS_rx_input(void);  /* handle the QS-RX input */

/*==========================================================================*/
/* NOTE: QS might be used with or without other QP components, in which
* case the separate definitions of the macros QF_CRIT_STAT_TYPE,
* QF_CRIT_ENTRY, and QF_CRIT_EXIT are needed. In this port QS is configured
* to be used with the other QP component, by simply including "qf_port.h"
* *before* "qs.h".
*/
#ifndef QF_PORT_H
#include "qf_port.h" /* use QS with QF */
#endif

#include "qs.h"      /* QS platform-independent public interface */

#endif /* QS_PORT_H  */

