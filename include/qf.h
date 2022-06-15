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
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QF/C platform-independent public interface.
*/
#ifndef QF_H
#define QF_H

#ifndef QPSET_H
#include "qpset.h"
#endif

/*==========================================================================*/
#ifndef QF_EVENT_SIZ_SIZE
    /*! Default value of the macro configurable value in qf_port.h */
    #define QF_EVENT_SIZ_SIZE 2U
#endif
#if (QF_EVENT_SIZ_SIZE == 1U)
    typedef uint8_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 2U)
    /*! The data type to store the block-size defined based on
    * the macro #QF_EVENT_SIZ_SIZE.
    * The dynamic range of this data type determines the maximum block
    * size that can be managed by the pool.
    */
    typedef uint16_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 4U)
    typedef uint32_t QEvtSize;
#else
    #error "QF_EVENT_SIZ_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

#ifndef QF_MAX_EPOOL
    /*! Default value of the macro configurable value in qf_port.h */
    #define QF_MAX_EPOOL         3U
#endif

#ifndef QF_MAX_TICK_RATE
    /*! Default value of the macro configurable value in qf_port.h.
    * Valid values: [0U..15U]; default 1
    */
    #define QF_MAX_TICK_RATE     1U
#elif (QF_MAX_TICK_RATE > 15U)
    #error "QF_MAX_TICK_RATE exceeds the maximum of 15"
#endif

#ifndef QF_TIMEEVT_CTR_SIZE
    /*! macro to override the default ::QTimeEvtCtr size.
    * Valid values: 1, 2, or 4; default 2
    */
    #define QF_TIMEEVT_CTR_SIZE  2U
#endif

/*==========================================================================*/
/*! @brief Active Object base class (based on ::QHsm implementation)
* @class QActive
*
* @extends QHsm
* @description
* Active objects in QP are encapsulated state machines (each embedding an
* event queue and a thread) that communicate with one another asynchronously
* by sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
* @n@n
* ::QActive represents an active object that uses the QHsm-style
* implementation strategy for state machines. This strategy is tailored
* to manual coding, but it is also supported by the QM modeling tool.
* The resulting code is slower than in the ::QMsm style implementation
* strategy.
*
* @tr{AQP212}
*
* @usage
* The following example illustrates how to derive an active object from
* ::QActive. Please note that the ::QActive member @c super is defined as the
* __first__ member of the derived struct (see @ref oop).
* @include qf_qactive.c
*/
typedef struct QActive {
    QHsm super; /*!< @protected inherits ::QHsm */

#ifdef QF_EQUEUE_TYPE
    /*! @private OS-dependent event-queue type.
    * @description
    * The type of the queue depends on the underlying operating system or
    * a kernel. Many kernels support "message queues" that can be adapted
    * to deliver QF events to the active object. Alternatively, QF provides
    * a native event queue implementation that can be used as well.
    *
    * @note
    * The native QF event queue is configured by defining the macro
    * #QF_EQUEUE_TYPE as ::QEQueue.
    */
    QF_EQUEUE_TYPE eQueue;
#endif

#ifdef QF_OS_OBJECT_TYPE
    /*! @private OS-dependent per-thread object.
    * @description
    * This data might be used in various ways, depending on the QF port.
    * In some ports osObject is used to block the calling thread when
    * the native QF queue is empty. In other QF ports the OS-dependent
    * object might be used differently.
    */
    QF_OS_OBJECT_TYPE osObject;
#endif

#ifdef QF_THREAD_TYPE
    /*! @private OS-dependent representation of the thread of the AO.
    * @description
    * This data might be used in various ways, depending on the QF port.
    * In some ports thread is used to store the thread handle. In other
    * ports thread can be the pointer to the Thread-Local-Storage (TLS).
    */
    QF_THREAD_TYPE thread;
#endif

#ifdef QXK_H  /* QXK kernel used? */
    /*! @private QXK dynamic priority (1..#QF_MAX_ACTIVE) of this AO/thread */
    uint8_t dynPrio;
#endif

    /*! @private QF priority (1..#QF_MAX_ACTIVE) of this active object. */
    uint8_t prio;

} QActive;

/*! @brief Virtual table for the ::QActive class */
typedef struct {
    struct QHsmVtable super; /*!< @protected inherits ::QHsmVtable */

    /*! @private virtual function to start the AO/thread
    * @sa QACTIVE_START()
    */
    void (*start)(QActive * const me, uint_fast8_t prio,
                  QEvt const * * const qSto, uint_fast16_t const qLen,
                  void * const stkSto, uint_fast16_t const stkSize,
                  void const * const par);

#ifdef Q_SPY
    /*! @private virtual function to asynchronously post (FIFO)
    * an event to the AO
    * @sa QACTIVE_POST() and QACTIVE_POST_X()
    */
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin, void const * const sender);
#else
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin);
#endif

    /*! @private virtual function to asynchronously post (LIFO)
    * an event to the AO
    * @sa QACTIVE_POST_LIFO()
    */
    void (*postLIFO)(QActive * const me, QEvt const * const e);

} QActiveVtable;

/* QActive public operations... */
/*! Polymorphically start an active object.
* @description
* Starts execution of the AO and registers the AO with the framework.
*
* @param[in,out] me_      pointer (see @ref oop)
* @param[in]     prio_    priority at which to start the active object
* @param[in]     qSto_    pointer to the storage for the ring buffer of the
*                         event queue (used only with the built-in ::QEQueue)
* @param[in]     qLen_    length of the event queue (in events)
* @param[in]     stkSto_  pointer to the stack storage (used only when
*                         per-AO stack is needed)
* @param[in]     stkSize_ stack size (in bytes)
* @param[in]     par_     pointer to the additional port-specific parameter(s)
*                         (might be NULL).
* @usage
* @include qf_start.c
*/
#define QACTIVE_START(me_, prio_, qSto_, qLen_, stkSto_, stkLen_, par_) do { \
    Q_ASSERT((Q_HSM_UPCAST(me_))->vptr);                                     \
    (*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->start)(          \
        (QActive *)(me_), (prio_),                                           \
        (qSto_), (qLen_), (stkSto_), (stkLen_), (par_));                     \
} while (false)

#ifdef Q_SPY
    /*! Polymorphically posts an event to an active object (FIFO)
    * with delivery guarantee.
    * @description
    * This macro asserts if the queue overflows and cannot accept the event.
    *
    * @param[in,out] me_   pointer (see @ref oop)
    * @param[in]     e_    pointer to the event to post
    * @param[in]     sender_ pointer to the sender object.
    *
    * @note
    * The @p sendedr_ parameter is actually only used when QS tracing
    * is enabled (macro #Q_SPY is defined). When QS software tracing is
    * disenabled, the QACTIVE_POST() macro does not pass the @p sender_
    * argument, so the overhead of passing this extra argument is entirely
    * avoided.
    *
    * @note
    * The pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QACTIVE_POST() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the sender of the event.
    *
    * @sa #QACTIVE_POST_X, QActive_post_().
    */
    #define QACTIVE_POST(me_, e_, sender_)                                   \
        ((void)(*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->post)(\
            (me_), (e_), QF_NO_MARGIN, (sender_)))

    /*! Polymorphically posts an event to an active object (FIFO)
    * without delivery guarantee.
    * @description
    * This macro does not assert if the queue overflows and cannot accept
    * the event with the specified margin of free slots remaining.
    *
    * @param[in,out] me_   pointer (see @ref oop)
    * @param[in]     e_    pointer to the event to post
    * @param[in]     margin_ the minimum free slots in the queue, which
    *                must still be available after posting the event.
    *                The special value #QF_NO_MARGIN causes asserting failure
    *                in case event allocation fails.
    * @param[in]     sender_ pointer to the sender object.
    *
    * @returns 'true' if the posting succeeded, and 'false' if the posting
    * failed due to insufficient margin of free slots available in the queue.
    *
    * @note
    * The @p sender_ parameter is actually only used when QS tracing
    * is enabled (macro #Q_SPY is defined). When QS software tracing is
    * disabled, the QACTIVE_POST() macro does not pass the @p sender_
    * argument, so the overhead of passing this extra argument is entirely
    * avoided.
    *
    * @note
    * The pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QACTIVE_POST_X() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the sender of the event.
    *
    * @usage
    * @include qf_postx.c
    */
    #define QACTIVE_POST_X(me_, e_, margin_, sender_)                        \
        ((*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->post)((me_),\
         (e_), (margin_), (sender_)))
#else

    #define QACTIVE_POST(me_, e_, sender_)                                   \
        ((void)(*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->post)(\
             (me_), (e_), QF_NO_MARGIN))

    #define QACTIVE_POST_X(me_, e_, margin_, sender_)                   \
        ((*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->post)( \
            (me_), (e_), (margin_)))

#endif

/*! Polymorphically posts an event to an active object using the
* Last-In-First-Out (LIFO) policy.
* @param[in,out] me_   pointer (see @ref oop)
* @param[in]     e_    pointer to the event to post
*/
#define QACTIVE_POST_LIFO(me_, e_)                                      \
    ((*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->postLIFO)( \
        (me_), (e_)))

/* QActive protected operations... */
/*! protected "constructor" of an ::QActive active object
* @protected @memberof QActive
*
* @description
* Performs the first step of active object initialization by assigning
* the virtual pointer and calling the superclass constructor.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     initial  pointer to the event to be dispatched to the MSM
*
* @note  Must be called only __once__ before QMSM_INIT().
* @sa QMsm_ctor() and QHsm_ctor()
*/
void QActive_ctor(QActive * const me, QStateHandler initial);

#ifdef QF_ACTIVE_STOP
    /*! Stops execution of an active object and removes it from the
    * framework's supervision.
    * @protected @memberof QActive
    *
    * @attention
    * QActive_stop() must be called only from the AO that is about
    * to stop its execution. By that time, any pointers or references
    * to the AO are considered invalid (dangling) and it becomes
    * illegal for the rest of the application to post events to the AO.
    */
    void QActive_stop(QActive * const me);
#endif

/*! Subscribes for delivery of signal @p sig to the active object @p me.
* @protected @memberof QActive
*
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Subscribing to an event means that the framework will
* start posting all published events with a given signal @p sig to the
* event queue of the active object @p me.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     sig event signal to subscribe
*
* @usage
* The following example shows how the Table active object subscribes
* to three signals in the initial transition:
* @include qf_subscribe.c
*
* @sa
* QF_publish_(), QActive_unsubscribe(), and QActive_unsubscribeAll()
*/
void QActive_subscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of signal @p sig to the AO @p me.
* @public @memberof QActive
*
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from an event means that the framework
* will stop posting published events with a given signal @p sig to the
* event queue of the active object @p me.
*
* @param[in] me  pointer (see @ref oop)
* @param[in] sig event signal to unsubscribe
*
* @note
* Due to the latency of event queues, an active object should NOT
* assume that a given signal @p sig will never be dispatched to the
* state machine of the active object after un-subscribing from that signal.
* The event might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events.
*
* @note
* Un-subscribing from a signal that has never been subscribed in the
* first place is considered an error and QF will raise an assertion.
*
* @sa
* QF_publish_(), QActive_subscribe(), and QActive_unsubscribeAll()
*/
void QActive_unsubscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of all signals to the AO @p me.
* @public @memberof QActive
*
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from all events means that the framework
* will stop posting any published events to the event queue of the active
* object @p me.
*
* @param[in] me  pointer (see @ref oop)
*
* @note
* Due to the latency of event queues, an active object should NOT
* assume that no events will ever be dispatched to the state machine of
* the active object after un-subscribing from all events.
* The events might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events. Also, the
* alternative event-delivery mechanisms, such as direct event posting or
* time events, can be still delivered to the event queue of the active
* object.
*
* @sa
* QF_publish_(), QActive_subscribe(), and QActive_unsubscribe()
*/
void QActive_unsubscribeAll(QActive const * const me);

/*! Defer an event @p e to a given event queue @p eq.
* @protected @memberof QActive
*
* @description
* This function is part of the event deferral support. An active object
* uses this function to defer an event @p e to the QF-supported native
* event queue @p eq. QF correctly accounts for another outstanding
* reference to the event and will not recycle the event at the end of
* the RTC step. Later, the active object might recall one event at a
* time from the event queue.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to recall
*                    an event from.
* @param[in]     e   pointer to the event to be deferred
*
* @returns
* 'true' (success) when the event could be deferred and 'false' (failure)
* if event deferral failed due to overflowing the queue.
*
* An active object can use multiple event queues to defer events of
* different kinds.
*
* @sa
* QActive_recall(), QEQueue, QActive_flushDeferred()
*/
bool QActive_defer(QActive const * const me,
                   QEQueue * const eq, QEvt const * const e);

/*! Recall a deferred event from a given event queue @p eq.
* @protected @memberof QActive
*
* @description
* This function is part of the event deferral support. An active object
* uses this function to recall a deferred event from a given QF
* event queue. Recalling an event means that it is removed from the
* deferred event queue @p eq and posted (LIFO) to the event queue of
* the active object.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to recall
*                    an event from.
* @returns
* 'true' if an event has been recalled and 'false' if not.
*
* @note
* An active object can use multiple event queues to defer events of
* different kinds.
*
* @sa
* QActive_recall(), ::QEQueue, QACTIVE_POST_LIFO()
*/
bool QActive_recall(QActive * const me, QEQueue * const eq);

/*! Flush the specified deferred queue @p eq.
* @protected @memberof QActive
*
* @description
* This function is part of the event deferral support. An active object
* can use this function to flush a given QF event queue. The function makes
* sure that the events are not leaked.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to flush.
*
* @returns
* the number of events actually flushed from the queue.
*
* @sa
* QActive_defer(), QActive_recall(), QEQueue
*/
uint_fast16_t QActive_flushDeferred(QActive const * const me,
                                    QEQueue * const eq);

/*! Generic setting of additional attributes (useful in QP ports)
* @protected @memberof QActive
*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2);

/*==========================================================================*/
/*! @brief QMActive active object base class (based on ::QMsm implementation)
* @class QMActive
* @extends QActive
*
* @description
* QMActive represents an active object that uses the ::QMsm style state
* machine implementation strategy. This strategy requires the use of the
* QM modeling tool to generate state machine code automatically, but the
* code is faster than in the ::QHsm style implementation strategy and needs
* less run-time support (smaller event-processor).
*
* @note
* ::QMActive is not intended to be instantiated directly, but rather serves
* as the base class for derivation of active objects in the application.
*
* @tr{AQP214}
*
* @usage
* The following example illustrates how to derive an active object from
* ::QMActive. Please note that the ::QActive member @c super is defined as
* the __first__ member of the derived struct (see @ref oop).
* @include qf_qmactive.c
*/
typedef struct {
    QActive super; /*!< @protected inherits ::QActive */
} QMActive;

/*! Virtual Table for the ::QMActive class (inherited from ::QActiveVtable
* @note
* ::QMActive inherits ::QActive exactly, without adding any new virtual
* functions and therefore, ::QMActiveVtable is typedef'ed as ::QActiveVtable.
*/
typedef QActiveVtable QMActiveVtable;

/* QMActive protected operations... */
/*! protected "constructor" of an ::QMActive active object.
* @protected @memberof QMActive
*
* @description
* Performs the first step of active object initialization by assigning
* the virtual pointer and calling the superclass constructor.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     initial  pointer to the event to be dispatched to the MSM
*
* @note  Must be called only ONCE before QMSM_INIT().
*
* @sa QHsm_ctor()
*/
void QMActive_ctor(QMActive * const me, QStateHandler initial);

/*==========================================================================*/
#if (QF_TIMEEVT_CTR_SIZE == 1U)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2U)

    /*! type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks.
    * @description
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: @n
    * uint8_t when (QF_TIMEEVT_CTR_SIZE == 1U), and @n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4U).
    */
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4U)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMEEVT_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/*! @brief Time Event class
* @class QTimeEvt
* @extends QEvt
*
* @description
* Time events are special QF events equipped with the notion of time passage.
* The basic usage model of the time events is as follows. An active object
* allocates one or more ::QTimeEvt objects (provides the storage for them).
* When the active object needs to arrange for a timeout, it arms one of its
* time events to fire either just once (one-shot) or periodically. Each time
* event times out independently from the others, so a QF application can make
* multiple parallel timeout requests (from the same or different active
* objects). When QF detects that the appropriate moment has arrived, it
* inserts the time event directly into the recipient's event queue. The
* recipient then processes the time event just like any other event.
*
* Time events, as any other QF events derive from the ::QEvt base structure.
* Typically, you will use a time event as-is, but you can also further
* derive more specialized time events from it by adding some more data
* members and/or specialized functions that operate on the specialized
* time events.
*
* Internally, the armed time events are organized into linked lists--one list
* for every supported ticking rate. These linked lists are scanned in every
* invocation of the QF_TICK_X() macro. Only armed (timing out) time events
* are in the list, so only armed time events consume CPU cycles.
*
* @sa ::QTimeEvt for the description of the data members @n @ref oop
*
* @tr{AQP215}
*
* @note
* QF manages the time events in the QF_TICK_X() macro, which must be called
* periodically, from the clock tick ISR or from other periodic source.
* QF_TICK_X() caYou might also use the special ::QTicker
* active object.
*
* @note
* Even though ::QTimeEvt is a subclass of ::QEvt, ::QTimeEvt instances can NOT
* be allocated dynamically from event pools. In other words, it is illegal to
* allocate ::QTimeEvt instances with the Q_NEW() or Q_NEW_X() macros.
*/
typedef struct QTimeEvt {
    QEvt super; /*!< @protected inherits ::QEvt */

    /*! @private link to the next time event in the list */
    struct QTimeEvt * volatile next;

    /*! @private the active object that receives the time events */
    void * volatile act;

    /*! @private internal down-counter of the time event.
    * @description
    * The down-counter is decremented by 1 in every QF_tickX_() invocation.
    * The time event fires (gets posted or published) when the down-counter
    * reaches zero.
    */
    QTimeEvtCtr volatile ctr;

    /*! @private interval for periodic time event
    * (zero for one-shot time event)
    * @description
    * The value of the interval is re-loaded to the internal down-counter
    * when the time event expires, so that the time event keeps timing out
    * periodically.
    */
    QTimeEvtCtr interval;
} QTimeEvt;

/* QTimeEvt public operations... */

/*! The extended "constructor" to initialize a Time Event.
* @public @memberof QTimeEvt
*
* @description
* When creating a time event, you must commit it to a specific active object
* @p act, tick rate @p tickRate and event signal @p sig. You cannot change
* these attributes later.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     act  pointer to the active object associated with this
*                     time event. The time event will post itself to this AO.
* @param[in]     sig  signal to associate with this time event.
* @param[in]     tickRate systemclock tick rate to associate with this
*                     time event in the range [0..15].
*
* @note You should call the constructor exactly once for every Time Event
* object **before** arming the Time Event. The ideal place for initializing
* the time event(s) associated with a given AO is the AO's constructor.
*/
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate);

/*! Arm a time event (one shot or periodic) for direct event posting.
* @public @memberof QTimeEvt
*
* @description
* Arms a time event to fire in a specified number of clock ticks and with
* a specified interval. If the interval is zero, the time event is armed for
* one shot ('one-shot' time event). When the timeout expires, the time event
* gets directly posted (using the FIFO policy) into the event queue of the
* host active object. After posting, a one-shot time event gets automatically
* disarmed while a periodic time event (interval != 0) is automatically
* re-armed.
*
* A time event can be disarmed at any time by calling QTimeEvt_disarm().
* Also, a time event can be re-armed to fire in a different number of clock
* ticks by calling the QTimeEvt_rearm().
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to rearm the time event with.
* @param[in]     interval interval (in clock ticks) for periodic time event.
*
* @attention
* Arming an already armed time event is __not__ allowed and is considered
* a programming error. The QP/C framework will assert if it detects an
* attempt to arm an already armed time event.
*
* @usage
* The following example shows how to arm a one-shot time event from a state
* machine of an active object:
* @include qf_state.c
*/
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval);

/*! Rearm a time event.
* @public @memberof QTimeEvt
*
* @description
* Rearms a time event with a new number of clock ticks. This function can
* be used to adjust the current period of a periodic time event or to
* prevent a one-shot time event from expiring (e.g., a watchdog time event).
* Rearming a periodic timer leaves the interval unchanged and is a convenient
* method to adjust the phasing of a periodic time event.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to rearm the time event with.
*
* @returns
* 'true' if the time event was running as it was re-armed. The 'false'
* return means that the time event was not truly rearmed because it was
* not running. The 'false' return is only possible for one-shot time events
* that have been automatically disarmed upon expiration. In this case the
* 'false' return means that the time event has already been posted or
* published and should be expected in the active object's state machine.
*/
bool QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks);

/*! Disarm a time event.
* @public @memberof QTimeEvt
*
* @description
* Disarm the time event so it can be safely reused.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' if the time event was truly disarmed, that is, it was running.
* The return of 'false' means that the time event was not truly disarmed,
* because it was not running. The 'false' return is only possible for one-
* shot time events that have been automatically disarmed upon expiration.
* In this case the 'false' return means that the time event has already
* been posted or published and should be expected in the active object's
* state machine.
*
* @note
* there is no harm in disarming an already disarmed time event
*/
bool QTimeEvt_disarm(QTimeEvt * const me);

/*! Check the "was disarmed" status of a time event.
* @public @memberof QTimeEvt
*
* @description
* Useful for checking whether a one-shot time event was disarmed in the
* QTimeEvt_disarm() operation.
*
* @param[in,out] me   pointer (see @ref oop)
*
* @returns
* 'true' if the time event was truly disarmed in the last QTimeEvt_disarm()
* operation. The 'false' return means that the time event was not truly
* disarmed, because it was not running at that time. The 'false' return is
* only possible for one-shot time events that have been automatically disarmed
* upon expiration. In this case the 'false' return means that the time event
* has already been posted or published and should be expected in the active
* object's event queue.
*
* @note
* This function has a **side effect** of setting the "was disarmed" status,
* which means that the second and subsequent times this function is called
* the function will return 'true'.
*/
bool QTimeEvt_wasDisarmed(QTimeEvt * const me);

/*! Get the current value of the down-counter of a time event.
* @public @memberof QTimeEvt
*
* @description
* Useful for checking how many clock ticks (at the tick rate associated
* with the time event) remain until the time event expires.
*
* @param[in,out] me   pointer (see @ref oop)
*
* @returns
* For an armed time event, the function returns the current value of the
* down-counter of the given time event. If the time event is not armed,
* the function returns 0.
*
* @note
* The function is thread-safe.
*/
QTimeEvtCtr QTimeEvt_currCtr(QTimeEvt const * const me);

/*==========================================================================*/
/*! @brief QF services (event-driven active object framework)
* @class QF
*
* @description
* This class groups together QF services. It has only static members and
* should not be instantiated.
*/
typedef struct {
    uint8_t dummy;
} QF;

/*! Subscriber-List structure
* @description
* This data type represents a set of active objects that subscribe to
* a given signal. The set is represented as a priority-set, where each
* bit corresponds to the unique priority of an active object.
*
* @sa ::QSubscrList for the description of the data members
*/
typedef QPSet QSubscrList;

/* public functions */

/*! QF initialization.
* @static @public @memberof QF
*
* @description
* Initializes QF and must be called exactly once before any other QF
* function. Typically, QF_init() is called from main() even before
* initializing the Board Support Package (BSP).
*
* @note QF_init() clears the internal QF variables, so that the framework
* can start correctly even if the startup code fails to clear the
* uninitialized data (as is required by the C Standard).
*/
void QF_init(void);

/*! Publish-subscribe initialization.
* @static @public @memberof QF
*
* @description
* This function initializes the publish-subscribe facilities of QF and must
* be called exactly once before any subscriptions/publications occur in
* the application.
*
* @param[in] subscrSto pointer to the array of subscriber lists
* @param[in] maxSignal the dimension of the subscriber array and at
*                      the same time the maximum signal that can be published
*                      or subscribed.
*
* The array of subscriber-lists is indexed by signals and provides a mapping
* between the signals and subscriber-lists. The subscriber-lists are bitmasks
* of type ::QSubscrList, each bit in the bit mask corresponding to the unique
* priority of an active object. The size of the ::QSubscrList bit mask
* depends on the value of the #QF_MAX_ACTIVE macro.
*
* @note
* The publish-subscribe facilities are optional, meaning that you might
* choose not to use publish-subscribe. In that case calling QF_psInit()
* and using up memory for the subscriber-lists is unnecessary.
*
* @sa ::QSubscrList
*
* @usage
* The following example shows the typical initialization sequence of QF:
* @include qf_main.c
*/
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal);

/*! Event pool initialization for dynamic allocation of events.
* @static @public @memberof QF
*
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
                 uint_fast16_t const evtSize);

/*! Obtain the block size of any registered event pools.
* @static @public @memberof QF
*
* @description
* Obtain the block size of any registered event pools
*/
uint_fast16_t QF_poolGetMaxBlockSize(void);

/*! Transfers control to QF to run the application.
* @static @public @memberof QF
*
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QACTIVE_START().
*
* @returns
* In QK, the QF_run() does not return.
*/
int_t QF_run(void);

/*! Function invoked by the application layer to stop the QF
* application and return control to the OS/Kernel.
* @static @public @memberof QF
*
* @description
* This function stops the QF application. After calling this function,
* QF attempts to gracefully stop the application. This graceful shutdown
* might take some time to complete. The typical use of this function is
* for terminating the QF application to return back to the operating
* system or for handling fatal errors that require shutting down
* (and possibly re-setting) the system.
*
* @attention
* After calling QF_stop() the application must terminate and cannot
* continue. In particular, QF_stop() is **not** intended to be followed
* by a call to QF_init() to "resurrect" the application.
*
* @sa QF_onCleanup()
*/
void QF_stop(void);

/*! Startup QF callback.
* @description
* The timeline for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
* @static @public @memberof QF
*/
void QF_onStartup(void);

/*! Cleanup QF callback.
* @description
* QF_onCleanup() is called in some QF ports before QF returns to the
* underlying operating system or RTOS.
*
* This function is strongly platform-specific and is not implemented in
* the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. Some QF ports might not require implementing
* QF_onCleanup() at all, because many embedded applications don't have
* anything to exit to.
*
* @sa QF_stop()
* @static @public @memberof QF
*/
void QF_onCleanup(void);

#ifdef Q_SPY

    /*! Publish event to the framework.
    * @static @private @memberof QF
    *
    * @description
    * This function posts (using the FIFO policy) the event @a e to **all**
    * active objects that have subscribed to the signal @a e->sig, which is
    * called _multicasting_. The multicasting performed in this function is
    * very efficient based on reference-counting inside the published event
    * ("zero-copy" event multicasting). This function is designed to be
    * callable from any part of the system, including ISRs, device drivers,
    * and active objects.
    *
    * @note
    * To avoid any unexpected re-ordering of events posted into AO queues,
    * the event multicasting is performed with scheduler __locked__. However,
    * the scheduler is locked only up to the priority level of the highest-
    * priority subscriber, so any AOs of even higher priority, which did not
    * subscribe to this event are _not_ affected.
    *
    * @attention this function should be called only via the macro
    * QF_PUBLISH()
    */
    void QF_publish_(QEvt const * const e,
                     void const * const sender, uint_fast8_t const qs_id);

    /*! Invoke the event publishing facility.
    * @description
    * This macro is the recommended way of publishing events, because it
    * provides the vital information for software tracing and avoids any
    * overhead when the tracing is disabled.
    *
    * @param[in] e_      pointer to the posted event
    * @param[in] sender_ pointer to the sender object. This argument is
    *            actually only used when QS software tracing is enabled
    *            (macro #Q_SPY is defined). When QS software tracing is
    *            disabled, the macro calls QF_publish_() without the
    *            @p sender_ parameter, so the overhead of passing this
    *            extra argument is entirely avoided.
    * @note
    * the pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QF_PUBLISH() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the publisher of the event.
    *
    * @sa QF_publish_().
    */
    #define QF_PUBLISH(e_, sender_) \
        (QF_publish_((e_), (void const *)(sender_), (sender_)->prio))

#else

    void QF_publish_(QEvt const * const e);
    #define QF_PUBLISH(e_, dummy_)   (QF_publish_(e_))

#endif

#ifdef Q_SPY

    /*! Processes all armed time events at every clock tick.
    * @static @private @memberof QF
    *
    * @static @private @memberof QF
    * @description
    * This internal helper function processes all armed ::QTimeEvt objects
    * associated wit the tick rate @p tickRate .
    *
    * This function must be called periodically from a time-tick ISR or from
    * a task so that QF can manage the timeout events assigned to the given
    * system clock tick rate.
    *
    * @param[in] tickRate  clock tick rate serviced in this call [1..15].
    * @param[in] sender    pointer to a sender object (only for QS tracing)
    *
    * @note
    * this function should be called only via the macro QF_TICK_X()
    *
    * @note
    * the calls to QF_tickX_() with different @p tickRate parameter can
    * preempt each other. For example, higher clock tick rates might be
    * serviced from interrupts while others from tasks (active objects).
    *
    * @sa ::QTimeEvt.
    * @sa QF_tickX_()
    * @sa QF_TICK()
    */
    void QF_tickX_(uint_fast8_t const tickRate, void const * const sender);

    /*! Invoke the system clock tick processing QF_tickX_().
    * @description
    * This macro processes all armed ::QTimeEvt objects associated with
    * the tick rate @p tickRate_ .
    *
    * @param[in] tickRate_ clock tick rate to be serviced through this call
    * @param[in] sender_   pointer to the sender object. This parameter
    *            is actually used only when QS software tracing is enabled
    *            (macro #Q_SPY is defined)
    * @note
    * When QS software tracing is disabled, the macro calls QF_tickX_()
    * without the @p sender_ parameter, so the overhead of passing this
    * extra parameter is entirely avoided.
    *
    * @note
    * The pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, when #QF_TICK_X() is called from
    * an interrupt, you would create a unique object just to unambiguously
    * identify the ISR as the sender of the time events.
    *
    * @usage
    * The following example shows how to invoke QF_TICK_X() for different
    * system tick rates:
    * @include qf_tickx.c
    *
    * @sa ::QTimeEvt
    * @sa QTimeEvt_ctorX()
    * @sa QF_tickX_()
    * @sa QF_TICK()
    * @sa QXThread_ctor()
    * @sa QXThread_delay()
    */
    #define QF_TICK_X(tickRate_, sender_) (QF_tickX_((tickRate_), (sender_)))

#else

    void QF_tickX_(uint_fast8_t const tickRate);
    #define QF_TICK_X(tickRate_, dummy)   (QF_tickX_(tickRate_))

#endif

/*! special value of margin that causes asserting failure in case
* event allocation or event posting fails
*/
#define QF_NO_MARGIN ((uint_fast16_t)0xFFFFU)

/*! Invoke the system clock tick processing for tick rate 0
* @sa QF_TICK_X()
*/
#define QF_TICK(sender_)   QF_TICK_X(0U, (sender_))

/*! Returns 'true' if there are no armed time events at a given tick rate.
* @static @public @memberof QF
*
* @description
* Find out if any time events are armed at the given clock tick rate.
*
* @param[in]  tickRate  system clock tick rate to find out about.
*
* @returns
* 'true' if no time events are armed at the given tick rate and
* 'false' otherwise.
*
* @note
* This function should be called in critical section.
*/
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate);

/*! Register an active object to be managed by the framework.
* @static @private @memberof QF
*
* @description
* This function adds a given active object to the active objects
* managed by the QF framework. It should not be called by the application
* directly, only by the QP ports.
*
* @param[in]  a  pointer to the active object to add to the framework.
*
* @note The priority of the active object @p a should be set before calling
* this function.
*
* @sa QF_remove_()
*/
void QF_add_(QActive * const a);

/*! Remove the active object from the framework.
* @static @private @memberof QF
*
* @description
* This function removes a given active object from the active objects managed
* by the QF framework. It should not be called by the application
* directly, only by the QP ports.
*
* @param[in]  a  pointer to the active object to remove from the framework.
*
* @note
* The active object that is removed from the framework can no longer
* participate in the publish-subscribe event exchange.
*
* @sa QF_add_()
*/
void QF_remove_(QActive * const a);

/*! Obtain the minimum of free entries of the given event pool.
* @static @public @memberof QF
*
* @description
* This function obtains the minimum number of free blocks in the given
* event pool since this pool has been initialized by a call to QF_poolInit().
*
* @param[in] poolId  event pool ID in the range 1..QF_maxPool_, where
*                    QF_maxPool_ is the number of event pools initialized
*                    with the function QF_poolInit().
*
* @returns
* the minimum number of unused blocks in the given event pool.
*/
uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId);

/*! This function returns the minimum of free entries of
* the given event queue.
* @static @public @memberof QF
*
* @description
* Queries the minimum of free ever present in the given event queue of
* an active object with priority @p prio, since the active object
* was started.
*
* @note
* This function is available only when the native QF event queue
* implementation is used. Requesting the queue minimum of an unused
* priority level raises an assertion in the QF. (A priority level becomes
* used in QF after the call to the QF_add_() function.)
*
* @param[in] prio  Priority of the active object, whose queue is queried
*
* @returns
* the minimum of free ever present in the given event queue of an active
* object with priority @p prio, since the active object was started.
*/
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio);

/*! Internal QF implementation of creating new dynamic event.
* @static @private @memberof QF
*
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
               uint_fast16_t const margin, enum_t const sig);

/*! Internal QF implementation of creating new event reference.
* @static @private @memberof QF
*
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
QEvt const *QF_newRef_(QEvt const * const e, void const * const evtRef);

/*! Internal QF implementation of deleting event reference.
* @static @private @memberof QF
*
* @description
* Deletes an existing reference to the event e
*
* @param[in] evtRef  the event reference
*
* @note
* The application code should not call this function directly.
* The only allowed use is thorough the macro Q_DELETE_REF().
*/
void QF_deleteRef_(void const * const evtRef);

#ifdef Q_EVT_CTOR /* Shall the ctor for the ::QEvt class be provided? */

    #define Q_NEW(evtT_, sig_, ...)                                   \
        (evtT_##_ctor((evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                      QF_NO_MARGIN, 0), (enum_t)(sig_), ##__VA_ARGS__))

    #define Q_NEW_X(e_, evtT_, margin_, sig_, ...) do {        \
        (e_) = (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                                 (margin_), 0);                \
        if ((e_) != (evtT_ *)0) {                              \
            evtT_##_ctor((e_), (enum_t)(sig_), ##__VA_ARGS__); \
        }                                                      \
     } while (false)

#else

    /*! Allocate a dynamic event.
    * @description
    * The macro calls the internal QF function QF_newX_() with
    * margin == #QF_NO_MARGIN, which causes an assertion when the event
    * cannot be successfully allocated.
    *
    * @param[in] evtT_ event type (class name) of the event to allocate
    * @param[in] sig_  signal to assign to the newly allocated event
    *
    * @returns a valid event pointer cast to the type @p evtT_.
    *
    * @note
    * If Q_EVT_CTOR is defined, the Q_NEW() macro becomes variadic and
    * takes all the arguments needed by the constructor of the event
    * class being allocated. The constructor is then called by means
    * of the placement-new operator.
    *
    * @usage
    * The following example illustrates dynamic allocation of an event:
    * @include qf_post.c
    */
    #define Q_NEW(evtT_, sig_)                           \
        ((evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                           QF_NO_MARGIN, (enum_t)(sig_)))

    /*! Allocate a dynamic event (non-asserting version).
    * @description
    * This macro allocates a new event and sets the pointer @p e_, while
    * leaving at least @p margin_ of events still available in the pool
    *
    * @param[in,out] e_  pointer to the newly allocated event
    * @param[in] evtT_   event type (class name) of the event to allocate
    * @param[in] margin_ number of events that must remain available
    *                    in the given pool after this allocation. The special
    *                    value #QF_NO_MARGIN causes asserting failure in case
    *                    event allocation or event posting fails.
    * @param[in] sig_    signal to assign to the newly allocated event
    *
    * @returns an event pointer cast to the type @p evtT_ or NULL if the
    * event cannot be allocated with the specified @p margin.
    *
    * @note
    * If Q_EVT_CTOR is defined, the Q_NEW_X() macro becomes variadic and
    * takes all the arguments needed by the constructor of the event
    * class being allocated. The constructor is then called and all the
    * extra arguments are passed to it.
    *
    * @usage
    * The following example illustrates dynamic allocation of an event:
    * @include qf_postx.c
    */
    #define Q_NEW_X(e_, evtT_, margin_, sig_) ((e_) =   \
        (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                          (margin_), (enum_t)(sig_)))

#endif /* Q_EVT_CTOR */

/*! Create a new reference of the current event `e`
* @description
* The current event processed by an active object is available only for
* the duration of the run-to-completion (RTC) step. After that step, the
* current event is no longer available and the framework might recycle
* (garbage-collect) the event. The macro Q_NEW_REF() explicitly creates
* a new reference to the current event that can be stored and used beyond
* the current RTC step, until the reference is explicitly recycled by
* means of the macro Q_DELETE_REF().
*
* @param[in,out] evtRef_  event reference to create
* @param[in]     evtT_    event type (class name) of the event reference
*
* @usage
* The example **defer** in the directory `examples/win32/defer` illustrates
* the use of Q_NEW_REF()
*
* @sa Q_DELETE_REF()
*/
#define Q_NEW_REF(evtRef_, evtT_)  \
    ((evtRef_) = (evtT_ const *)QF_newRef_(e, (evtRef_)))

/*! Delete the event reference
* @description
* Every event reference created with the macro Q_NEW_REF() needs to be
* eventually deleted by means of the macro Q_DELETE_REF() to avoid leaking
* the event.
*
* @param[in,out] evtRef_  event reference to delete
*
* @usage
* The example **defer** in the directory `examples/win32/defer` illustrates
* the use of Q_DELETE_REF()
*
* @sa Q_NEW_REF()
*/
#define Q_DELETE_REF(evtRef_) do { \
    QF_deleteRef_((evtRef_));      \
    (evtRef_) = (void *)0;         \
} while (false)

/*! Recycle a dynamic event
* @static @private @memberof QF
*
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
void QF_gc(QEvt const * const e);

/*! Clear a specified region of memory to zero.
* @static @public @memberof QF
*
* @description
* Clears a memory buffer by writing zeros byte-by-byte.
*
* @param[in]  start  pointer to the beginning of a memory buffer.
* @param[in]  len    length of the memory buffer to clear (in bytes)
*
* @note The main application of this function is clearing the internal QF
* variables upon startup. This is done to avoid problems with non-standard
* startup code provided with some compilers and toolsets (e.g., TI DSPs or
* Microchip MPLAB), which does not zero the uninitialized variables, as
* required by the ANSI C standard.
*/
void QF_bzero(void * const start, uint_fast16_t len);

#ifndef QF_CRIT_EXIT_NOP
    /*! No-operation for exiting a critical section
    * @description
    * In some QF ports the critical section exit takes effect only on the
    * next machine instruction. If this next instruction is another entry
    * to a critical section, the critical section won't be really exited,
    * but rather the two adjacent critical sections would be merged.
    * The #QF_CRIT_EXIT_NOP() macro contains minimal code required to
    * prevent such merging of critical sections in QF ports, in which it
    * can occur.
    */
    #define QF_CRIT_EXIT_NOP()   ((void)0)
#endif

/*! array of registered active objects
* @note Not to be used by Clients directly, only in ports of QF
*/
extern QActive *QF_active_[QF_MAX_ACTIVE + 1U];

/*==========================================================================*/
/*! @brief "Ticker" Active Object class
* @class QTicker
* @extends QActive
*
* @description
* QTicker is an efficient active object specialized to process
* QF system clock tick at a specified tick rate [0..#QF_MAX_TICK_RATE].
* Placing system clock tick processing in an active object allows you
* to remove the non-deterministic QF_TICK_X() processing from the interrupt
* level and move it into the thread-level, where you can prioritize it
* as low as you wish.
*
* @usage
* The following example illustrates use of QTicker active objects:
* @include qf_ticker.c
*/
typedef struct {
    QActive super; /*!< @protected inherits ::QActive */
} QTicker;

/*! Constructor of the QTicker Active Object class
* @public @memberof QTicker
*/
void QTicker_ctor(QTicker * const me, uint_fast8_t tickRate);

#endif /* QF_H */
