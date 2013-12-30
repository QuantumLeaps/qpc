/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 02, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"

/**
* \file
* \ingroup qf
* \brief QF_subscrList_, QF_maxSignal_ definitions,
* and QF_psInit() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QSubscrList *QF_subscrList_;
enum_t QF_maxSignal_;

/*..........................................................................*/
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal) {
    QF_subscrList_ = subscrSto;
    QF_maxSignal_  = maxSignal;
                                    /* zero the subscriber list, see NOTE01 */
    QF_bzero(subscrSto,
             (uint_t)((uint_t)maxSignal * (uint_t)sizeof(QSubscrList)));
}

/*****************************************************************************
* NOTE01:
* The QF_psInit() function clears the subscriber list, so that the
* framework can start correctly even if the startup code fails to clear
* the uninitialized data (as is required by the C Standard).
*/

