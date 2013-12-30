/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 24, 2013
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
#ifndef qp_port_h
#define qp_port_h

/**
* \file
* \ingroup qep qf qk qs
* \brief QP/C public interface.
*
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP/C.
*/

#ifndef QP_API_VERSION

/** \brief Macro that specifies the backwards compatibility with the
* QP/C API version.
*
* For example, QP_API_VERSION=450 will cause generating the compatibility
* layer with QP/C version 4.5.0 and newer, but not older than 4.5.0.
* QP_API_VERSION=0 causes generation of the compatibility layer "from the
* begining of time", which is the maximum backwards compatibilty. This is
* the default.
*
* Conversely, QP_API_VERSION=9999 means that no compatibility layer should
* be generated. This setting is useful for checking if an application
* complies with the latest QP/C API.
*/
#define QP_API_VERSION 0
#endif                                            /* #ifndef QP_API_VERSION */


#include "qf_port.h"                   /* QF/C port from the port directory */
#include "qassert.h"                                       /* QP assertions */

#ifdef Q_SPY                                   /* software tracing enabled? */
    #include "qs_port.h"               /* QS/C port from the port directory */
#else
    #include "qs_dummy.h"                /* QS/C dummy (inactive) interface */
#endif

/* QP API compatibility layer ----------------------------------------------*/
#if (QP_API_VERSION < 500)

/** \brief Deprecated macro for odd 8-bit CPUs. */
#define Q_ROM_VAR

/** \brief Deprecated call to the QHsm init operation */
#define QHsm_init(me_, e_)     QMSM_INIT((me_), (e_))

/** \brief Deprecated call to the QHsm dispatch operation */
#define QHsm_dispatch(me_, e_) QMSM_DISPATCH((me_), (e_))

/** \brief Deprecated call to the QFsm init operation */
#define QFsm_init(me_, e_)     QMSM_INIT((me_), (e_))

/** \brief Deprecated to the QFsm dispatch operation */
#define QFsm_dispatch(me_, e_) QMSM_DISPATCH((me_), (e_))

/** \brief Deprecated name of the QActive start operation */
#define QActive_start QActive_start_


#ifdef Q_SPY

    /** \brief deprecated call to QActive post FIFO operation */
    #define QActive_postFIFO(me_, e_, sender_) \
        QACTIVE_POST((me_), (e_), (sender_))

    /** \brief Deprecated call of QF system clock tick (for rate 0) */
    #define QF_tick(sender_)   QF_TICK_X((uint8_t)0, (sender_))

#else

    #define QActive_postFIFO(me_, e_) \
        QACTIVE_POST((me_), (e_), dummy)
    #define QF_tick()          QF_TICK_X((uint8_t)0, dummy)

#endif

/** \brief Deprecated time event constructor */
#define QTimeEvt_ctor(me_, sig_) \
    QTimeEvt_ctorX((me_), (QActive *)0, (sig_), (uint8_t)0)

/** \brief Deprecated time event one-shot arm operation */
#define QTimeEvt_postIn(me_, act_, nTicks_) do { \
    (me_)->act = (act_); \
    QTimeEvt_armX((me_), (nTicks_), (uint8_t)0); \
} while (0)

/** \brief Deprecated time event periodic arm operation */
#define QTimeEvt_postEvery(me_, act_, nTicks_) do { \
    (me_)->act = (act_); \
    QTimeEvt_armX((me_), (nTicks_), (nTicks_)); \
} while (0)

/** \brief Deprecated macro for generating QS-Reset trace record. */
#define QS_RESET() ((void)0)

/*..........................................................................*/
#if (QP_API_VERSION < 450)

/** \brief deprecated typedef for backwards compatibility */
typedef QEvt QEvent;

#endif                                              /* QP_API_VERSION < 450 */
#endif                                              /* QP_API_VERSION < 500 */
/*--------------------------------------------------------------------------*/

#endif                                                         /* qp_port_h */
