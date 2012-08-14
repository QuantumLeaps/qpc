/*****************************************************************************
* Product: QK/C, PIC24/dsPIC, QK kernel, MPLAB-C30 compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 03, 2012
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
#define QK_ISR(psv_) \
void  __attribute__((__interrupt__(__preprologue__( \
    "push   RCOUNT      \n" \
    "push.d w0          \n" \
    "mov.w  [w15-8],w0  \n" \
    "lsr.w  w0,#13,w1   \n" \
    "mov.w  #1,w0       \n" \
    "sl     w0,w1,w0    \n" \
    "ior.b  _QK_intNest_\n" \
    "bra    .+6         ")) \
    , psv_))

#define QK_ISR_EXIT() do { \
    register uint16_t this_sr; \
    __asm__ volatile ( \
        "mov.w  SR,%0    \n" \
        "lsr    %0,#5,w0 \n" \
        "and.w  w0,#7,w0 \n" \
        "mov.w  #1,w1    \n" \
        "sl     w1,w0,w0 \n" \
        "ior.b  #1,w0    \n" \
        "com.b  w0,w0    \n" \
        "disi   #0x3FFF  \n" \
        "and.b  _QK_intNest_" : "=r"(this_sr) : : "w0", "w1"); \
    if (QK_intNest_ == 0) { \
        uint8_t p = QK_schedPrio_(); \
        if (p != (uint8_t)0) { \
            __asm__ volatile ("clr.b SR"); \
            QK_sched_(p); \
            __asm__ volatile ("mov.w %0,SR" : : "r"(this_sr)); \
        } \
    } \
    __asm__ volatile ("disi #0x0000"); \
} while (0);

#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */

