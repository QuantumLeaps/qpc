/**
* @file
* @brief Various macros for configuring and porting QP/C
*/

/*! The maximum number of active objects in the application.
*
* @description
* This macro *must* be defined in the QF port and should be in range
* of 1U..64U, inclusive. The value of this macro determines the maximum
* priority level of an active object in the system. Not all priority
* levels must be used, but the maximum priority cannot exceed
* #QF_MAX_ACTIVE.
*
* @note Once you choose a certain value of #QF_MAX_ACTIVE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_ACTIVE               32U

/*! The maximum number of clock tick rates in the application.
*
* @description
* This macro can be defined in the QF ports and should be in range
* of 1U..15U, inclusive. The value of this macro determines the maximum
* number of clock tick rates for time events (::QTimeEvt).
*
* If the macro is not defined, the default value is 1U.
*
* @note Once you choose a certain value of #QF_MAX_TICK_RATE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_TICK_RATE            1U

/*! The maximum number of event pools in the application.
*
* @description
* This macro can be defined in the QF ports and should be in range
* of 1U..255U, inclusive. The value of this macro determines the maximum
* event pools in the system. Not all event pools must be actually used,
* but the maximum number of pools cannot exceed #QF_MAX_EPOOL.
*
* If the macro is not defined, the default value is 3U. Defining the value
* below the maximum saves some memory, mostly for the subscriber-lists.
* @sa ::QSubscrList.
*
* @note Once you choose a certain value of #QF_MAX_EPOOL, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MAX_EPOOL                3U

/*! The size (in bytes) of the event-size representation in the QF.
* Valid values: 1U, 2U, or 4U; default 2U
*
* @description
* This macro can be defined in the QF ports to configure the size
* of the event-size.
*
* @note Once you choose a certain value of #QF_EVENT_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EVENT_SIZ_SIZE           2U

/*! The size (in bytes) of the ring-buffer counters used in the native QF
* event queue implementation. Valid values: 1U, 2U, or 4U; default 1U
*
* @description
* This macro can be defined in the QF ports to configure the ::QEQueueCtr
* type. If the macro is not defined, the default of 1 byte will be chosen in
* qequeue.h. The valid #QF_EQUEUE_CTR_SIZE values of 1U, 2U, or 4U, correspond
* to ::QEQueueCtr of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QEQueueCtr data type determines the dynamic range of numerical values of
* ring-buffer counters inside event queues, or, in other words, the maximum
* number of events that the native QF event queue can manage.
* @sa ::QEQueue
*
* @note Once you choose a certain value of #QF_EQUEUE_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_EQUEUE_CTR_SIZE          1U

/*! The size (in bytes) of the block-size representation in the native QF
* event pool. Valid values: 1U, 2U, or 4U; default #QF_EVENT_SIZ_SIZE.
*
* @description
* This macro can be defined in the QF ports to configure the ::QMPoolSize
* type. If the macro is not defined, the default of #QF_EVENT_SIZ_SIZE
* will be chosen in qmpool.h, because the memory pool is primarily used for
* implementing event pools.
*
* The valid #QF_MPOOL_SIZ_SIZE values of 1U, 2U, or 4U, correspond to
* ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively. The
* ::QMPoolSize data type determines the dynamic range of block-sizes that
* the native ::QMPool can handle.
* @sa #QF_EVENT_SIZ_SIZE, ::QMPool
*
* @note Once you choose a certain value of #QF_MPOOL_SIZ_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_SIZ_SIZE           2U

/*! The size (in bytes) of the block-counter representation in the
* native QF event pool. Valid values: 1U, 2U, or 4U; default 2U.
*
* @description
* This macro can be defined in the QF ports to configure the ::QMPoolCtr
* type. If the macro is not defined, the default of 2 bytes will be chosen
* in qmpool.h. The valid #QF_MPOOL_CTR_SIZE values of 1U, 2U, or 4U,
* correspond to ::QMPoolSize of uint8_t, uint16_t, and uint32_t, respectively.
* The ::QMPoolCtr data type determines the dynamic range of block-counters
* that the native ::QMPool can handle, or, in other words, the maximum number
* of blocks that the native QF event pool can manage.
* @sa ::QMPool
*
* @note Once you choose a certain value of #QF_MPOOL_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_MPOOL_CTR_SIZE           2U

/*! The size (in bytes) of the time event-counter representation
* in the ::QTimeEvt struct. Valid values: 1U, 2U, or 4U; default 2U.
*
* @description
* This macro can be defined in the QF ports to configure the internal tick
* counters of Time Events. If the macro is not defined, the default of 2
* bytes will be chosen in qf.h. The valid #QF_TIMEEVT_CTR_SIZE values of 1,
* 2, or 4, correspond to tick counters of uint8_t, uint16_t, and uint32_t,
* respectively. The tick counter representation determines the dynamic range
* of time delays that a Time Event can handle.
* @sa ::QTimeEvt
*
* @note Once you choose a certain value of #QF_TIMEEVT_CTR_SIZE, you must
* consistently use the same value in building all the QP component libraries
* and your own application code. The consistency is guaranteed if you define
* this macro only once in the qf_port.h header file and henceforth include
* this header file in all builds.
*/
#define QF_TIMEEVT_CTR_SIZE         2U

/*! Define the interrupt disabling policy.
*
* @description
* This macro encapsulates platform-specific way of disabling interrupts
* from "C" for a given CPU and compiler.
*
* @note
* the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_DISABLE()            intDisable()

/*! Define the interrupt enabling policy.
*
* @description
* This macro encapsulates platform-specific way of enabling interrupts
* from "C" for a given CPU and compiler.
*
* @note the #QF_INT_DISABLE macro should always be used in pair with the
* macro #QF_INT_ENABLE.
*/
#define QF_INT_ENABLE()             intEnable()

void intDisable(void);
void intEnable(void);

/*! Define the type of the critical section status.
*
* @description
* Defining this macro configures the "saving and restoring critical section
* status" policy. Coversely, if this macro is not defined, the simple
* "unconditional critical section exit" is used.
*/
#define QF_CRIT_STAT_TYPE           crit_stat_t

/*! Define the critical section entry policy.
*
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro sets the @a status_ argument to the critical
* section status just before the entry. When the policy of "unconditional
* critical section exit" is used, the macro does not use the @a status_
* argument.
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_ENTRY(stat_)        ((stat_) = critEntry())

/*! Define the critical section exit policy.
/*
* @description
* This macro enters a critical section (often by means of disabling
* interrupts). When the "saving and restoring critical section status"
* policy is used, the macro restores the critical section status from the
* @a status_ argument. When the policy of "unconditional critical section
* exit" is used, the macro does not use the @a status argument and
* exits the critical section unconditionally (often by means of enabling
* interrupts).
*
* @note the #QF_CRIT_ENTRY macro should always be used in pair with the
* macro #QF_CRIT_EXIT.
*/
#define QF_CRIT_EXIT(stat_)         critExit(stat_)

typedef unsigned int crit_stat_t;
crit_stat_t critEntry(void);
void critExit(crit_stat_t stat);

/*! Enable the QActive_stop() API in the QF port.
*
* @description
* Defining this macro enables the QActive_stop() API in a given port.
* This feature should be used with caution, as stopping and re-starting
* active objects **cleanly** can be tricky.
*/
#define QF_ACTIVE_STOP

/*! The preprocessor switch to disable checking assertions
*
* @description
* When defined, Q_NASSERT disables the following macros #Q_ASSERT,
* #Q_REQUIRE, #Q_ENSURE, #Q_INVARIANT, #Q_ERROR as well as
* #Q_ASSERT_ID, #Q_REQUIRE_ID, #Q_ENSURE_ID, #Q_INVARIANT_ID, and
* #Q_ERROR_ID do _not_ evaluate the test condition passed as the
* argument to these macros.
*
* @note The notable exceptions are the macros #Q_ALLEGE and
* #Q_ALLEGE_ID, that still evaluate the test condition, but do not
* report assertion failures when the switch #Q_NASSERT is defined.
*/
#define Q_NASSERT

/*! The preprocessor switch to activate the QS software tracing
* instrumentation in the code
*
* @description
* When defined, Q_SPY activates the QS software tracing instrumentation.
* When Q_SPY is not defined, the QS instrumentation in the code does
* not generate any code.
*/
#define Q_SPY

/*! The preprocessor switch to activate the QUTest unit testing
* instrumentation in the code
*
* @note
* This macro requires that #Q_SPY be defined as well.
*/
#define Q_UTEST

/*! The preprocessor switch to enable constructor in the ::QEvt class
* instrumentation in the code
*
* @tr{RQP005}
*/
#define Q_EVT_CTOR

/*! This macro defines the type of the thread handle used for AOs */
#define QF_THREAD_TYPE         void*

/*! This macro defines the type of the event-queue used for AOs */
#define QF_EQUEUE_TYPE         QEQueue

/*! This macro defines the type of the OS-Object used for blocking
* the native ::QEQueue when the queue is empty
*
* @description
* This macro is used when ::QEQueue is used as the event-queue for AOs
* but also the AO queue must *block* when the queue is empty.
* In that case, #QF_OS_OBJECT_TYPE specifies the blocking mechanism.
* For example, in the POSIX port, the blocking mechanism is a condition
* variable.
*/
#define QF_OS_OBJECT_TYPE      pthread_cond_t

/*! Platform-dependent macro defining how QF should block the
* calling task when the QF native queue is empty
*
* @note
* This is just an example of QACTIVE_EQUEUE_WAIT_() for the QK-port
* of QF. QK never activates a task that has no events to process, so in
* this case the macro asserts that the queue is not empty. In other QF
* ports you need to define the macro appropriately for the underlying
* kernel/OS you're using.
*/
#define QACTIVE_EQUEUE_WAIT_(me_)                  \
    (Q_ASSERT((me_)->eQueue.frontEvt != (QEvt *)0))

#if (QF_MAX_ACTIVE <= 8U)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) do {       \
        QPSet8_insert(&QF_readySet_, (me_)->prio); \
        if (QF_intNest_ == 0U) {                   \
            uint_fast8_t p = QK_schedPrio_();      \
            if (p != 0U) {                         \
                QK_sched_(p);                      \
            }                                      \
        }                                          \
    } while (0)
#else
    /*! Platform-dependent macro defining how QF should signal the
    * active object task that an event has just arrived.
    *
    * @description
    * The macro is necessary only when the native QF event queue is used.
    * The signaling of task involves unblocking the task if it is blocked.
    *
    * @note QACTIVE_EQUEUE_SIGNAL_() is called from a critical section.
    * It might leave the critical section internally, but must restore
    * the critical section before exiting to the caller.
    *
    * @note This is just an example of QACTIVE_EQUEUE_SIGNAL_() for the
    * QK-port of QF. In other QF ports you need to define the macro
    * appropriately for the underlying kernel/OS you're using.
    */
    #define QACTIVE_EQUEUE_SIGNAL_(me_) do {        \
        QPSet64_insert(&QF_readySet_, (me_)->prio); \
        if (QF_intNest_ == 0U) {                    \
            uint_fast8_t p = QK_schedPrio_();       \
            if (p != 0U) {                          \
                QK_sched_(p);                       \
            }                                       \
        }                                           \
    } while (0)
#endif

/*! This macro defines the type of the event pool used in the QK kernel.
*
* @note This is a specific implementation for the QK-port of QF.
* In other QF ports you need to define the macro appropriately for
* the underlying kernel/OS you're using.
*/
#define QF_EPOOL_TYPE_            QMPool

/*! This macro enables calling the QK context-switch callback
* QK_onContextSw()
*/
#define QK_ON_CONTEXT_SW

/*! This macro enables calling the QXK context-switch callback
* QXK_onContextSw()
*/
#define QXK_ON_CONTEXT_SW

/*! Platform-dependent macro defining the event pool initialization
*
* @note
* This is an example implementation based on the native ::QMPool class.
* In other QF ports, the port might be using a memory pool from the
* underlying kernel/OS.
*/
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (QMPool_init(&(p_), (poolSto_), (poolSize_), (QMPoolSize)(evtSize_)))

/*! Platform-dependent macro defining how QF should obtain the
* event pool block-size
*
* @note
* This is a specific implementation for the built-in kernels.
* In other QF ports you need to define the macro appropriately for
* the underlying kernel/OS you're using.
*/
#define QF_EPOOL_EVENT_SIZE_(p_)  ((uint32_t)(p_).blockSize)

/*! Platform-dependent macro defining how QF should obtain an event
* @a e_ from the event pool @a p_ with the free margin @a m_.
*
* @note
* This is an example implementation based on the native ::QMPool class.
* In other QF ports, the port might be using a memory pool from the
* underlying kernel/OS.
*/
#define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
    ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))

/*! Platform-dependent macro defining how QF should return an event
* @a e_ to the event pool @a p_
*
* @note
* This is an example implementation based on the native ::QMPool class.
* In other QF ports, the port might be using a memory pool from the
* underlying kernel/OS.
*/
#define QF_EPOOL_PUT_(p_, e_, qs_id_) \
    (QMPool_put(&(p_), (e_), (qs_id_)))

/*! Macro defined only for the internal QP implementation. It should
* be not defined for the application-level code
*/
#define QP_IMPL

