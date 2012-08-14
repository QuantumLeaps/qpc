/*****************************************************************************
* Product:  QEP/C port, TMS320C55x, TI-C2000 compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 06, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
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
