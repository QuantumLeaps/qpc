/*****************************************************************************
* Product:  QK/C, Renesas RX, QK port, IAR compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 08, 2012
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
#ifndef qk_port_h
#define qk_port_h

                                             /* QK interrupt entry and exit */
#define QK_ISR_ENTRY() do { \
    ++QK_intNest_; \
    __enable_interrupt(); \
} while (0)

#define QK_ISR_EXIT()  do { \
    __disable_interrupt(); \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        uint8_t p = QK_schedPrio_(); \
        if (p != (uint8_t)0) { \
            __set_interrupt_level(0); \
            QK_schedExt_(p); \
        } \
    } \
} while (0)

#define QK_EXT_SAVE(act_) \
    ((act_)->thread = (void *)_builtin_get_fpsw())

#define QK_EXT_RESTORE(act_) \
    _builtin_set_fpsw((unsigned long)(act_)->thread)

#include "machine.h"         /* for _builtin_get_fpsw()/_builtin_set_fpsw() */
#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */
