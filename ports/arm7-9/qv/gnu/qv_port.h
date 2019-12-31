/**
* @file
* @brief QP/C port to ARM7-9, cooperative QV kernel, generic C compiler
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
* Web:   www.state-machine.com/licensing
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef QV_PORT_H
#define QV_PORT_H

void QV_irq(void);

void BSP_irq(void);
/* void BSP_fiq(void); see NOTE1 */

#include "qv.h" /* QV platform-independent public interface */

/*****************************************************************************
* NOTE1:
* The FIQ-type interrupts are never disabled in this port, so the FIQ is
* a "kernel-unaware" interrupt.
*
* If the FIQ is ever used in the application, it must be implemented in
* assembly.
*/

#endif /* QV_PORT_H */

