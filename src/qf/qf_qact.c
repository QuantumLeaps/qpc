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
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QActive_ctor() definition
*
* @description
* This file must remain separate from the rest to avoid pulling in the
* "virtual" functions QHsm_init_() and QHsm_dispatch_() in case they
* are not used by the application.
*
* @sa qf_qmact.c
*
* @ingroup qf
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */

/*Q_DEFINE_THIS_MODULE("qf_qact")*/

/*==========================================================================*/
/*!
* @protected @memberof QActive
* @description
* Performs the first step of active object initialization by assigning
* the virtual pointer and calling the superclass constructor.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     initial  pointer to the event to be dispatched to the MSM
*
* @note  Must be called only __once__ before QMSM_INIT().
* @sa QMsm_ctor() and QHsm_ctor()
*/
void QActive_ctor(QActive * const me, QStateHandler initial) {
    static QActiveVtable const vtable = {  /* QActive virtual table */
        { &QHsm_init_,
          &QHsm_dispatch_
#ifdef Q_SPY
          ,&QHsm_getStateHandler_
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

    QHsm_ctor(&me->super, initial); /* explicitly call superclass' ctor */
    me->super.vptr = &vtable.super; /* hook the vptr to QActive vtable */
}
