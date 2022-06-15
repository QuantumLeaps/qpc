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
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QMActive_ctor() definition
*
* @description
* This file must remain separate from the rest to avoid pulling in the
* "virtual" functions QHsm_init_() and QHsm_dispatch_() in case they
* are not used by the application.
*
* @sa qf_qact.c
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */

/*Q_DEFINE_THIS_MODULE("qf_qmact")*/

/*
* This internal macro encapsulates the violation of MISRA-C 2012
* Rule 11.3(req) "A cast shall not be performed between a pointer to
* object type and a poiner to a different object type".
*/
#define QMSM_CAST_(ptr_) ((QMsm *)(ptr_))

/*..........................................................................*/
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

    /*!
    * @note QMActive inherits QActive, so by the @ref oop convention
    * it should call the constructor of the superclass, i.e., QActive_ctor().
    * However, this would pull in the QActiveVtable, which in turn will pull
    * in the code for QHsm_init_() and QHsm_dispatch_() implemetations,
    * which is expensive. To avoid this code size penalty, in case QHsm is
    * not used in a given project, the call to QMsm_ctor() avoids pulling
    * in the code for QHsm.
    */
    QMsm_ctor(QMSM_CAST_(&me->super.super), initial);

    me->super.super.vptr = &vtable.super; /* hook vptr to QMActive vtable */
}
