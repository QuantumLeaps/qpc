/*****************************************************************************
* Product: QK/C
* Last Updated for Version: 4.4.02
* Date of the Last Update:  Apr 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#ifndef qk_pkg_h
#define qk_pkg_h

/**
* \file
* \ingroup qk
* \brief Internal (package scope) QK/C interface.
*/

#include "qf_port.h"                 /* QF port include; includes qk_port.h */

                                            /* QF-specific critical section */
#ifndef QF_CRIT_STAT_TYPE
    /** \brief This is an internal macro for defining the critical section
    * status type.
    *
    * The purpose of this macro is to enable writing the same code for the
    * case when critical sectgion status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * provides the definition of the critical section status varaible.
    * Otherwise this macro is empty.
    * \sa #QF_CRIT_STAT_TYPE
    */
    #define QF_CRIT_STAT_

    /** \brief This is an internal macro for entering a critical section.
    *
    * The purpose of this macro is to enable writing the same code for the
    * case when critical sectgion status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_ENTRY passing the key variable as the parameter.
    * Otherwise #QF_CRIT_ENTRY is invoked with a dummy parameter.
    * \sa #QF_CRIT_ENTRY
    */
    #define QF_CRIT_ENTRY_()    QF_CRIT_ENTRY(dummy)

    /** \brief This is an internal macro for exiting a cricial section.
    *
    * The purpose of this macro is to enable writing the same code for the
    * case when critical sectgion status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_EXIT passing the key variable as the parameter.
    * Otherwise #QF_CRIT_EXIT is invoked with a dummy parameter.
    * \sa #QF_CRIT_EXIT
    */
    #define QF_CRIT_EXIT_()     QF_CRIT_EXIT(dummy)

#else
    #define QF_CRIT_STAT_       QF_CRIT_STAT_TYPE critStat_;
    #define QF_CRIT_ENTRY_()    QF_CRIT_ENTRY(critStat_)
    #define QF_CRIT_EXIT_()     QF_CRIT_EXIT(critStat_)
#endif
                                                /* package-scope objects... */
#ifndef QK_NO_MUTEX
    extern uint8_t QK_ceilingPrio_;          /**< QK mutex priority ceiling */
#endif

/****************************************************************************/
/* QS software tracing integration, only if enabled                         */
#ifdef Q_SPY                                /* QS software tracing enabled? */
    #include "qs_port.h"                                 /* include QS port */
#else
    #include "qs_dummy.h"                /* disable the QS software tracing */
#endif                                                             /* Q_SPY */

#endif                                                          /* qk_pkg_h */

