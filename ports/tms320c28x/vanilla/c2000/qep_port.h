/*****************************************************************************
* Product:  QEP/C port
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 18, 2013
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
typedef signed   char  int8_t;                                /* see NOTE01 */
typedef signed   int   int16_t;
typedef signed   long  int32_t;
typedef unsigned char  uint8_t;                               /* see NOTE01 */
typedef unsigned int   uint16_t;
typedef unsigned long  uint32_t;

#include "qep.h"               /* QEP platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The TMS320C28x cannot separately address 8-bit bytes (the smallest
* separately-addressable entity is a 16-bit word). Therefore the TMS320C28x
* char is 16 bits (to make it separately addressable).
*/

#endif                                                        /* qep_port_h */
