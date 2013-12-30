/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 20, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_act")

/**
* \file
* \ingroup qf
* \brief QF_active_[], and QF_add_()/QF_remove_(), QF_bzero() implementation.
*/

/* public objects ----------------------------------------------------------*/
QActive *QF_active_[QF_MAX_ACTIVE + 1];      /* to be used by QF ports only */

/*..........................................................................*/
void QF_add_(QActive * const a) {
    uint8_t p = a->prio;
    QF_CRIT_STAT_

    Q_REQUIRE(((uint8_t)0 < p) && (p <= (uint8_t)QF_MAX_ACTIVE)
              && (QF_active_[p] == (QActive *)0));

    QF_CRIT_ENTRY_();

    QF_active_[p] = a;       /* register the active object at this priority */

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_ADD, QS_priv_.aoObjFilter, a)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(a);                                    /* the active object */
        QS_U8_(p);                     /* the priority of the active object */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}
/*..........................................................................*/
void QF_remove_(QActive const * const a) {
    uint8_t p = a->prio;
    QF_CRIT_STAT_

    Q_REQUIRE(((uint8_t)0 < p) && (p <= (uint8_t)QF_MAX_ACTIVE)
              && (QF_active_[p] == a));

    QF_CRIT_ENTRY_();

    QF_active_[p] = (QActive *)0;             /* free-up the priority level */

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_REMOVE, QS_priv_.aoObjFilter, a)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(a);                                    /* the active object */
        QS_U8_(p);                     /* the priority of the active object */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}

/* macro to encapsulate pointer incrementing, which violates
* MISRA-C:2004 required rule 17.4 (pointer arithmetic used).
*/
#define QF_PTR_INC_(p_) (++(p_))

/*..........................................................................*/
void QF_bzero(void * const start, uint_t len) {
    uint8_t *ptr = (uint8_t *)start;
    while (len != (uint_t)0) {
        *ptr = (uint8_t)0;
        QF_PTR_INC_(ptr);
        --len;
    }
}

