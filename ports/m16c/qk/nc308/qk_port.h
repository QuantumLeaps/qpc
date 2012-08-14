/*****************************************************************************
* Product:  QK/C port, M32C, QK port, NC308-compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Oct 29, 2008
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

                                        /* interrupt entry code, see NOTE01 */
#define QK_ISR_ENTRY() do { \
    ++QK_intNest_; \
    _asm("FSET I"); \
} while (0)

                              /* interrupt exit code, see NOTE01 and NOTE02 */
#define QK_ISR_EXIT() do { \
    _asm("LDC #0,FLG"); \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        QK_schedule_(); \
    } \
} while (0)


#include "qk.h"                 /* QK platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The M32 CPU supports interrupt prioritization. Therefore it
* is safe to unlock interrupts inside ISRs. By assigning priorities to
* interrupts you have full control over interrupt nesting. In particular,
* you can avoid interrupt nesting by assigning the same priority level to
* all interrupts.
*
* The simple policy of unconditional unlocking of interrupts upon exit from
* a critical section precludes nesting of critical sections. This policy
* means that you *must* unlock interrupts inside every ISR before invoking
* any QF service, such as QF_postFIFO(), QF_publish(), or QF_tick(). In fact,
* this QK port to M16C provides a pair of macros QK_ISR_ENTRY() and
* QK_ISR_EXIT() that you need to ivoke upon entry to every ISR and exit
* from the ISR, respectively. These macros, among others, unlock and lock
* the interrupts, respectively.
*
* NOTE02:
* The QK_ISR_EXIT() macro starts with loading 0 into the FLG register. This
* accomplishes two things: (1) it lowers the Interrupt Priority Level (IPL)
* to zero (task level), and at the same time disables interrupts by claring
* the I-bit. After this the QK priority is restored from the stack variable
* and the QK scheduler is invoked to perform the asynchronous preemptions.
*/

#endif                                                         /* qk_port_h */

