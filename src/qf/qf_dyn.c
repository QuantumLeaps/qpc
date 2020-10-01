/**
* @file
* @brief QF/C dynamic event management
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-03
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
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_dyn")


/* Package-scope objects ****************************************************/
QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL]; /* allocate the event pools */
uint_fast8_t QF_maxPool_; /* number of initialized event pools */

/****************************************************************************/
#ifdef Q_EVT_CTOR  /* Provide the constructor for the ::QEvt class? */

/**
* @description
* Constructor for the ::QEvt class provided when the switch #Q_EVT_CTOR
* is defined.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     sig  signal to be assigned to the event
*/
QEvt *QEvt_ctor(QEvt * const me, enum_t const sig) {
    /** @pre the me pointer must be valid */
    Q_REQUIRE_ID(100, me != (QEvt *)0);
    me->sig = (QSignal)sig;
    return me;
}

#endif /* Q_EVT_CTOR */


/****************************************************************************/
/**
* @description
* This function initializes one event pool at a time and must be called
* exactly once for each event pool before the pool can be used.
*
* @param[in] poolSto  pointer to the storage for the event pool
* @param[in] poolSize size of the storage for the pool in bytes
* @param[in] evtSize  the block-size of the pool in bytes, which determines
*            the maximum size of events that can be allocated from the pool.
*
* @attention
* You might initialize many event pools by making many consecutive calls
* to the QF_poolInit() function. However, for the simplicity of the internal
* implementation, you must initialize event pools in the **ascending order**
* of the event size.
*
* Many RTOSes provide fixed block-size heaps, a.k.a. memory pools that can
* be adapted for QF event pools. In case such support is missing, QF provides
* a native QF event pool implementation. The macro #QF_EPOOL_TYPE_ determines
* the type of event pool used by a particular QF port. See structure ::QMPool
* for more information.
*
* @note The actual number of events available in the pool might be actually
* less than (@p poolSize / @p evtSize) due to the internal alignment
* of the blocks that the pool might perform. You can always check the
* capacity of the pool by calling QF_getPoolMin().
*
* @note The dynamic allocation of events is optional, meaning that you
* might choose not to use dynamic events. In that case calling QF_poolInit()
* and using up memory for the memory blocks is unnecessary.
*
* @sa QF initialization example for QF_init()
*/
void QF_poolInit(void * const poolSto, uint_fast32_t const poolSize,
                 uint_fast16_t const evtSize)
{
    /** @pre cannot exceed the number of available memory pools */
    Q_REQUIRE_ID(200, QF_maxPool_ < Q_DIM(QF_pool_));
    /** @pre please initialize event pools in ascending order of evtSize: */
    Q_REQUIRE_ID(201, (QF_maxPool_ == 0U)
        || (QF_EPOOL_EVENT_SIZE_(QF_pool_[QF_maxPool_ - 1U])
            < evtSize));

    /* perform the platform-dependent initialization of the pool */
    QF_EPOOL_INIT_(QF_pool_[QF_maxPool_], poolSto, poolSize, evtSize);
    ++QF_maxPool_; /* one more pool */

#ifdef Q_SPY
    /* generate the object-dictionary entry for the initialized pool */
    {
        char_t obj_name[9] = "EvtPool?";
        obj_name[7] = '0' + (QF_maxPool_ & 0x7FU);
        QS_obj_dict_pre_(&QF_pool_[QF_maxPool_ - 1U], obj_name);
    }
#endif /* Q_SPY*/
}

/****************************************************************************/
/**
* @description
* Allocates an event dynamically from one of the QF event pools.
*
* @param[in] evtSize the size (in bytes) of the event to allocate
* @param[in] margin  the number of un-allocated events still available
*                    in a given event pool after the allocation completes.
*                    The special value #QF_NO_MARGIN means that this function
*                    will assert if allocation fails.
* @param[in] sig     the signal to be assigned to the allocated event
*
* @returns
* pointer to the newly allocated event. This pointer can be NULL only if
* margin != #QF_NO_MARGIN and the event cannot be allocated with the
* specified margin still available in the given pool.
*
* @note
* The internal QF function QF_newX_() raises an assertion when the
* @p margin parameter is #QF_NO_MARGIN and allocation of the event turns
* out to be impossible due to event pool depletion, or incorrect (too big)
* size of the requested event.
*
* @note
* The application code should not call this function directly.
* The only allowed use is thorough the macros Q_NEW() or Q_NEW_X().
*/
QEvt *QF_newX_(uint_fast16_t const evtSize,
               uint_fast16_t const margin, enum_t const sig)
{
    QEvt *e;
    uint_fast8_t idx;
    QS_CRIT_STAT_

    /* find the pool index that fits the requested event size ... */
    for (idx = 0U; idx < QF_maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_pool_[idx])) {
            break;
        }
    }
    /* cannot run out of registered pools */
    Q_ASSERT_ID(310, idx < QF_maxPool_);

    /* get e -- platform-dependent */
#ifdef Q_SPY
    QF_EPOOL_GET_(QF_pool_[idx], e,
                  ((margin != QF_NO_MARGIN) ? margin : 0U),
                  (uint_fast8_t)QS_EP_ID + idx + 1U);
#else
    QF_EPOOL_GET_(QF_pool_[idx], e,
                  ((margin != QF_NO_MARGIN) ? margin : 0U), 0U);
#endif

    /* was e allocated correctly? */
    if (e != (QEvt *)0) {
        e->sig = (QSignal)sig;     /* set signal for this event */
        e->poolId_ = (uint8_t)(idx + 1U); /* store the pool ID */
        e->refCtr_ = 0U; /* set the reference counter to 0 */

        QS_BEGIN_PRE_(QS_QF_NEW, (uint_fast8_t)QS_EP_ID + e->poolId_)
            QS_TIME_PRE_();        /* timestamp */
            QS_EVS_PRE_(evtSize);  /* the size of the event */
            QS_SIG_PRE_(sig);      /* the signal of the event */
        QS_END_PRE_()
    }
    /* event cannot be allocated */
    else {
        /* must tolerate failed allocation */
        Q_ASSERT_ID(320, margin != QF_NO_MARGIN);

        QS_BEGIN_PRE_(QS_QF_NEW_ATTEMPT, (uint_fast8_t)QS_EP_ID + idx + 1U)
            QS_TIME_PRE_();        /* timestamp */
            QS_EVS_PRE_(evtSize);  /* the size of the event */
            QS_SIG_PRE_(sig);      /* the signal of the event */
        QS_END_PRE_()
    }
    return e; /* can't be NULL if we can't tolerate failed allocation */
}

/****************************************************************************/
/**
* @description
* This function implements a simple garbage collector for the dynamic events.
* Only dynamic events are candidates for recycling. (A dynamic event is one
* that is allocated from an event pool, which is determined as non-zero
* e->poolId_ attribute.) Next, the function decrements the reference counter
* of the event (e->refCtr_), and recycles the event only if the counter drops
* to zero (meaning that no more references are outstanding for this event).
* The dynamic event is recycled by returning it to the pool from which
* it was originally allocated.
*
* @param[in]  e  pointer to the event to recycle
*
* @note
* QF invokes the garbage collector at all appropriate contexts, when
* an event can become garbage (automatic garbage collection), so the
* application code should have no need to call QF_gc() directly. The QF_gc()
* function is exposed only for special cases when your application sends
* dynamic events to the "raw" thread-safe queues (see ::QEQueue). Such
* queues are processed outside of QF and the automatic garbage collection
* is **NOT** performed for these events. In this case you need to call
* QF_gc() explicitly.
*/
void QF_gc(QEvt const * const e) {

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        QF_CRIT_STAT_
        QF_CRIT_E_();

        /* isn't this the last reference? */
        if (e->refCtr_ > 1U) {

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC_ATTEMPT,
                                 (uint_fast8_t)QS_EP_ID + e->poolId_)
                QS_TIME_PRE_();         /* timestamp */
                QS_SIG_PRE_(e->sig);    /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            QF_EVT_REF_CTR_DEC_(e); /* decrement the ref counter */

            QF_CRIT_X_();
        }
        /* this is the last reference to this event, recycle it */
        else {
            uint_fast8_t idx = (uint_fast8_t)e->poolId_ - 1U;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC,
                                 (uint_fast8_t)QS_EP_ID + e->poolId_)
                QS_TIME_PRE_();         /* timestamp */
                QS_SIG_PRE_(e->sig);    /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            QF_CRIT_X_();

            /* pool ID must be in range */
            Q_ASSERT_ID(410, idx < QF_maxPool_);

            /* cast 'const' away, which is OK, because it's a pool event */
#ifdef Q_SPY
            QF_EPOOL_PUT_(QF_pool_[idx], QF_EVT_CONST_CAST_(e),
                          (uint_fast8_t)QS_EP_ID + e->poolId_);
#else
            QF_EPOOL_PUT_(QF_pool_[idx], QF_EVT_CONST_CAST_(e), 0U);
#endif
        }
    }
}

/****************************************************************************/
/**
* @description
* Creates and returns a new reference to the current event e
*
* @param[in] e       pointer to the current event
* @param[in] evtRef  the event reference
*
* @returns
* the newly created reference to the event `e`
*
* @note
* The application code should not call this function directly.
* The only allowed use is thorough the macro Q_NEW_REF().
*/
QEvt const *QF_newRef_(QEvt const * const e, void const * const evtRef) {
    QF_CRIT_STAT_

    /*! @pre the event must be dynamic and the provided event reference
    * must not be already in use */
    Q_REQUIRE_ID(500,
        (e->poolId_ != 0U)
        && (evtRef == (void *)0));

    QF_CRIT_E_();

    QF_EVT_REF_CTR_INC_(e); /* increments the ref counter */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_NEW_REF,
                         (uint_fast8_t)QS_EP_ID + e->poolId_)
        QS_TIME_PRE_();      /* timestamp */
        QS_SIG_PRE_(e->sig); /* the signal of the event */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
    QS_END_NOCRIT_PRE_()

    QF_CRIT_X_();

    return e;
}
/****************************************************************************/
/**
* @description
* Deletes an existing reference to the event e
*
* @param[in] evtRef  the event reference
*
* @note
* The application code should not call this function directly.
* The only allowed use is thorough the macro Q_DELETE_REF().
*/
void QF_deleteRef_(void const * const evtRef) {
    QS_CRIT_STAT_
    QEvt const * const e = (QEvt const *)evtRef;

    QS_BEGIN_PRE_(QS_QF_DELETE_REF,
                  (uint_fast8_t)QS_EP_ID + e->poolId_)
        QS_TIME_PRE_();      /* timestamp */
        QS_SIG_PRE_(e->sig); /* the signal of the event */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
    QS_END_PRE_()

    QF_gc(e);
}

/****************************************************************************/
/**
* @description
* Obtain the block size of any registered event pools
*/
uint_fast16_t QF_poolGetMaxBlockSize(void) {
    return QF_EPOOL_EVENT_SIZE_(QF_pool_[QF_maxPool_ - 1U]);
}

