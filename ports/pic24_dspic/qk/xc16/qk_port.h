/**
* @file
* @brief QK/C port, PIC24/dsPIC, QK kernel, MPLABX-XC16 compiler
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.5.1
* Last updated on  2019-06-10
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/* QK interrupt entry and exit */
#define QK_ISR(psv_)                                \
void  __attribute__((__interrupt__(__preprologue__( \
    "push   RCOUNT      \n"                         \
    "push.d w0          \n"                         \
    "mov.w  [w15-8],w0  \n"                         \
    "lsr.w  w0,#13,w1   \n"                         \
    "mov.w  #1,w0       \n"                         \
    "sl     w0,w1,w0    \n"                         \
    "ior.b  _QK_IPL_set_\n"                         \
    "bra    .+6         "))                         \
    , psv_))

#define QK_ISR_EXIT() do {                                     \
    register uint16_t this_sr;                                 \
    __asm__ volatile (                                         \
        "mov.w  SR,%0    \n"                                   \
        "lsr    %0,#5,w0 \n"                                   \
        "and.w  w0,#7,w0 \n"                                   \
        "mov.w  #1,w1    \n"                                   \
        "sl     w1,w0,w0 \n"                                   \
        "ior.b  #1,w0    \n"                                   \
        "com.b  w0,w0    \n"                                   \
        "disi   #0x3FFF  \n"                                   \
        "and.b  _QK_IPL_set_" : "=r"(this_sr) : : "w0", "w1"); \
    if (QK_IPL_set_ == 0) {                                    \
        if (QK_sched_() != 0U) {                               \
            __asm__ volatile ("clr.b SR");                     \
            QK_activate_();                                    \
            __asm__ volatile ("mov.w %0,SR" : : "r"(this_sr)); \
        }                                                      \
    }                                                          \
    __asm__ volatile ("disi #0x0000");                         \
} while (false);

#include "qk.h"  /* QK platform-independent public interface */

/* set of active interrupt priority levels (IPLs) */
extern uint8_t volatile QK_IPL_set_;

#endif /* QK_PORT_H */

