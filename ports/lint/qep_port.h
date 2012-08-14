/*****************************************************************************
* Product:  QEP/C port for Lint, Generic C compiler
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qep_port_h
#define qep_port_h

/**
* \file
* \ingroup qep qf qk qs
* \brief QEP/C port to QK for a "generic" C compiler.
*
* \note This is just an example of a QEP port used for "linting" the QEP.
*/

/****************************************************************************/
/** \brief The size (in bytes) of the signal of an event. Valid values:
* 1, 2, or 4; default 2
*
* This macro can be defined in the QEP ports to configure the ::QSignal type.
* If the macro is not defined, the default of 1 byte will be chosen in qep.h.
* The valid #Q_SIGNAL_SIZE values of 1, 2, or 4, correspond to ::QSignal of
* uint8_t, uint16_t, and uint32_t, respectively. The ::QSingal data type
* determines the dynamic range of numerical values of signals you your
* application.
* \sa ::QEvt
*
* \note Once you choose a certain value of #Q_SIGNAL_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qep_port.h header file and henceforth include
* this header file in all builds.
*/
#define Q_SIGNAL_SIZE  2

             /* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
#include <stdint.h>

#include "qep.h"               /* QEP platform-independent public interface */

#endif                                                        /* qep_port_h */

