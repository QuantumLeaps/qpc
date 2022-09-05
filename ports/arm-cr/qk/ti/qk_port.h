/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-09-03
* @version Last updated for: @ref qpc_7_1_1
*
* @file
* @brief QK/C port to ARM Cortex-R, TI-ARM toolset
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/* QK-specific Interrupt Request handler BEGIN */
#define QK_IRQ_BEGIN(name_)              \
    void name_(void);                    \
    __asm(" .def " #name_ "\n"           \
    " .arm\n"                            \
    " .align 4\n"                        \
    " .armfunc " #name_ "\n"             \
    #name_":\n"                          \
    " SUB LR, LR, #4\n"                  \
    " SRSDB #31!\n"                      \
    " CPS #31\n"                         \
    " PUSH {R0-R3, R12}\n"               \
    " .if __TI_VFPV3D16_SUPPORT__ = 1\n" \
    " FMRX R12, FPSCR\n"                 \
    " STMFD SP!, {R12}\n"                \
    " FMRX R12, FPEXC\n"                 \
    " STMFD SP!, {R12}\n"                \
    " FSTMDBD SP!, {D0-D7}\n"            \
    " .endif\n"                          \
    " AND R3, SP, #4\n"                  \
    " SUB SP, SP, R3\n"                  \
    " PUSH {R3, LR}\n"                   \
    " BLX " #name_ "_isr\n"              \
    " POP {R3, LR}\n"                    \
    " ADD SP, SP, R3\n"                  \
    " .if __TI_VFPV3D16_SUPPORT__ = 1\n" \
    " FLDMIAD SP!, {D0-D7}\n"            \
    " LDMFD SP!, {R12}\n"                \
    " FMXR FPEXC, R12 \n"                \
    " LDMFD SP!, {R12} \n"               \
    " FMXR FPSCR, R12\n"                 \
    " .endif\n"                          \
    " POP {R0-R3, R12}\n"                \
    " RFEIA SP!\n");                     \
    void name_ ## _isr(void) {           \
    ++QF_intNest_; {

/* QK-specific Interrupt Request handler END */
#define QK_IRQ_END()              \
    } --QF_intNest_;              \
    if (QF_intNest_ == 0U) {      \
        if (QK_sched_() != 0U) {  \
            QK_activate_(1U);     \
        }                         \
    }                             \
}

#include "qk.h" /* QK platform-independent public interface */

#endif /* QK_PORT_H */
