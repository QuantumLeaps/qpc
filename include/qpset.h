/**
* @file
* @brief QP native, platform-independent priority sets of 32 or 64 elements.
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.1.0
* Last updated on  2018-02-12
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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

#if (QF_MAX_ACTIVE < 1) || (64 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..64"
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
    uint32_t volatile bits;  /*!< bitmask with a bit for each element */
} QPSet;

/*! Makes the priority set @p me_ empty */
#define QPSet_setEmpty(me_) ((me_)->bits = (uint32_t)0)

/*! Evaluates to TRUE if the priority set @p me_ is empty */
#define QPSet_isEmpty(me_) ((me_)->bits == (uint32_t)0)

/*! Evaluates to TRUE if the priority set @p me_ is not empty */
#define QPSet_notEmpty(me_) ((me_)->bits != (uint32_t)0)

/*! Evaluates to TRUE if the priority set @p me_ has element @p n_ */
#define QPSet_hasElement(me_, n_) \
    (((me_)->bits & ((uint32_t)1 << ((n_) - (uint_fast8_t)1))) != (uint32_t)0)

/*! Insert element @p n_ into the set @p me_, n_= 1..32 */
#define QPSet_insert(me_, n_) \
    ((me_)->bits |= (uint32_t)((uint32_t)1 << ((n_) - (uint_fast8_t)1)))

/*! Remove element n_ from the set @p me_, n_= 1..32 */
#define QPSet_remove(me_, n_) \
    ((me_)->bits &= (uint32_t)(~((uint32_t)1U << ((n_) - (uint_fast8_t)1))))

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

/* is this compiler C99 or newer? */
#if (defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

    /*! function that returns (log2(x_) + 1), where @p x_ is uint32_t */
    /**
    * @description
    * This function returns the number of the most significant 1-bit of a
    * 32-bit number. This function can be replaced in the QP ports, if the
    * CPU supports special instructions, such as CLZ (count leading zeros).
    */
    static inline uint_fast8_t QF_LOG2(uint32_t x) {
        static uint8_t const log2LUT[16] = {
            (uint8_t)0, (uint8_t)1, (uint8_t)2, (uint8_t)2,
            (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
            (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
            (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4
        };
        uint_fast8_t n = (uint_fast8_t)0;
        uint32_t     t = (x >> 16);

        if (t != (uint32_t)0) {
            n += 16;
            x = t;
        }
        t = (x >> 8);
        if (t != (uint32_t)0) {
            n += 8;
            x = t;
        }
        t = (x >> 4);
        if (t != (uint32_t)0) {
            n += 4;
            x = t;
        }
        return n + (uint_fast8_t)log2LUT[x];
    }

#else /* older C compiler */

    /* out-of-line definition */
    uint_fast8_t QF_LOG2(uint32_t x);

#endif /* __STDC_VERSION__ */

#endif /* QF_LOG2 */

#endif /* qpset_h */
