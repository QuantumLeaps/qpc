/**
* @file
* @brief Internal (package scope) QXK/C interface.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.9.7
* Last updated on  2017-08-20
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qxk_pkg_h
#define qxk_pkg_h

/*! internal QXK function to start multitasking (typically in assembly) */
void QXK_start_(void);

/*! initialize the private stack of a given QXThread */
void QXK_stackInit_(void *thr, QXThreadHandler handler,
                    void *stkSto, uint_fast16_t stkSize);

/*! called when a thread function returns */
void QXK_threadRet_(void);

/*! timeout signals */
enum QXK_Timeouts {
    QXK_DELAY_SIG = Q_USER_SIG,
    QXK_QUEUE_SIG,
    QXK_SEMA_SIG
};

/*! internal functin to block (suspend) a given extended thread */
void QXThread_block_(QXThread const * const me);

/*! internal function to unblock (resume) a given extended thread. */
void QXThread_unblock_(QXThread const * const me);

/*! internal function to arm the private time event for a given thread. */
void QXThread_teArm_(QXThread * const me,
                     QSignal sig,
                     uint_fast16_t const nTicks);

/*! internal function to disarm the private time event for a given thread. */
bool QXThread_teDisarm_(QXThread * const me);

#include "qf_pkg.h"  /* QF package-scope interface */

#endif /* qxk_pkg_h */

