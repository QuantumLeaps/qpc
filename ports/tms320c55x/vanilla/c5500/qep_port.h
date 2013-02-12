/*****************************************************************************
* Product:  QEP/C port, TMS320C55x, TI-C2000 compiler
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 16, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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

                                   /* 2-byte (64K) signal space, see NOTE01 */
#define Q_SIGNAL_SIZE 2

             /* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
#include <stdint.h>

typedef int16_t  int8_t;                                      /* see NOTE01 */
typedef uint16_t uint8_t;

#include "qep.h"               /* QEP platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The TMS320C55x cannot separately address 8-bit bytes (the smallest
* separately-addressable entity is a 16-bit word). Therefore the <stdint.h>
* header file does not define int8_t/uint8_t. However, for the QP framework
* they are approximated as int16_t/uint16_t, respectively.
*/

#endif                                                        /* qep_port_h */
