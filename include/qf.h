/**
* \file
* \brief QF/C platform-independent public interface.
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
#ifndef qf_h
#define qf_h

/****************************************************************************/
#if (QF_MAX_ACTIVE < 1) || (63 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..63"
#endif

#ifndef QF_EVENT_SIZ_SIZE
    /*! Default value of the macro configurable value in qf_port.h */
    #define QF_EVENT_SIZ_SIZE 2
#endif
#if (QF_EVENT_SIZ_SIZE == 1)
    typedef uint8_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 2)
    /*! The data type to store the block-size defined based on
    * the macro #QF_EVENT_SIZ_SIZE. */
    /**
    * The dynamic range of this data type determines the maximum block
    * size that can be managed by the pool.
    */
    typedef uint16_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 4)
    typedef uint32_t QEvtSize;
#else
    #error "QF_EVENT_SIZ_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

#ifndef QF_MAX_EPOOL
    /*! Default value of the macro configurable value in qf_port.h */
    #define QF_MAX_EPOOL         3
#endif

#ifndef QF_MAX_TICK_RATE
    /*! Default value of the macro configurable value in qf_port.h     */
    #define QF_MAX_TICK_RATE     1
#endif

#ifndef QF_TIMEEVT_CTR_SIZE
    /*! macro to override the default ::QTimeEvtCtr size. Valid values:
    * 1, 2, or 4; default 2
    */
    #define QF_TIMEEVT_CTR_SIZE  2
#endif

/****************************************************************************/
struct QEQueue; /* forward declaration */

/*! QActive is the base structure for derivation of active objects */
/**
* \description
* Active objects in QP are encapsulated state machines (each embedding an
* event queue and a thread) that communicate with one another asynchronously
* by sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
*
* \note ::QActive is not intended to be instantiated directly, but rather
* serves as the base structure for derivation of active objects in the
* application code.
*
* \usage
* The following example illustrates how to derive an active object from
* QActive. Please note that the QActive member \c super is defined as the
* __first__ member of the derived struct (see \ref derivation).
* \include qf_qactive.c
*
* \sa ::QMActive
*/
typedef struct {
    QHsm super; /*!< inherits ::QHsm */

#ifdef QF_EQUEUE_TYPE
    /*! OS-dependent event-queue type. */
    /**
    * \description
    * The type of the queue depends on the underlying operating system or
    * a kernel. Many kernels support "message queues" that can be adapted
    * to deliver QF events to the active object. Alternatively, QF provides
    * a native event queue implementation that can be used as well.
    *
    * \note The native QF event queue is configured by defining the macro
    * #QF_EQUEUE_TYPE as ::QEQueue.
    */
    QF_EQUEUE_TYPE eQueue;
#endif

#ifdef QF_OS_OBJECT_TYPE
    /*! OS-dependent per-thread object. */
    /**
    * \description
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
    * \description
    * This data might be used in various ways, depending on the QF port.
    * In some ports thread is used to store the thread handle. In other
    * ports thread can be the pointer to the Thread-Local-Storage (TLS).
    */
    QF_THREAD_TYPE thread;
#endif

    /*! QF priority associated with the active object. */
    uint_fast8_t prio;

} QActive;

/*! Virtual table for the ::QActiveVtbl class */
typedef struct {
    QMsmVtbl super; /*!< inherits QMsmVtbl */

    /*! virtual function to start the active object (thread) */
    /** \sa QACTIVE_START() */
    void (*start)(QActive * const me, uint_fast8_t prio,
                  QEvt const *qSto[], uint_fast16_t qLen,
                  void *stkSto, uint_fast16_t stkSize,
                  QEvt const *ie);

#ifdef Q_SPY
    /*! virtual function to asynchronously post (FIFO) an event to an AO */
    /** \sa QACTIVE_POST() and QACTIVE_POST_X() */
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin, void const * const sender);
#else
    bool (*post)(QActive * const me, QEvt const * const e,
                 uint_fast16_t const margin);
#endif

    /*! virtual function to asynchronously post (LIFO) an event to an AO */
    /** \sa QACTIVE_POST_LIFO() */
    void (*postLIFO)(QActive * const me, QEvt const * const e);

} QActiveVtbl;

/* public functions */
/*! Implementation of the active object start operation. */
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie);

/*! Polymorphically start an active object. */
/**
* \description
* Performs the first step of FSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* \arguments
* \arg[in,out] \c me_      pointer (see \ref derivation)
* \arg[in]     \c prio_    priority at which to start the active object
* \arg[in]     \c qSto_    pointer to the storage for the ring buffer of the
*                          event queue (used only with the built-in ::QEQueue)
* \arg[in]     \c qLen_    length of the event queue (in events)
* \arg[in]     \c stkSto_  pointer to the stack storage (used only when
*                          per-AO stack is needed)
* \arg[in]     \c stkSize_ stack size (in bytes)
* \arg[in]     \c ie_      pointer to the optional initialization event
*                          (might be NULL).
*/
#define QACTIVE_START(me_, prio_, qSto_, qLen_, stkSto_, stkLen_, ie_) \
    ((*((QActiveVtbl const *)((me_)->super.vptr))->start)( \
        (me_), (prio_), (qSto_), (qLen_), (stkSto_), (stkLen_), (ie_)))

#ifdef Q_SPY
    /*! Implementation of the active object post (FIFO) operation */
    bool QActive_post_(QActive * const me, QEvt const * const e,
                         uint_fast16_t const margin,
                         void const * const sender);

    /*! Polymorphically posts an event to an active object (FIFO)
    * with delivery guarantee. */
    /**
    * \description
    * This macro asserts if the queue overflows and cannot accept the event.
    *
    * \arguments
    * \arg[in,out] \c me_   pointer (see \ref derivation)
    * \arg[in]     \c e_    pointer to the event to post
    * \arg[in]     \c sender_ pointer to the sender object.
    *
    * \note The \c sendedr_ argument is actually only used when QS tracing
    * is enabled (macro #Q_SPY is defined). When QS software tracing is
    * disenabled, the QACTIVE_POST() macro does not pass the \c sender_
    * argument, so the overhead of passing this extra argument is entirely
    * avoided.
    *
    * \note the pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QACTIVE_POST() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the sender of the event.
    *
    * \sa #QACTIVE_POST_X, QActive_post_().
    */
    #define QACTIVE_POST(me_, e_, sender_) \
        ((void)(*((QActiveVtbl const *)((me_)->super.vptr))->post)((me_), \
         (e_), (uint_fast16_t)0, (sender_)))

    /*! Polymorphically posts an event to an active object (FIFO)
    * without delivery guarantee. */
    /**
    * \description
    * This macro does not assert if the queue overflows and cannot accept
    * the event with the specified margin of free slots remaining.
    *
    * \arguments
    * \arg[in,out] \c me_   pointer (see \ref derivation)
    * \arg[in]     \c e_    pointer to the event to post
    * \arg[in]     \c margin_ the minimum free slots in the queue, which
    *                         must still be available after posting the event
    * \arg[in]     \c sender_ pointer to the sender object.
    *
    * \returns 'true' if the posting succeeded, and 'false' if the posting
    * failed due to insufficient margin of free slots available in the queue.
    *
    * \note The \c sender_ argument is actually only used when QS tracing
    * is enabled (macro #Q_SPY is defined). When QS software tracing is
    * disabled, the QACTIVE_POST() macro does not pass the \c sender_
    * argument, so the overhead of passing this extra argument is entirely
    * avoided.
    *
    * \note the pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QACTIVE_POST() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the sender of the event.
    *
    * \usage
    * \include qf_postx.c
    */
    #define QACTIVE_POST_X(me_, e_, margin_, sender_) \
        ((*((QActiveVtbl const *)((me_)->super.vptr))->post)((me_), \
         (e_), (margin_), (sender_)))
#else

    bool QActive_post_(QActive * const me, QEvt const * const e,
                         uint_fast16_t const margin);

    #define QACTIVE_POST(me_, e_, sender_) \
        ((void)(*((QActiveVtbl const *)((me_)->super.vptr))->post)((me_), \
                  (e_), (uint_fast16_t)0))

    #define QACTIVE_POST_X(me_, e_, margin_, sender_) \
        ((*((QActiveVtbl const *)((me_)->super.vptr))->post)((me_), \
                  (e_), (margin_)))

#endif

/*! Implementation of the active object post LIFO operation */
void QActive_postLIFO_(QActive * const me, QEvt const * const e);

/*! Polymorphically posts an event to an active object using the
* Last-In-First-Out (LIFO) policy. */
/**
* \arguments
* \arg[in,out] \c me_   pointer (see \ref derivation)
* \arg[in]     \c e_    pointer to the event to post
*/
#define QACTIVE_POST_LIFO(me_, e_) \
    ((*((QActiveVtbl const *)((me_)->super.vptr))->postLIFO)((me_), (e_)))


/* protected functions ...*/

/*! protected "constructor" of an active object. */
void QActive_ctor(QActive * const me, QStateHandler initial);

/*! Stops execution of an active object and removes it from the
* framework's supervision. */
void QActive_stop(QActive * const me);

/*! Subscribes for delivery of signal \a sig to the active object \a me. */
void QActive_subscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of signal \a sig to the AO \a me. */
void QActive_unsubscribe(QActive const * const me, enum_t const sig);

/*! Un-subscribes from the delivery of all signals to the AO \a me. */
void QActive_unsubscribeAll(QActive const * const me);


/*! Defer an event to a given separate event queue. */
bool QActive_defer(QActive * const me,
                   QEQueue * const eq, QEvt const * const e);

/*! Recall a deferred event from a given event queue. */
bool QActive_recall(QActive * const me, QEQueue * const eq);

/*! Get an event from the event queue of an active object. */
QEvt const *QActive_get_(QActive *const me);

/****************************************************************************/
/*! QM Active Object */
/**
* \description
* QMActive represents an active object version based on the ::QMsm state
* machine. The application-level active object derived from QMActive
* typically require the use of QM, but are the fastest and need the least
* run-time support (the smallest event-processor taking up the least code
* space). QMActive inherits QActive "as is" without adding new attributes,
* so it is typedef'ed as ::QActive.
*/
typedef QActive QMActive;

/*! protected "constructor" of an QMActive active object. */
void QMActive_ctor(QMActive * const me, QStateHandler initial);

/****************************************************************************/
#if (QF_TIMEEVT_CTR_SIZE == 1)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2)

    /*! type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks. */
    /**
    * \description
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: \n
    * uint8_t when (QF_TIMEEVT_CTR_SIZE == 1), and \n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4).
    */
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMEEVT_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/*! Time Event structure */
/**
* \description
* Time events are special QF events equipped with the notion of time passage.
* The basic usage model of the time events is as follows. An active object
* allocates one or more QTimeEvt objects (provides the storage for them).
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
* \sa ::QTimeEvt for the description of the data members \n \ref derivation
*
* \note QF manages the time events in the function QF_tickX_(), which
* must be called periodically, preferably from the clock tick ISR.
*
* \note In this version of QF QTimeEvt objects should be allocated statically
* rather than dynamically from event pools. Currently, QF will not correctly
* recycle the dynamically allocated Time Events.
*/
typedef struct QTimeEvt {
    /*! base structure from which QTimeEvt derives */
    QEvt super;

    /*! link to the next time event in the list */
    struct QTimeEvt * volatile next;

    /*! the active object that receives the time events */
    void * volatile act;

    /*! the internal down-counter of the time event. */
    /**
    * \description
    * The down-counter is decremented by 1 in every QF_tickX_() invocation.
    * The time event fires (gets posted or published) when the down-counter
    * reaches zero.
    */
    QTimeEvtCtr volatile ctr;

    /*! the interval for periodic time event (zero for one-shot time event) */
    /**
    * \description
    * The value of the interval is re-loaded to the internal down-counter
    * when the time event expires, so that the time event keeps timing out
    * periodically.
    */
    QTimeEvtCtr interval;
} QTimeEvt;

/* public functions */

/*! The extended "constructor" to initialize a Time Event. */
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate);

/*! Arm a time event (one shot or periodic) for direct event posting. */
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval);

/*! Rearm a time event. */
bool QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks);

/*! Disarm a time event. */
bool QTimeEvt_disarm(QTimeEvt * const me);

/*! Get the current value of the down-counter of a time event. */
QTimeEvtCtr QTimeEvt_ctr(QTimeEvt const * const me);

/****************************************************************************/
/* QF facilities */

/*! Subscriber-List structure */
/**
* \description
* This data type represents a set of active objects that subscribe to
* a given signal. The set is represented as an array of bits, where each
* bit corresponds to the unique priority of an active object.
*
* \sa ::QSubscrList for the description of the data members
*/
typedef struct {

    /*! An array of bits representing subscriber active objects. */
    /**
    * \description
    * Each bit in the array corresponds to the unique priority of the AO.
    * The size of the array is determined of the maximum number of AOs
    * in the application configured by the #QF_MAX_ACTIVE macro.
    * For example, an active object of priority p is a subscriber if the
    * following is true: ((bits[QF_div8Lkup[p]] & QF_pwr2Lkup[p]) != 0)
    *
    * \sa QF_psInit(), ::QF_div8Lkup, ::QF_pwr2Lkup, #QF_MAX_ACTIVE
    */
    uint8_t bits[((QF_MAX_ACTIVE - 1) / 8) + 1];
} QSubscrList;

/* public functions */

/*! QF initialization. */
void QF_init(void);

/*! Publish-subscribe initialization. */
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal);

/*! Event pool initialization for dynamic allocation of events. */
void QF_poolInit(void * const poolSto, uint_fast16_t const poolSize,
                 uint_fast16_t const evtSize);

/*! Transfers control to QF to run the application. */
int_t QF_run(void);

/*! Function invoked by the application layer to stop the QF
* application and return control to the OS/Kernel. */
void QF_stop(void);

/*! Startup QF callback. */
/**
* \description
* The timeline for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*/
void QF_onStartup(void);

/*! Cleanup QF callback. */
/**
* \description
* QF_onCleanup() is called in some QF ports before QF returns to the
* underlying operating system or RTOS.
*
* This function is strongly platform-specific and is not implemented in
* the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. Some QF ports might not require implementing
* QF_onCleanup() at all, because many embedded applications don't have
* anything to exit to.
*
* \sa QF_stop()
*/
void QF_onCleanup(void);

#ifdef Q_SPY

    /*! Publish event to the framework. */
    void QF_publish_(QEvt const * const e, void const * const sender);

    /*! Invoke the event publishing facility QF_publish_(). */
    /**
    * \description
    * This macro is the recommended way of publishing events, because it
    * provides the vital information for software tracing and avoids any
    * overhead when the tracing is disabled.
    *
    * \arguments
    * \arg[in] \c e_      pointer to the posted event
    * \arg[in] \c sender_ pointer to the sender object. This argument is
    *          actually only used when QS software tracing is enabled
    *          (macro #Q_SPY is defined). When QS software tracing is
    *          disabled, the macro calls QF_publish_() without the
    *          \c sender_ argument, so the overhead of passing this
    *          extra argument is entirely avoided.
    *
    * \note the pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, if QF_PUBLISH() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the publisher of the event.
    *
    * \sa QF_publish_().
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
    * \description
    * This macro is the recommended way of invoking clock tick processing,
    * because it provides the vital information for software tracing and
    * avoids any overhead when the tracing is disabled.
    *
    * \arguments
    * \arg[in] \c tickRate clock tick rate to be serviced through this call
    * \arg[in] \c sender   pointer to the sender object. This argument
    *            is actually only used when QS software tracing is enabled
    *            (macro #Q_SPY is defined)
    * \note
    * When QS software tracing is disabled, the macro calls QF_tickX_()
    * without the \c sender argument, so the overhead of passing this
    * extra argument is entirely avoided.
    *
    * \note
    * The pointer to the sender object is not necessarily a pointer
    * to an active object. In fact, when #QF_TICK_X() is called from
    * an interrupt, you would create a unique object just to unambiguously
    * identify the ISR as the sender of the time events.
    *
    * \sa QF_tickX_().
    */
    #define QF_TICK_X(tickRate_, sender_) (QF_tickX_((tickRate_), (sender_)))

#else

    void QF_tickX_(uint_fast8_t const tickRate);
    #define QF_TICK_X(tickRate_, dummy)   (QF_tickX_(tickRate_))

#endif

/*! Invoke the system clock tick processing for rate 0 */
#define QF_TICK(sender_)   QF_TICK_X((uint_fast8_t)0, (sender_))

/*! Returns 'true' if there are no armed time events at a given tick rate */
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate);

/*! Register an active object to be managed by the framework */
void QF_add_(QActive * const a);

/*! Remove the active object from the framework. */
void QF_remove_(QActive const * const a);

/*! Obtain the minimum of free entries of the given event pool. */
uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId);

/*! This function returns the minimum of free entries of
* the given event queue. */
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio);

/*! Internal QP implementation of the dynamic event allocator. */
QEvt *QF_newX_(uint_fast16_t const evtSize,
               uint_fast16_t const margin, enum_t const sig);

#ifdef Q_EVT_CTOR /* Shall the constructor for the QEvt class be provided? */

    #define Q_NEW(evtT_, sig_, ...) \
        (evtT_##_ctor((evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                      (uint_fast16_t)0, (enum_t)0), (sig_), ##__VA_ARGS__))

    #define Q_NEW_X(e_, evtT_, margin_, sig_, ...) do { \
        (e_) = (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                                 (margin_), (enum_t)0); \
        if ((e_) != (evtT_ *)0) { \
            evtT_##_ctor((e_), (sig_), ##__VA_ARGS__); \
        } \
     } while (0)

#else

    /*! Allocate a dynamic event. */
    /**
    * \description
    * The macro calls the internal QF function QF::newX_() with
    * margin == 0, which causes an assertion when the event cannot be
    * successfully allocated.
    *
    * \arguments
    * \arg[in] \c evtT_ event type (class name) of the event to allocate
    * \arg[in] \c sig_  signal to assign to the newly allocated event
    *
    * \returns a valid event pointer cast to the type \a evtT_.
    *
    * \note
    * If #Q_EVT_CTOR is defined, the Q_NEW() macro becomes variadic and
    * takes all the arguments needed by the constructor of the event
    * class being allocated. The constructor is then called by means
    * of the placement-new operator.
    *
    * \usage
    * The following example illustrates dynamic allocation of an event:
    * \include qf_post.c
    */
    #define Q_NEW(evtT_, sig_) \
        ((evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), \
                           (uint_fast16_t)0, (sig_)))

    /*! Allocate a dynamic event (non-asserting version). */
    /**
    * \description
    * \description
    * This macro allocates a new event and sets the pointer \a e_, while
    * leaving at least \a margin_ of events still available in the pool
    *
    * \arguments
    * \arg[in] \c evtT_   event type (class name) of the event to allocate
    * \arg[in] \c margin_ number of events that must remain available
    *                     in the given pool after this allocation
    * \arg[in] \c sig_    signal to assign to the newly allocated event
    *
    * \returns an event pointer cast to the type \a evtT_ or NULL if the
    * event cannot be allocated with the specified \a margin.
    *
    * \note
    * If #Q_EVT_CTOR is defined, the Q_NEW_X() macro becomes variadic and
    * takes all the arguments needed by the constructor of the event
    * class being allocated. The constructor is then called and all the
    * extra arguments are passed to it.
    *
    * \usage
    * The following example illustrates dynamic allocation of an event:
    * \include qf_postx.c
    */
    #define Q_NEW_X(e_, evtT_, margin_, sig_) ((e_) = \
        (evtT_ *)QF_newX_((uint_fast16_t)sizeof(evtT_), (margin_), (sig_)))

#endif /* Q_EVT_CTOR */


/*! Recycle a dynamic event. */
void QF_gc(QEvt const * const e);

/*! Clear a specified region of memory to zero. */
void QF_bzero(void * const start, uint_fast16_t len);

#ifndef QF_CRIT_EXIT_NOP
    /*! No-operation for exiting a critical section */
    /**
    * \description
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

/****************************************************************************/
/* Useful lookup tables ...*/
/*! Lookup table for \c (1 << ((n-1) % 8)), where n is the index into
* the table. */
/**
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM QF_pwr2Lkup[65];

/*! Lookup table for \c ~(1 << ((n-1) % 8)), where n is the index
* into the table. */
/**
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM QF_invPwr2Lkup[65];

/*! Lookup table for \c (n-1)/8 , where n is the index into the table. */
/**
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM QF_div8Lkup[65];

/* Log-base-2 calculations ...*/
#ifndef QF_LOG2

    /*! Macro to return (log2(n_) + 1), where \a n_ = 0..255. */
    /**
    * \description
    * This macro delivers the 1-based number of the most significant 1-bit
    * of a byte. This macro can be re-implemented in the QP ports, if the CPU
    * supports special instructions, such as CLZ (count leading zeros).
    *
    * If the macro is not defined in the port, the default implementation
    * uses a lookup table.
    */
    #define QF_LOG2(n_) (Q_ROM_BYTE(QF_log2Lkup[(n_)]))

    /*! Lookup table for (log2(n) + 1), where n is the index into the table */
    /**
    * \description
    * This lookup delivers the 1-based number of the most significant 1-bit
    * of a byte.
    */
    extern uint8_t const Q_ROM QF_log2Lkup[256];

    #define QF_LOG2LKUP 1

#endif /* QF_LOG2 */

/*! array of registered active objects */
/**
* \note Not to be used by Clients directly, only in ports of QF
*/
extern QActive *QF_active_[QF_MAX_ACTIVE + 1];

/****************************************************************************/
/*! Returns the QF version. */
/**
* \description
* version of QF as a constant 5-character string of the form X.Y.Z,
* where X is a 1-digit major version number, Y is a 1-digit minor
* version number, and Z is a 1-digit release number.
*/
#define QF_getVersion() (QP_VERSION_STR)

#endif /* qf_h */


