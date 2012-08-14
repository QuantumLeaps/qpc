/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 4.4.02
* Date of the Last Update:  Apr 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qpset_h
#define qpset_h

/**
* \file
* \ingroup qf qk
* \brief platform-independent priority sets of 8 or 64 elements.
*
* This header file must be included in those QF ports that use the
* cooperative "vanilla" kernel or the QK preemptive kernel.
*/

/****************************************************************************/
/** \brief Priority Set of up to 8 elements for building various schedulers
*
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 8 priority levels.
*
* The priority set allows to build cooperative multitasking schedulers
* to manage up to 8 tasks.
*/
typedef struct QPSet8Tag {
    uint8_t bits;       /**< \brief bimask representing elements of the set */
} QPSet8;

/** \brief the macro evaluates to TRUE if the priority set \a me has elements
*/
#define QPSet8_isEmpty(me_) ((me_)->bits == (uint8_t)0)

/** \brief the macro evaluates to TRUE if the priority set \a me is empty
*/
#define QPSet8_notEmpty(me_) ((me_)->bits != (uint8_t)0)

/** \brief the macro evaluates to TRUE if the priority set \a me_
* has element \a n_.
*/
#define QPSet8_hasElement(me_, n_) \
    (((me_)->bits & Q_ROM_BYTE(QF_pwr2Lkup[n_])) != 0)

/** \brief insert element \a n_ into the set \a me_, n_= 1..8
*/
#define QPSet8_insert(me_, n_) \
    ((me_)->bits |= Q_ROM_BYTE(QF_pwr2Lkup[n_]))

/** \brief remove element n_ from the set \a me_, n_= 1..8
*/
#define QPSet8_remove(me_, n_) \
    ((me_)->bits &= Q_ROM_BYTE(QF_invPwr2Lkup[n_]))

/** \brief find the maximum element in the set, and assign it to n_,
* \note if the set \a me_ is empty, \a n_ is set to zero.
*/
#define QPSet8_findMax(me_, n_) \
    ((n_) = Q_ROM_BYTE(QF_log2Lkup[(me_)->bits]))


/****************************************************************************/
/** \brief Priority Set of up to 64 elements for building various schedulers
*
* The priority set represents the set of active objects that are ready to
* run and need to be considered by the scheduling algorithm. The set is
* capable of storing up to 64 priority levels.
*
* The priority set allows to build cooperative multitasking schedulers
* to manage up to 64 tasks. It is also used in the Quantum Kernel (QK)
* preemptive scheduler.
*/
typedef struct QPSet64Tag {

    /** \brief 8-bit superset of 8-bit subsets
    *
    * Each bit in the super.bits set represents a subset (8-elements)
    * as follows: \n
    * bit 0 in super.bits is 1 when subset[0] is not empty \n
    * bit 1 in super.bits is 1 when subset[1] is not empty \n
    * bit 2 in super.bits is 1 when subset[2] is not empty \n
    * bit 3 in super.bits is 1 when subset[3] is not empty \n
    * bit 4 in super.bits is 1 when subset[4] is not empty \n
    * bit 5 in super.bits is 1 when subset[5] is not empty \n
    * bit 6 in super.bits is 1 when subset[6] is not empty \n
    * bit 7 in super.bits is 1 when subset[7] is not empty \n
    */
    QPSet8 super;                            /* QPSet64 derives from QPSet8 */

    /** \brief subsets representing elements in the set as follows: \n
    * subset[0] represent elements  1..8  \n
    * subset[1] represent elements  9..16 \n
    * subset[2] represent elements 17..24 \n
    * subset[3] represent elements 25..32 \n
    * subset[4] represent elements 33..40 \n
    * subset[5] represent elements 41..48 \n
    * subset[6] represent elements 49..56 \n
    * subset[7] represent elements 57..64 \n
    */
    QPSet8 subset[8];
} QPSet64;

/** \brief the macro evaluates to TRUE if the priority set \a me_ has elements
*/
#define QPSet64_isEmpty(me_)    QPSet8_isEmpty(&(me_)->super)

/** \brief the macro evaluates to TRUE if the priority set \a me is empty
*/
#define QPSet64_notEmpty(me_)   QPSet8_notEmpty(&(me_)->super)

/** \brief the macro evaluates to TRUE if the priority set \a me_
* has element \a n_.
*/
#define QPSet64_hasElement(me_, n_) \
    QPSet8_hasElement(&(me_)->subset[Q_ROM_BYTE(QF_div8Lkup[n_])], n_)

/** \brief insert element \a n_ into the set \a me_, n_= 1..64
*/
#define QPSet64_insert(me_, n_) do { \
    QPSet8_insert(&(me_)->super, \
                  (uint8_t)(Q_ROM_BYTE(QF_div8Lkup[n_]) + (uint8_t)1)); \
    QPSet8_insert(&(me_)->subset[Q_ROM_BYTE(QF_div8Lkup[n_])], n_); \
} while (0)

/** \brief remove element n_ from the set \a me_, n_= 1..64
*/
#define QPSet64_remove(me_, n_) do { \
    if (QPSet8_remove(&(me_)->subset[Q_ROM_BYTE(QF_div8Lkup[n_])], n_) \
        == (uint8_t)0) \
    { \
        QPSet8_remove(&(me_)->super, \
                      (uint8_t)(Q_ROM_BYTE(QF_div8Lkup[n_]) + (uint8_t)1)); \
    } \
} while (0)

/** \brief find the maximum element in the set, and assign it to \a n_
* \note if the set \a me_ is empty, \a n_ is set to zero.
*/
#define QPSet64_findMax(me_, n_) do { \
    if (QPSet64_notEmpty(me_)) { \
        (n_) = (uint8_t)(Q_ROM_BYTE(QF_log2Lkup[(me_)->super.bits]) \
                         - (uint8_t)1); \
        (n_) = (uint8_t)(Q_ROM_BYTE(QF_log2Lkup[(me_)->subset[n_].bits]) \
                         + (uint8_t)((n_) << 3)); \
    } \
    else { \
       (n_) = (uint8_t)0; \
    } \
} while (0)

#endif                                                           /* qpset_h */

