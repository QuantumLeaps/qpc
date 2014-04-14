/**
* \file
* \brief ::QEP_reservedEvt_[] definition.
* \ingroup qep
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-04-08
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
******************************************************************************
* \endcond
*/
#include "qep_port.h"     /* QEP port */
#include "qep_pkg.h"

/****************************************************************************/
/**
* \description
* Static, preallocated standard events that the QEP event processor sends
* to state handler functions of ::QHsm and ::QFsm subclasses to execute
* entry actions, exit actions, and initial transitions.
*/
QEvt const QEP_reservedEvt_[] = {
    { (QSignal)QEP_EMPTY_SIG_, (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG,    (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG,     (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_INIT_SIG,     (uint8_t)0, (uint8_t)0 }
};
