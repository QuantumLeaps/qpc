/**
* \file
* \brief QMActive_ctor() definition
* \ingroup qf
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-02-19
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
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"

/****************************************************************************/
/**
* \description
* Performs the first step of active object initialization by assigning
* the virtual pointer and calling the superclass constructor.
*
* \arguments
* \arg[in,out] \c me       pointer (see \ref derivation)
* \arg[in]     \c initial  pointer to the event to be dispatched to the MSM
*
* \note  Must be called only ONCE before QMSM_INIT().
*
* \sa QHsm_ctor() and QFsm_ctor()
*/
void QMActive_ctor(QMActive * const me, QStateHandler initial) {
    static QActiveVtbl const vtbl = { /* QMActive virtual table */
        { &QMsm_init_,
          &QMsm_dispatch_ },
        &QActive_start_,
        &QActive_post_,
        &QActive_postLIFO_
    };

    /**
    * \note QMActive inherits QActive, so by the \ref derivation convention
    * it should call the constructor of the superclass, i.e., QActive_ctor().
    * However, this would pull in the QActiveVtbl, which in turn will pull
    * in the code for QHsm_init_() and QHsm_dispatch_() implemetations,
    * which is expensive. To avoid this code size penalty, in case QHsm is
    * not used in a given project, the call to QMsm_ctor() avoids pulling
    * in the code for QHsm.
    */
    QMsm_ctor(&me->super, initial);

    me->super.vptr = &vtbl.super; /* hook vptr to QMActive virtual table */
}
