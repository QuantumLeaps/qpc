/**
* \file
* \brief platform-independent priority sets of 8 or 64 elements.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#ifndef qpset_h
#define qpset_h

/****************************************************************************/
/*! Priority Set of up to 8 elements for building various schedulers */
/**
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 8 priority levels.
*
* The priority set allows to build cooperative multitasking schedulers
* to manage up to 8 tasks.
*/
typedef struct {
    uint_fast8_t volatile bits;  /*!< bitmask with a bit for each element */
} QPSet8;

/*! Evaluates to TRUE if the priority set \c me_ has elements */
#define QPSet8_isEmpty(me_) ((me_)->bits == (uint_fast8_t)0)

/*! Evaluates to TRUE if the priority set \c me_ is empty */
#define QPSet8_notEmpty(me_) ((me_)->bits != (uint_fast8_t)0)

/*! Evaluates to TRUE if the priority set \c me_ has element \c n_ */
#define QPSet8_hasElement(me_, n_) \
    (((me_)->bits & (uint_fast8_t)Q_ROM_BYTE(QF_pwr2Lkup[(n_)])) \
    != (uint_fast8_t)0)

/*! Insert element \c n_ into the set \c me_, n_= 1..8 */
#define QPSet8_insert(me_, n_) \
    ((me_)->bits |= (uint_fast8_t)Q_ROM_BYTE(QF_pwr2Lkup[(n_)]))

/*! Remove element n_ from the set \c me_, n_= 1..8 */
#define QPSet8_remove(me_, n_) \
    ((me_)->bits &= (uint_fast8_t)Q_ROM_BYTE(QF_invPwr2Lkup[(n_)]))

/*! Find the maximum element in the set, and assign it to n_ */
/** \note if the set \c me_ is empty, \c n_ is set to zero.
*/
#define QPSet8_findMax(me_, n_) \
    ((n_) = (uint_fast8_t)QF_LOG2((me_)->bits))


/****************************************************************************/
/*! Priority Set of up to 64 elements for building various schedulers */
/**
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 64 priority levels.
*
* The priority set allows to build cooperative multitasking schedulers
* to manage up to 64 tasks. It is also used in the Quantum Kernel (QK)
* preemptive scheduler.
*/
typedef struct {

    /** \brief bimask representing 8-element subsets of the set
    *
    * Each bit in the bytes set represents a subset (8-elements)
    * as follows: \n
    * bit 0 in bytes is 1 when bits[0] is not empty \n
    * bit 1 in bytes is 1 when bits[1] is not empty \n
    * bit 2 in bytes is 1 when bits[2] is not empty \n
    * bit 3 in bytes is 1 when bits[3] is not empty \n
    * bit 4 in bytes is 1 when bits[4] is not empty \n
    * bit 5 in bytes is 1 when bits[5] is not empty \n
    * bit 6 in bytes is 1 when bits[6] is not empty \n
    * bit 7 in bytes is 1 when bits[7] is not empty \n
    */
    uint_fast8_t volatile bytes;

    /** \brief bits representing elements in the set as follows: \n
    * bits[0] represent elements  1..8  \n
    * bits[1] represent elements  9..16 \n
    * bits[2] represent elements 17..24 \n
    * bits[3] represent elements 25..32 \n
    * bits[4] represent elements 33..40 \n
    * bits[5] represent elements 41..48 \n
    * bits[6] represent elements 49..56 \n
    * bits[7] represent elements 57..64 \n
    */
    uint_fast8_t volatile bits[8];
} QPSet64;

/*! Evaluates to TRUE if the priority set \c me_ has elements */
#define QPSet64_isEmpty(me_)    ((me_)->bytes == (uint_fast8_t)0)

/*! Evaluates to TRUE if the priority set \c me is empty */
#define QPSet64_notEmpty(me_)   ((me_)->bytes != (uint_fast8_t)0)

/*! Evaluates to TRUE if the priority set \c me_ has element \c n_. */
#define QPSet64_hasElement(me_, n_) \
    (((me_)->bits[Q_ROM_BYTE(QF_div8Lkup[(n_)])] \
      & (uint_fast8_t)Q_ROM_BYTE(QF_pwr2Lkup[(n_)])) != ((uint_fast8_t)0)

/*! insert element \c n_ into the set \c me_, n_= 1..64 */
#define QPSet64_insert(me_, n_) do { \
    uint_fast8_t m_  = (uint_fast8_t)Q_ROM_BYTE(QF_div8Lkup[(n_)]); \
    (me_)->bits[m_] |= (uint_fast8_t)Q_ROM_BYTE(QF_pwr2Lkup[(n_)]); \
    (me_)->bytes    |= (uint_fast8_t)Q_ROM_BYTE(QF_pwr2Lkup[m_ \
                                               + (uint_fast8_t)1]); \
} while (0)

/*! Remove element n_ from the set \c me_, n_= 1..64 */
#define QPSet64_remove(me_, n_) do { \
    uint_fast8_t m_ = (uint_fast8_t)Q_ROM_BYTE(QF_div8Lkup[(n_)]); \
    if (((me_)->bits[m_] &= (uint_fast8_t)Q_ROM_BYTE( \
                            QF_invPwr2Lkup[(n_)])) == (uint_fast8_t)0) { \
        (me_)->bytes &= (uint_fast8_t)Q_ROM_BYTE( \
                                      QF_invPwr2Lkup[m_ + (uint_fast8_t)1]); \
    } \
} while (0)

/*! Find the maximum element in the set, and assign it to \c n_ */
/** \note if the set \c me_ is empty, \c n_ is set to zero.
*/
#define QPSet64_findMax(me_, n_) do { \
    if ((me_)->bytes != (uint_fast8_t)0) { \
        (n_) = (uint_fast8_t)QF_LOG2((me_)->bytes) - (uint_fast8_t)1; \
        (n_) = (uint_fast8_t)QF_LOG2((me_)->bits[(n_)]) \
                              + (uint_fast8_t)((n_) << 3); \
    } \
    else { \
       (n_) = (uint_fast8_t)0; \
    } \
} while (0)

#endif /* qpset_h */

