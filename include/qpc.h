/**
* @file
* @brief QP/C public interface including backwards-compatibility layer
* @ingroup qep qf qv qk qxk qs
* @cond
******************************************************************************
* Last updated for version 6.4.0
* Last updated on  2019-02-13
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
#ifndef qpc_h
#define qpc_h

/**
* @description
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP/C.
*/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
#include "qf_port.h"      /* QF/C port from the port directory */
#include "qassert.h"      /* QP embedded systems-friendly assertions */

#ifdef Q_SPY /* software tracing enabled? */
    #include "qs_port.h"  /* QS/C port from the port directory */
#else
    #include "qs_dummy.h" /* QS/C dummy (inactive) interface */
#endif

/*! Symbols for time-stamping the application build */
/**
* @description
* The exernal symbols Q_APP_BUILD_DATE and Q_APP_BUILD_TIME are defined in
* the module qstamp.c. This module needs to be re-compiled for every software
* build.
*/
extern char_t const Q_APP_BUILD_DATE[12];
extern char_t const Q_APP_BUILD_TIME[9];


/****************************************************************************/
#ifndef QP_API_VERSION

/*! Specifies the backwards compatibility with the QP/C API version. */
/**
* @description
* For example, QP_API_VERSION=540 will cause generating the compatibility
* layer with QP/C version 5.4.0 and newer, but not older than 5.4.0.
* QP_API_VERSION=0 causes generation of the maximum currently supported
* backwards compatibilty. This is the default.@n
* @n
* Conversely, QP_API_VERSION=9999 means that no compatibility layer should
* be generated. This setting is useful for checking if an application
* complies with the latest QP/C API.
*/
#define QP_API_VERSION 0

#endif /* #ifndef QP_API_VERSION */

/* QP API compatibility layer... */

/****************************************************************************/
#if (QP_API_VERSION < 580)

/*! @deprecated call to the QMSM_INIT() operation; instead use: QHSM_INIT() */
#define QMSM_INIT(me_, e_)     QHSM_INIT((me_), (e_))

/*! @deprecated call to the QMSM_DISPATCH() operation;
* instead use: QHSM_DISPATCH() */
#define QMSM_DISPATCH(me_, e_) QHSM_DISPATCH((me_), (e_))

/****************************************************************************/
#if (QP_API_VERSION < 540)

/*! @deprecated QFsm state machine;
* instead use: ::QHsm. Legacy state machines coded in the "QFsm-style" will
* continue to work, but will use the ::QHsm implementation internally.
* There is no longer any efficiency advantage in using the "QFsm-style"
* state machines.
*
* @note
* For efficiency, the recommended migration path is to use the ::QMsm
* state machine and the QM modeling tool.
*/
typedef QHsm        QFsm;
/*! @deprecated QFsm state machine constructor; instead use: QHsm_ctor() */
#define QFsm_ctor   QHsm_ctor

/*! deprecated macro to call in QFsm state-handler when it
* ignores (does not handle) an event (instead use Q_SUPER())
*/
#define Q_IGNORED() (Q_SUPER(&QHsm_top))

/*! @deprecated macro for cooperativew "Vanilla" kernel;
* instead use: QV_onIdle() */
#define QF_onIdle   QV_onIdle

/****************************************************************************/
#if (QP_API_VERSION < 500)

/*! @deprecated macro for odd 8-bit CPUs. */
#define Q_ROM

/*! @deprecated macro for odd 8-bit CPUs. */
#define Q_ROM_BYTE(rom_var_)   (rom_var_)

/*! @deprecated macro for odd 8-bit CPUs. */
#define Q_ROM_VAR

/*! @deprecated call to the QMsm init operation; instead use: QHSM_INIT() */
#define QMsm_init(me_, e_)     QHSM_INIT((me_), (e_))

/*! @deprecated call to the QMsm dispatch operation;
* instead use: QHSM_DISPATCH() */
#define QMsm_dispatch(me_, e_) QHSM_DISPATCH((me_), (e_))

/*! @deprecated call to the QFsm init operation; instead use: QHSM_INIT()  */
#define QFsm_init(me_, e_)     QHSM_INIT((me_), (e_))

/*! @deprecated to the QFsm dispatch operation;
* instead use: QHSM_DISPATCH() */
#define QFsm_dispatch(me_, e_) QHSM_DISPATCH((me_), (e_))

/*! @deprecated interface defined for backwards compatibility;
* instead use: QEQueue_post()  */
#define QEQueue_postFIFO(me_, e_) \
    ((void)QEQueue_post((me_), (e_), QF_NO_MARGIN))

/*! @deprecated name of the QActive start operation;
* instead use: QACTIVE_START() */

#ifdef Q_SPY

    /*! @deprecated call to QActive post FIFO operation;
    * instead use: QACTIVE_POST() */
    #define QActive_postFIFO(me_, e_, sender_) \
        QACTIVE_POST((me_), (e_), (sender_))

    /*! @deprecated call of QF system clock tick (for rate 0);
    * instead use: QF_TICK() */
    #define QF_tick(sender_)   QF_TICK((sender_))

#else

    #define QActive_postFIFO(me_, e_) \
        QACTIVE_POST((me_), (e_), dummy)
    #define QF_tick()          QF_TICK(dummy)

#endif

/*! @deprecated time event constructor; instead use: QTimeEvt_ctorX() */
#define QTimeEvt_ctor(me_, sig_) \
    QTimeEvt_ctorX((me_), (QActive *)0, (sig_), (uint8_t)0)

/*! @deprecated time event one-shot arm operation;
* instead use: QTimeEvt_armX() */
#define QTimeEvt_postIn(me_, act_, nTicks_) do { \
    (me_)->act = (act_); \
    QTimeEvt_armX((me_), (nTicks_), (uint8_t)0); \
} while (0)

/*! @deprecated time event periodic arm operation;
* instead use: QTimeEvt_armX() */
#define QTimeEvt_postEvery(me_, act_, nTicks_) do { \
    (me_)->act = (act_); \
    QTimeEvt_armX((me_), (nTicks_), (nTicks_)); \
} while (0)

/*! @deprecated macro for generating QS-Reset trace record. */
#define QS_RESET() ((void)0)


/****************************************************************************/
#if (QP_API_VERSION < 450)

/*! @deprecated typedef for backwards compatibility; instead use ::QEvt */
typedef QEvt QEvent;

#endif /* QP_API_VERSION < 450 */
#endif /* QP_API_VERSION < 500 */
#endif /* QP_API_VERSION < 540 */
#endif /* QP_API_VERSION < 580 */

#ifdef __cplusplus
}
#endif

#endif /* qpc_h */

