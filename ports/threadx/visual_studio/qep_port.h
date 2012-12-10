/*****************************************************************************
* Product:  QEP/C port to ThreadX
* Last Updated for Version: 4.2.04
* Date of the Last Update:  Sep 30, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
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

#include "tx_api.h"    /* ThreadX interface (for exact-width integer types) */

/* Exact-width types from TreadX (WG14/N843 C99 Standard, Section 7.18.1.1) */
typedef CHAR      int8_t;
typedef SHORT     int16_t;
typedef LONG      int32_t;
typedef UCHAR     uint8_t;
typedef USHORT    uint16_t;
typedef ULONG     uint32_t;

#include "qep.h"               /* QEP platform-independent public interface */

#endif                                                        /* qep_port_h */
