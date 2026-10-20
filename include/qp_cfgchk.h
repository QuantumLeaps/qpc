//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License, version 3 or later (GPL-3.0-or-later), or,
// alternatively, under the terms of one of the closed-source Quantum Leaps
// commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
//
// NOTE:
// The GPL does NOT permit distributing this code as part of proprietary
// (closed-source) programs. For such use, please contact Quantum Leaps for
// a commercial license, which is an alternative to the GPL designed
// explicitly for closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#ifndef QP_CFGCHK_H_
#define QP_CFGCHK_H_

// QP configuration check (link-time)
//
// PURPOSE:
// Detect at link time a QP library (ports/kernels) built with a different
// configuration than the application, such as a different "qp_config.h",
// extra -D options, a Spy/non-Spy mix, or a different QP version.
//
// HOW IT WORKS:
// This header computes the macro QF_INIT_CFGCHK_, which expands to a name
// that encodes the QP configuration as seen by the code being compiled, e.g.:
// QF_816U_a32t01p03e4q1m2s00
//
// - QP_VERSION (3 digits ending with U) between "QF_" and "_"
// - a: QF_MAX_ACTIVE (2 digits)
// - t: QF_MAX_TICK_RATE (2 digits)
// - p: QF_MAX_EPOOL (2 digits)
// - e: QF_TIMEEVT_CTR_SIZE (1 digit)
// - q: QF_EQUEUE_CTR_SIZE (1 digit)
// - m: QF_MPOOL_CTR_SIZE (1 digit)
// - s: QS_TIME_SIZE and QS_CTR_SIZE with Q_SPY, 00 without Q_SPY
// - c: QF_ON_CONTEXT_SW (present when defined, absent when not)
// - u: Q_UNSAFE  (present when defined, absent when not)
// - x: Q_UTEST  (present when defined, absent when not)
//
// For example, the same configuration with Q_SPY, Q_UNSAFE, and Q_UTEST:
// QF_816U_a32t01p03e4q1m2s42ux
//
// The settings are normalized (e.g., 32, 32U, and (32U) give the same name).
// The QP port/kernel defines QF_INIT_CFGCHK_(), so it gets the name of
// the library's configuration. The inline QF_init() called by the
// application calls QF_INIT_CFGCHK_(), so it uses the name of the
// application's configuration. If the two configurations differ, the
// build fails at link time with an "undefined reference" to the
// QF_<version>_... name of the application's configuration.
//
// NOTE: This header must be included only from "qp.h", after the default
// configuration settings.

#ifndef QP_VERSION
    #error "qp_cfgchk.h" must be included only from "qp.h"
#endif

//! @cond INTERNAL

// QF_MAX_ACTIVE (1..64) as two decimal digits
#if   ((QF_MAX_ACTIVE / 10U) == 0U)
    #define QP_CFGCHK_A1_ 0
#elif ((QF_MAX_ACTIVE / 10U) == 1U)
    #define QP_CFGCHK_A1_ 1
#elif ((QF_MAX_ACTIVE / 10U) == 2U)
    #define QP_CFGCHK_A1_ 2
#elif ((QF_MAX_ACTIVE / 10U) == 3U)
    #define QP_CFGCHK_A1_ 3
#elif ((QF_MAX_ACTIVE / 10U) == 4U)
    #define QP_CFGCHK_A1_ 4
#elif ((QF_MAX_ACTIVE / 10U) == 5U)
    #define QP_CFGCHK_A1_ 5
#else
    #define QP_CFGCHK_A1_ 6
#endif

#if   ((QF_MAX_ACTIVE % 10U) == 0U)
    #define QP_CFGCHK_A0_ 0
#elif ((QF_MAX_ACTIVE % 10U) == 1U)
    #define QP_CFGCHK_A0_ 1
#elif ((QF_MAX_ACTIVE % 10U) == 2U)
    #define QP_CFGCHK_A0_ 2
#elif ((QF_MAX_ACTIVE % 10U) == 3U)
    #define QP_CFGCHK_A0_ 3
#elif ((QF_MAX_ACTIVE % 10U) == 4U)
    #define QP_CFGCHK_A0_ 4
#elif ((QF_MAX_ACTIVE % 10U) == 5U)
    #define QP_CFGCHK_A0_ 5
#elif ((QF_MAX_ACTIVE % 10U) == 6U)
    #define QP_CFGCHK_A0_ 6
#elif ((QF_MAX_ACTIVE % 10U) == 7U)
    #define QP_CFGCHK_A0_ 7
#elif ((QF_MAX_ACTIVE % 10U) == 8U)
    #define QP_CFGCHK_A0_ 8
#else
    #define QP_CFGCHK_A0_ 9
#endif

// QF_MAX_TICK_RATE (0..15) as two decimal digits
#if ((QF_MAX_TICK_RATE / 10U) == 0U)
    #define QP_CFGCHK_T1_ 0
#else
    #define QP_CFGCHK_T1_ 1
#endif

#if   ((QF_MAX_TICK_RATE % 10U) == 0U)
    #define QP_CFGCHK_T0_ 0
#elif ((QF_MAX_TICK_RATE % 10U) == 1U)
    #define QP_CFGCHK_T0_ 1
#elif ((QF_MAX_TICK_RATE % 10U) == 2U)
    #define QP_CFGCHK_T0_ 2
#elif ((QF_MAX_TICK_RATE % 10U) == 3U)
    #define QP_CFGCHK_T0_ 3
#elif ((QF_MAX_TICK_RATE % 10U) == 4U)
    #define QP_CFGCHK_T0_ 4
#elif ((QF_MAX_TICK_RATE % 10U) == 5U)
    #define QP_CFGCHK_T0_ 5
#elif ((QF_MAX_TICK_RATE % 10U) == 6U)
    #define QP_CFGCHK_T0_ 6
#elif ((QF_MAX_TICK_RATE % 10U) == 7U)
    #define QP_CFGCHK_T0_ 7
#elif ((QF_MAX_TICK_RATE % 10U) == 8U)
    #define QP_CFGCHK_T0_ 8
#else
    #define QP_CFGCHK_T0_ 9
#endif

// QF_MAX_EPOOL (0..15) as two decimal digits
#if ((QF_MAX_EPOOL / 10U) == 0U)
    #define QP_CFGCHK_P1_ 0
#else
    #define QP_CFGCHK_P1_ 1
#endif

#if   ((QF_MAX_EPOOL % 10U) == 0U)
    #define QP_CFGCHK_P0_ 0
#elif ((QF_MAX_EPOOL % 10U) == 1U)
    #define QP_CFGCHK_P0_ 1
#elif ((QF_MAX_EPOOL % 10U) == 2U)
    #define QP_CFGCHK_P0_ 2
#elif ((QF_MAX_EPOOL % 10U) == 3U)
    #define QP_CFGCHK_P0_ 3
#elif ((QF_MAX_EPOOL % 10U) == 4U)
    #define QP_CFGCHK_P0_ 4
#elif ((QF_MAX_EPOOL % 10U) == 5U)
    #define QP_CFGCHK_P0_ 5
#elif ((QF_MAX_EPOOL % 10U) == 6U)
    #define QP_CFGCHK_P0_ 6
#elif ((QF_MAX_EPOOL % 10U) == 7U)
    #define QP_CFGCHK_P0_ 7
#elif ((QF_MAX_EPOOL % 10U) == 8U)
    #define QP_CFGCHK_P0_ 8
#else
    #define QP_CFGCHK_P0_ 9
#endif

// QF_TIMEEVT_CTR_SIZE (2U or 4U, validated above)
#if (QF_TIMEEVT_CTR_SIZE == 2U)
    #define QP_CFGCHK_E_ 2
#else
    #define QP_CFGCHK_E_ 4
#endif

// QF_EQUEUE_CTR_SIZE (default must match "qequeue.h")
#ifndef QF_EQUEUE_CTR_SIZE
    #define QP_CFGCHK_Q_ 1
#elif (QF_EQUEUE_CTR_SIZE == 1U)
    #define QP_CFGCHK_Q_ 1
#elif (QF_EQUEUE_CTR_SIZE == 2U)
    #define QP_CFGCHK_Q_ 2
#else
    #error QF_EQUEUE_CTR_SIZE defined incorrectly, expected 1U or 2U
#endif

// QF_MPOOL_CTR_SIZE (default must match "qmpool.h")
#ifndef QF_MPOOL_CTR_SIZE
    #define QP_CFGCHK_M_ 2
#elif (QF_MPOOL_CTR_SIZE == 1U)
    #define QP_CFGCHK_M_ 1
#elif (QF_MPOOL_CTR_SIZE == 2U)
    #define QP_CFGCHK_M_ 2
#else
    #error QF_MPOOL_CTR_SIZE defined incorrectly, expected 1U or 2U
#endif

// Q_SPY with QS_TIME_SIZE and QS_CTR_SIZE (defaults must match "qs.h")
#ifndef Q_SPY
    #define QP_CFGCHK_S1_ 0
    #define QP_CFGCHK_S0_ 0
#else
    #ifndef QS_TIME_SIZE
        #define QP_CFGCHK_S1_ 4
    #elif (QS_TIME_SIZE == 1U)
        #define QP_CFGCHK_S1_ 1
    #elif (QS_TIME_SIZE == 2U)
        #define QP_CFGCHK_S1_ 2
    #elif (QS_TIME_SIZE == 4U)
        #define QP_CFGCHK_S1_ 4
    #else
        #error QS_TIME_SIZE defined incorrectly, expected 1U, 2U, or 4U
    #endif
    #ifndef QS_CTR_SIZE
        #define QP_CFGCHK_S0_ 2
    #elif (QS_CTR_SIZE == 1U)
        #define QP_CFGCHK_S0_ 1
    #elif (QS_CTR_SIZE == 2U)
        #define QP_CFGCHK_S0_ 2
    #elif (QS_CTR_SIZE == 4U)
        #define QP_CFGCHK_S0_ 4
    #else
        #error QS_CTR_SIZE defined incorrectly, expected 1U, 2U, or 4U
    #endif
#endif // Q_SPY

// NOTE: The name is built by pasting one token at a time. QP_CFGCHK_CAT_() is
// the only macro with the ## operator, and it has only one ## operator, so
// the order of the pastes is fully specified (no unspecified behavior).
// The field tags (a, t, p, ...) are literal operands of ##, so they are
// never macro-expanded, even if the application defines such macros.

// pastes the token t_ to the (already expanded) name n_
#define QP_CFGCHK_CAT_(n_, t_) n_ ## t_

// expands the argument d_ (a digit), then pastes it to the name n_
#define QP_CFGCHK_XCAT_(n_, d_) QP_CFGCHK_CAT_(n_, d_)

// appends two digits to the name n_
#define QP_CFGCHK_D2_(n_, d1_, d0_) \
    QP_CFGCHK_XCAT_(QP_CFGCHK_XCAT_(n_, d1_), d0_)

// prefix "QF_" followed by the QP version, e.g.: QF_816U
#define QP_CFGCHK_V_(v_)  QP_CFGCHK_CAT_(QF_, v_)

// field tags
#define QP_CFGCHK_TA_(n_) QP_CFGCHK_CAT_(n_, _a)
#define QP_CFGCHK_TT_(n_) QP_CFGCHK_CAT_(n_, t)
#define QP_CFGCHK_TP_(n_) QP_CFGCHK_CAT_(n_, p)
#define QP_CFGCHK_TE_(n_) QP_CFGCHK_CAT_(n_, e)
#define QP_CFGCHK_TQ_(n_) QP_CFGCHK_CAT_(n_, q)
#define QP_CFGCHK_TM_(n_) QP_CFGCHK_CAT_(n_, m)
#define QP_CFGCHK_TS_(n_) QP_CFGCHK_CAT_(n_, s)

// QF_ON_CONTEXT_SW, Q_UNSAFE, Q_UTEST: one-letter flags appended to the
// name (in this order), present when defined, absent when not
#ifdef QF_ON_CONTEXT_SW
    #define QP_CFGCHK_FC_(n_) QP_CFGCHK_CAT_(n_, c)
#else
    #define QP_CFGCHK_FC_(n_) n_
#endif

#ifdef Q_UNSAFE
    #define QP_CFGCHK_FU_(n_) QP_CFGCHK_CAT_(n_, u)
#else
    #define QP_CFGCHK_FU_(n_) n_
#endif

#ifdef Q_UTEST
    #define QP_CFGCHK_FX_(n_) QP_CFGCHK_CAT_(n_, x)
#else
    #define QP_CFGCHK_FX_(n_) n_
#endif

// the name built field by field, e.g.: QF_816U_a32t01p03e4q1m2s00
#define QP_CFGCHK_N1_ QP_CFGCHK_V_(QP_VERSION)
#define QP_CFGCHK_N2_ QP_CFGCHK_D2_(QP_CFGCHK_TA_(QP_CFGCHK_N1_), \
    QP_CFGCHK_A1_, QP_CFGCHK_A0_)
#define QP_CFGCHK_N3_ QP_CFGCHK_D2_(QP_CFGCHK_TT_(QP_CFGCHK_N2_), \
    QP_CFGCHK_T1_, QP_CFGCHK_T0_)
#define QP_CFGCHK_N4_ QP_CFGCHK_D2_(QP_CFGCHK_TP_(QP_CFGCHK_N3_), \
    QP_CFGCHK_P1_, QP_CFGCHK_P0_)
#define QP_CFGCHK_N5_ QP_CFGCHK_XCAT_(QP_CFGCHK_TE_(QP_CFGCHK_N4_), \
    QP_CFGCHK_E_)
#define QP_CFGCHK_N6_ QP_CFGCHK_XCAT_(QP_CFGCHK_TQ_(QP_CFGCHK_N5_), \
    QP_CFGCHK_Q_)
#define QP_CFGCHK_N7_ QP_CFGCHK_XCAT_(QP_CFGCHK_TM_(QP_CFGCHK_N6_), \
    QP_CFGCHK_M_)
#define QP_CFGCHK_N8_ QP_CFGCHK_D2_(QP_CFGCHK_TS_(QP_CFGCHK_N7_), \
    QP_CFGCHK_S1_, QP_CFGCHK_S0_)

// configuration-specific name of the QF_INIT_CFGCHK_() function, e.g.:
// QF_816U_a32t01p03e4q1m2s00
#define QF_INIT_CFGCHK_ \
    QP_CFGCHK_FX_(QP_CFGCHK_FU_(QP_CFGCHK_FC_(QP_CFGCHK_N8_)))

//! @endcond

#endif // QP_CFGCHK_H_
