/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 25, 2012
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
#ifndef qf_h
#define qf_h

/**
* \file
* \ingroup qf qk
* \brief QF/C platform-independent public interface.
*
* This header file must be included directly or indirectly
* in all modules (*.c files) that use QF or QK.
*/

/****************************************************************************/
#if (QF_MAX_ACTIVE < 1) || (63 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..63"
#endif

/****************************************************************************/
#ifndef QF_EVENT_SIZ_SIZE
    /** \brief Default value of the macro configurable value in qf_port.h
    */
    #define QF_EVENT_SIZ_SIZE 2
#endif
#if (QF_EVENT_SIZ_SIZE == 1)
    /** \brief The data type to store the block-size defined based on
    * the macro #QF_EVENT_SIZ_SIZE.
    *
    * The dynamic range of this data type determines the maximum block
    * size that can be managed by the pool.
    */
    typedef uint8_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 2)
    typedef uint16_t QEvtSize;
#elif (QF_EVENT_SIZ_SIZE == 4)
    typedef uint32_t QEvtSize;
#else
    #error "QF_EVENT_SIZ_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
#ifndef QF_MAX_EPOOL
    /** \brief Default value of the macro configurable value in qf_port.h
    */
    #define QF_MAX_EPOOL 3
#endif

/****************************************************************************/
#ifndef QF_ACTIVE_SUPER_
    /** \brief the base structure for derivation of the ::QActive structure.
    *
    * By default, the ::QActive structure is derived from ::QHsm. However,
    * if the macro #QF_ACTIVE_SUPER_ is defined, QActive is derived from
    * QF_ACTIVE_SUPER.
    *
    * Clients might choose, for example, to define QF_ACTIVE_SUPER_ as QFsm
    * to avoid the 1-2KB overhead of the hierarchical event processor.
    *
    * Clients might also choose to define QF_ACTIVE_SUPER_ as their own
    * completely customized class that has nothing to do with QHsm or QFsm.
    * The QF_ACTIVE_SUPER_ struct must provide functions XXX_init() and
    * XXX_dispatch(), consistent with the signatures of QHsm_init() and
    * QHsm_dispatch(). But the implementatin of these functions is
    * completely open.
    *
    * \sa \ref derivation
    */
    #define QF_ACTIVE_SUPER_               QHsm

    /** \brief The "constructor" of the base structure for ::QActive.
    * \note this macro must be consistenet with #QF_ACTIVE_SUPER_ definition.
    */
    #define QF_ACTIVE_CTOR_(me_, initial_) QHsm_ctor((me_), (initial_))

    /** \brief The initialization of the ::QActive state machine.
    * \note this macro must be consistenet with #QF_ACTIVE_SUPER_ definition.
    */
    #define QF_ACTIVE_INIT_(me_, e_)       QHsm_init((me_), (e_))

    /** \brief The dispatching events to the ::QActive state machine.
    * \note this macro must be consistenet with #QF_ACTIVE_SUPER_ definition.
    */
    #define QF_ACTIVE_DISPATCH_(me_, e_)   QHsm_dispatch((me_), (e_))

    /** \brief The argument of the base class' constructor.
    */
    #define QF_ACTIVE_STATE_               QStateHandler

#endif

struct QEQueueTag;                                   /* forward declaration */

/****************************************************************************/
/** \brief Active Object structure
*
* QActive is the base structure for derivation of active objects. Active
* objects in QF are encapsulated tasks (each embedding a state machine and
* an event queue) that communicate with one another asynchronously by
* sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
*
* \note QActive is not intended to be instantiated directly, but rather
* serves as the base structure for derivation of active objects in the
* application code.
*
* The following example illustrates how to derive an active object from
* QActive. Please note that the QActive member super_ is defined as the
* FIRST member of the derived struct.
* \include qf_qactive.c
*
* \sa ::QActiveTag for the description of the data members \n \ref derivation
*/
typedef struct QActiveTag {
    /** base structure of QActive.
    * \sa ::QF_ACTIVE_SUPER_ \n \ref derivation
    */
    QF_ACTIVE_SUPER_ super;

#ifdef QF_EQUEUE_TYPE
    /** \brief OS-dependent event-queue type.
    *
    * The type of the queue depends on the underlying operating system or
    * a kernel. Many kernels support "message queues" that can be adapted
    * to deliver QF events to the active object. Alternatively, QF provides
    * a native event queue implementation that can be used as well.
    *
    * The native QF event queue is configured by defining the macro
    * #QF_EQUEUE_TYPE as ::QEQueue.
    */
    QF_EQUEUE_TYPE eQueue;
#endif

#ifdef QF_OS_OBJECT_TYPE
    /** \brief OS-dependent per-thread object.
    *
    * This data might be used in various ways, depending on the QF port.
    * In some ports osObject is used to block the calling thread when
    * the native QF queue is empty. In other QF ports the OS-dependent
    * object might be used differently.
    */
    QF_OS_OBJECT_TYPE osObject;
#endif

#ifdef QF_THREAD_TYPE
    /** \brief OS-dependent representation of the thread of the active object.
    *
    * This data might be used in various ways, depending on the QF port.
    * In some ports thread is used to store the thread handle. In other
    * ports thread can be the pointer to the Thread-Local-Storage (TLS).
    */
    QF_THREAD_TYPE thread;
#endif

    /** \brief QF priority associated with the active object.
    * \sa QActive_start()
    */
    uint8_t prio;

} QActive;

/* public functions */
/** \brief Starts execution of an active object and registers the object
* with the framework.
*
* The function takes seven arguments.
* \a me is a pointer to the active object structure.
* \a prio is the priority of the active object. QF allows you to start up
* to 63 active objects, each one having a unique priority number between
* 1 and 63 inclusive, where higher numerical values correspond to higher
* priority (urgency) of the active object relative to the others.
* \a qSto[] and \a qLen arguments are the storage and size of the event
* queue used by this active object.
* \a stkSto and \a stkSize are the stack storage and size in bytes. Please
* note that a per-active object stack is used only when the underlying OS
* requies it. If the stack is not required, or the underlying OS allocates
* the stack internally, the \a stkSto should be NULL and/or \a stkSize
* should be 0.
* \a ie is an optional initialization event that can be used to pass
* additional startup data to the active object. (Pass NULL if your active
* object does not expect the initialization event).
*
* \note This function is strongly OS-dependent and must be defined in the
* QF port to a particular platform.
*
* The following example shows starting of the Philosopher object when a
* per-task stack is required:
* \include qf_start.c
*/
void QActive_start(QActive * const me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie);

#ifdef Q_SPY

    /** \brief Posts an event \a e directly to the event queue of the acitve
    * object \a me using the First-In-First-Out (FIFO) policy.
    *
    * \note this function should be called only via the macro #QACTIVE_POST
    *
    * Direct event posting is the simplest asynchronous communication method
    * available in QF. The following example illustrates how the Philo active
    * object posts directly the HUNGRY event to the Table active object.
    * \include qf_post.c
    *
    * \note The producer of the event (Philo in this case) must only "know"
    * the recipient (Table) by a generic (QActive *AO_Table) pointer, but
    * the specific definition of the Table structure is not required.
    *
    * \note Direct event posting should not be confused with direct event
    * dispatching. In contrast to asynchronous event posting through event
    * queues, direct event dispatching is synchronous. Direct event
    * dispatching occurs when you call QHsm_dispatch(), or QFsm_dispatch()
    * function.
    */
    void QActive_postFIFO(QActive * const me, QEvt const * const e,
                          void const * const sender);

    /** \brief Invoke the direct event posting facility QActive_postFIFO().
    * This macro is the recommended way of posting events, because it provides
    * the vital information for software tracing and avoids any overhead when
    * the tracing is disabled.
    *
    * This macro takes the last argument \a sender_, which is a pointer to
    * the sender object. This argument is actually only used when QS
    * tracing is disabled (macro #Q_SPY is defined). When QS software
    * tracing is not enabled, the macro calls QActive_postFIFO() without
    * the \a sender_ argument, so the overhead of passing this extra
    * argument is entirely avoided.
    *
    * \note the pointer to the sender object is not necessarily a poiner
    * to an active object. In fact, if QACTIVE_POST() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the publisher of the event.
    *
    * \sa ::QActive_postFIFO().
    */
    #define QACTIVE_POST(me_, e_, sender_) \
        QActive_postFIFO((me_), (e_), (void const *)(sender_))

#else

    void QActive_postFIFO(QActive * const me, QEvt const * const e);
    #define QACTIVE_POST(me_, e_, dummy_) QActive_postFIFO((me_), (e_))

#endif

/** \brief Posts an event directly to the event queue of the active object
* \a me using the Last-In-First-Out (LIFO) policy.
*
* \note The LIFO policy should be used only for self-posting and with
* caution because it alters order of events in the queue.
*
* \sa QActive_postFIFO()
*/
void QActive_postLIFO(QActive * const me, QEvt const * const e);

/* protected functions ...*/

/** \brief protected "constructor" of an active object.
* Performs the first step of active object initialization by assigning the
* initial pseudostate to the currently active state of the state machine.
*
* \note Must be called only by the "constructors" of the derived active
* objects, as shown in the following example:
* \include qf_ctor.c
*
* \note Must be called before QActive_start().
*
* The following example illustrates how to invoke QFsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qhsm_ctor.c
*
* \sa #QHsm_ctor and #QFsm_ctor
*/
#define QActive_ctor(me_, initial_) QF_ACTIVE_CTOR_(&(me_)->super, (initial_))

/** \brief Stops execution of an active object and removes it from the
* framework's supervision.
*
* The preferred way of calling this function is from within the active
* object that needs to stop. In other words, an active object should stop
* itself rather than being stopped by some other entity. This policy works
* best, because only the active object itself "knows" when it has reached
* the appropriate state for the shutdown.
*
* \note This function is strongly OS-dependent and should be defined in the
* QF port to a particular platform. This function is optional in embedded
* systems where active objects never need to be stopped.
*/
void QActive_stop(QActive * const me);

/** \brief Subscribes for delivery of signal \a sig to the active object
* \a me.
*
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Subscribing to an event means that the framework will
* start posting all published events with a given signal \a sig to the
* event queue of the active object \a me.
*
* The following example shows how the Table active object subscribes
* to three signals in the initial transition:
* \include qf_subscribe.c
*
* \sa QF_publish(), QActive_unsubscribe(), and QActive_unsubscribeAll()
*/
void QActive_subscribe(QActive const * const me, enum_t const sig);

/** \brief Un-subscribes from the delivery of signal \a sig to the
* active object \a me.
*
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from an event means that the framework
* will stop posting published events with a given signal \a sig to the
* event queue of the active object \a me.
*
* \note Due to the latency of event queues, an active object should NOT
* assume that a given signal \a sig will never be dispatched to the
* state machine of the active object after un-subscribing from that signal.
* The event might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events.
*
* \note Un-subscribing from a signal that has never been subscribed in the
* first place is considered an error and QF will rise an assertion.
*
* \sa QF_publish(), QActive_subscribe(), and QActive_unsubscribeAll()
*/
void QActive_unsubscribe(QActive const * const me, enum_t const sig);

/** \brief Un-subscribes from the delivery of all signals to the active
* object \a me.
*
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from all events means that the framework
* will stop posting any published events to the event queue of the active
* object \a me.
*
* \note Due to the latency of event queues, an active object should NOT
* assume that no events will ever be dispatched to the state machine of
* the active object after un-subscribing from all events.
* The events might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events. Also, the
* alternative event-delivery mechanisms, such as direct event posting or
* time events, can be still delivered to the event queue of the active
* object.
*
* \sa QF_publish(), QActive_subscribe(), and QActive_unsubscribe()
*/
void QActive_unsubscribeAll(QActive const * const me);


/** \brief Defer an event to a given separate event queue.
*
* This function is part of the event deferral support. An active object
* uses this function to defer an event \a e to the QF-supported native
* event queue \a eq. QF correctly accounts for another outstanding
* reference to the event and will not recycle the event at the end of
* the RTC step. Later, the active object might recall one event at a
* time from the event queue.
*
* An active object can use multiple event queues to defer events of
* different kinds.
*
* \sa QActive_recall(), QEQueue
*/
void QActive_defer(QActive * const me,
                   QEQueue * const eq, QEvt const * const e);

/** \brief Recall a deferred event from a given event queue.
*
* This function is part of the event deferral support. An active object
* uses this function to recall a deferred event from a given QF
* event queue. Recalling an event means that it is removed from the
* deferred event queue \a eq and posted (LIFO) to the event queue of
* the active object.
*
* QActive_recall() returns 1 (TRUE) if an event has been recalled.
* Otherwise the function returns 0.
*
* An active object can use multiple event queues to defer events of
* different kinds.
*
* \sa QActive_recall(), QEQueue, QActive_postLIFO()
*/
uint8_t QActive_recall(QActive * const me, QEQueue * const eq);

/** \brief Get an event from the event queue of an active object.
*
* This function is used internally by a QF port to extract events from
* the event queue of an active object. This function depends on the event
* queue implementation and is sometimes implemented in the QF port
* (qf_port.c file). Depending on the underlying OS or kernel, the
* function might block the calling thread when no events are available.
*
* \sa QActive_postFIFO(), QActive_postLIFO()
*/
QEvt const *QActive_get_(QActive *const me);
/* friend class QF; */
/* friend class QTimeEvt; */

/****************************************************************************/
#ifndef QF_TIMEEVT_CTR_SIZE
    /** \brief macro to override the default QTimeEvtCtr size.
    * Valid values 1, 2, or 4; default 2
    */
    #define QF_TIMEEVT_CTR_SIZE 2
#endif
#if (QF_TIMEEVT_CTR_SIZE == 1)

    /** \brief type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks.
    *
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: \n
    * uint8_t when (QF_TIMEEVT_CTR_SIZE == 1), and \n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4).
    */
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2)
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMEEVT_CTR_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/** \brief Time Event structure
*
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
* Internally, the armed time events are organized into a bi-directional
* linked list. This linked list is scanned in every invocation of the
* QF_tick() function. Only armed (timing out) time events are in the list,
* so only armed time events consume CPU cycles.
*
* \sa ::QTimeEvtTag for the description of the data members \n \ref derivation
*
* \note QF manages the time events in the function QF_tick(), which
* must be called periodically, preferably from the clock tick ISR.
*
* \note In this version of QF QTimeEvt objects should be allocated statically
* rather than dynamically from event pools. Currently, QF will not correctly
* recycle the dynamically allocated Time Events.
*/
typedef struct QTimeEvtTag {
    /** base structure from which QTimeEvt derives
    * \sa ::QEvt \n \ref derivation
    */
    QEvt super;

    /** link to the next time event in the list
    */
    struct QTimeEvtTag *next;

    /** the active object that receives the time events.
    */
    QActive *act;

    /** the internal down-counter of the time event. The down-counter
    * is decremented by 1 in every QF_tick() invocation. The time event
    * fires (gets posted or published) when the down-counter reaches zero.
    */
    QTimeEvtCtr ctr;

    /** the interval for the periodic time event (zero for the one-shot
    * time event). The value of the interval is re-loaded to the internal
    * down-counter when the time event expires, so that the time event
    * keeps timing out periodically.
    */
    QTimeEvtCtr interval;
} QTimeEvt;

/* public functions */

/** \brief The "constructor" to initialize a Time Event.
*
* You should call this function exactly once for every Time Event object
* BEFORE arming the Time Event. The most important action performed in
* this function is assigning a signal to the Time Event. You can reuse
* the Time Event any number of times, but you should not change the signal.
* This is because pointers to Time Events might still be held in event
* queues and changing signal could to hard-to-detect errors.
*
* The following example shows the invocation of QTimeEvt_ctor() from the
* "constructor" of the Philosopher active object that owns the time event
* \include qf_ctor.c
*/
void QTimeEvt_ctor(QTimeEvt * const me, enum_t const sig);

/** \brief Arm a one-shot time event for direct event posting.
*
* Arms a time event \a me_ to fire in \a nTicks_ clock ticks
* (one-shot time event). The time event gets directly posted (using the
* FIFO policy) into the event queue of the active object \a act_.
*
* After posting, the time event gets automatically disarmed and can be
* reused for a one-shot or periodic timeout requests.
*
* A one-shot time event can be disarmed at any time by calling the
* QTimeEvt_disarm() function. Also, a one-shot time event can be re-armed
* to fire in a different number of clock ticks by calling the
* QTimeEvt_rearm() function.
*
* The following example shows how to arm a one-shot time event from a state
* machine of an active object:
* \include qf_state.c
*/
#define QTimeEvt_postIn(me_, act_, nTicks_) do { \
    (me_)->interval = (QTimeEvtCtr)0; \
    QTimeEvt_arm_((me_), (act_), (nTicks_)); \
} while (0)

/** \brief Arm a periodic time event for direct event posting.
*
* Arms a time event \a me_ to fire every \a nTicks_ clock ticks
* (periodic time event). The time event gets directly posted (using the
* FIFO policy) into the event queue of the active object \a act_.
*
* After posting, the time event gets automatically re-armed to fire again
* in the specified \a nTicks_ clock ticks.
*
* A periodic time event can be disarmed only by calling the QTimeEvt_disarm()
* function. After disarming, the time event can be reused for a one-shot or
* periodic timeout requests.
*
* \note An attempt to reuse (arm again) a running periodic time event
* raises an assertion.
*
* Also, a periodic time event can be re-armed to shorten or extend the
* current period by calling the QTimeEvt_rearm() function. After adjusting
* the current period, the periodic time event goes back timing out at the
* original rate.
*/
#define QTimeEvt_postEvery(me_, act_, nTicks_) do { \
    (me_)->interval = (nTicks_); \
    QTimeEvt_arm_((me_), (act_), (nTicks_)); \
} while (0)

/** \brief Disarm a time event.
*
* The time event \a me gets disarmed and can be reused. The function
* returns 1 (TRUE) if the time event was truly disarmed, that is, it
* was running. The return of 0 (FALSE) means that the time event was
* not truly disarmed because it was not running. The FALSE return is only
* possible for one-shot time events that have been automatically disarmed
* upon expiration. In this case the FALSE return means that the time event
* has already been posted or published and should be expected in the
* active object's state machine.
*/
uint8_t QTimeEvt_disarm(QTimeEvt * const me);

/** \brief Rearm a time event.
*
* The time event \a me gets rearmed with a new number of clock ticks
* \a nTicks. This facility can be used to prevent a one-shot time event
* from expiring (e.g., a watchdog time event), or to adjusts the
* current period of a periodic time event. Rearming a periodic timer
* leaves the interval unchanged and is a convenient method to adjust the
* phasing of the periodic time event.
*
* The function returns 1 (TRUE) if the time event was running as it
* was re-armed. The return of 0 (FALSE) means that the time event was
* not truly rearmed because it was not running. The FALSE return is only
* possible for one-shot time events that have been automatically disarmed
* upon expiration. In this case the FALSE return means that the time event
* has already been posted or published and should be expected in the
* active object's state machine.
*/
uint8_t QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks);

/** \brief Get the current value of the down-counter of a time event.
*
* If the time event is armed, the function returns the current value of
* the down-counter of the given time event. If the time event is not
* armed, the function returns 0.
*
* /note The function is thread-safe.
*/
QTimeEvtCtr QTimeEvt_ctr(QTimeEvt const * const me);

/* private functions */

/** \brief Arm a time event (internal function to be used through macros
* only).
*
* \sa #QTimeEvt_postIn, #QTimeEvt_postEvery,
*/
void QTimeEvt_arm_(QTimeEvt * const me,
                   QActive * const act, QTimeEvtCtr const nTicks);

/* friend class QF; */

/*****************************************************************************
* QF facilities
*/

/** \brief Subscriber-List structure
*
* This data type represents a set of active objects that subscribe to
* a given signal. The set is represented as an array of bits, where each
* bit corresponds to the unique priority of an active object.
*
* \sa ::QSubscrListTag for the description of the data members
*/
typedef struct QSubscrListTag {

    /** An array of bits representing subscriber active objects. Each bit
    * in the array corresponds to the unique priority of the active object.
    * The size of the array is determined of the maximum number of active
    * objects in the application configured by the #QF_MAX_ACTIVE macro.
    * For example, an active object of priority p is a subscriber if the
    * following is true: ((bits[QF_div8Lkup[p]] & QF_pwr2Lkup[p]) != 0)
    *
    * \sa QF_psInit(), ::QF_div8Lkup, ::QF_pwr2Lkup, #QF_MAX_ACTIVE
    */
    uint8_t bits[((QF_MAX_ACTIVE - 1) / 8) + 1];
} QSubscrList;

/* public functions */

/** \brief QF initialization.
*
* This function initializes QF and must be called exactly once before any
* other QF function.
*/
void QF_init(void);

/** \brief Publish-subscribe initialization.
*
* This function initializes the publish-subscribe facilities of QF and must
* be called exactly once before any subscriptions/publications occur in
* the application. The arguments are as follows: \a subscrSto is a pointer
* to the array of subscriber-lists. \a maxSignal is the dimension of this
* array and at the same time the maximum signal that can be published or
* subscribed.
*
* The array of subscriber-lists is indexed by signals and provides mapping
* between the signals and subscriber-lists. The subscriber-lists are bitmasks
* of type ::QSubscrList, each bit in the bitmask corresponding to the unique
* priority of an active object. The size of the ::QSubscrList bitmask depends
* on the value of the #QF_MAX_ACTIVE macro.
*
* \note The publish-subscribe facilities are optional, meaning that you
* might choose not to use publish-subscribe. In that case calling QF_psInit()
* and using up memory for the subscriber-lists is unnecessary.
*
* \sa ::QSubscrList
*
* The following example shows the typical initialization sequence of QF:
* \include qf_main.c
*/
void QF_psInit(QSubscrList * const subscrSto, uint32_t const maxSignal);

/** \brief Event pool initialization for dynamic allocation of events.
*
* This function initializes one event pool at a time and must be called
* exactly once for each event pool before the pool can be used.
* The arguments are as follows: \a poolSto is a pointer to the memory
* block for the events. \a poolSize is the size of the memory block in
* bytes. \a evtSize is the block-size of the pool in bytes, which determines
* the maximum size of events that can be allocated from the pool.
*
* You might initialize one, two, and up to three event pools by making
* one, two, or three calls to the QF_poolInit() function. However,
* for the simplicity of the internal implementation, you must initialize
* event pools in the ascending order of the event size.
*
* Many RTOSes provide fixed block-size heaps, a.k.a. memory pools that can
* be adapted for QF event pools. In case such support is missing, QF provides
* a native QF event pool implementation. The macro #QF_EPOOL_TYPE_ determines
* the type of event pool used by a particular QF port. See structure ::QMPool
* for more information.
*
* \note The actual number of events available in the pool might be actually
* less than (\a poolSize / \a evtSize) due to the internal alignment
* of the blocks that the pool might perform. You can always check the
* capacity of the pool by calling QF_getPoolMargin().
*
* \note The dynamic allocation of events is optional, meaning that you
* might choose not to use dynamic events. In that case calling QF_poolInit()
* and using up memory for the memory blocks is unnecessary.
*
* \sa QF initialization example for QF_init()
*/
void QF_poolInit(void * const poolSto, uint32_t const poolSize,
                 uint32_t const evtSize);

/** \brief Transfers control to QF to run the application.
*
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QActive_start(). Also,
* QF_start() call must precede the transfer of control to QF_run(), but
* some QF ports might call QF_start() from QF_run(). QF_run() typically
* never returns to the caller, but when it does, it returns the error code
* (0 for success).
*
* \note This function is strongly platform-dependent and is not implemented
* in the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. All QF ports must implement QF_run().
*
* \note When the Quantum Kernel (QK) is used as the underlying real-time
* kernel for the QF, all platfrom dependencies are handled in the QK, so
* no porting of QF is necessary. In other words, you only need to recompile
* the QF platform-independent code with the compiler for your platform, but
* you don't need to provide any platform-specific implementation (so, no
* qf_port.c file is necessary). Moreover, QK implements the function QF_run()
* in a platform-independent way, in the modile qk.c.
*/
int16_t QF_run(void);

/** \brief Function invoked by the application layer to stop the QF
* application and return control to the OS/Kernel.
*
* This function stops the QF application. After calling this function,
* QF attempts to gracefully stop the  application. This graceful shutdown
* might take some time to complete. The typical use of this funcition is
* for terminating the QF application to return back to the operating
* system or for handling fatal errors that require shutting down
* (and possibly re-setting) the system.
*
* This function is strongly platform-specific and is not implemented in
* the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. Some QF ports might not require implementing
* QF_stop() at all, because many embedded applications don't have anything
* to exit to.
*
* \sa QF_onCleanup()
*/
void QF_stop(void);

/** \brief Startup QF callback.
*
* The timeline for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*/
void QF_onStartup(void);

/** \brief Cleanup QF callback.
*
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

/** \brief QF idle callback (customized in BSPs for QF)
*
* QF_onIdle() is called by the non-preemptive "Vanilla" scheduler built into
* QF when the framework detects that no events are available for active
* objects (the idle condition). This callback gives the application an
* opportunity to enter a power-saving CPU mode, or perform some other idle
* processing (such as Q-Spy output).
*
* \note QF_onIdle() is invoked with interrupts DISABLED because the idle
* condition can be asynchronously changed at any time by an interrupt.
* QF_onIdle() MUST enable the interrupts internally, but not before
* putting the CPU into the low-power mode. (Ideally, enabling interrupts and
* low-power mode should happen atomically). At the very least, the function
* MUST enable interrupts, otherwise interrups will remain disabled
* permanently.
*
* \note QF_onIdle() is only used by the non-preemptive "Vanilla" scheduler
* in the "bare metal" QF port, and is NOT used in any other QF ports. When
* QF is combined with QK, the QK idle loop calls a different function
* QK_onIdle(), with different semantics than QF_onIdle(). When QF is
* combined with a 3rd-party RTOS or kernel, the idle processing mechanism
* of the RTOS or kernal is used instead of QF_onIdle().
*/
void QF_onIdle(void);

#ifdef Q_SPY

    /** \brief Publish event to the framework.
    *
    * This function posts (using the FIFO policy) the event \a e to ALL
    * active objects that have subscribed to the signal \a e->sig.
    * This function is designed to be callable from any part of the system,
    * including ISRs, device drivers, and active objects.
    *
    * In the general case, event publishing requires multi-casting the
    * event to multiple subscribers. This happens in the caller's thread with
    * the scheduler locked to prevent preemptions during the multi-casting
    * process. (Please note that the interrupts are not locked.)
    *
    * \note this function should be called only via the macro #QF_PUBLISH
    */
    void QF_publish(QEvt const * const e, void const * const sender);

    /** \brief Invoke the event publishing facility QF_publish(). This macro
    * is the recommended way of publishing events, because it provides the
    * vital information for software tracing and avoids any overhead when the
    * tracing is disabled.
    *
    * This macro takes the last argument \a sender_, which is a pointer to
    * the sender object. This argument is actually only used when QS software
    * tracing is enabled (macro #Q_SPY is defined). When QS software
    * tracing is disabled, the macro calls QF_publish() without the
    * \a sender_ argument, so the overhead of passing this extra argument
    * is entirely avoided.
    *
    * \note the pointer to the sender object is not necessarily a poiner
    * to an active object. In fact, if QF_PUBLISH() is called from an
    * interrupt or other context, you can create a unique object just to
    * unambiguously identify the publisher of the event.
    *
    * \sa ::QF_publish().
    */
    #define QF_PUBLISH(e_, sender_)  QF_publish((e_), (void const *)(sender_))

#else

    void QF_publish(QEvt const * const e);
    #define QF_PUBLISH(e_, dummy_)   QF_publish(e_)

#endif

#ifdef Q_SPY

    /** \brief Processes all armed time events at every clock tick.
    *
    * This function must be called periodically from a time-tick ISR or from
    * the highest-priority task so that QF can manage the timeout events.
    *
    * \note The QF_tick() function is not reentrant meaning that it must
    * run to completion before it is called again. Also, QF_tick() assumes
    * that it never will get preempted by a task, which is always the case
    * when it is called from an ISR or the highest-priority task.
    *
    * \note this function should be called only via the macro #QF_TICK
    *
    * \sa ::QTimeEvt.
    */
    void QF_tick(void const * const sender);

    /** \brief Invoke the system clock tick processing QF_tick(). This macro
    * is the recommended way of invoking clock tick processing, because it
    * provides the vital information for software tracing and avoids any
    * overhead when the tracing is disabled.
    *
    * This macro takes the argument \a sender_, which is a pointer to the
    * sender object. This argument is actually only used when QS software
    * tracing is enabled (macro #Q_SPY is defined). When QS software
    * tracing is disabled, the macro calls QF_tick() without any
    * arguments, so the overhead of passing this extra argument is
    * entirely avoided.
    *
    * \note the pointer to the sender object is not necessarily a poiner
    * to an active object. In fact, typically QF_TICK() will be called from
    * an interrupt, in which case you would create a unique object just to
    * unambiguously identify the ISR as the sender of the time events.
    *
    * \sa ::QF_tick().
    */
    #define QF_TICK(sender_)              QF_tick(sender_)

#else

    void QF_tick(void);
    #define QF_TICK(dummy_)               QF_tick()

#endif

/** \brief Returns true if all time events are inactive and false
* any time event is active.
*
* \note This function should be called in critical section.
*/
uint8_t QF_noTimeEvtsActive(void);


/* functions used in the QF ports only -------------------------------------*/

/** \brief Register an active object to be managed by the framework
*
* This function should not be called by the application directly, only
* through the function QActive_start(). The priority of the active object
* \a a should be set before calling this function.
*
* \note This function raises an assertion if the priority of the active
* object exceeds the maximum value #QF_MAX_ACTIVE. Also, this function
* raises an assertion if the priority of the active object is already in
* use. (QF requires each active object to have a UNIQUE priority.)
*/
void QF_add_(QActive * const a);

/** \brief Remove the active object from the framework.
*
* This function should not be called by the application directly, only
* inside the QF port. The priority level occupied by the active object
* is freed-up and can be reused for another active object.
*
* The active object that is removed from the framework can no longer
* participate in the publish-subscribe event exchange.
*
* \note This function raises an assertion if the priority of the active
* object exceeds the maximum value #QF_MAX_ACTIVE or is not used.
*/
void QF_remove_(QActive const * const a);

/** \brief Returns the QF version.
*
* This function returns constant version string in the format x.y.zz,
* where x (one digit) is the major version, y (one digit) is the minor
* version, and zz (two digits) is the maintenance release version.
* An example of the version string is "3.1.03".
*
* The following example illustrates the usage of this function:
* \include qf_version.c
*/
char_t const Q_ROM * Q_ROM_VAR QF_getVersion(void);

/** \brief Returns the QF-port version.
*
* This function returns constant version string in the format x.y.zz,
* where x (one digit) is the major version, y (one digit) is the minor
* version, and zz (two digits) is the maintenance release version.
* An example of the QF-port version string is "1.1.03".
*
* \sa QF_getVersion()
*/
char_t const Q_ROM * Q_ROM_VAR QF_getPortVersion(void);

/** \brief This function returns the margin of the given event pool.
*
* This function returns the margin of the given event pool \a poolId, where
* poolId is the ID of the pool initialized by the call to QF_poolInit().
* The poolId of the first initialized pool is 1, the second 2, and so on.
*
* The returned pool margin is the minimal number of free blocks encountered
* in the given pool since system startup.
*
* \note Requesting the margin of an un-initialized pool raises an assertion
* in the QF.
*/
uint32_t QF_getPoolMargin(uint8_t const poolId);

/** \brief This function returns the margin of the given event queue.
*
* This function returns the margin of the given event queue of an active
* object with priority \a prio. (QF priorities start with 1 and go up to
* #QF_MAX_ACTIVE.) The margin is the minimal number of free events
* encountered in the given queue since system startup.
*
* \note QF_getQueueMargin() is available only when the native QF event
* queue implementation is used. Requesting the queue margin of an unused
* priority level raises an assertion in the QF. (A priority level becomes
* used in QF after the call to the QF_add_() function.)
*/
uint32_t QF_getQueueMargin(uint8_t const prio);


#ifdef Q_EVT_CTOR            /* Provide the constructor for the QEvt class? */

    /** \brief Internal QF implementation of the dynamic event allocator.
    *
    * \note The application code should not call this function directly.
    * Please use the macro #Q_NEW.
    */
    QEvt *QF_new_(QEvtSize const evtSize);

    /** \brief Allocate a dynamic event.
    *
    * This macro returns an event pointer cast to the type \a evtT_.
    * The event is initialized with the signal \a sig_. Internally,
    * the macro calls the internal QF function QF_new_(), which always
    * returns a valid event pointer.
    *
    * \note The internal QF function QF_new_() raises an assertion when
    * the allocation of the event turns out to be impossible due to event
    * pool depletion, or incorrect (too big) size of the requested event.
    *
    * The following example illustrates dynamic allocation of an event:
    * \include qf_post.c
    */
    #define Q_NEW(evtT_, sig_, ...) \
        evtT_##_ctor((evtT_ *)QF_new_((QEvtSize)sizeof(evtT_)), \
                     (sig_), ##__VA_ARGS__)
#else

    QEvt *QF_new_(QEvtSize const evtSize, enum_t const sig);

    #define Q_NEW(evtT_, sig_) \
        ((evtT_ *)QF_new_((QEvtSize)sizeof(evtT_), (sig_)))

#endif                                                        /* Q_EVT_CTOR */


/** \brief Recycle a dynamic event.
*
* This function implements a simple garbage collector for the dynamic events.
* Only dynamic events are candidates for recycling. (A dynamic event is one
* that is allocated from an event pool, which is determined as non-zero
* e->attrQF__ attribute.) Next, the function decrements the reference counter
* of the event, and recycles the event only if the counter drops to zero
* (meaning that no more references are outstanding for this event).
* The dynamic event is recycled by returning it to the pool from which
* it was originally allocated. The pool-of-origin information is stored in
* the upper 2-MSBs of the e->attrQF__ attribute.)
*
* \note QF invokes the garbage collector at all appropriate contexts, when
* an event can become garbage (automatic garbage collection), so the
* application code should have no need to call QF_gc() directly. The QF_gc()
* function is exposed only for special cases when your application sends
* dynamic events to the "raw" thread-safe queues (see ::QEQueue). Such
* queues are processed outside of QF and the automatic garbage collection
* is CANNOT be performed for these events. In this case you need to call
* QF_gc() explicitly.
*/
void QF_gc(QEvt const * const e);

/** \brief Lookup table for (log2(n) + 1), where n is the index
* into the table.
*
* This lookup delivers the 1-based number of the most significant 1-bit
* of a byte.
*
* \note Index range n = 0..255. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM Q_ROM_VAR QF_log2Lkup[256];

/** \brief Lookup table for (1 << ((n-1) % 8)), where n is the index
* into the table.
*
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM Q_ROM_VAR QF_pwr2Lkup[65];

/** \brief Lookup table for ~(1 << ((n-1) % 8)), where n is the index
* into the table.
*
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM Q_ROM_VAR QF_invPwr2Lkup[65];

/** \brief Lookup table for (n-1)/8
*
* \note Index range n = 0..64. The first index (n == 0) should never be used.
*/
extern uint8_t const Q_ROM Q_ROM_VAR QF_div8Lkup[65];


/** \brief array of registered active objects
*
* \note Not to be used by Clients directly, only in ports of QF
*/
extern QActive *QF_active_[];

#endif                                                              /* qf_h */
