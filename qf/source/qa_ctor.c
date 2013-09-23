/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 11, 2013
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
#include "qf_pkg.h"
/*#include "qassert.h"*/

/*Q_DEFINE_THIS_MODULE("qa_ctor")*/

/**
* \file
* \ingroup qf
* \brief QActive_ctor() implementation.
*/

/*..........................................................................*/
void QActive_ctor(QActive * const me, QStateHandler initial) {
    static QActiveVtbl const vtbl = {              /* QActive virtual table */
        { &QHsm_init,
          &QHsm_dispatch },
        &QActive_start,
        &QActive_post,
        &QActive_postLIFO
    };
    QHsm_ctor(&me->super, initial);
    me->super.vptr = &vtbl.super; /* hook the vptr to QActive virtual table */
}
