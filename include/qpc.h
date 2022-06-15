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
* @brief QP/C public interface including backwards-compatibility layer
* @description
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP/C.
*/
#ifndef QPC_H
#define QPC_H

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
#include "qf_port.h"      /* QF/C port from the port directory */
#include "qassert.h"      /* QP embedded systems-friendly assertions */

#ifdef Q_SPY /* software tracing enabled? */
    #include "qs_port.h"  /* QS/C port from the port directory */
#else
    #include "qs_dummy.h" /* QS/C dummy (inactive) interface */
#endif

/*==========================================================================*/

#ifndef QP_API_VERSION

/*! Specifies the backwards compatibility with the QP/C API version.
* @description
* For example, QP_API_VERSION==691 will cause generating the compatibility
* layer with QP/C version 6.9.1 and newer, but not older than 6.9.1.
* QP_API_VERSION==0 causes generation of the maximum currently supported
* backwards compatibility. This is the default.<br>
* <br>
* Conversely, QP_API_VERSION==9999 means that no compatibility layer should
* be generated. This setting is useful for checking if an application
* complies with the latest QP/C API.
*/
#define QP_API_VERSION 0

#endif /* #ifndef QP_API_VERSION */

/*==========================================================================*/
/* QP API compatibility layer... */

#if (QP_API_VERSION < 700)

/*! @deprecated plain 'char' is no longer forbidden in MISRA-C 2012 */
typedef char char_t;

/*==========================================================================*/
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

/*==========================================================================*/
#if (QP_API_VERSION < 660)

/*! @deprecated casting to QXThreadHandler
* instead use: the new signature of QXThreadHandler and don't cast
*/
#define Q_XTHREAD_CAST(handler_) ((QXThreadHandler)(handler_))

/*==========================================================================*/
#if (QP_API_VERSION < 580)

/*! @deprecated call to the QMSM_INIT() operation; instead use: QHSM_INIT() */
#define QMSM_INIT(me_, e_)     QHSM_INIT((me_), (e_))

/*! @deprecated call to the QMSM_DISPATCH() operation;
* instead use: QHSM_DISPATCH() */
#define QMSM_DISPATCH(me_, e_) QHSM_DISPATCH((me_), (e_), 0U)

/*==========================================================================*/
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
#endif /* QP_API_VERSION < 700 */

#ifdef __cplusplus
}
#endif

#endif /* QPC_H */
