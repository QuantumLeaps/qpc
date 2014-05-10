/*****************************************************************************
* Product: QK/C, ARM Cortex-M, QK port, Generic C compiler
* Last updated for version 5.3.0
* Last updated on  2014-04-23
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#ifndef qk_port_h
#define qk_port_h

/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() do { \
    QF_INT_DISABLE(); \
    ++QK_intNest_; \
    QF_INT_ENABLE(); \
} while (0)

#define QK_ISR_EXIT()  do { \
    QF_INT_DISABLE(); \
    --QK_intNest_; \
    if (QK_schedPrio_() != (uint_fast8_t)0) { \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)0x10000000U; \
    } \
    QF_INT_ENABLE(); \
} while (0)

#include "qk.h" /* QK platform-independent public interface */

#endif /* qk_port_h */
