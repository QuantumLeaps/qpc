/**
* @file
* @brief QXK/C port to ARM Cortex-R, IAR-ARM toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.7.1
* Date of the Last Update:  2016-09-22
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2016 Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qxk_port_h
#define qxk_port_h

/* QXK-specific Interrupt Request handler BEGIN */
#ifdef __ARMVFP__
#define QXK_IRQ_BEGIN(name_) \
    __stackless __arm void name_(void) { \
    __asm(" SUB LR, LR, #4\n" \
    " SRSDB #31!\n" \
    " CPS #31\n" \
    " PUSH {R0-R3, R12}"); \
    __asm(" FMRX R12, FPSCR\n" \
    " STMFD SP!, {R12}\n" \
    " FMRX R12, FPEXC\n" \
    " STMFD SP!, {R12}\n" \
    " FSTMDBD SP!, {D0-D7}"); \
    __asm(" AND R3, SP, #4\n" \
    " SUB SP, SP, R3\n" \
    " PUSH {R3, LR}\n"); \
    ++QXK_attr_.intNest; {
#else
#define QXK_IRQ_BEGIN(name_) \
    __stackless __arm void name_(void) { \
    __asm(" SUB LR, LR, #4\n" \
    " SRSDB #31!\n" \
    " CPS #31\n" \
    " PUSH {R0-R3, R12}"); \
    __asm(" AND R3, SP, #4\n" \
    " SUB SP, SP, R3\n" \
    " PUSH {R3, LR}\n"); \
    ++QXK_attr_.intNest; {
#endif

/* QXK-specific Interrupt Request handler END */
#ifdef __ARMVFP__
#define QXK_IRQ_END() \
    } --QXK_attr_.intNest; \
    if (QXK_attr_.intNest == (uint_fast8_t)0) { \
        uint_fast8_t p = QXK_schedPrio_(); \
        if (p != (uint_fast8_t)0) { \
            QXK_sched_(p); \
        } \
    } \
    __asm(" POP {R3, LR}\n" \
    " ADD SP, SP, R3"); \
    __asm(" FLDMIAD SP!, {D0-D7}\n" \
    " LDMFD SP!, {R12}\n" \
    " FMXR FPEXC, R12 \n" \
    " LDMFD SP!, {R12} \n" \
    " FMXR FPSCR, R12"); \
    __asm(" POP {R0-R3, R12}\n" \
    " RFEIA SP!"); \
}
#else
#define QXK_IRQ_END() \
    } --QXK_attr_.intNest; \
    if (QXK_attr_.intNest == (uint_fast8_t)0) { \
        uint_fast8_t p = QXK_schedPrio_(); \
        if (p != (uint_fast8_t)0) { \
            QXK_sched_(p); \
        } \
    } \
    __asm(" POP {R3, LR}\n" \
    " ADD SP, SP, R3"); \
    __asm(" POP {R0-R3, R12}\n" \
    " RFEIA SP!"); \
}
#endif

#include "qxk.h" /* QXK platform-independent public interface */

#endif /* qxk_port_h */
