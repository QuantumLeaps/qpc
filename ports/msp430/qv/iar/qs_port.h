/**
* @file
* @brief QS/C port  port to MSP430, IAR-430 compiler
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qs_port_h
#define qs_port_h

#define QS_TIME_SIZE            4
#define QS_OBJ_PTR_SIZE         2

#if __CORE__ == __430X__
    #define QS_FUN_PTR_SIZE     4
#else
    #define QS_FUN_PTR_SIZE     2
#endif

/*****************************************************************************
* NOTE:
* QS might be used with or without other QP components, in which case
* the separate definitions of the macros Q_ROM, Q_ROM_VAR, QF_CRIT_STAT_TYPE,
* QF_CRIT_ENTRY, and QF_CRIT_EXIT are needed. In this port QS is configured
* to be used with the other QP component, by simply including "qf_port.h"
* *before* "qs.h".
*/
#include "qf_port.h"  /* use QS with QF */
#include "qs.h"       /* QS platform-independent public interface */

#endif /* qs_port_h  */
