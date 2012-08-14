/*****************************************************************************
* Product:  QEP/C port, AVR-xmega, IAR compiler
* Last Updated for Version: 4.1.00
* Date of the Last Update:  Dec 10, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
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

             /* the extended keyword __flash allocates const objects to ROM */
#define Q_ROM                   __flash

#include <stdint.h>                    /* C99-standard exact-width integers */
#include "qep.h"               /* QEP platform-independent public interface */

#endif                                                        /* qep_port_h */
