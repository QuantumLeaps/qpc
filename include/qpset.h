/**
* @file
* @brief QP native, platform-independent priority sets of 32 or 64 elements.
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.3.8
* Last updated on  2019-01-10
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
#ifndef qpset_h
#define qpset_h

#if (!defined QF_MAX_ACTIVE) || (QF_MAX_ACTIVE < 1) || (64 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..64"
#elif (QF_MAX_ACTIVE <= 8)
    typedef uint8_t QPSetBits;
#elif (QF_MAX_ACTIVE <= 16)
    typedef uint16_t QPSetBits;
#else
    /*! bitmask for the internal representation of QPSet elements */
    typedef uint32_t QPSetBits;
#endif

#if (QF_MAX_ACTIVE <= 32)

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
#define QPSet_setEmpty(me_) ((me_)->bits = (QPSetBits)0)

/*! Evaluates to TRUE if the priority set @p me_ is empty */
#define QPSet_isEmpty(me_) ((me_)->bits == (QPSetBits)0)

/*! Evaluates to TRUE if the priority set @p me_ is not empty */
#define QPSet_notEmpty(me_) ((me_)->bits != (QPSetBits)0)

/*! Evaluates to TRUE if the priority set @p me_ has element @p n_ */
#define QPSet_hasElement(me_, n_) \
    (((me_)->bits & ((QPSetBits)1 << ((n_)-(uint_fast8_t)1))) != (QPSetBits)0)

/*! Insert element @p n_ into the set @p me_, n_= 1..32 */
#define QPSet_insert(me_, n_) \
    ((me_)->bits |= (QPSetBits)((QPSetBits)1 << ((n_) - (uint_fast8_t)1)))

/*! Remove element n_ from the set @p me_, n_= 1..32 */
#define QPSet_remove(me_, n_) \
    ((me_)->bits &= (QPSetBits)(~((QPSetBits)1 << ((n_) - (uint_fast8_t)1))))

/*! Find the maximum element in the set, and assign it to n_ */
/** @note if the set @p me_ is empty, @p n_ is set to zero.
*/
#define QPSet_findMax(me_, n_) \
    ((n_) = QF_LOG2((me_)->bits))

#else /* QF_MAX_ACTIVE > 32 */

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
#define QPSet_setEmpty(me_)  do { \
    (me_)->bits[0] = (uint32_t)0; \
    (me_)->bits[1] = (uint32_t)0; \
} while (0)

/*! Evaluates to TRUE if the priority set @p me_ is empty */
/* the following logic avoids UB in volatile access for MISRA compliantce */
#define QPSet_isEmpty(me_) \
    (((me_)->bits[0] == (uint32_t)0) \
     ? ((me_)->bits[1] == (uint32_t)0) \
     : 0)

/*! Evaluates to TRUE if the priority set @p me_ is not empty */
/* the following logic avoids UB in volatile access for MISRA compliantce */
#define QPSet_notEmpty(me_) \
    (((me_)->bits[0] != (uint32_t)0) \
     ? 1 \
     : ((me_)->bits[1] != (uint32_t)0))

/*! Evaluates to TRUE if the priority set @p me_ has element @p n_. */
#define QPSet_hasElement(me_, n_) \
    (((n_) <= (uint_fast8_t)32) \
     ? (((me_)->bits[0] & ((uint32_t)1 << ((n_) - (uint_fast8_t)1 ))) \
       != (uint32_t)0) \
     : (((me_)->bits[1] & ((uint32_t)1 << ((n_) - (uint_fast8_t)33))) \
       != (uint32_t)0))

/*! insert element @p n_ into the set @p me_, n_ = 1..64 */
#define QPSet_insert(me_, n_) do { \
    if ((n_) <= (uint_fast8_t)32) { \
        ((me_)->bits[0] |= ((uint32_t)1 << ((n_) - (uint_fast8_t)1 ))); \
    } \
    else { \
        ((me_)->bits[1] |= ((uint32_t)1 << ((n_) - (uint_fast8_t)33))); \
    } \
} while (0)

/*! Remove element n_ from the set @p me_, n_= 1..64 */
#define QPSet_remove(me_, n_) do { \
    if ((n_) <= (uint_fast8_t)32) { \
        ((me_)->bits[0] &= \
            (uint32_t)(~((uint32_t)1 << ((n_) - (uint_fast8_t)1 )))); \
    } \
    else { \
        ((me_)->bits[1] &= \
            (uint32_t)(~((uint32_t)1 << ((n_) - (uint_fast8_t)33)))); \
    } \
} while (0)

/*! Find the maximum element in the set, and assign it to @p n_ */
/** @note if the set @p me_ is empty, @p n_ is set to zero.
*/
#define QPSet_findMax(me_, n_) \
    ((n_) = ((me_)->bits[1] != (uint32_t)0) \
        ? (QF_LOG2((me_)->bits[1]) + (uint_fast8_t)32) \
        : (QF_LOG2((me_)->bits[0])))

#endif /* QF_MAX_ACTIVE */


/****************************************************************************/
/* Log-base-2 calculations ...*/
#ifndef QF_LOG2
    uint_fast8_t QF_LOG2(QPSetBits x);
#endif /* QF_LOG2 */

#endif /* qpset_h */
