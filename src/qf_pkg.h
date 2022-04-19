/*============================================================================
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of open-source GPL 3.0
* (or any later version), or alternatively, under the terms of one of the
* closed-source Quantum Leaps commercial licenses.
*
* The terms of the open source GPL 3.0 license can be found at:
* <www.gnu.org/licenses/gpl-3.0.txt>
*
* The terms of the closed-source Quantum Leaps commercial licenses
* can be found at:
* <www.state-machine.com/licensing>
*
* NOTE: Please do NOT plagiarize this software to sidestep the license
* obligations. This is both unfair and illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/**
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief Internal (package scope) QF/C interface.
* @ingroup qf
*/
#ifndef QF_PKG_H
#define QF_PKG_H

/*==========================================================================*/

/* QF-specific critical section */
#ifndef QF_CRIT_STAT_TYPE
    /*! This is an internal macro for defining the critical section
    * status type. */
    /**
    * @description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * provides the definition of the critical section status variable.
    * Otherwise this macro is empty.
    * @sa #QF_CRIT_STAT_TYPE
    */
    #define QF_CRIT_STAT_

    /*! This is an internal macro for entering a critical section. */
    /**
    * @description
    * The purpose of this macro is to enable writing the same code for the
    * case whe6n critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes QF_CRIT_ENTRY() passing the key variable as the parameter.
    * Otherwise QF_CRIT_ENTRY() is invoked with a dummy parameter.
    * @sa QF_CRIT_ENTRY()
    */
    #define QF_CRIT_E_()     QF_CRIT_ENTRY(dummy)

    /*! This is an internal macro for exiting a critical section. */
    /**
    * @description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_EXIT passing the key variable as the parameter.
    * Otherwise #QF_CRIT_EXIT is invoked with a dummy parameter.
    * @sa #QF_CRIT_EXIT
    */
    #define QF_CRIT_X_()     QF_CRIT_EXIT(dummy)

#elif (!defined QF_CRIT_STAT_)
    #define QF_CRIT_STAT_    QF_CRIT_STAT_TYPE critStat_;
    #define QF_CRIT_E_()     QF_CRIT_ENTRY(critStat_)
    #define QF_CRIT_X_()     QF_CRIT_EXIT(critStat_)
#endif

/*==========================================================================*/

/* Assertions inside the crticial section */
#ifdef Q_NASSERT /* Q_NASSERT defined--assertion checking disabled */

    #define Q_ASSERT_CRIT_(id_, test_)  ((void)0)
    #define Q_REQUIRE_CRIT_(id_, test_) ((void)0)
    #define Q_ERROR_CRIT_(id_)          ((void)0)

#else  /* Q_NASSERT not defined--assertion checking enabled */

    #define Q_ASSERT_CRIT_(id_, test_) do {               \
        if ((test_)) {} else {                            \
            QF_CRIT_X_();                                 \
            Q_onAssert(&Q_this_module_[0], (int_t)(id_)); \
        }                                                 \
    } while (false)

    #define Q_REQUIRE_CRIT_(id_, test_) Q_ASSERT_CRIT_((id_), (test_))

    #define Q_ERROR_CRIT_(id_) do {                       \
        QF_CRIT_X_();                                     \
        Q_onAssert(&Q_this_module_[0], (int_t)(id_));     \
    } while (false)

#endif /* Q_NASSERT */

/*==========================================================================*/

/* internal prototypes (should be used via vtable only) */

/*! Prototype of the internal active object start implementation */
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par);

/*! Get an event from the event queue of an active object.
* @private @memberof QActive
*/
QEvt const *QActive_get_(QActive *const me);

#ifdef Q_SPY
    /*! Prototype of the internal active object post (FIFO) implementation.
    * @private @memberof QActive
    */
    bool QActive_post_(QActive * const me, QEvt const * const e,
                       uint_fast16_t const margin,
                       void const * const sender);
#else
    bool QActive_post_(QActive * const me, QEvt const * const e,
                       uint_fast16_t const margin);
#endif

/*! Prototype of the internal active object post LIFO implementation.
* @private @memberof QActive
*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e);

/*==========================================================================*/

/*! heads of linked lists of time events, one for every clock tick rate.
* @static @private @memberof QF
*/
extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

/* The following flags and bitmasks are for the fields of the @c refCtr_
* attribute of the QTimeEvt struct (inherited from QEvt). This attribute
* is NOT used for reference counting in time events, because the @c poolId_
* attribute is zero ("static events").
*/
#define TE_IS_LINKED      (1U << 7U)
#define TE_WAS_DISARMED   (1U << 6U)
#define TE_TICK_RATE      0x0FU

extern QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL]; /*!< allocate event pools */
extern uint_fast8_t QF_maxPool_;     /*!< # of initialized event pools */
extern QSubscrList *QF_subscrList_;  /*!< the subscriber list array */
extern enum_t QF_maxPubSignal_;      /*!< the maximum published signal */

/*! structure representing a free block in the Native QF Memory Pool. */
typedef struct QFreeBlock {
    struct QFreeBlock * volatile next;
} QFreeBlock;

/* internal helper macros ***************************************************/

/*! helper macro to cast const away from an event pointer @p e_ */
#define QF_EVT_CONST_CAST_(e_)  ((QEvt *)(e_))

/*! increment the refCtr of an event @p e_ casting const away */
#define QF_EVT_REF_CTR_INC_(e_) (++QF_EVT_CONST_CAST_(e_)->refCtr_)

/*! decrement the refCtr of an event @p e_ casting const away */
#define QF_EVT_REF_CTR_DEC_(e_) (--QF_EVT_CONST_CAST_(e_)->refCtr_)

/*! access element at index @p i_ from the base pointer @p base_ */
#define QF_PTR_AT_(base_, i_)   ((base_)[(i_)])

/**
* @description
* This macro is specifically and exclusively used for checking the range
* of a block pointer returned to the pool. Such a check must rely on the
* pointer arithmetic not compliant with the MISRA-C 2012 Rule 18.3(R).
* Defining a specific macro for this purpose allows to selectively disable
* the warnings for this particular case.
*/
#define QF_PTR_RANGE_(x_, min_, max_)  (((min_) <= (x_)) && ((x_) <= (max_)))

#endif /* QF_PKG_H */
