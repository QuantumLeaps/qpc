/**
* @file
* @brief ::QActive services and ::QF support code
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.3.8
* Last updated on  2019-01-10
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_act")

/* public objects ***********************************************************/
QActive *QF_active_[QF_MAX_ACTIVE + 1]; /* to be used by QF ports only */

/****************************************************************************/
/**
* @description
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
    uint_fast8_t p = (uint_fast8_t)a->prio;
    QF_CRIT_STAT_

    /** @pre the priority of the active object must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the priority of the active
    * object must not be already in use. QF requires each active object to
    * have a __unique__ priority.
    */
    Q_REQUIRE_ID(100, ((uint_fast8_t)0 < p)
                       && (p <= (uint_fast8_t)QF_MAX_ACTIVE)
                       && (QF_active_[p] == (QActive *)0));

    QF_CRIT_ENTRY_();
    QF_active_[p] = a; /* register the active object at this priority */
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
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
    uint_fast8_t p = (uint_fast8_t)a->prio;
    QF_CRIT_STAT_

    /** @pre the priority of the active object must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the priority of the active
    * object must be already registered with the framework.
    */
    Q_REQUIRE_ID(200, ((uint_fast8_t)0 < p)
                       && (p <= (uint_fast8_t)QF_MAX_ACTIVE)
              && (QF_active_[p] == a));

    QF_CRIT_ENTRY_();
    QF_active_[p] = (QActive *)0; /* free-up the priority level */
    a->super.state.fun = Q_STATE_CAST(0); /* invalidate the state */
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* macro to encapsulate pointer increment, which violates MISRA-C:2004
* required rule 17.4 (pointer arithmetic used).
*
* @param[in,out]  p_  pointer to be incremented.
*/
#define QF_PTR_INC_(p_) (++(p_))

/****************************************************************************/
/**
* @description
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
    while (len != (uint_fast16_t)0) {
        *ptr = (uint8_t)0;
        QF_PTR_INC_(ptr);
        --len;
    }
}

/* log-base-2 implementation ************************************************/
#ifndef QF_LOG2

    uint_fast8_t QF_LOG2(QPSetBits x) {
        static uint8_t const log2LUT[16] = {
            (uint8_t)0, (uint8_t)1, (uint8_t)2, (uint8_t)2,
            (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
            (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
            (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4
        };
        uint_fast8_t n = (uint_fast8_t)0;
        QPSetBits    t;

#if (QF_MAX_ACTIVE > 16)
        t = (QPSetBits)(x >> 16);
        if (t != (QPSetBits)0) {
            n += (uint_fast8_t)16;
            x = t;
        }
#endif
#if (QF_MAX_ACTIVE > 8)
        t = (x >> 8);
        if (t != (QPSetBits)0) {
            n += (uint_fast8_t)8;
            x = t;
        }
#endif
        t = (x >> 4);
        if (t != (QPSetBits)0) {
            n += (uint_fast8_t)4;
            x = t;
        }
        return n + (uint_fast8_t)log2LUT[x];
    }

#endif /* QF_LOG2 */

