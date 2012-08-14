/*****************************************************************************
* Product:  QK/C, Renesas RX, QK port, GNU-RX (KPIT) compiler
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
    QF_INT_ENABLE(); \
} while (0)

#define QK_ISR_EXIT()  do { \
    QF_INT_DISABLE(); \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        uint8_t p = QK_schedPrio_(); \
        if (p != (uint8_t)0) { \
            __builtin_rx_mvtc(0, 0); \
            QK_schedExt_(p); \
        } \
    } \
} while (0)

                  /* saving and restoring the extended QK context (RX FPSW) */
#define QK_EXT_SAVE(act_) \
    ((act_)->thread = (void *)__builtin_rx_mvfc(3))

#define QK_EXT_RESTORE(act_) \
    __builtin_rx_mvtc(3, (int)(act_)->thread)


#include "qk.h"                 /* QK platform-independent public interface */


/*****************************************************************************
* NOTE01:
* The inline function clr_ipl() is more efficient than
*/

#endif                                                         /* qk_port_h */
