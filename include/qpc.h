/**
* @file
* @brief QP/C public interface including backwards-compatibility layer
* @ingroup qep qf qv qk qxk qs
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-30
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QPC_H
#define QPC_H

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

/****************************************************************************/
#ifndef QP_API_VERSION

/*! Specifies the backwards compatibility with the QP/C API version. */
/**
* @description
* For example, QP_API_VERSION=580 will cause generating the compatibility
* layer with QP/C version 5.8.0 and newer, but not older than 5.8.0.
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
#if (QP_API_VERSION < 691)

/*! @deprecated enable the QS global filter */
#define QS_FILTER_ON(rec_)        QS_GLB_FILTER((rec_))

/*! @deprecated disable the QS global filter */
#define QS_FILTER_OFF(rec_)       QS_GLB_FILTER(-(rec_))

/*! @deprecated enable the QS local filter for SM (state machine) object */
#define QS_FILTER_SM_OBJ(obj_)    ((void)0)

/*! @deprecated enable the QS local filter for AO (active objects) */
#define QS_FILTER_AO_OBJ(obj_)    ((void)0)

/*! @deprecated enable the QS local filter for MP (memory pool) object */
#define QS_FILTER_MP_OBJ(obj_)    ((void)0)

/*! @deprecated enable the QS local filter for EQ (event queue) object */
#define QS_FILTER_EQ_OBJ(obj_)    ((void)0)

/*! @deprecated enable the QS local filter for TE (time event) object */
#define QS_FILTER_TE_OBJ(obj_)    ((void)0)

#ifdef Q_SPY

/*! @deprecated local Filter for a generic application object @p obj_. */
#define QS_FILTER_AP_OBJ(obj_)  (QS_priv_.locFilter_AP = (obj_))

/*! @deprecated begin of a user QS record, instead use QS_BEGIN_ID() */
#define QS_BEGIN(rec_, obj_)                             \
    if (((QS_priv_.glbFilter[(uint_fast8_t)(rec_) >> 3U] \
          & (1U << ((uint_fast8_t)(rec_) & 7U))) != 0U)  \
        && ((QS_priv_.locFilter_AP == (void *)0)         \
            || (QS_priv_.locFilter_AP == (obj_))))       \
    {                                                    \
        QS_CRIT_STAT_                                    \
        QS_CRIT_E_();                                    \
        QS_beginRec_((uint_fast8_t)(rec_));              \
        QS_TIME_PRE_(); {

/*! @deprecated Output formatted uint32_t to the QS record */
#define QS_U32_HEX(width_, data_) \
    (QS_u32_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)0x0FU, (data_)))

#else

#define QS_FILTER_AP_OBJ(obj_)    ((void)0)
#define QS_BEGIN(rec_, obj_)      if (false) {
#define QS_U32_HEX(width_, data_) ((void)0)

#endif

/****************************************************************************/
#if (QP_API_VERSION < 660)

/*! @deprecated casting to QXThreadHandler
* instead use: the new signature of QXThreadHandler and don't cast */
#define Q_XTHREAD_CAST(handler_) ((QXThreadHandler)(handler_))

/****************************************************************************/
#if (QP_API_VERSION < 580)

/*! @deprecated call to the QMSM_INIT() operation; instead use: QHSM_INIT() */
#define QMSM_INIT(me_, e_)     QHSM_INIT((me_), (e_))

/*! @deprecated call to the QMSM_DISPATCH() operation;
* instead use: QHSM_DISPATCH() */
#define QMSM_DISPATCH(me_, e_) QHSM_DISPATCH((me_), (e_), 0U)

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

#endif /* QP_API_VERSION < 540 */
#endif /* QP_API_VERSION < 580 */
#endif /* QP_API_VERSION < 660 */
#endif /* QP_API_VERSION < 691 */

#ifdef __cplusplus
}
#endif

#endif /* QPC_H */

