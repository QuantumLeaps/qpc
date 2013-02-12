/*****************************************************************************
* Product: QK/C port to Altera Nios II, GNU compiler
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 10, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/

#include "qf_port.h"                                     /* QF port include */

/*..........................................................................*/
void QF_tick_(void) {
    /* enable lower-priority interrupts if the SYS_CLK_TIMER_IRQ is not
    * already the highest-priority IRQ in the system
    */
#if !defined(ALT_ENHANCED_INTERRUPT_API_PRESENT) && (SYS_CLK_TIMER_IRQ > 0U)
    alt_u32 prio_mask = alt_irq_interruptible(SYS_CLK_TIMER_IRQ);
#endif

    QF_TICK((void *)0);

#if !defined(ALT_ENHANCED_INTERRUPT_API_PRESENT) && (SYS_CLK_TIMER_IRQ > 0U)
    alt_irq_non_interruptible(prio_mask);
#endif
}
