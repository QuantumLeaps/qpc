/*****************************************************************************
* Product:  QEP/C port, Keil compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 14, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
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

          /* special extended keyword 'code' allocates const objects to ROM */
#define Q_ROM           code

                                       /* 1-byte (255 signals) signal space */
#define Q_SIGNAL_SIZE   1


             /* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed   char   int8_t;
typedef signed   int    int16_t;
typedef signed   long   int32_t;
typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;

#include "qep.h"               /* QEP platform-independent public interface */

#endif                                                        /* qep_port_h */
