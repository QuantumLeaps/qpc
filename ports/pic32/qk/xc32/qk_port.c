/**
* @file
* @brief QK/C port, preemptive QK kernel, MPLAB-X XC32 compiler
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.5.1
* Last updated on  2019-06-12
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
    QK_activate_();  /* execute higher priority active ojbect */
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
