/**
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
* @cond
******************************************************************************
* Last updated for version 6.3.7
* Last updated on  2018-11-07
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */

/*Q_DEFINE_THIS_MODULE("qf_qact")*/

/****************************************************************************/
/**
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
    static QActiveVtbl const vtbl = {  /* QActive virtual table */
        { &QHsm_init_,
          &QHsm_dispatch_ },
        &QActive_start_,
        &QActive_post_,
        &QActive_postLIFO_
    };
    /* clear the whole QActive object, so that the framework can start
    * correctly even if the startup code fails to clear the uninitialized
    * data (as is required by the C Standard).
    */
    QF_bzero(me, (uint_fast16_t)sizeof(*me));

    QHsm_ctor(&me->super, initial); /* explicitly call superclass' ctor */
    me->super.vptr = &vtbl.super; /* hook the vptr to QActive virtual table */
}

