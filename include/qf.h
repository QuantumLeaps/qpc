/**
* @file
* @brief QF/C platform-independent public interface.
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.8.2
* Last updated on  2020-07-14
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QF_H
#define QF_H

#ifndef QPSET_H
#include "qpset.h"
#endif

/****************************************************************************/
#ifndef QF_EVENT_SIZ_SIZE
    /*! Default value of the macro configurable value in qf_port.h */
    #define QF_EVENT_SIZ_SIZE 2U
#endif
#if (QF_EVENT_SIZ_SIZE == 1U)
    typedef uint8_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 2U)
    /*! The data type to store the block-size defined based on
    * the macro #QF_EVENT_SIZ_SIZE. */
    /**
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

/****************************************************************************/
struct QEQueue; /* forward declaration */

/****************************************************************************/
/*! Active Object base class (based on ::QHsm implementation)
* @extends QHsm
*/
/**
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
* @usage
* The following example illustrates how to derive an active object from
* ::QActive. Please note that the ::QActive member @c super is defined as the
* __first__ member of the derived struct (see @ref oop).
* @include qf_qactive.c
*/
typedef struct QActive {
    QHsm super; /*!< inherits ::QHsm */

#ifdef QF_EQUEUE_TYPE
    /*! OS-dependent event-queue type. */
    /**
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
    /*! OS-dependent per-thread object. */
    /**
    * @description
    * This data might be used in various ways, depending on the QF port.
    * In some ports osObject is used to block the calling thread when
    * the native QF queue is empty. In other QF ports the OS-dependent
    * object might be used differently.
    */
    QF_OS_OBJECT_TYPE osObject;
#endif

#ifdef QF_THREAD_TYPE
    /*! OS-dependent representation of the thread of the active object. */
    /**
    * @description
    * This data might be used in various ways, depending on the QF port.
    * In some ports thread is used to store the thread handle. In other
    * ports thread can be the pointer to the Thread-Local-Storage (TLS).
    */
    QF_THREAD_TYPE thread;
#endif

#ifdef QXK_H  /* QXK kernel used? */
    /*! QXK dynamic priority (1..#QF_MAX_ACTIVE) of this AO/thread */
    uint8_t dynPrio;
#endif

    /*! QF priority (1..#QF_MAX_ACTIVE) of this active object. */
    uint8_t prio;

} QActive;

/*! Virtual table for the ::QActive class */
typedef struct {
    struct QHsmVtable super; /*!< inherits ::QHsmVtable */

    /*! virtual function to start the active object (thread) */
    /** @sa QACTIVE_START() */
    void (*start)(QActive * const me, uint_fast8_t prio,
                  QEvt const * * const qSto, uint_fast16_t const qLen,
                  void * const stkSto, uint_fast16_t const stkSize,
                  void const * const par);

#ifdef Q_SPY
    /*! virtual function to asynchronously post (FIFO) an event to an AO */
    /** @sa QACTIVE_POST() and QACTIVE_POST_X() */
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin, void const * const sender);
#else
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin);
#endif

    /*! virtual function to asynchronously post (LIFO) an event to an AO */
    /** @sa QACTIVE_POST_LIFO() */
    void (*postLIFO)(QActive * const me, QEvt const * const e);

} QActiveVtable;

/* QActive public operations... */
/*! Polymorphically start an active object.
* @public @memberof QActive
*/
/**
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
    * @public @memberof QActive
    */
    /**
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
    * @public @memberof QActive
    */
    /**
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
* @public @memberof QActive
*/
/**
* @param[in,out] me_   pointer (see @ref oop)
* @param[in]     e_    pointer to the event to post
*/
#define QACTIVE_POST_LIFO(me_, e_)                                      \
    ((*((QActiveVtable const *)((Q_HSM_UPCAST(me_))->vptr))->postLIFO)( \
        (me_), (e_)))

/* QActive protected operations... */
/*! protected "constructor" of an ::QActive active object
* @protected @memberof QActive
*/
void QActive_ctor(QActive * const me, QStateHandler initial);

#ifdef QF_ACTIVE_STOP
    /*! Stops execution of an active object and removes it from the
    * framework's supervision.
    * @protected @memberof QActive
    */
    /** @attention
    * QActive_stop() must be called only from the AO that is about
    * to stop its execution. By that time, any pointers or references
    * to the AO are considered invalid (dangling) and it becomes
    * illegal for the rest of the application to post events to the AO.
    */
    void QActive_stop(QActive * const me);
#endif

/*! Subscribes for delivery of signal @p sig to the active object @p me.
* @protected @memberof QActive
*/
void QActive_subscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of signal @p sig to the AO @p me.
* @protected @memberof QActive
*/
void QActive_unsubscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of all signals to the AO @p me.
* @protected @memberof QActive
*/
void QActive_unsubscribeAll(QActive const * const me);

/*! Defer an event @p e to a given event queue @p eq.
* @protected @memberof QActive
*/
bool QActive_defer(QActive const * const me,
                   QEQueue * const eq, QEvt const * const e);

/*! Recall a deferred event from a given event queue @p eq.
* @protected @memberof QActive
*/
bool QActive_recall(QActive * const me, QEQueue * const eq);

/*! Flush the specified deferred queue @p eq.
* @protected @memberof QActive
*/
uint_fast16_t QActive_flushDeferred(QActive const * const me,
                                    QEQueue * const eq);

/*! Generic setting of additional attributes (useful in QP ports)
* @protected @memberof QActive
*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2);


/****************************************************************************/
/*! QMActive active object base class (based on ::QMsm implementation)
* @extends QActvie
*/
/**
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
* @sa ::QActive
*
* @usage
* The following example illustrates how to derive an active object from
* ::QMActive. Please note that the ::QActive member @c super is defined as
* the __first__ member of the derived struct (see @ref oop).
* @include qf_qmactive.c
*/
typedef struct {
    QActive super; /*!< inherits ::QActive */
} QMActive;

/*! Virtual Table for the ::QMActive class (inherited from ::QActiveVtable */
/**
* @note
* ::QMActive inherits ::QActive exactly, without adding any new virtual
* functions and therefore, ::QMActiveVtable is typedef'ed as ::QActiveVtable.
*/
typedef QActiveVtable QMActiveVtable;

/* QMActive protected operations... */
/*! protected "constructor" of an ::QMActive active object.
* @protected @memberof QMActive
*/
void QMActive_ctor(QMActive * const me, QStateHandler initial);


/****************************************************************************/
#if (QF_TIMEEVT_CTR_SIZE == 1U)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2U)

    /*! type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks. */
    /**
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

/*! Time Event class
* @extends QEvt
*/
/**
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
* invocation of the QF_tickX_() function. Only armed (timing out) time events
* are in the list, so only armed time events consume CPU cycles.
*
* @sa ::QTimeEvt for the description of the data members @n @ref oop
*
* @note
* QF manages the time events in the function QF_tickX_(), which must be called
* periodically, from the clock tick ISR or from the special ::QTicker
* active object.
*
* @note
* Even though ::QTimeEvt is a subclass of ::QEvt, ::QTimeEvt instances can NOT
* be allocated dynamically from event pools. In other words, it is illegal to
* allocate ::QTimeEvt instances with the Q_NEW() or Q_NEW_X() macros.
*/
typedef struct QTimeEvt {
    QEvt super; /*<! inherits ::QEvt */

    /*! link to the next time event in the list */
    struct QTimeEvt * volatile next;

    /*! the active object that receives the time events */
    void * volatile act;

    /*! the internal down-counter of the time event. */
    /**
    * @description
    * The down-counter is decremented by 1 in every QF_tickX_() invocation.
    * The time event fires (gets posted or published) when the down-counter
    * reaches zero.
    */
    QTimeEvtCtr volatile ctr;

    /*! the interval for periodic time event (zero for one-shot time event) */
    /**
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
*/
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate);

/*! Arm a time event (one shot or periodic) for direct event posting.
* @public @memberof QTimeEvt
*/
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval);

/*! Rearm a time event.
* @public @memberof QTimeEvt
*/
bool QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks);

/*! Disarm a time event.
* @public @memberof QTimeEvt
*/
bool QTimeEvt_disarm(QTimeEvt * const me);

/*! Check the "was disarmed" status of a time event.
* @public @memberof QTimeEvt
*/
bool QTimeEvt_wasDisarmed(QTimeEvt * const me);

/*! Get the current value of the down-counter of a time event.
* @public @memberof QTimeEvt
*/
QTimeEvtCtr QTimeEvt_currCtr(QTimeEvt const * const me);


/****************************************************************************/
/* QF facilities */

/*! Subscriber-List structure */
/**
* @description
* This data type represents a set of active objects that subscribe to
* a given signal. The set is represented as a priority-set, where each
* bit corresponds to the unique priority of an active object.
*
* @sa ::QSubscrList for the description of the data members
*/
typedef QPSet QSubscrList;

/* public functions */

/*! QF initialization. */
void QF_init(void);

/*! Publish-subscribe initialization. */
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal);

/*! Event pool initialization for dynamic allocation of events. */
void QF_poolInit(void * const poolSto, uint_fast32_t const poolSize,
                 uint_fast16_t const evtSize);

/*! Obtain the block size of any registered event pools */
uint_fast16_t QF_poolGetMaxBlockSize(void);

/*! Transfers control to QF to run the application. */
int_t QF_run(void);

/*! Function invoked by the application layer to stop the QF
* application and return control to the OS/Kernel. */
void QF_stop(void);

/*! Startup QF callback. */
/**
* @description
* The timeline for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*/
void QF_onStartup(void);

/*! Cleanup QF callback. */
/**
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
*/
void QF_onCleanup(void);

#ifdef Q_SPY

    /*! Publish event to the framework. */
    void QF_publish_(QEvt const * const e, void const * const sender);

    /*! Invoke the event publishing facility QF_publish_(). */
    /**
    * @description
    * This macro is the recommended way of publishing events, because it
    * provides the vital information for software tracing and avoids any
    * overhead when the tracing is disabled.
    *
    * @param[in] e_      pointer to the posted event
    * @param[in] sender_ pointer to the sender object. This argument is
    *          actually only used when QS software tracing is enabled
    *          (macro #Q_SPY is defined). When QS software tracing is
    *          disabled, the macro calls QF_publish_() without the
    *          @p sender_ parameter, so the overhead of passing this
    *          extra argument is entirely avoided.
    *
    * @note
    * the pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QF_PUBLISH() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the publisher of the event.
    *
    * @sa QF_publish_().
    */
    #define QF_PUBLISH(e_, sender_) \
        (QF_publish_((e_), (void const *)(sender_)))
#else

    void QF_publish_(QEvt const * const e);
    #define QF_PUBLISH(e_, dummy_)   (QF_publish_(e_))

#endif

#ifdef Q_SPY

    /*! Processes all armed time events at every clock tick. */
    void QF_tickX_(uint_fast8_t const tickRate, void const * const sender);

    /*! Invoke the system clock tick processing QF_tickX_(). */
    /**
    * @description
    * This macro is the recommended way of invoking clock tick processing,
    * because it provides the vital information for software tracing and
    * avoids any overhead when the tracing is disabled.
    *
    * @param[in] tickRate_ clock tick rate to be serviced through this call
    * @param[in] sender_   pointer to the sender object. This argument
    *            is actually only used when QS software tracing is enabled
    *            (macro #Q_SPY is defined)
    * @note
    * When QS software tracing is disabled, the macro calls QF_tickX_()
    * without the @p sender parameter, so the overhead of passing this
    * extra argument is entirely avoided.
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
    * @sa QF_tickX_().
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

/*! Invoke the system clock tick processing for rate 0 */
#define QF_TICK(sender_)   QF_TICK_X(0U, (sender_))

/*! Returns 'true' if there are no armed time events at a given tick rate */
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate);

/*! Register an active object to be managed by the framework */
void QF_add_(QActive * const a);

/*! Remove the active object from the framework. */
void QF_remove_(QActive * const a);

/*! Obtain the minimum of free entries of the given event pool. */
uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId);

/*! This function returns the minimum of free entries of
* the given event queue. */
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio);

/*! Internal QF implementation of creating new dynamic event. */
QEvt *QF_newX_(uint_fast16_t const evtSize,
               uint_fast16_t const margin, enum_t const sig);

/*! Internal QF implementation of creating new event reference. */
QEvt const *QF_newRef_(QEvt const * const e, void const * const evtRef);

/*! Internal QF implementation of deleting event reference. */
void QF_deleteRef_(void const * const evtRef);

#ifdef Q_EVT_CTOR /* Shall the ctor for the ::QEvt class be provided? */

    #define Q_NEW(evtT_, sig_, ...)                                   \
        (evtT_##_ctor((evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                      QF_NO_MARGIN, 0), (sig_), ##__VA_ARGS__))

    #define Q_NEW_X(e_, evtT_, margin_, sig_, ...) do {        \
        (e_) = (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                                 (margin_), 0);                \
        if ((e_) != (evtT_ *)0) {                              \
            evtT_##_ctor((e_), (sig_), ##__VA_ARGS__);         \
        }                                                      \
     } while (false)

#else

    /*! Allocate a dynamic event. */
    /**
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
                           QF_NO_MARGIN, (sig_)))

    /*! Allocate a dynamic event (non-asserting version). */
    /**
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
    #define Q_NEW_X(e_, evtT_, margin_, sig_) ((e_) = \
        (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), (margin_), (sig_)))

#endif /* Q_EVT_CTOR */

/*! Create a new reference of the current event `e` */
/**
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

/*! Delete the event reference */
/**
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

/*! Recycle a dynamic event. */
void QF_gc(QEvt const * const e);

/*! Clear a specified region of memory to zero. */
void QF_bzero(void * const start, uint_fast16_t len);

#ifndef QF_CRIT_EXIT_NOP
    /*! No-operation for exiting a critical section */
    /**
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

/*! array of registered active objects */
/**
* @note Not to be used by Clients directly, only in ports of QF
*/
extern QActive *QF_active_[QF_MAX_ACTIVE + 1U];


/****************************************************************************/
/*! QTicker Active Object class
* @extends QActive
*/
/**
* @description
* The QTicker is an efficient active object specialized to process
* QF system clock tick at a specified tick frequency [0..::QF_MAX_TICK_RATE].
* Placing system clock tick processing in an active object allows you
* to remove the non-deterministic QF::TICK_X() processing from the interrupt
* level and move it into the thread-level, where you can prioritize it
* as low as you wish.
*/
typedef struct {
    QActive super; /*!< inherits ::QActive */
} QTicker;

/*! Constructor of the QTicker Active Object class */
void QTicker_ctor(QTicker * const me, uint_fast8_t tickRate);

#endif /* QF_H */

