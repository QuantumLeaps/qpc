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
* @brief QK/C port, preemptive QK kernel, MPLAB-X XC32 compiler
* @ingroup ports
*/
#include "qf_port.h"

/*..........................................................................*/
void QK_init(void) {
    INTCONSET = _INTCON_MVEC_MASK; /* configure multi-vectored interrupts */

    IPC0bits.CS0IP = 1; /* priority 1 for Core Software Interrupt 0, NOTE1 */
    IPC0bits.CS0IS = 0; /* sub-priority 0 for Core Software Interrupt 0 */

    IFS0CLR = _IFS0_CS0IF_MASK; /* clear the Core Software Interrupt 0 flag */
    IEC0SET = _IEC0_CS0IE_MASK; /* enable the Core Software Interrupt 0 */
}

/*..........................................................................*/
__attribute__((vector(_CORE_SOFTWARE_0_VECTOR),interrupt(IPL1AUTO), nomips16))
void QK_isr_(void) { /* NOTE2 */
    IFS0CLR = _IFS0_CS0IF_MASK; /* clear the Core Software Interrupt 0 flag */

    QF_INT_DISABLE();
    _mtc0(_CP0_STATUS, _CP0_STATUS_SELECT, 0U); /* drop IPL to 0 */
    QK_activate_(1U); /* execute higher priority active object */
    _mtc0(_CP0_STATUS, _CP0_STATUS_SELECT, 1U << 10); /*restore IPL to 1 */
    QF_INT_ENABLE();
}

/*****************************************************************************
* NOTE1:
* The Core Software Interrupt 0 at priority 1, sub-priority 0 is reserved for
* QK. All other interrupts must have higher priority, e.g. 2-7. The user may
* _not_ set the shadow register set to priority 1.
*
* NOTE2:
* The QK_isr_() Core Software Interrupt 0 is used to perform the scheduling
* (asynchronous preemption in QK). This ISR is configured to use software
* register stacking (IPL1SOFT), because it can nest on itself. The QK_isr_()
* runs at IPL1, which should be the lowest of all other ISRs. This will
* guarantee that the Core Software Interrupt 0 will not execute until all
* other interrupts have completed, including any nesting.
*
* Any ISR that interacts with QP must trigger Core Software Interrupt 0.
* This triggering is accomplished in the macros QK_ISR_ENTRY()/
* QK_ISR_EXIT(), which _must_ be placed at the entry and exit of every ISR,
* respectively.
*/
