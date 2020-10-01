/**
* @file
* @brief Command-line macros and macros for porting QP
*/

/*! The preprocessor switch to disable checking assertions */
/**
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
* instrumentation in the code */
/**
* When defined, Q_SPY activates the QS software tracing instrumentation.
* When Q_SPY is not defined, the QS instrumentation in the code does
* not generate any code.
*/
#define Q_SPY

/*! The preprocessor switch to activate the QUTest unit testing
* instrumentation in the code */
/**
* @note
* This macro requires that #Q_SPY be defined as well.
*/
#define Q_UTEST

/*! This macro defines the type of the thread handle used for AOs */
#define QF_THREAD_TYPE         void*

/*! This macro defines the type of the event-queue used for AOs */
#define QF_EQUEUE_TYPE         QEQueue

/*! This macro defines the type of the OS-Object used for blocking
* the native ::QEQueue when the queue is empty */
/**
* @description
* This macro is used when ::QEQueue is used as the event-queue for AOs
* but also the AO queue must *block* when the queue is empty.
* In that case, #QF_OS_OBJECT_TYPE specifies the blocking mechanism.
* For examle, in the POSIX port, the blocking mechanism is a condition
* variable.
*/
#define QF_OS_OBJECT_TYPE      pthread_cond_t

/*! Platform-dependent macro defining how QF should block the
* calling task when the QF native queue is empty */
/**
* @note This is just an example of QACTIVE_EQUEUE_WAIT_() for the QK-port
* of QF. QK never activates a task that has no events to process, so in
* this case the macro asserts that the queue is not empty. In other QF
* ports you need to define the macro appropriately for the underlying
* kernel/OS you're using.
*/
#define QACTIVE_EQUEUE_WAIT_(me_)                  \
    (Q_ASSERT((me_)->eQueue.frontEvt != (QEvt *)0))

#if (QF_MAX_ACTIVE <= 8)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) do {       \
        QPSet8_insert(&QK_readySet_, (me_)->prio); \
        if (QK_intNest_ == (uint_fast8_t)0) {      \
            uint_fast8_t p = QK_schedPrio_();      \
            if (p != (uint_fast8_t)0) {            \
                QK_sched_(p);                      \
            }                                      \
        }                                          \
    } while (0)
#else
    /*! Platform-dependent macro defining how QF should signal the
    * active object task that an event has just arrived. */
    /**
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
        QPSet64_insert(&QK_readySet_, (me_)->prio); \
        if (QK_intNest_ == (uint_fast8_t)0) {       \
            uint_fast8_t p = QK_schedPrio_();       \
            if (p != (uint_fast8_t)0) {             \
                QK_sched_(p);                       \
            }                                       \
        }                                           \
    } while (0)
#endif

/*! This macro defines the type of the event pool used in the QK kernel. */
/**
* @note This is a specific implementation for the QK-port of QF.
* In other QF ports you need to define the macro appropriately for
* the underlying kernel/OS you're using.
*/
#define QF_EPOOL_TYPE_            QMPool

/*! This macro enables calling the QK context-switch callback
* QK_onContextSw() */
#define QK_ON_CONTEXT_SW

/*! This macro enables calling the QXK context-switch callback
* QXK_onContextSw() */
#define QXK_ON_CONTEXT_SW

/*! Platform-dependent macro defining the event pool initialization */
/**
* @note
* This is an example implementation based on the native ::QMPool class.
* In other QF ports, the port might be using a memory pool from the
* underlying kernel/OS.
*/
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (QMPool_init(&(p_), (poolSto_), (poolSize_), (QMPoolSize)(evtSize_)))

/*! Platform-dependent macro defining how QF should obtain the
* event pool block-size */
/**
* @note This is a specific implementation for the QK-port of QF.
* In other QF ports you need to define the macro appropriately for
* the underlying kernel/OS you're using.
*/
#define QF_EPOOL_EVENT_SIZE_(p_)  ((QEvtSize)(p_).blockSize)

/*! Platform-dependent macro defining how QF should obtain an event
* @a e_ from the event pool @a p_ with the free margin @a m_. */
/**
* @note
* This is an example implementation based on the native ::QMPool class.
* In other QF ports, the port might be using a memory pool from the
* underlying kernel/OS.
*/
#define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
    ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))

/*! Platform-dependent macro defining how QF should return an event
* @a e_ to the event pool @a p_ */
/**
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

/*! Macro that should be defined (typically on the compiler's command line)
in the Win32-GUI applications that use the @ref win32 or @ref win32-qv ports.
*/
#define WIN32_GUI


