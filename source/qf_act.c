/**
* @file
* @brief QActive services and QF support code
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 5.4.0
* Last updated on  2015-03-13
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
* This function removes a given active object from the active objects
* managed by the QF framework. It should not be called by the application
* directly, only through the function QActive_stop().
*
* @param[in]  a  pointer to the active object to add to the framework.
*
* @note The priority of the active object @p a should be set before calling
* this function.
*
* @sa QF_remove_()
*/
void QF_add_(QActive * const a) {
    uint_fast8_t p = a->prio;
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

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_ADD, QS_priv_.aoObjFilter, a)
        QS_TIME_();         /* timestamp */
        QS_OBJ_(a);         /* the active object */
        QS_U8_((uint8_t)p); /* the priority of the active object */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function removes a given active object from the active objects managed
* by the QF framework. It should not be called by the application directly,
* only through the function QActive_stop().
*
* @param[in]  a  pointer to the active object to remove from the framework.
*
* @note The active object that is removed from the framework can no longer
* participate in the publish-subscribe event exchange.
*
* @sa QF_add_()
*/
void QF_remove_(QActive const * const a) {
    uint_fast8_t p = a->prio;
    QF_CRIT_STAT_

    /** @pre the priority of the active object must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the priority of the active
    * object must be already registered with the framework.
    */
    Q_REQUIRE_ID(200, ((uint_fast8_t)0 < p)
                       && (p <= (uint_fast8_t)QF_MAX_ACTIVE)
              && (QF_active_[p] == a));

    QF_CRIT_ENTRY_();

    QF_active_[p] = (QActive *)0;  /* free-up the priority level */

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_REMOVE, QS_priv_.aoObjFilter, a)
        QS_TIME_();         /* timestamp */
        QS_OBJ_(a);         /* the active object */
        QS_U8_((uint8_t)p); /* the priority of the active object */
    QS_END_NOCRIT_()

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

/* power-of-two lookup tables ***********************************************/
uint8_t const Q_ROM QF_pwr2Lkup[65] = {
    (uint8_t)0x00, /* unused location */
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80,
    (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x04, (uint8_t)0x08,
    (uint8_t)0x10, (uint8_t)0x20, (uint8_t)0x40, (uint8_t)0x80
};

uint8_t const Q_ROM QF_invPwr2Lkup[65] = {
    (uint8_t)0xFF, /* unused location */
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F,
    (uint8_t)0xFE, (uint8_t)0xFD, (uint8_t)0xFB, (uint8_t)0xF7,
    (uint8_t)0xEF, (uint8_t)0xDF, (uint8_t)0xBF, (uint8_t)0x7F
};

uint8_t const Q_ROM QF_div8Lkup[65] = {
    (uint8_t)0, /* unused location */
    (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0,
    (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0,
    (uint8_t)1, (uint8_t)1, (uint8_t)1, (uint8_t)1,
    (uint8_t)1, (uint8_t)1, (uint8_t)1, (uint8_t)1,
    (uint8_t)2, (uint8_t)2, (uint8_t)2, (uint8_t)2,
    (uint8_t)2, (uint8_t)2, (uint8_t)2, (uint8_t)2,
    (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
    (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7
};

#ifdef QF_LOG2LKUP

/* log-base-2 lookup table **************************************************/
uint8_t const Q_ROM QF_log2Lkup[256] = {
    (uint8_t)0, (uint8_t)1, (uint8_t)2, (uint8_t)2,
    (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8
};

#endif /* #ifdef QF_LOG2LKUP */
