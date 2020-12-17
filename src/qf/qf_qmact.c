/**
* @file
* @brief QMActive_ctor() definition
*
* @description
* This file must remain separate from the rest to avoid pulling in the
* "virtual" functions QHsm_init_() and QHsm_dispatch_() in case they
* are not used by the application.
*
* @sa qf_qact.c
*
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2020-12-16
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */

/*Q_DEFINE_THIS_MODULE("qf_qmact")*/

/****************************************************************************/
/**
* @description
* Performs the first step of active object initialization by assigning
* the virtual pointer and calling the superclass constructor.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     initial  pointer to the event to be dispatched to the MSM
*
* @note  Must be called only ONCE before QMSM_INIT().
*
* @sa QHsm_ctor()
*/
void QMActive_ctor(QMActive * const me, QStateHandler initial) {
    static QMActiveVtable const vtable = { /* QMActive virtual table */
        { &QMsm_init_,
          &QMsm_dispatch_
#ifdef Q_SPY
         ,&QMsm_getStateHandler_
#endif
        },
        &QActive_start_,
        &QActive_post_,
        &QActive_postLIFO_
    };

    /* clear the whole QActive object, so that the framework can start
    * correctly even if the startup code fails to clear the uninitialized
    * data (as is required by the C Standard).
    */
    QF_bzero(me, sizeof(*me));

    /**
    * @note QMActive inherits QActive, so by the @ref oop convention
    * it should call the constructor of the superclass, i.e., QActive_ctor().
    * However, this would pull in the QActiveVtable, which in turn will pull
    * in the code for QHsm_init_() and QHsm_dispatch_() implemetations,
    * which is expensive. To avoid this code size penalty, in case QHsm is
    * not used in a given project, the call to QMsm_ctor() avoids pulling
    * in the code for QHsm.
    */
    QMsm_ctor((QMsm *)&me->super.super, initial);

    me->super.super.vptr = &vtable.super; /* hook vptr to QMActive vtable */
}

