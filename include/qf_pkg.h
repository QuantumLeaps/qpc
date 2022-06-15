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
/*!
* @date Last updated on: 2022-06-15
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief Internal (package scope) QF/C interface.
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
    * case when critical section status type is defined and when it is not.
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

/* Assertions inside the critical section */
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

/*! Prototype of the internal active object start implementation
* @private @memberof QActive
*
* @description
* Starts execution of the AO and registers the AO with the framework.
* Also takes the top-most initial transition in the AO's state machine.
* This initial transition is taken in the callee's thread of execution.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     prio    priority at which to start the active object
* @param[in]     qSto    pointer to the storage for the ring buffer of the
*                        event queue (used only with the built-in ::QEQueue)
* @param[in]     qLen    length of the event queue [events]
* @param[in]     stkSto  pointer to the stack storage (must be NULL in QK)
* @param[in]     stkSize stack size [bytes]
* @param[in]     par     pointer to an extra parameter (might be NULL).
*
* @note This function should be called via the macro QACTIVE_START().
*
* @usage
* The following example shows starting an AO when a per-task stack is needed:
* @include qf_start.c
*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par);

/*! Get an event from the event queue of an active object.
* @private @memberof QActive
*
* @description
* The behavior of this function depends on the kernel/OS used in the QF port.
* For built-in kernels (QV or QK) the function can be called only when
* the queue is not empty, so it doesn't block. For a blocking kernel/OS
* the function can block and wait for delivery of an event.
*
* @param[in,out] me  pointer (see @ref oop)
*
* @returns
* a pointer to the received event. The returned pointer is guaranteed to be
* valid (can't be NULL).
*
* @note
* This function is used internally by a QF port to extract events from
* the event queue of an active object. This function depends on the event
* queue implementation and is sometimes customized in the QF port
* (file qf_port.h). Depending on the definition of the macro
* QACTIVE_EQUEUE_WAIT_(), the function might block the calling thread when
* no events are available.
*/
QEvt const *QActive_get_(QActive *const me);

#ifdef Q_SPY
    /*! Prototype of the internal active object post (FIFO) implementation.
    * @private @memberof QActive
    *
    * @description
    * Direct event posting is the simplest asynchronous communication method
    * available in QF.
    *
    * @param[in,out] me     pointer (see @ref oop)
    * @param[in]     e      pointer to the event to be posted
    * @param[in]     margin number of required free slots in the queue after
    *                       posting the event. The special value #QF_NO_MARGIN
    *                       means that this function will assert if
    *                       posting fails.
    * @param[in]     sender pointer to a sender object (used only for
    *                       QS tracing)
    *
    * @returns
    * 'true' (success) if the posting succeeded (with the provided margin) and
    * 'false' (failure) when the posting fails.
    *
    * @note
    * The #QF_NO_MARGIN value of the @p margin parameter is special and
    * denotes situation when the post() operation is assumed to succeed
    * (event deliveryguarantee). An assertion fires, when the event cannot
    * be delivered in this case.
    *
    * @attention
    * This function should be called only via the macro QACTIVE_POST()
    * or QACTIVE_POST_X().
    *
    * @usage
    * @include qf_post.c
    *
    * @sa QActive_post_(), QActive_postLIFO()
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
*
* @description
* posts an event to the event queue of the active object @p me using the
* Last-In-First-Out (LIFO) policy.
*
* @note
* The LIFO policy should be used only for self-posting and with caution,
* because it alters order of events in the queue.
*
* @param[in] me pointer (see @ref oop)
* @param[in] e  pointer to the event to post to the queue
*
* @attention
* This function should be called only via the macro QACTIVE_POST_LIFO().
*
* @sa QActive_post_(), QACTIVE_POST(), QACTIVE_POST_X()
*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e);

/*==========================================================================*/

/*! heads of linked lists of time events, one for every clock tick rate.
* @static @private @memberof QF
*/
extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

/* The following bitmasks are for the fields of the @c refCtr_ attribute
* of the QTimeEvt struct (inherited from QEvt). This attribute is NOT used
* for reference counting in time events, because the @c poolId_ attribute
* is zero ("immutable events").
*/
#define QTE_IS_LINKED      (1U << 7U)
#define QTE_WAS_DISARMED   (1U << 6U)
#define QTE_TICK_RATE      0x0FU

extern QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL]; /*!< allocate event pools */
extern uint_fast8_t QF_maxPool_;     /*!< # of initialized event pools */
extern QSubscrList *QF_subscrList_;  /*!< the subscriber list array */
extern enum_t QF_maxPubSignal_;      /*!< the maximum published signal */

/*! @brief structure representing a free block in the Native QF Memory Pool */
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
