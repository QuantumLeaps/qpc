/*============================================================================
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of open-source GPL 3.0
* (or any later version), or alternatively, under the terms of one of the
* closed-source Quantum Leaps commercial licenses.
*
* The terms of the open source GPL 3.0 license can be found at:
* <www.gnu.org/licenses/gpl-3.0.txt>
*
* The terms of the closed-source Quantum Leaps commercial licenses
* can be found at:
* <www.state-machine.com/licensing>
*
* NOTE: Please do NOT plagiarize this software to sidestep the license
* obligations. This is both unfair and illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/**
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief Internal (package scope) QXK/C interface.
*/
#ifndef QXK_PKG_H
#define QXK_PKG_H

/*! internal QXK function to start multitasking (typically in assembly) */
void QXK_start_(void);

/*! initialize the private stack of a given QXThread */
void QXK_stackInit_(void *thr, QXThreadHandler const handler,
                    void * const stkSto, uint_fast16_t const stkSize);

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

/*! intertnal macro to encapsulate casting of pointers for MISRA deviations */
/**
* @details
* This macro is specifically and exclusively used for casting pointers
* that are never de-referenced, but only used for internal bookkeeping and
* checking (via assertions) the correct operation of the QXK kernel.
* Such pointer casting is not compliant with MISRA-2012-Rule 11.3(req)
* as well as other messages (e.g., PC-Lint-Plus warning 826).
* Defining this specific macro for this purpose allows to selectively
* disable the warnings for this particular case.
*/
#define QXK_PTR_CAST_(type_, ptr_) ((type_)(ptr_))

#include "qf_pkg.h"  /* QF package-scope interface */

#endif /* QXK_PKG_H */
