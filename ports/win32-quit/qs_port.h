/**
* @file
* @brief QS/C port to Win32 with GNU or Visual C++ compilers
* @ingroup ports
* @cond
******************************************************************************
* Last Updated for Version: 5.9.0
* Date of the Last Update:  2017-05-16
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2005-2017 Quantum Leaps, LLC. All rights reserved.
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
#ifndef QS_PORT_H
#define QS_PORT_H

/* QS time-stamp size in bytes */
#define QS_TIME_SIZE     4

#ifdef _WIN64 /* 64-bit architecture? */
    #define QS_OBJ_PTR_SIZE 8
    #define QS_FUN_PTR_SIZE 8
#else         /* 32-bit architecture */
    #define QS_OBJ_PTR_SIZE 4
    #define QS_FUN_PTR_SIZE 4
#endif

/* flush the QS output buffer after each QS record */
#define QS_REC_DONE()  QS_onFlush()

/*****************************************************************************
* NOTE: QS might be used with or without other QP components, in which
* case the separate definitions of the macros QF_CRIT_STAT_TYPE,
* QF_CRIT_ENTRY, and QF_CRIT_EXIT are needed. In this port QS is configured
* to be used with the other QP component, by simply including "qf_port.h"
* *before* "qs.h".
*/
#include "qf_port.h" /* use QS with QF */
#include "qs.h"      /* QS platform-independent public interface */

#endif /* QS_PORT_H */
