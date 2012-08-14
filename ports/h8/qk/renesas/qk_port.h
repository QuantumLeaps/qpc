/*****************************************************************************
* Product:  QK/C port, H8, QK port, Renesas H8 compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 01, 2008
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
#ifndef qk_port_h
#define qk_port_h

                                                    /* interrupt entry code */
#define QK_ISR_ENTRY()    (++QK_intNest_)

                                                     /* interrupt exit code */
#define QK_ISR_EXIT()     do { \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        QK_schedule_(0x00); \
    } \
} while (0)


#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */

