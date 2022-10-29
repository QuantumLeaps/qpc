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
* @date Last updated on: 2021-12-30
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QP native, platform-independent priority sets of 32 or 64 elements.
* @ingroup qf
*/
#ifndef QPSET_H
#define QPSET_H

#ifndef QF_MAX_ACTIVE
    /* default value when NOT defined */
    #define QF_MAX_ACTIVE 32U
#endif

#if (QF_MAX_ACTIVE < 1U) || (64U < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE out of range. Valid range is 1U..64U"
#elif (QF_MAX_ACTIVE <= 8U)
    typedef uint8_t QPSetBits;
#elif (QF_MAX_ACTIVE <= 16U)
    typedef uint16_t QPSetBits;
#else
    /*! bitmask for the internal representation of QPSet elements */
    typedef uint32_t QPSetBits;
#endif

/*==========================================================================*/
/* Log-base-2 calculations ...*/
#ifndef QF_LOG2
    uint_fast8_t QF_LOG2(QPSetBits x);
#endif /* QF_LOG2 */

#if (QF_MAX_ACTIVE <= 32U)

/*==========================================================================*/
/*! Priority Set of up to 32 elements
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 32 priority levels.
*/
typedef struct {
    QPSetBits volatile bits;  /*!< bitmask with a bit for each element */
} QPSet;

/*! Makes the priority set @p me empty */
static inline void QPSet_setEmpty(QPSet * const me) {
    me->bits = 0U;
}

/*! Evaluates to TRUE if the priority set @p me is empty */
static inline bool QPSet_isEmpty(QPSet * const me) {
    return me->bits == 0U;
}

/*! Evaluates to TRUE if the priority set @p me is not empty */
static inline bool QPSet_notEmpty(QPSet * const me) {
    return me->bits != 0U;
}

/*! Evaluates to TRUE if the priority set @p me has element @p n */
static inline bool QPSet_hasElement(QPSet * const me, uint_fast8_t const n) {
    return (me->bits & ((QPSetBits)1 << (n - 1U))) != 0U;
}

/*! Insert element @p n_ into the set @p me, n = 1U..32U */
static inline void QPSet_insert(QPSet * const me, uint_fast8_t const n) {
    me->bits |= ((QPSetBits)1 << (n - 1U));
}

/*! Remove element n_ from the set @p me, n = 1U..32U */
static inline void QPSet_remove(QPSet * const me, uint_fast8_t const n) {
    me->bits &= (QPSetBits)(~((QPSetBits)1 << (n - 1U)));
}

/*! Find the maximum element in the set @p me
* @note if the set @p me is empty, the function returns zero.
*/
static inline uint_fast8_t QPSet_findMax(QPSet * const me) {
    return QF_LOG2(me->bits);
}

#else /* QF_MAX_ACTIVE > 32U */

/*==========================================================================*/
/*! Priority Set of up to 64 elements
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 64 priority levels.
*/
typedef struct {
    uint32_t volatile bits[2];
} QPSet;

/*! Makes the priority set @p me_ empty */
static inline void QPSet_setEmpty(QPSet * const me) {
    me->bits[0] = 0U;
    me->bits[1] = 0U;
}

/*! Evaluates to TRUE if the priority set @p me is empty */
static inline bool QPSet_isEmpty(QPSet * const me) {
    /* the following logic avoids UB in volatile access for MISRA */
    return (me->bits[0] == 0U)
           ? (me->bits[1] == 0U)
           : false;
}

/*! Evaluates to TRUE if the priority set @p me is not empty */
static inline bool QPSet_notEmpty(QPSet * const me) {
    /* the following logic avoids UB in volatile access for MISRA */
    return (me->bits[0] != 0U)
           ? true
           : (me->bits[1] != 0U);
}

/*! Evaluates to TRUE if the priority set @p me has element @p n. */
static inline bool QPSet_hasElement(QPSet * const me, uint_fast8_t const n) {
    return (n <= 32U)
            ? ((me->bits[0] & ((uint32_t)1 << (n - 1U ))) != 0U)
            : ((me->bits[1] & ((uint32_t)1 << (n - 33U))) != 0U);
}

/*! insert element @p n into the set @p me, n = 1..64 */
static inline void QPSet_insert(QPSet * const me, uint_fast8_t const n) {
    if (n <= 32U) {
        me->bits[0] |= ((uint32_t)1 << (n - 1U ));
    }
    else {
        me->bits[1] |= ((uint32_t)1 << (n - 33U));
    }
}

/*! Remove element n from the set @p me, n_= 1..64 */
static inline void QPSet_remove(QPSet * const me, uint_fast8_t const n) {
    if (n <= 32U) {
        me->bits[0] &= (uint32_t)(~((uint32_t)1 << (n - 1U )));
    }
    else {
        me->bits[1] &= (uint32_t)(~((uint32_t)1 << (n - 33U)));
    }
}

/*! Find the maximum element in the set @p me
* @note if the set @p me is empty, the function returns zero.
*/
static inline uint_fast8_t QPSet_findMax(QPSet * const me) {
    return (me->bits[1] != 0U)
           ? (QF_LOG2(me->bits[1]) + 32U)
           : QF_LOG2(me->bits[0]);
}

#endif /* QF_MAX_ACTIVE */

#endif /* QPSET_H */
