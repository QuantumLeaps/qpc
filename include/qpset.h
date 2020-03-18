/**
* @file
* @brief QP native, platform-independent priority sets of 32 or 64 elements.
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-01-21
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

#if (QF_MAX_ACTIVE <= 32U)

/****************************************************************************/
/*! Priority Set of up to 32 elements */
/**
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 32 priority levels.
*/
typedef struct {
    QPSetBits volatile bits;  /*!< bitmask with a bit for each element */
} QPSet;

/*! Makes the priority set @p me_ empty */
#define QPSet_setEmpty(me_) ((me_)->bits = 0U)

/*! Evaluates to TRUE if the priority set @p me_ is empty */
#define QPSet_isEmpty(me_) ((me_)->bits == 0U)

/*! Evaluates to TRUE if the priority set @p me_ is not empty */
#define QPSet_notEmpty(me_) ((me_)->bits != 0U)

/*! Evaluates to TRUE if the priority set @p me_ has element @p n_ */
#define QPSet_hasElement(me_, n_) \
    (((me_)->bits & ((QPSetBits)1 << ((n_) - 1U))) != 0U)

/*! Insert element @p n_ into the set @p me_, n_= 1U..32U */
#define QPSet_insert(me_, n_) \
    ((me_)->bits |= ((QPSetBits)1 << ((n_) - 1U)))

/*! Remove element n_ from the set @p me_, n_= 1..32 */
#define QPSet_remove(me_, n_) \
    ((me_)->bits &= (QPSetBits)(~((QPSetBits)1 << ((n_) - 1U))))

/*! Find the maximum element in the set, and assign it to n_ */
/** @note if the set @p me_ is empty, @p n_ is set to zero.
*/
#define QPSet_findMax(me_, n_) \
    ((n_) = QF_LOG2((me_)->bits))

#else /* QF_MAX_ACTIVE > 32U */

/****************************************************************************/
/*! Priority Set of up to 64 elements */
/**
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 64 priority levels.
*/
typedef struct {
    uint32_t volatile bits[2];
} QPSet;

/*! Makes the priority set @p me_ empty */
#define QPSet_setEmpty(me_) do { \
    (me_)->bits[0] = 0U;         \
    (me_)->bits[1] = 0U;         \
} while (false)

/*! Evaluates to TRUE if the priority set @p me_ is empty */
/* the following logic avoids UB in volatile access for MISRA compliantce */
#define QPSet_isEmpty(me_)    \
    (((me_)->bits[0] == 0U)   \
     ? ((me_)->bits[1] == 0U) \
     : false)

/*! Evaluates to TRUE if the priority set @p me_ is not empty */
/* the following logic avoids UB in volatile access for MISRA compliantce */
#define QPSet_notEmpty(me_)   \
    (((me_)->bits[0] != 0U)   \
     ? true                   \
     : ((me_)->bits[1] != 0U))

/*! Evaluates to TRUE if the priority set @p me_ has element @p n_. */
#define QPSet_hasElement(me_, n_)                               \
    (((n_) <= 32U)                                              \
     ? (((me_)->bits[0] & ((uint32_t)1 << ((n_) - 1U ))) != 0U) \
     : (((me_)->bits[1] & ((uint32_t)1 << ((n_) - 33U))) != 0U))

/*! insert element @p n_ into the set @p me_, n_ = 1..64 */
#define QPSet_insert(me_, n_) do {                         \
    if ((n_) <= 32U) {                                     \
        ((me_)->bits[0] |= ((uint32_t)1 << ((n_) - 1U ))); \
    }                                                      \
    else {                                                 \
        ((me_)->bits[1] |= ((uint32_t)1 << ((n_) - 33U))); \
    }                                                      \
} while (false)

/*! Remove element n_ from the set @p me_, n_= 1..64 */
#define QPSet_remove(me_, n_) do {                                      \
    if ((n_) <= 32U) {                                                  \
        ((me_)->bits[0] &= (uint32_t)(~((uint32_t)1 << ((n_) - 1U )))); \
    }                                                                   \
    else {                                                              \
        ((me_)->bits[1] &= (uint32_t)(~((uint32_t)1 << ((n_) - 33U)))); \
    }                                                                   \
} while (false)

/*! Find the maximum element in the set, and assign it to @p n_ */
/** @note if the set @p me_ is empty, @p n_ is set to zero.
*/
#define QPSet_findMax(me_, n_)            \
    ((n_) = ((me_)->bits[1] != 0U)        \
        ? (QF_LOG2((me_)->bits[1]) + 32U) \
        : (QF_LOG2((me_)->bits[0])))

#endif /* QF_MAX_ACTIVE */


/****************************************************************************/
/* Log-base-2 calculations ...*/
#ifndef QF_LOG2
    uint_fast8_t QF_LOG2(QPSetBits x);
#endif /* QF_LOG2 */

#endif /* QPSET_H */
