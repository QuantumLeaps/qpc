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
* @brief ::QActive services and @ref qf support code
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_act")

/*==========================================================================*/
/* global objects */
/*! @static @private @memberof QF */
QActive *QF_active_[QF_MAX_ACTIVE + 1U]; /* to be used by QF ports only */

/*==========================================================================*/
/*!
* @static @private @memberof QF
* @details
* This function adds a given active object to the active objects
* managed by the QF framework. It should not be called by the application
* directly, only by the QP ports.
*
* @param[in]  a  pointer to the active object to add to the framework.
*
* @note The priority of the active object @p a should be set before calling
* this function.
*
* @sa QF_remove_()
*/
void QF_add_(QActive * const a) {
    uint_fast8_t const p = (uint_fast8_t)a->prio;

    /*! @pre the priority of the active object must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the priority of the active
    * object must not be already in use. QF requires each active object to
    * have a __unique__ priority.
    */
    Q_REQUIRE_ID(100, (0U < p)
                       && (p <= QF_MAX_ACTIVE)
                       && (QF_active_[p] == (QActive *)0));
    QF_CRIT_STAT_
    QF_CRIT_E_();
    QF_active_[p] = a; /* register the active object at this priority */
    QF_CRIT_X_();
}

/*==========================================================================*/
/*!
* @static @private @memberof QF
* @details
* This function removes a given active object from the active objects managed
* by the QF framework. It should not be called by the application
* directly, only by the QP ports.
*
* @param[in]  a  pointer to the active object to remove from the framework.
*
* @note
* The active object that is removed from the framework can no longer
* participate in the publish-subscribe event exchange.
*
* @sa QF_add_()
*/
void QF_remove_(QActive * const a) {
    uint_fast8_t const p = (uint_fast8_t)a->prio;

    /*! @pre the priority of the active object must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the priority of the active
    * object must be already registered with the framework.
    */
    Q_REQUIRE_ID(200, (0U < p) && (p <= QF_MAX_ACTIVE)
                       && (QF_active_[p] == a));
    QF_CRIT_STAT_
    QF_CRIT_E_();
    QF_active_[p] = (QActive *)0; /* free-up the priority level */
    a->super.state.fun = Q_STATE_CAST(0); /* invalidate the state */
    QF_CRIT_X_();
}

/*==========================================================================*/
/*!
* @static @public @memberof QF
* @details
* Clears a memory buffer by writing zeros byte-by-byte.
*
* @param[in]  start  pointer to the beginning of a memory buffer.
* @param[in]  len    length of the memory buffer to clear (in bytes)
*
* @note The main application of this function is clearing the internal QF
* variables upon startup. This is done to avoid problems with non-standard
* startup code provided with some compilers and toolsets (e.g., TI DSPs or
* Microchip MPLAB), which does not zero the uninitialized variables, as
* required by the ANSI C standard.
*/
void QF_bzero(void * const start, uint_fast16_t len) {
    uint8_t *ptr = (uint8_t *)start;
    for (uint_fast16_t n = len; n > 0U; --n) {
        *ptr = 0U;
        ++ptr;
    }
}

/*==========================================================================*/
/* log-base-2 implementation */
#ifndef QF_LOG2

    /*! @static @public @memberof QF */
    uint_fast8_t QF_LOG2(QPSetBits x) {
        static uint8_t const log2LUT[16] = {
            0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
            4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
        };
        uint_fast8_t n = 0U;
        QPSetBits    t;

#if (QF_MAX_ACTIVE > 16U)
        t = (QPSetBits)(x >> 16U);
        if (0U != t) {
            n += 16U;
            x = t;
        }
#endif
#if (QF_MAX_ACTIVE > 8U)
        t = (x >> 8U);
        if (0U != t) {
            n += 8U;
            x = t;
        }
#endif
        t = (x >> 4U);
        if (0U != t) {
            n += 4U;
            x = t;
        }
        return n + log2LUT[x];
    }

#endif /* QF_LOG2 */
