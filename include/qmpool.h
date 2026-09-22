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
#ifndef QMPOOL_H_
#define QMPOOL_H_

#ifndef QF_MPOOL_CTR_SIZE
    #define QF_MPOOL_CTR_SIZE 2U
#endif

#define QF_MPOOL_EL(evType_) struct { \
    void * sto_[((sizeof(evType_) - 1U) / sizeof(void *)) + \
                    (sizeof(evType_) < (2U * sizeof(void *)) ? 2U : 1U)]; }

//============================================================================
typedef uint16_t QMPoolSize;

#if (QF_MPOOL_CTR_SIZE == 1U)
    typedef uint8_t QMPoolCtr;
#elif (QF_MPOOL_CTR_SIZE == 2U)
    typedef uint16_t QMPoolCtr;
#else
    #error QF_MPOOL_CTR_SIZE defined incorrectly, expected 1U or 2U
#endif

//============================================================================
//! @class QMPool
typedef struct {
    void * *start;          //!< @private @memberof QMPool
    void * *end;            //!< @private @memberof QMPool
    void * *freeHead;       //!< @private @memberof QMPool
    QMPoolSize blockSize;   //!< @private @memberof QMPool
    QMPoolCtr nTot;         //!< @private @memberof QMPool
    QMPoolCtr nFree;        //!< @private @memberof QMPool
    QMPoolCtr nMin;         //!< @private @memberof QMPool
} QMPool;

//! @public @memberof QMPool
void QMPool_init(QMPool * const me,
    void * const poolSto,
    uint_fast32_t const poolSize,
    uint_fast16_t const blockSize);

//! @public @memberof QMPool
void * QMPool_get(QMPool * const me,
    uint_fast16_t const margin,
    uint_fast8_t const qsId);

//! @public @memberof QMPool
void QMPool_put(QMPool * const me,
    void * const block,
    uint_fast8_t const qsId);

//! @public @memberof QMPool
uint16_t QMPool_getUse(QMPool const * const me);

//! @public @memberof QMPool
uint16_t QMPool_getFree(QMPool const * const me);

//! @public @memberof QMPool
uint16_t QMPool_getMin(QMPool const * const me);

//! @public @memberof QMPool
QMPoolSize QMPool_getBlockSize(QMPool const * const me);

#endif // QMPOOL_H_
