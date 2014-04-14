/**
* \file
* \brief QS/C platform-independent public interface.
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
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
#ifndef qs_h
#define qs_h

#ifndef Q_SPY
    #error "Q_SPY must be defined to include qs.h"
#endif

/****************************************************************************/
/*! Quantum Spy record types. */
/**
* \description
* This enumeration specifies the record types used in the QP components.
* You can specify your own record types starting from ::QS_USER offset.
* Currently, the maximum of all records cannot exceed 256.
* \sa QS_filterIn()/QS_FILTER_ON and QS_filterOut()/QS_FILTER_OFF
*/
enum QSpyRecords {
    QS_QP_RESET,          /*!< reset the QP (start of a new QS session) */

    /* [1] QEP records */
    QS_QEP_STATE_ENTRY,   /*!< a state was entered */
    QS_QEP_STATE_EXIT,    /*!< a state was exited */
    QS_QEP_STATE_INIT,    /*!< an initial transition was taken in a state */
    QS_QEP_INIT_TRAN,     /*!< the top-most initial transition was taken */
    QS_QEP_INTERN_TRAN,   /*!< an internal transition was taken */
    QS_QEP_TRAN,          /*!< a regular transition was taken */
    QS_QEP_IGNORED,       /*!< an event was ignored (silently discarded) */
    QS_QEP_DISPATCH,      /*!< an event was dispatched (begin of RTC step) */
    QS_QEP_UNHANDLED,     /*!< an event was unhandled due to a guard */

    /* [10] QF records */
    QS_QF_ACTIVE_ADD,     /*!< an AO has been added to QF (started) */
    QS_QF_ACTIVE_REMOVE,  /*!< an AO has been removed from QF (stopped) */
    QS_QF_ACTIVE_SUBSCRIBE, /*!< an AO subscribed to an event */
    QS_QF_ACTIVE_UNSUBSCRIBE, /*!< an AO unsubscribed to an event */
    QS_QF_ACTIVE_POST_FIFO, /*!< an event was posted (FIFO) directly to AO */
    QS_QF_ACTIVE_POST_LIFO, /*!< an event was posted (LIFO) directly to AO */
    QS_QF_ACTIVE_GET, /*!< AO got an event and its queue is still not empty */
    QS_QF_ACTIVE_GET_LAST,  /*!< AO got an event and its queue is empty */
    QS_QF_EQUEUE_INIT,    /*!< an event queue was initialized */
    QS_QF_EQUEUE_POST_FIFO, /*!< an event was posted (FIFO) to a raw queue */
    QS_QF_EQUEUE_POST_LIFO, /*!< an event was posted (LIFO) to a raw queue */
    QS_QF_EQUEUE_GET,     /*!< get an event and queue still not empty */
    QS_QF_EQUEUE_GET_LAST,  /*!< get the last event from the queue */
    QS_QF_MPOOL_INIT,     /*!< a memory pool was initialized */
    QS_QF_MPOOL_GET,      /*!< a memory block was removed from memory pool */
    QS_QF_MPOOL_PUT,      /*!< a memory block was returned to memory pool */
    QS_QF_PUBLISH,        /*!< an event was published */
    QS_QF_RESERVED8,
    QS_QF_NEW,            /*!< new event creation */
    QS_QF_GC_ATTEMPT,     /*!< garbage collection attempt */
    QS_QF_GC,             /*!< garbage collection */
    QS_QF_TICK,           /*!< QF_tickX() was called */
    QS_QF_TIMEEVT_ARM,    /*!< a time event was armed */
    QS_QF_TIMEEVT_AUTO_DISARM, /*!< a time event expired and was disarmed */
    QS_QF_TIMEEVT_DISARM_ATTEMPT,/*!< attempt to disarm a disarmed QTimeEvt */
    QS_QF_TIMEEVT_DISARM, /*!< true disarming of an armed time event */
    QS_QF_TIMEEVT_REARM,  /*!< rearming of a time event */
    QS_QF_TIMEEVT_POST,   /*!< a time event posted itself directly to an AO */
    QS_QF_TIMEEVT_CTR,    /*!< a time event counter was requested */
    QS_QF_CRIT_ENTRY,     /*!< critical section was entered */
    QS_QF_CRIT_EXIT,      /*!< critical section was exited */
    QS_QF_ISR_ENTRY,      /*!< an ISR was entered */
    QS_QF_ISR_EXIT,       /*!< an ISR was exited */
    QS_QF_INT_DISABLE,    /*!< interrupts were disabled */
    QS_QF_INT_ENABLE,     /*!< interrupts were enabled */
    QS_QF_ACTIVE_POST_ATTEMPT,/*!< attempt to post an evt to AO failed */
    QS_QF_EQUEUE_POST_ATTEMPT,/*!< attempt to post an evt to QEQueue failed */
    QS_QF_MPOOL_GET_ATTEMPT,  /*!< attempt to get a memory block failed */
    QS_QF_RESERVED1,
    QS_QF_RESERVED0,

    /* [50] QK records */
    QS_QK_MUTEX_LOCK,     /*!< the QK mutex was locked */
    QS_QK_MUTEX_UNLOCK,   /*!< the QK mutex was unlocked */
    QS_QK_SCHEDULE,       /*!< the QK scheduled a new task to execute */
    QS_QK_RESERVED1,
    QS_QK_RESERVED0,

    /* [55] Additional QEP records */
    QS_QEP_TRAN_HIST,     /*!< a transition to history was taken */
    QS_QEP_TRAN_EP,   /*!< a transition to entry point into a submachine */
    QS_QEP_TRAN_XP,   /*!< a transition to exit  point out of a submachine */
    QS_QEP_RESERVED1,
    QS_QEP_RESERVED0,

    /* [60] Miscellaneous QS records */
    QS_SIG_DICT,          /*!< signal dictionary entry */
    QS_OBJ_DICT,          /*!< object dictionary entry */
    QS_FUN_DICT,          /*!< function dictionary entry */
    QS_USR_DICT,          /*!< user QS record dictionary entry */
    QS_EMPTY,             /*!< QS record for cleanly starting a session */
    QS_RESERVED3,
    QS_RESERVED2,
    QS_TEST_RUN,          /*!< a given test is being run */
    QS_TEST_FAIL,         /*!< a test assertion failed */
    QS_ASSERT_FAIL,       /*!< assertion failed in the code */

    /* [70] Application-specific QS records */
    QS_USER               /*!< the first record available for QS user */
};

/*! Specification of all QS records for  QS_FILTER_ON() and QS_FILTER_OFF() */
#define QS_ALL_RECORDS          ((uint_fast8_t)0xFF)

#ifndef QS_TIME_SIZE

    /*! The size (in bytes) of the QS time stamp. Valid values: 1, 2, or 4;
    * default 4.
    */
    /**
    * \description
    * This macro can be defined in the QS port file (qs_port.h) to
    * configure the ::QSTimeCtr type. Here the macro is not defined so the
    * default of 4 byte is chosen.
    */
    #define QS_TIME_SIZE 4
#endif

#if (QS_TIME_SIZE == 1)
    typedef uint8_t QSTimeCtr;
    #define QS_TIME_()          (QS_u8_(QS_onGetTime()))
#elif (QS_TIME_SIZE == 2)
    typedef uint16_t QSTimeCtr;
    #define QS_TIME_()          (QS_u16_(QS_onGetTime()))
#elif (QS_TIME_SIZE == 4)
    /*! The type of the QS time stamp. This type determines the dynamic
    * range of QS time stamps
    */
    typedef uint32_t QSTimeCtr;

    /*! Internal macro to output time stamp to a QS record */
    #define QS_TIME_()          (QS_u32_(QS_onGetTime()))
#else
    #error "QS_TIME_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

#ifndef Q_ROM      /* provide the default if Q_ROM NOT defined */
    #define Q_ROM
#endif
#ifndef Q_ROM_BYTE /* provide the default if Q_ROM_BYTE NOT defined */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

/****************************************************************************/
/* QS services. */

/*! Initialize the QS data buffer. */
void QS_initBuf(uint8_t sto[], uint_fast16_t stoSize);

/*! Turn the global Filter on for a given record type \a rec. */
void QS_filterOn(uint_fast8_t rec);

/*! Turn the global Filter off for a given record type \a rec. */
void QS_filterOff(uint_fast8_t rec);

/*! Mark the begin of a QS record \a rec */
void QS_beginRec(uint_fast8_t rec);

/*! Mark the end of a QS record \a rec */
void QS_endRec(void);

/* unformatted data elements output ........................................*/
void QS_u8_(uint8_t d);

/*! output two uint8_t data elements without format information */
void QS_u8u8_(uint8_t d1, uint8_t d2);

/*! Output uint16_t data element without format information */
void QS_u16_(uint16_t d);

/*! Output uint32_t data element without format information */
void QS_u32_(uint32_t d);

/*! Output zero-terminated ASCII string element without format information */
void QS_str_(char_t const *s);

/*! Output zero-terminated ASCII string element allocated in ROM
* without format information
*/
void QS_str_ROM_(char_t const Q_ROM *s);

/* formatted data elements output ..........................................*/
/*! Output uint8_t data element with format information */
void QS_u8(uint8_t format, uint8_t d);

/*! output uint16_t data element with format information */
void QS_u16(uint8_t format, uint16_t d);

/*! Output uint32_t data element with format information */
void QS_u32(uint8_t format, uint32_t d);

/*! Output 32-bit floating point data element with format information */
void QS_f32(uint8_t format, float32_t f);

/*! Output 64-bit floating point data element with format information */
void QS_f64(uint8_t format, float64_t d);

/*! Output zero-terminated ASCII string element with format information */
void QS_str(char_t const *s);

/*! Output zero-terminated ASCII string element allocated in ROM
* with format information
*/
void QS_str_ROM(char_t const Q_ROM *s);

/*! Output memory block of up to 255-bytes with format information */
void QS_mem(uint8_t const *blk, uint8_t size);

#if (QS_OBJ_PTR_SIZE == 8) || (QS_FUN_PTR_SIZE == 8)
    /*! Output uint64_t data element without format information */
    void QS_u64_(uint64_t d);

    /*! Output uint64_t data element with format information */
    void QS_u64(uint8_t format, uint64_t d);
#endif

/*! Output signal dictionary record */
void QS_sig_dict(enum_t const sig, void const * const obj,
                 char_t const Q_ROM * const name);

/*! Output object dictionary record */
void QS_obj_dict(void const * const obj,
                 char_t const Q_ROM * const name);

/*! Output function dictionary record */
void QS_fun_dict(void (* const fun)(void),
                 char_t const Q_ROM *const name);

/*! Output user dictionary record */
void QS_usr_dict(enum_t const rec,
                 char_t const Q_ROM * const name);

/* QS buffer access *********************************************************/
/*! Byte-oriented interface to the QS data buffer. */
uint16_t QS_getByte(void);

/*! Constant for End-Of-Data condition returned from QS_getByte() */
#define QS_EOD ((uint16_t)0xFFFF)

/*! Block-oriented interface to the QS data buffer. */
uint8_t const *QS_getBlock(uint16_t *pNbytes);


/* platform-specific callback functions, need to be implemented by clients */

/*! Callback to startup the QS facility */
/**
* \description
* This is a platform-dependent "callback" function invoked through the macro
* #QS_INIT. You need to implement this function in your application.
* At a minimum, the function must configure the QS buffer by calling
* QS_initBuf(). Typically, you will also want to open/configure the QS output
* channel, such as a serial port, or a data file. The void* argument \a arg
* can be used to pass parameter(s) needed to configure the output channel.
*
* \returns the staus of initialization. Typically 1 (true) when the QS
* initialization was successful, or 0 (false) when it failed.
*
* \usage
* The following example illustrates an implementation of QS_onStartup():
* \include qs_startup.c
*/
uint8_t QS_onStartup(void const *arg);

/*! Callback to cleanup the QS facility */
/**
* \description
* This is a platform-dependent "callback" function invoked through the macro
* #QS_EXIT. You need to implement this function in your application.
* The main purpose of this function is to close the QS output channel, if
* necessary.
*/
void QS_onCleanup(void);

/*! Callback to flush the QS trace data to the host */
/**
* \description
* This is a platform-dependent "callback" function to flush the QS trace
* buffer to the host. The function typically busy-waits until all the data
* in the buffer is sent to the host. This is acceptable only in the initial
* transient.
*/
void QS_onFlush(void);

/*! Callback to obtain a timestamp for a QS record. */
/**
* \description
* This is a platform-dependent "callback" function invoked from the macro
* #QS_TIME_ to add the time stamp to a QS record.
*
* \note Some of the predefined QS records from QP do not output the time
* stamp. However, ALL user records do output the time stamp.
* \note QS_onGetTime() is called in a critical section and should not
* exit the critical section.
*
* \returns the current timestamp.
*
* \usage
* The following example shows using a system call to implement QS
* time stamping:
* \include qs_onGetTime.c
*/
QSTimeCtr QS_onGetTime(void);

/****************************************************************************/
/* Macros for adding QS instrumentation to the client code */

/*! Initialize the QS facility. */
/**
* \description
* This macro provides an indirection layer to invoke the QS initialization
* routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
* \sa QS_onStartup(), example of setting up a QS filter in QS_FILTER_ON
*/
#define QS_INIT(arg_)           (QS_onStartup(arg_) != (uint8_t)0)

/*! Cleanup the QS facility. */
/**
* \description
* This macro provides an indirection layer to invoke the QS cleanup
* routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
* \sa QS_exit()
*/
#define QS_EXIT()               (QS_onCleanup())

/*! Global Filter ON for a given record type \a rec. */
/**
* \description
* This macro provides an indirection layer to call QS_filterOn() if #Q_SPY
* is defined, or do nothing if #Q_SPY is not defined.
*
* The following example shows how to use QS filters:
* \include qs_filter.c
*/
#define QS_FILTER_ON(rec_)      (QS_filterOn((uint_fast8_t)(rec_)))

/*! Global filter OFF for a given record type \a rec. */
/**
* \description
* This macro provides an indirection layer to call QS_filterOff() if #Q_SPY
* is defined, or do nothing if #Q_SPY is not defined.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_OFF(rec_)     (QS_filterOff((uint_fast8_t)(rec_)))

/*! Local Filter for a given state machine object \a obj_. */
/**
* \description
* This macro sets up the state machine object local filter if #Q_SPY is
* defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
* is the pointer to the state machine object that you want to monitor.\n
* \n
* The state machine object filter allows you to filter QS records pertaining
* only to a given state machine object. With this filter disabled, QS will
* output records from all state machines in your application. The object
* filter is disabled by setting the state machine pointer to NULL.\n
* \n
* The state machine filter affects the following QS records:
* ::QS_QEP_STATE_ENTRY, ::QS_QEP_STATE_EXIT, ::QS_QEP_STATE_INIT,
* ::QS_QEP_INTERN_TRAN, ::QS_QEP_TRAN, ::QS_QEP_IGNORED,
* ::QS_QEP_TRAN_HIST, ::Q_RET_TRAN_EP, ::Q_RET_TRAN_XP
*
* \note
* Because active objects are state machines at the same time, the state
* machine filter (QS_FILTER_SM_OBJ) pertains to active objects as well.
* However, the state machine filter is more general, because it can be
* used only for state machines that are not active objects, such as
* "Orthogonal Components".
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_SM_OBJ(obj_)  (QS_priv_.smObjFilter = (obj_))

/*! Local Filter for a given active object \a obj_. */
/**
* \description
* This macro sets up the active object local filter if #Q_SPY is defined,
* or does nothing if #Q_SPY is not defined. The argument \a obj_ is the
* pointer to the active object that you want to monitor.
*
* The active object filter allows you to filter QS records pertaining
* only to a given active object. With this filter disabled, QS will
* output records from all active objects in your application. The object
* filter is disabled by setting the active object pointer \a obj_ to NULL.
*
* The active object filter affects the following QS records:
* ::QS_QF_ACTIVE_ADD, ::QS_QF_ACTIVE_REMOVE, ::QS_QF_ACTIVE_SUBSCRIBE,
* ::QS_QF_ACTIVE_UNSUBSCRIBE, ::QS_QF_ACTIVE_POST, ::QS_QF_ACTIVE_POST_LIFO,
* ::QS_QF_ACTIVE_GET, and ::QS_QF_ACTIVE_GET_LAST.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_AO_OBJ(obj_)  (QS_priv_.aoObjFilter = (obj_))

/*! Local Filter for a given memory pool object \a obj_. */
/**
* \description
* This macro sets up the memory pool local object filter if #Q_SPY is
* defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
* is the pointer to the memory buffer used during the initialization of
* the event pool with QF_poolInit().
*
* The memory pool filter allows you to filter QS records pertaining
* only to a given memory pool. With this filter disabled, QS will
* output records from all memory pools in your application. The object
* filter is disabled by setting the memory pool pointer \a obj_ to NULL.
*
* The memory pool filter affects the following QS records:
* ::QS_QF_MPOOL_INIT, ::QS_QF_MPOOL_GET, and ::QS_QF_MPOOL_PUT.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_MP_OBJ(obj_)  (QS_priv_.mpObjFilter = (obj_))

/*! Local Filter for a given event queue object \a obj_. */
/**
* \description
* This macro sets up the event queue object local filter if #Q_SPY is
* defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
* is the pointer to the "raw" thread-safe queue object you want to monitor.
*
* The event queue filter allows you to filter QS records pertaining
* only to a given event queue. With this filter disabled, QS will
* output records from all event queues in your application. The object
* filter is disabled by setting the event queue pointer \a obj_ to NULL.
*
* The event queue filter affects the following QS records:
* ::QS_QF_EQUEUE_INIT, ::QS_QF_EQUEUE_POST, ::QS_QF_EQUEUE_POST_LIFO,
* ::QS_QF_EQUEUE_GET, and ::QS_QF_EQUEUE_GET_LAST.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_EQ_OBJ(obj_)  (QS_priv_.eqObjFilter = (obj_))

/*! Local Filter for a given time event object \a obj_. */
/**
* \description
* This macro sets up the time event object local filter if #Q_SPY is defined,
* or does nothing if #Q_SPY is not defined. The argument \a obj_ is the
* pointer to the time event object you want to monitor.
*
* The time event filter allows you to filter QS records pertaining
* only to a given time event. With this filter disabled, QS will
* output records from all time events in your application. The object
* filter is disabled by setting the time event pointer \a obj_ to NULL.
*
* The time event filter affects the following QS records:
* ::QS_QF_TIMEEVT_ARM, ::QS_QF_TIMEEVT_AUTO_DISARM,
* ::QS_QF_TIMEEVT_DISARM_ATTEMPT, ::QS_QF_TIMEEVT_DISARM,
* ::QS_QF_TIMEEVT_REARM, ::QS_QF_TIMEEVT_POST, and ::QS_QF_TIMEEVT_PUBLISH.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_TE_OBJ(obj_)  (QS_priv_.teObjFilter = (obj_))

/*! Local Filter for a generic application object \a obj_. */
/**
* \description
* This macro sets up the application object local filter if #Q_SPY is
* defined, or does nothing if #Q_SPY is not defined. The argument \a obj_
* is the pointer to the application object you want to monitor.
*
* The application object filter allows you to filter QS records pertaining
* only to a given application object. With this filter disabled, QS will
* output records from all application-records enabled by the global filter.
* The local filter is disabled by setting the time event pointer \a obj_
* to NULL.
*
* \sa Example of using QS filters in #QS_FILTER_ON documentation
*/
#define QS_FILTER_AP_OBJ(obj_)  (QS_priv_.apObjFilter = (obj_))

/****************************************************************************/
/* Macros to generate user QS records */

/*! Begin a QS user record without entering critical section. */
#define QS_BEGIN_NOCRIT(rec_, obj_) \
    if ((((uint_fast8_t)QS_priv_.glbFilter[(uint8_t)(rec_) >> 3] \
          & (uint8_fast8_t)(1U << ((uint8_t)(rec_) & (uint8_t)7))) \
            != (uint_fast8_t)0) \
        && ((QS_priv_.apObjFilter == (void *)0) \
           || (QS_priv_.apObjFilter == (obj_)))) \
    { \
        QS_beginRec((uint_fast8_t)(rec_)); \
        QS_TIME_(); {

/*! End a QS user record without exiting critical section. */
#define QS_END_NOCRIT() } \
    QS_END_NOCRIT_()

/* QS-specific critical section *********************************************/
#ifdef QS_CRIT_ENTRY /* separate QS critical section defined? */

#ifndef QS_CRIT_STAT_TYPE
    #define QS_CRIT_STAT_
    #define QS_CRIT_ENTRY_()    QS_CRIT_ENTRY(dummy)
    #define QS_CRIT_EXIT_()     QS_CRIT_EXIT(dummy)
#else
    #define QS_CRIT_STAT_       QS_CRIT_STAT_TYPE critStat_;
    #define QS_CRIT_ENTRY_()    QS_CRIT_ENTRY(critStat_)
    #define QS_CRIT_EXIT_()     QS_CRIT_EXIT(critStat_)
#endif

#else /* separate QS critical section not defined--use the QF definition */

#ifndef QF_CRIT_STAT_TYPE
    /*! This is an internal macro for defining the critical section
    * status type. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * provides the definition of the critical section status variable.
    * Otherwise this macro is empty.
    * \sa #QF_CRIT_STAT_TYPE
    */
    #define QS_CRIT_STAT_

    /*! This is an internal macro for entering a critical section. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_ENTRY passing the key variable as the parameter.
    * Otherwise #QF_CRIT_ENTRY is invoked with a dummy parameter.
    * \sa #QF_CRIT_ENTRY
    */
    #define QS_CRIT_ENTRY_()    QF_CRIT_ENTRY(dummy)

    /*! This is an internal macro for exiting a critical section. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_EXIT passing the key variable as the parameter.
    * Otherwise #QF_CRIT_EXIT is invoked with a dummy parameter.
    * \sa #QF_CRIT_EXIT
    */
    #define QS_CRIT_EXIT_()     QF_CRIT_EXIT(dummy)

#else
    #define QS_CRIT_STAT_       QF_CRIT_STAT_TYPE critStat_;
    #define QS_CRIT_ENTRY_()    QF_CRIT_ENTRY(critStat_)

    #define QS_CRIT_EXIT_()     QF_CRIT_EXIT(critStat_)
#endif

#endif /* QS_CRIT_ENTRY */

/*! Begin a user QS record with entering critical section. */
/**
* \usage
* The following example shows how to build a user QS record using the
* macros #QS_BEGIN, #QS_END, and the formatted output macros: #QS_U8 and
* #QS_STR.
* \include qs_user.c
* \note Must always be used in pair with #QS_END
*/
#define QS_BEGIN(rec_, obj_) \
    if ((((uint_fast8_t)QS_priv_.glbFilter[(uint8_t)(rec_) >> 3] \
        & (uint_fast8_t)((uint_fast8_t)1 << ((uint8_t)(rec_) & (uint8_t)7))) \
            != (uint_fast8_t)0) \
        && ((QS_priv_.apObjFilter == (void *)0) \
            || (QS_priv_.apObjFilter == (obj_)))) \
    { \
        QS_CRIT_STAT_ \
        QS_CRIT_ENTRY_(); \
        QS_beginRec((uint_fast8_t)(rec_)); \
        QS_TIME_(); {

/*! End a QS record with exiting critical section. */
/** \sa example for #QS_BEGIN
* \note Must always be used in pair with #QS_BEGIN
*/
#define QS_END() } \
    QS_END_()

/****************************************************************************/

/*! Internal QS macro to begin a QS record with entering critical section. */
/**
* \note This macro is intended to use only inside QP components and NOT
* at the application level. \sa #QS_BEGIN
*/
#define QS_BEGIN_(rec_, objFilter_, obj_) \
    if ((((uint_fast8_t)QS_priv_.glbFilter[(uint8_t)(rec_) >> 3] \
        & (uint_fast8_t)((uint_fast8_t)1 << ((uint8_t)(rec_) & (uint8_t)7))) \
            != (uint_fast8_t)0) \
        && (((objFilter_) == (void *)0) \
            || ((objFilter_) == (obj_)))) \
    { \
        QS_CRIT_ENTRY_(); \
        QS_beginRec((uint_fast8_t)(rec_));

/*!  Internal QS macro to end a QS record with exiting critical section. */
/**
* \note This macro is intended to use only inside QP components and NOT
* at the application level. \sa #QS_END
*/
#define QS_END_() \
        QS_endRec(); \
        QS_CRIT_EXIT_(); \
    }

/*! Internal macro to begin a QS record without entering critical section. */
/**
* \note This macro is intended to use only inside QP components and NOT
* at the application level. \sa #QS_BEGIN_NOCRIT
*/
#define QS_BEGIN_NOCRIT_(rec_, objFilter_, obj_) \
    if ((((uint_fast8_t)QS_priv_.glbFilter[(uint8_t)(rec_) >> 3] \
        & (uint_fast8_t)((uint_fast8_t)1 << ((uint8_t)(rec_) & (uint8_t)7))) \
             != (uint_fast8_t)0) \
        && (((objFilter_) == (void *)0) \
            || ((objFilter_) == (obj_)))) \
    { \
        QS_beginRec((uint_fast8_t)(rec_));

/*! Internal QS macro to end a QS record without exiting critical section. */
/**
* \note This macro is intended to use only inside QP components and NOT
* at the application level. \sa #QS_END_NOCRIT
*/
#define QS_END_NOCRIT_() \
        QS_endRec(); \
    }

/*! Internal QS macro to output an unformatted uint8_t data element */
#define QS_U8_(data_)           (QS_u8_((uint8_t)(data_)))

/*! Internal QS macro to output 2 unformatted uint8_t data elements */
#define QS_2U8_(data1_, data2_) (QS_u8u8_((data1_), (data2_)))

/*! Internal QS macro to output an unformatted uint16_t data element */
#define QS_U16_(data_)          (QS_u16_((uint16_t)(data_)))

/*! Internal QS macro to output an unformatted uint32_t data element */
#define QS_U32_(data_)          (QS_u32_((uint32_t)(data_)))


#if (Q_SIGNAL_SIZE == 1)
    /*! Internal macro to output an unformatted event signal data element */
    /**
    * \note the size of the pointer depends on the macro #Q_SIGNAL_SIZE.
    */
    #define QS_SIG_(sig_)       (QS_u8_(sig_))
#elif (Q_SIGNAL_SIZE == 2)
    #define QS_SIG_(sig_)       (QS_u16_(sig_))
#elif (Q_SIGNAL_SIZE == 4)
    #define QS_SIG_(sig_)       (QS_u32_(sig_))
#endif


#if (QS_OBJ_PTR_SIZE == 1)
    #define QS_OBJ_(obj_)       (QS_u8_((uint8_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 2)
    #define QS_OBJ_(obj_)       (QS_u16_((uint16_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 4)
    #define QS_OBJ_(obj_)       (QS_u32_((uint32_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 8)
    #define QS_OBJ_(obj_)       (QS_u64_((uint64_t)(obj_)))
#else

    /*! Internal macro to output an unformatted object pointer data element */
    /** \note the size of the pointer depends on the macro #QS_OBJ_PTR_SIZE.
    * If the size is not defined the size of pointer is assumed 4-bytes.
    */
    #define QS_OBJ_(obj_)       (QS_u32_((uint32_t)(obj_))
#endif


#if (QS_FUN_PTR_SIZE == 1)
    #define QS_FUN_(fun_)       (QS_u8_((uint8_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 2)
    #define QS_FUN_(fun_)       (QS_u16_((uint16_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 4)
    #define QS_FUN_(fun_)       (QS_u32_((uint32_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 8)
    #define QS_FUN_(fun_)       (QS_u64_((uint64_t)(fun_)))
#else

    /*! Internal macro to output an unformatted function pointer */
    /** \note the size of the pointer depends on the macro #QS_FUN_PTR_SIZE.
    * If the size is not defined the size of pointer is assumed 4-bytes.
    */
    #define QS_FUN_(fun_)       (QS_u32_((uint32_t)(fun_)))
#endif


/*! Internal QS macro to output a zero-terminated ASCII string element */
#define QS_STR_(msg_)           (QS_str_((msg_)))

/*! Internal QS macro to output a zero-terminated ASCII string allocated
in ROM data element
*/
#define QS_STR_ROM_(msg_)       (QS_str_ROM_((msg_)))

/* Macros for use in the client code .......................................*/

/*! Enumerates data formats recognized by QS */
/**
* \description
* QS uses this enumeration is used only internally for the formatted user
* data elements.
*/
enum {
    QS_I8_T,              /*!< signed 8-bit integer format */
    QS_U8_T,              /*!< unsigned 8-bit integer format */
    QS_I16_T,             /*!< signed 16-bit integer format */
    QS_U16_T,             /*!< unsigned 16-bit integer format */
    QS_I32_T,             /*!< signed 32-bit integer format */
    QS_U32_T,             /*!< unsigned 32-bit integer format */
    QS_F32_T,             /*!< 32-bit floating point format */
    QS_F64_T,             /*!< 64-bit floating point format */
    QS_STR_T,             /*!< zero-terminated ASCII string format */
    QS_MEM_T,             /*!< up to 255-bytes memory block format */
    QS_SIG_T,             /*!< event signal format */
    QS_OBJ_T,             /*!< object pointer format */
    QS_FUN_T,             /*!< function pointer format */
    QS_I64_T,             /*!< signed 64-bit integer format */
    QS_U64_T,             /*!< unsigned 64-bit integer format */
    QS_U32_HEX_T          /*!< unsigned 32-bit integer in hex format */
};

/*! Output formatted int8_t to the QS record */
#define QS_I8(width_, data_) \
    (QS_u8((uint8_t)(((width_) << 4)) | (uint8_t)QS_I8_T, (data_)))

/*! Output formatted uint8_t to the QS record */
#define QS_U8(width_, data_) \
    (QS_u8((uint8_t)(((width_) << 4)) | (uint8_t)QS_U8_T, (data_)))

/*! Output formatted int16_t to the QS record */
#define QS_I16(width_, data_) \
    (QS_u16((uint8_t)(((width_) << 4)) | (uint8_t)QS_I16_T, (data_)))

/*! Output formatted uint16_t to the QS record */
#define QS_U16(width_, data_) \
    (QS_u16((uint8_t)(((width_) << 4)) | (uint8_t)QS_U16_T, (data_)))

/*! Output formatted int32_t to the QS record */
#define QS_I32(width_, data_) \
    (QS_u32((uint8_t)(((width_) << 4)) | (uint8_t)QS_I32_T, (data_)))

/*! Output formatted uint32_t to the QS record */
#define QS_U32(width_, data_) \
    (QS_u32((uint8_t)(((width_) << 4)) | (uint8_t)QS_U32_T, (data_)))

/*! Output formatted 32-bit floating point number to the QS record */
#define QS_F32(width_, data_) \
    (QS_f32((uint8_t)(((width_) << 4)) | (uint8_t)QS_F32_T, (data_)))

/*! Output formatted 64-bit floating point number to the QS record */
#define QS_F64(width_, data_) \
    (QS_f64((uint8_t)(((width_) << 4)) | (uint8_t)QS_F64_T, (data_)))

/*! Output formatted int64_t to the QS record */
#define QS_I64(width_, data_) \
    (QS_u64((uint8_t)(((width_) << 4)) | (uint8_t)QS_I64_T, (data_)))

/*! Output formatted uint64_t to the QS record */
#define QS_U64(width_, data_) \
    (QS_u64((uint8_t)(((width_) << 4)) | (uint8_t)QS_U64_T, (data_)))

/*! Output formatted uint32_t to the QS record */
#define QS_U32_HEX(width_, data_) \
    (QS_u32((uint8_t)(((width_) << 4)) | (uint8_t)QS_U32_HEX_T, (data_)))

/*! Output formatted zero-terminated ASCII string to the QS record */
#define QS_STR(str_)            (QS_str((str_)))

/*! Output formatted zero-terminated ASCII string from ROM
* to the QS record
*/
#define QS_STR_ROM(str_)        (QS_str_ROM((str_)))

/*! Output formatted memory block of up to 255 bytes to the QS record */
#define QS_MEM(mem_, size_)     (QS_mem((mem_), (size_)))


#if (QS_OBJ_PTR_SIZE == 1)
    #define QS_OBJ(obj_)        (QS_u8(QS_OBJ_T, (uint8_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 2)
    #define QS_OBJ(obj_)        (QS_u16(QS_OBJ_T, (uint16_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 4)
    #define QS_OBJ(obj_)        (QS_u32(QS_OBJ_T, (uint32_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 8)
    #define QS_OBJ(obj_)        (QS_u64(QS_OBJ_T, (uint64_t)(obj_)))
#else
    /*! Output formatted object pointer to the QS record */
    #define QS_OBJ(obj_)        (QS_u32(QS_OBJ_T, (uint32_t)(obj_)))
#endif


#if (QS_FUN_PTR_SIZE == 1)
    #define QS_FUN(fun_)        (QS_u8(QS_FUN_T, (uint8_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 2)
    #define QS_FUN(fun_)        (QS_u16(QS_FUN_T, (uint16_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 4)
    #define QS_FUN(fun_)        (QS_u32(QS_FUN_T, (uint32_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 8)
    #define QS_FUN(fun_)        (QS_u64(QS_FUN_T, (uint64_t)(fun_)))
#else

    /*! Output formatted function pointer to the QS record */
    #define QS_FUN(fun_)        (QS_u32(QS_FUN_T, (uint32_t)(fun_)))
#endif


#if (Q_SIGNAL_SIZE == 1)

    /*! Output formatted event signal (of type ::QSignal) and
    * the state machine object to the user QS record
    */
    #define QS_SIG(sig_, obj_) \
        QS_u8(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#elif (Q_SIGNAL_SIZE == 2)
    #define QS_SIG(sig_, obj_) \
        QS_u16(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#elif (Q_SIGNAL_SIZE == 4)
    #define QS_SIG(sig_, obj_) \
        QS_u32(QS_SIG_T, (sig_)); \
        QS_OBJ_(obj_)
#endif

/****************************************************************************/
/* Dictionary records */

/*! Output signal dictionary record */
/**
* \description
* A signal dictionary record associates the numerical value of the signal
* and the binary address of the state machine that consumes that signal
* with the human-readable name of the signal.
*
* Providing a signal dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable names.
*
* A signal dictionary entry is associated with both the signal value \a sig_
* and the state machine \a obj_, because signals are required to be unique
* only within a given state machine and therefore the same numerical values
* can represent different signals in different state machines.
*
* For the "global" signals that have the same meaning in all state machines
* (such as globally published signals), you can specify a signal dictionary
* entry with the \a obj_ parameter set to NULL.
*
* The following example shows the definition of signal dictionary entries
* in the initial transition of the Table active object. Please note that
* signals HUNGRY_SIG and DONE_SIG are associated with the Table state machine
* only ("me" \a obj_ pointer). The EAT_SIG signal, on the other hand,
* is global (0 \a obj_ pointer):
* \include qs_sigDic.c
*
* \note The QSpy log utility must capture the signal dictionary record
* in order to use the human-readable information. You need to connect to
* the target before the dictionary entries have been transmitted.
*
* The following QSpy log example shows the signal dictionary records
* generated from the Table initial transition and subsequent records that
* show human-readable names of the signals:
* \include qs_sigLog.txt
*
* The following QSpy log example shows the same sequence of records, but
* with dictionary records removed. The human-readable signal names are not
* available.
* \include qs_sigLog0.txt
*/
#define QS_SIG_DICTIONARY(sig_, obj_) do { \
    if (((QS_priv_.glbFilter[(uint8_t)QS_SIG_DICT >> 3] \
      & (uint8_t)(1U << ((uint8_t)QS_SIG_DICT & (uint8_t)7))) != (uint8_t)0))\
    { \
        static char_t const Q_ROM sig_name_[] = #sig_; \
        QS_sig_dict((sig_), (obj_), &sig_name_[0]); \
    } \
} while (0)

/*! Output object dictionary record */
/**
* \description
* An object dictionary record associates the binary address of an object
* in the target's memory with the human-readable name of the object.
*
* Providing an object dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable object names.
*
* The following example shows the definition of object dictionary entry
* for the Table active object:
* \include qs_objDic.c
*/
#define QS_OBJ_DICTIONARY(obj_) do { \
    if (((QS_priv_.glbFilter[(uint8_t)QS_OBJ_DICT >> 3] \
      & (uint8_t)(1U << ((uint8_t)QS_OBJ_DICT & (uint8_t)7))) != (uint8_t)0))\
    { \
        static char_t const Q_ROM obj_name_[] = #obj_; \
        QS_obj_dict((obj_), &obj_name_[0]); \
    } \
} while (0)

/*! Output function dictionary record */
/**
* \description
* A function dictionary record associates the binary address of a function
* in the target's memory with the human-readable name of the function.
*
* Providing a function dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable function names.
*
* The example from #QS_SIG_DICTIONARY shows the definition of a function
* dictionary.
*/
#define QS_FUN_DICTIONARY(fun_) do { \
    if (((QS_priv_.glbFilter[(uint8_t)QS_FUN_DICT >> 3] \
      & (uint8_t)(1U << ((uint8_t)QS_FUN_DICT & (uint8_t)7))) != (uint8_t)0))\
    { \
        static char_t const Q_ROM fun_name_[] = #fun_; \
        QS_fun_dict((void (*)(void))(fun_), &fun_name_[0]); \
    } \
} while (0)

/*! Output user QS rectord dictionary record */
/**
* \description
* A user QS record dictionary record associates the numerical value of a
* user record with the human-readable identifier.
*/
#define QS_USR_DICTIONARY(rec_) do { \
    if (((QS_priv_.glbFilter[(uint8_t)QS_USR_DICT >> 3] \
      & (uint8_t)(1U << ((uint8_t)QS_USR_DICT & (uint8_t)7))) != (uint8_t)0))\
    { \
        static char_t const Q_ROM usr_name_[] = #rec_; \
        QS_usr_dict((rec_), &usr_name_[0]); \
    } \
} while (0)

/*! Output the assertion failure trace record */
/**
* \description
* This trace record is intended to use from the Q_onAssert() callback.
*/
#define QS_ASSERTION(module_, loc_) do { \
    QS_BEGIN_NOCRIT_(QS_ASSERT_FAIL, (void *)0, (void *)0) \
        QS_TIME_(); \
        QS_U16_((uint16_t)(loc_)); \
        QS_STR_ROM_(module_); \
    QS_END_NOCRIT_() \
    QS_onFlush(); \
} while (0)

/*! Flush the QS trace data to the host */
/**
* \description
* This macro invokes the QS_flush() platform-dependent callback function
* to flush the QS trace buffer to the host. The function typically
* busy-waits until all the data in the buffer is sent to the host.
* This is acceptable only in the initial transient.
*/
#define QS_FLUSH()   (QS_onFlush())

/*! Output the critical section entry */
#define QF_QS_CRIT_ENTRY() \
    QS_BEGIN_NOCRIT_(QS_QF_CRIT_ENTRY, (void *)0, (void *)0) \
        QS_TIME_(); \
        QS_U8_(++QS_priv_.critNest); \
    QS_END_NOCRIT_()

/*! Output the critical section exit */
#define QF_QS_CRIT_EXIT() \
    QS_BEGIN_NOCRIT_(QS_QF_CRIT_EXIT, (void *)0, (void *)0) \
        QS_TIME_(); \
        QS_U8_(QS_priv_.critNest--); \
    QS_END_NOCRIT_()

/*! Output the interrupt entry record */
#define QF_QS_ISR_ENTRY(isrnest_, prio_) \
    QS_BEGIN_NOCRIT_(QS_QF_ISR_ENTRY, (void *)0, (void *)0) \
        QS_TIME_(); \
        QS_2U8_(isrnest_, prio_); \
    QS_END_NOCRIT_()

/*! Output the interrupt exit record */
#define QF_QS_ISR_EXIT(isrnest_, prio_) \
    QS_BEGIN_NOCRIT_(QS_QF_ISR_EXIT, (void *)0, (void *)0) \
        QS_TIME_(); \
        QS_2U8_(isrnest_, prio_); \
    QS_END_NOCRIT_()

/*! Execute an action that is only necessary for QS output */
#define QF_QS_ACTION(act_)      (act_)

/*! obtain the current QS version number string */
/**
* \description
* version of QS as a constant 5-character string of the form X.Y.Z,
* where X is a 1-digit major version number, Y is a 1-digit minor
* version number, and Z is a 1-digit release number.
*/
#define QS_getVersion() (QP_VERSION_STR)

/****************************************************************************/
/* QS private data */
typedef uint_fast16_t QSCtr;  /*!< QS ring buffer counter and offset type */

/*! Private QS data to keep track of the filters and the trace buffer. */
typedef struct {
    uint8_t    glbFilter[16]; /*!< global on/off QS filter */
    void const *smObjFilter;  /*!< state machine for QEP local filter */
    void const *aoObjFilter;  /*!< active object for QF local filter */
    void const *mpObjFilter;  /*!<  event pool for QF local filter */
    void const *eqObjFilter;  /*!<  raw queue for QF local filter */
    void const *teObjFilter;  /*!<  time event for QF local filter */
    void const *apObjFilter;  /*!<  generic object Application local filter */

    uint8_t *buf;         /*!< pointer to the start of the ring buffer */
    QSCtr    end;         /*!< offset of the end of the ring buffer */
    QSCtr    head;        /*!< offset to where next byte will be inserted */
    QSCtr    tail;        /*!< offset of where next event will be extracted */
    QSCtr    used;        /*!< number of bytes currently in the ring buffer */
    uint8_t  seq;         /*!< the record sequence number */
    uint8_t  chksum;      /*!< the checksum of the current record */

    uint_fast8_t critNest; /*!< critical section nesting level */
} QSPriv;

extern QSPriv QS_priv_;

#endif /* qs_h  */


