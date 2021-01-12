/**
* @file
* @brief QS/C platform-independent public interface.
* @ingroup qs qpspy
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2021-01-12
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2020 Quantum Leaps, LLC. All rights reserved.
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
#ifndef QS_H
#define QS_H

#ifndef Q_SPY
    #error "Q_SPY must be defined to include qs.h"
#endif

/****************************************************************************/
/*! Quantum Spy record types. */
/**
* @description
* This enumeration specifies the record types used in the QP components.
* You can specify your own record types starting from ::QS_USER offset.
* Currently, the maximum of all records cannot exceed 125.
*
* @note
* The QS records labeled as "not maskable" are always enabled and cannot
* be turend off with the QS_GLB_FILTER() macro. Other QS trace records
* can be disabled by means of the "global filters"
*
* @sa QS_GLB_FILTER() macro
*/
enum QSpyRecords {
    /* [0] QS session (not maskable) */
    QS_EMPTY,             /*!< QS record for cleanly starting a session */

    /* [1] SM records */
    QS_QEP_STATE_ENTRY,   /*!< a state was entered */
    QS_QEP_STATE_EXIT,    /*!< a state was exited */
    QS_QEP_STATE_INIT,    /*!< an initial transition was taken in a state */
    QS_QEP_INIT_TRAN,     /*!< the top-most initial transition was taken */
    QS_QEP_INTERN_TRAN,   /*!< an internal transition was taken */
    QS_QEP_TRAN,          /*!< a regular transition was taken */
    QS_QEP_IGNORED,       /*!< an event was ignored (silently discarded) */
    QS_QEP_DISPATCH,      /*!< an event was dispatched (begin of RTC step) */
    QS_QEP_UNHANDLED,     /*!< an event was unhandled due to a guard */

    /* [10] Active Object (AO) records */
    QS_QF_ACTIVE_DEFER,   /*!< AO deferred an event */
    QS_QF_ACTIVE_RECALL,  /*!< AO recalled an event */
    QS_QF_ACTIVE_SUBSCRIBE,   /*!< an AO subscribed to an event */
    QS_QF_ACTIVE_UNSUBSCRIBE, /*!< an AO unsubscribed to an event */
    QS_QF_ACTIVE_POST,      /*!< an event was posted (FIFO) directly to AO */
    QS_QF_ACTIVE_POST_LIFO, /*!< an event was posted (LIFO) directly to AO */
    QS_QF_ACTIVE_GET,     /*!< AO got an event and its queue is not empty */
    QS_QF_ACTIVE_GET_LAST,/*!< AO got an event and its queue is empty */
    QS_QF_ACTIVE_RECALL_ATTEMPT, /*!< AO attempted to recall an event */

    /* [19] Event Queue (EQ) records */
    QS_QF_EQUEUE_POST,      /*!< an event was posted (FIFO) to a raw queue */
    QS_QF_EQUEUE_POST_LIFO, /*!< an event was posted (LIFO) to a raw queue */
    QS_QF_EQUEUE_GET,     /*!< get an event and queue still not empty */
    QS_QF_EQUEUE_GET_LAST,/*!< get the last event from the queue */

    /* [23] Framework (QF) records */
    QS_QF_NEW_ATTEMPT,   /*!< an attempt to allocate an event failed */

    /* [24] Memory Pool (MP) records */
    QS_QF_MPOOL_GET,      /*!< a memory block was removed from memory pool */
    QS_QF_MPOOL_PUT,      /*!< a memory block was returned to memory pool */

    /* [26] Additional Framework (QF) records */
    QS_QF_PUBLISH,        /*!< an event was published */
    QS_QF_NEW_REF,        /*!< new event reference was created */
    QS_QF_NEW,            /*!< new event was created */
    QS_QF_GC_ATTEMPT,     /*!< garbage collection attempt */
    QS_QF_GC,             /*!< garbage collection */
    QS_QF_TICK,           /*!< QF_tickX() was called */

    /* [32] Time Event (TE) records */
    QS_QF_TIMEEVT_ARM,    /*!< a time event was armed */
    QS_QF_TIMEEVT_AUTO_DISARM, /*!< a time event expired and was disarmed */
    QS_QF_TIMEEVT_DISARM_ATTEMPT,/*!< attempt to disarm a disarmed QTimeEvt */
    QS_QF_TIMEEVT_DISARM, /*!< true disarming of an armed time event */
    QS_QF_TIMEEVT_REARM,  /*!< rearming of a time event */
    QS_QF_TIMEEVT_POST,   /*!< a time event posted itself directly to an AO */

    /* [38] Additional Framework (QF) records */
    QS_QF_DELETE_REF,     /*!< an event reference is about to be deleted */
    QS_QF_CRIT_ENTRY,     /*!< critical section was entered */
    QS_QF_CRIT_EXIT,      /*!< critical section was exited */
    QS_QF_ISR_ENTRY,      /*!< an ISR was entered */
    QS_QF_ISR_EXIT,       /*!< an ISR was exited */
    QS_QF_INT_DISABLE,    /*!< interrupts were disabled */
    QS_QF_INT_ENABLE,     /*!< interrupts were enabled */

    /* [45] Additional Active Object (AO) records */
    QS_QF_ACTIVE_POST_ATTEMPT,/*!< attempt to post an evt to AO failed */

    /* [46] Additional Event Queue (EQ) records */
    QS_QF_EQUEUE_POST_ATTEMPT,/*!< attempt to post an evt to QEQueue failed */

    /* [47] Additional Memory Pool (MP) records */
    QS_QF_MPOOL_GET_ATTEMPT,  /*!< attempt to get a memory block failed */

    /* [48] Scheduler (SC) records */
    QS_MUTEX_LOCK,        /*!< a mutex was locked */
    QS_MUTEX_UNLOCK,      /*!< a mutex was unlocked */
    QS_SCHED_LOCK,        /*!< scheduler was locked */
    QS_SCHED_UNLOCK,      /*!< scheduler was unlocked */
    QS_SCHED_NEXT,        /*!< scheduler found next task to execute */
    QS_SCHED_IDLE,        /*!< scheduler became idle */
    QS_SCHED_RESUME,      /*!< scheduler resumed previous task (not idle) */

    /* [55] Additional QEP records */
    QS_QEP_TRAN_HIST,     /*!< a tran to history was taken */
    QS_QEP_TRAN_EP,       /*!< a tran to entry point into a submachine */
    QS_QEP_TRAN_XP,       /*!< a tran to exit  point out of a submachine */

    /* [58] Miscellaneous QS records (not maskable) */
    QS_TEST_PAUSED,       /*!< test has been paused */
    QS_TEST_PROBE_GET,    /*!< reports that Test-Probe has been used */
    QS_SIG_DICT,          /*!< signal dictionary entry */
    QS_OBJ_DICT,          /*!< object dictionary entry */
    QS_FUN_DICT,          /*!< function dictionary entry */
    QS_USR_DICT,          /*!< user QS record dictionary entry */
    QS_TARGET_INFO,       /*!< reports the Target information */
    QS_TARGET_DONE,       /*!< reports completion of a user callback */
    QS_RX_STATUS,         /*!< reports QS data receive status */
    QS_QUERY_DATA,        /*!< reports the data from "current object" query */
    QS_PEEK_DATA,         /*!< reports the data from the PEEK query */
    QS_ASSERT_FAIL,       /*!< assertion failed in the code */
    QS_QF_RUN,            /*!< QF_run() was entered */

    /* [71] Reserved QS records */
    QS_RESERVED_71,
    QS_RESERVED_72,
    QS_RESERVED_73,
    QS_RESERVED_74,
    QS_RESERVED_75,
    QS_RESERVED_76,
    QS_RESERVED_77,
    QS_RESERVED_78,
    QS_RESERVED_79,
    QS_RESERVED_80,
    QS_RESERVED_81,
    QS_RESERVED_82,
    QS_RESERVED_83,
    QS_RESERVED_84,
    QS_RESERVED_85,
    QS_RESERVED_86,
    QS_RESERVED_87,
    QS_RESERVED_88,
    QS_RESERVED_89,
    QS_RESERVED_90,
    QS_RESERVED_91,
    QS_RESERVED_92,
    QS_RESERVED_93,
    QS_RESERVED_94,
    QS_RESERVED_95,
    QS_RESERVED_96,
    QS_RESERVED_97,
    QS_RESERVED_98,
    QS_RESERVED_99,

    /* [100] Application-specific (User) QS records */
    QS_USER               /*!< the first record available to QS users */
};

/*! QS record groups for QS_GLB_FILTER() */
enum QSpyRecordGroups {
    QS_ALL_RECORDS = 0xF0,/*!< all maskable QS records */
    QS_SM_RECORDS,        /*!< State Machine QS records */
    QS_AO_RECORDS,        /*!< Active Object QS records */
    QS_EQ_RECORDS,        /*!< Event Queues QS records */
    QS_MP_RECORDS,        /*!< Memory Pools QS records */
    QS_TE_RECORDS,        /*!< Time Events QS records */
    QS_QF_RECORDS,        /*!< QF QS records */
    QS_SC_RECORDS,        /*!< Scheduler QS records */
    QS_U0_RECORDS,        /*!< User Group 100-104 records */
    QS_U1_RECORDS,        /*!< User Group 105-109 records */
    QS_U2_RECORDS,        /*!< User Group 110-114 records */
    QS_U3_RECORDS,        /*!< User Group 115-119 records */
    QS_U4_RECORDS,        /*!< User Group 120-124 records */
    QS_UA_RECORDS         /*!< All User records */
};

/*! QS user record group offsets for QS_GLB_FILTER() */
enum QSpyUserOffsets {
    QS_USER0 = (enum_t)QS_USER,      /*!< offset for User Group 0 */
    QS_USER1 = (enum_t)QS_USER0 + 5, /*!< offset for User Group 1 */
    QS_USER2 = (enum_t)QS_USER1 + 5, /*!< offset for User Group 2 */
    QS_USER3 = (enum_t)QS_USER2 + 5, /*!< offset for User Group 3 */
    QS_USER4 = (enum_t)QS_USER3 + 5  /*!< offset for User Group 4 */
};

/*! QS ID offsets for QS_LOC_FILTER() */
enum QSpyIdOffsets {
    QS_AO_ID = 0,  /*!< offset for AO priorities */
    QS_EP_ID = 64, /*!< offset for event-pool IDs */
    QS_EQ_ID = 80, /*!< offset for event-queue IDs */
    QS_AP_ID = 96, /*!< offset for Application-specific IDs */
};

/*! QS ID groups for QS_LOC_FILTER() */
enum QSpyIdGroups {
    QS_ALL_IDS = 0xF0,                      /*!< all QS IDs */
    QS_AO_IDS  = (0x80 + (enum_t)QS_AO_ID), /*!< AO IDs (priorities) */
    QS_EP_IDS  = (0x80 + (enum_t)QS_EP_ID), /*!< event-pool IDs */
    QS_EQ_IDS  = (0x80 + (enum_t)QS_EQ_ID), /*!< event-queue IDs */
    QS_AP_IDS  = (0x80 + (enum_t)QS_AP_ID), /*!< Application-specific IDs */
};

#ifndef QS_TIME_SIZE

    /*! The size [bytes] of the QS time stamp. Valid values: 1U, 2U, or 4U;
    * default 4U.
    */
    /**
    * @description
    * This macro can be defined in the QS port file (qs_port.h) to
    * configure the ::QSTimeCtr type. Here the macro is not defined so the
    * default of 4 byte is chosen.
    */
    #define QS_TIME_SIZE 4U
#endif

#if (QS_TIME_SIZE == 1U)
    typedef uint8_t QSTimeCtr;
    #define QS_TIME_PRE_()      (QS_u8_raw_(QS_onGetTime()))
#elif (QS_TIME_SIZE == 2U)
    typedef uint16_t QSTimeCtr;
    #define QS_TIME_PRE_()      (QS_u16_raw_(QS_onGetTime()))
#elif (QS_TIME_SIZE == 4U)
    /*! The type of the QS time stamp. This type determines the dynamic
    * range of QS time stamps
    */
    typedef uint32_t QSTimeCtr;

    /*! Internal macro to output time stamp to a QS record */
    #define QS_TIME_PRE_()      (QS_u32_raw_(QS_onGetTime()))
#else
    #error "QS_TIME_SIZE defined incorrectly, expected 1, 2, or 4"
#endif


/****************************************************************************/
/* QS services. */

/*! Initialize the QS data buffer. */
void QS_initBuf(uint8_t sto[], uint_fast16_t stoSize);

/*! Set/clear the global Filter for a given QS record or group of records. */
void QS_glbFilter_(int_fast16_t const filter);

/*! Set/clear the local Filter for a given object-id or group of object-ids.*/
void QS_locFilter_(int_fast16_t const filter);

/*! Mark the begin of a QS record @p rec */
void QS_beginRec_(uint_fast8_t rec);

/*! Mark the end of a QS record @p rec */
void QS_endRec_(void);

/* unformatted (raw) data elements output ..................................*/
/*! output raw uint8_t data element (without format information) */
void QS_u8_raw_(uint8_t d);

/*! output two raw uint8_t data elements (without format information) */
void QS_2u8_raw_(uint8_t d1, uint8_t d2);

/*! Output raw uint16_t data element (without format information) */
void QS_u16_raw_(uint16_t d);

/*! Output raw uint32_t data element (without format information) */
void QS_u32_raw_(uint32_t d);

/*! Output raw zero-terminated string element (without format information) */
void QS_str_raw_(char_t const *str);

/* formatted data elements output ..........................................*/
/*! Output uint8_t data element with format information */
void QS_u8_fmt_(uint8_t format, uint8_t d);

/*! output uint16_t data element with format information */
void QS_u16_fmt_(uint8_t format, uint16_t d);

/*! Output uint32_t data element with format information */
void QS_u32_fmt_(uint8_t format, uint32_t d);

/*! Output 32-bit floating point data element with format information */
void QS_f32_fmt_(uint8_t format, float32_t f);

/*! Output 64-bit floating point data element with format information */
void QS_f64_fmt_(uint8_t format, float64_t d);

/*! Output obj pointer data element without format information */
void QS_obj_raw_(void const * const obj);

/*! Output zero-terminated ASCII string element with format information */
void QS_str_fmt_(char_t const *str);

/*! Output memory block of up to 255-bytes with format information */
void QS_mem_fmt_(uint8_t const *blk, uint8_t size);

/*! Output raw uint64_t data element without format information */
void QS_u64_raw_(uint64_t d);

/*! Output uint64_t data element with format information */
void QS_u64_fmt_(uint8_t format, uint64_t d);

/* QS buffer access *********************************************************/
/*! Byte-oriented interface to the QS data buffer. */
uint16_t QS_getByte(void);

/*! Constant for End-Of-Data condition returned from QS_getByte() */
#define QS_EOD ((uint16_t)0xFFFFU)

/*! Block-oriented interface to the QS data buffer. */
uint8_t const *QS_getBlock(uint16_t *pNbytes);


/* platform-specific callback functions, need to be implemented by clients */

/*! Callback to startup the QS facility */
/**
* @description
* This is a platform-dependent "callback" function invoked through the macro
* QS_INIT(). You need to implement this function in your application.
* At a minimum, the function must configure the QS buffer by calling
* QS_initBuf(). Typically, you will also want to open/configure the QS output
* channel, such as a serial port, or a data file. The void* argument @p arg
* can be used to pass parameter(s) needed to configure the output channel.
*
* @returns the staus of initialization. Typically 1 (true) when the QS
* initialization was successful, or 0 (false) when it failed.
*
* @usage
* The following example illustrates an implementation of QS_onStartup():
* @include qs_startup.c
*/
uint8_t QS_onStartup(void const *arg);

/*! Callback to cleanup the QS facility */
/**
* @description
* This is a platform-dependent "callback" function invoked through the macro
* QS_EXIT(). You need to implement this function in your application.
* The main purpose of this function is to close the QS output channel, if
* necessary.
*/
void QS_onCleanup(void);

/*! Callback to flush the QS trace data to the host */
/**
* @description
* This is a platform-dependent "callback" function to flush the QS trace
* buffer to the host. The function typically busy-waits until all the data
* in the buffer is sent to the host. This is acceptable only in the initial
* transient.
*/
void QS_onFlush(void);

/*! Callback to obtain a timestamp for a QS record. */
/**
* @description
* This is a platform-dependent "callback" function invoked from the macro
* QS_TIME_PRE_() to add the time stamp to a QS record.
*
* @note Some of the predefined QS records from QP do not output the time
* stamp. However, ALL user records do output the time stamp.
* @note QS_onGetTime() is called in a critical section and should not
* exit the critical section.
*
* @returns the current timestamp.
*
* @usage
* The following example shows using a system call to implement QS
* time stamping:
* @include qs_onGetTime.c
*/
QSTimeCtr QS_onGetTime(void);


/****************************************************************************/
/* Macros for adding QS instrumentation to the client code */

/*! Initialize the QS facility. */
/**
* @description
* This macro provides an indirection layer to invoke the QS initialization
* routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
* @sa QS_onStartup(), example of setting up a QS filter in QS_GLB_FILTER()
*/
#define QS_INIT(arg_)           (QS_onStartup(arg_))

/*! Cleanup the QS facility. */
/**
* @description
* This macro provides an indirection layer to invoke the QS cleanup
* routine if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
* @sa QS_onCleanup()
*/
#define QS_EXIT()               (QS_onCleanup())

/*! Flush the QS trace data to the host */
/**
* @description
* This macro invokes the QS_flush() platform-dependent callback function
* to flush the QS trace buffer to the host. The function typically
* busy-waits until all the data in the buffer is sent to the host.
* This is acceptable only in the initial transient.
*/
#define QS_FLUSH()   (QS_onFlush())

/*! Global Filter for a given record type @p rec. */
/**
* @description
* This macro provides an indirection layer to call QS_glbFilter_()
* if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
*
* The following example shows how to use QS filters:
* @include qs_filter.c
*/
#define QS_GLB_FILTER(rec_)  (QS_glbFilter_((int_fast16_t)(rec_)))

/*! Local Filter for a given object-id @p qs_id. */
/**
* @description
* This macro provides an indirection layer to call QS_locFilter_()
* if #Q_SPY is defined, or do nothing if #Q_SPY is not defined.
*
* The following example shows how to use QS filters:
* @include qs_filter.c
*/
#define QS_LOC_FILTER(qs_id_)  (QS_locFilter_((int_fast16_t)(qs_id_)))


/****************************************************************************/
/* Facilities for QS ciritical section */

/* QS-specific critical section */
#ifdef QS_CRIT_ENTRY /* separate QS critical section defined? */

#ifndef QS_CRIT_STAT_TYPE
    #define QS_CRIT_STAT_
    #define QS_CRIT_E_()     QS_CRIT_ENTRY(dummy)
    #define QS_CRIT_X_()     QS_CRIT_EXIT(dummy); QS_REC_DONE()
#else
    #define QS_CRIT_STAT_    QS_CRIT_STAT_TYPE critStat_;
    #define QS_CRIT_E_()     QS_CRIT_ENTRY(critStat_)
    #define QS_CRIT_X_()     QS_CRIT_EXIT(critStat_); QS_REC_DONE()
#endif /* QS_CRIT_STAT_TYPE */

#else /* separate QS critical section not defined--use the QF definition */

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
    #define QS_CRIT_STAT_

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
    #define QS_CRIT_E_()     QF_CRIT_ENTRY(dummy)

    /*! This is an internal macro for exiting a critical section. */
    /**
    * @description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes QF_CRIT_EXIT() passing the key variable as the parameter.
    * Otherwise QF_CRIT_EXIT() is invoked with a dummy parameter.
    * @sa QF_CRIT_EXIT()
    */
    #define QS_CRIT_X_()     QF_CRIT_EXIT(dummy); QS_REC_DONE()

#elif (!defined QS_CRIT_STAT_)

    #define QS_CRIT_STAT_    QF_CRIT_STAT_TYPE critStat_;
    #define QS_CRIT_E_()     QF_CRIT_ENTRY(critStat_)
    #define QS_CRIT_X_()     QF_CRIT_EXIT(critStat_); QS_REC_DONE()

#endif /* simple unconditional interrupt disabling used */

#endif /* separate QS critical section not defined */


/****************************************************************************/
/* Macros to generate application-specific (user) QS records */

/*! Begin a QS user record without entering critical section. */
#define QS_BEGIN_NOCRIT(rec_, qs_id_)                   \
    if (QS_GLB_CHECK_(rec_) && QS_LOC_CHECK_(qs_id_)) { \
        QS_beginRec_((uint_fast8_t)(rec_));             \
        QS_TIME_PRE_(); {

/*! End a QS user record without exiting critical section. */
#define QS_END_NOCRIT() } \
        QS_endRec_();     \
    }

#ifndef QS_REC_DONE
    /*! macro to hook up user code when a QS record is produced */
    #define QS_REC_DONE() ((void)0)
#endif /* QS_REC_DONE */

/*! helper macro for checking the global QS filter */
#define QS_GLB_CHECK_(rec_)                                         \
    (((uint_fast8_t)QS_priv_.glbFilter[(uint_fast8_t)(rec_) >> 3U]  \
          & ((uint_fast8_t)1U << ((uint_fast8_t)(rec_) & 7U))) != 0U)

/*! helper macro for checking the local QS filter */
#define QS_LOC_CHECK_(qs_id_)                                        \
    (((uint_fast8_t)QS_priv_.locFilter[(uint_fast8_t)(qs_id_) >> 3U] \
          & ((uint_fast8_t)1U << ((uint_fast8_t)(qs_id_) & 7U))) != 0U)

/*! Begin an application-specific (user) QS record with object-id
 * for the local filter. */
/**
* @usage
* The following example shows how to build a user QS record using the
* macros QS_BEGIN_ID(), QS_END(), and the formatted output macros:
* QS_U8(), QS_STR(), etc..
* @include qs_ap.c
* @note Must always be used in pair with QS_END()
*/
#define QS_BEGIN_ID(rec_, qs_id_)                       \
    if (QS_GLB_CHECK_(rec_) && QS_LOC_CHECK_(qs_id_)) { \
        QS_CRIT_STAT_                                   \
        QS_CRIT_E_();                                   \
        QS_beginRec_((uint_fast8_t)(rec_));             \
        QS_TIME_PRE_(); {

/*! End a QS record with exiting critical section. */
/** @sa example for QS_BEGIN_ID()
* @note Must always be used in pair with QS_BEGIN_ID()
*/
#define QS_END() }    \
        QS_endRec_(); \
        QS_CRIT_X_(); \
    }

/*! formats for application-specific data elements */
/**
* @description
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
    QS_HEX_FMT            /*!< HEX format for the "width" filed */
};

/*! Output formatted int8_t to the QS record */
#define QS_I8(width_, data_) \
    (QS_u8_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_I8_T, (data_)))

/*! Output formatted uint8_t to the QS record */
#define QS_U8(width_, data_) \
    (QS_u8_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_U8_T, (data_)))

/*! Output formatted int16_t to the QS record */
#define QS_I16(width_, data_) \
    (QS_u16_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_I16_T, (data_)))

/*! Output formatted uint16_t to the QS record */
#define QS_U16(width_, data_) \
    (QS_u16_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_U16_T, (data_)))

/*! Output formatted int32_t to the QS record */
#define QS_I32(width_, data_) \
    (QS_u32_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_I32_T, (data_)))

/*! Output formatted uint32_t to the QS record */
#define QS_U32(width_, data_) \
    (QS_u32_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_U32_T, (data_)))

/*! Output formatted 32-bit floating point number to the QS record */
#define QS_F32(width_, data_) \
    (QS_f32_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_F32_T, (data_)))

/*! Output formatted 64-bit floating point number to the QS record */
#define QS_F64(width_, data_) \
    (QS_f64_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_F64_T, (data_)))

/*! Output formatted int64_t to the QS record */
#define QS_I64(width_, data_) \
    (QS_u64_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_I64_T, (data_)))

/*! Output formatted uint64_t to the QS record */
#define QS_U64(width_, data_) \
    (QS_u64_fmt_((uint8_t)(((width_) << 4)) | (uint8_t)QS_U64_T, (data_)))

/*! Output formatted zero-terminated ASCII string to the QS record */
#define QS_STR(str_)            (QS_str_fmt_((str_)))

/*! Output formatted memory block of up to 255 bytes to the QS record */
#define QS_MEM(mem_, size_)     (QS_mem_fmt_((mem_), (size_)))


#if (QS_OBJ_PTR_SIZE == 1U)
    #define QS_OBJ(obj_)        (QS_u8_fmt_(QS_OBJ_T, (uint8_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 2U)
    #define QS_OBJ(obj_)        (QS_u16_fmt_(QS_OBJ_T, (uint16_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 4U)
    #define QS_OBJ(obj_)        (QS_u32_fmt_(QS_OBJ_T, (uint32_t)(obj_)))
#elif (QS_OBJ_PTR_SIZE == 8U)
    #define QS_OBJ(obj_)        (QS_u64_fmt_(QS_OBJ_T, (uint64_t)(obj_)))
#else
    /*! Output formatted object pointer to the QS record */
    #define QS_OBJ(obj_)        (QS_u32_fmt_(QS_OBJ_T, (uint32_t)(obj_)))
#endif


#if (QS_FUN_PTR_SIZE == 1U)
    #define QS_FUN(fun_)        (QS_u8_fmt_(QS_FUN_T, (uint8_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 2U)
    #define QS_FUN(fun_)        (QS_u16_fmt_(QS_FUN_T, (uint16_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 4U)
    #define QS_FUN(fun_)        (QS_u32_fmt_(QS_FUN_T, (uint32_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 8U)
    #define QS_FUN(fun_)        (QS_u64_fmt_(QS_FUN_T, (uint64_t)(fun_)))
#else
    /*! Output formatted function pointer to the QS record */
    #define QS_FUN(fun_)        (QS_u32_fmt_(QS_FUN_T, (uint32_t)(fun_)))
#endif

#if (Q_SIGNAL_SIZE == 1)

    #define QS_SIG(sig_, obj_)   \
        QS_u8_fmt_(QS_SIG_T, (sig_)); \
        QS_obj_raw_(obj_)

#elif (Q_SIGNAL_SIZE == 2)

    #define QS_SIG(sig_, obj_)    \
        QS_u16_fmt_(QS_SIG_T, (sig_)); \
        QS_obj_raw_(obj_)

#elif (Q_SIGNAL_SIZE == 4)

    #define QS_SIG(sig_, obj_)    \
        QS_u32_fmt_(QS_SIG_T, (sig_)); \
        QS_obj_raw_(obj_)

#else
    /*! Output formatted event signal (of type ::QSignal) and
    * the state machine object to the user QS record
    */
    #define QS_SIG(sig_, obj_)    \
        QS_u16_fmt_(QS_SIG_T, (sig_)); \
        QS_obj_raw_(obj_)

#endif


/****************************************************************************/
/* Dictionary trace records */

/*! Output signal dictionary record */
/**
* @description
* A signal dictionary record associates the numerical value of the signal
* and the binary address of the state machine that consumes that signal
* with the human-readable name of the signal.
*
* Providing a signal dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable names.
*
* A signal dictionary entry is associated with both the signal value @p sig_
* and the state machine @p obj_, because signals are required to be unique
* only within a given state machine and therefore the same numerical values
* can represent different signals in different state machines.
*
* For the "global" signals that have the same meaning in all state machines
* (such as globally published signals), you can specify a signal dictionary
* entry with the @p obj_ parameter set to NULL.
*
* The following example shows the definition of signal dictionary entries
* in the initial transition of the Table active object. Please note that
* signals HUNGRY_SIG and DONE_SIG are associated with the Table state machine
* only ("me" @p obj_ pointer). The EAT_SIG signal, on the other hand,
* is global (0 @p obj_ pointer):
* @include qs_sigDic.c
*
* @note The QSpy log utility must capture the signal dictionary record
* in order to use the human-readable information. You need to connect to
* the target before the dictionary entries have been transmitted.
*
* The following QSpy log example shows the signal dictionary records
* generated from the Table initial transition and subsequent records that
* show human-readable names of the signals:
* @include qs_sigLog.txt
*
* The following QSpy log example shows the same sequence of records, but
* with dictionary records removed. The human-readable signal names are not
* available.
* @include qs_sigLog0.txt
*/
#define QS_SIG_DICTIONARY(sig_, obj_) \
    (QS_sig_dict_pre_((sig_), (obj_), #sig_))

/*! Output object dictionary record */
/**
* @description
* An object dictionary record associates the binary address of an object
* in the target's memory with the human-readable name of the object.
*
* Providing an object dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable object names.
*
* The following example shows the definition of object dictionary entry
* for the Table active object:
* @include qs_objDic.c
*/
#define QS_OBJ_DICTIONARY(obj_) \
    (QS_obj_dict_pre_((obj_), #obj_))

/*! Output function dictionary record */
/**
* @description
* A function dictionary record associates the binary address of a function
* in the target's memory with the human-readable name of the function.
*
* Providing a function dictionary QS record can vastly improve readability of
* the QS log, because instead of dealing with cryptic machine addresses the
* QSpy host utility can display human-readable function names.
*
* The example from QS_SIG_DICTIONARY() shows the definition of a function
* dictionary.
*/
#define QS_FUN_DICTIONARY(fun_) \
    (QS_fun_dict_pre_((void (*)(void))(fun_), #fun_))

/*! Output user QS-record dictionary record */
/**
* @description
* A user QS-record dictionary record associates the numerical value of a
* user record with the human-readable identifier.
*/
#define QS_USR_DICTIONARY(rec_) \
    (QS_usr_dict_pre_((rec_), #rec_))

/*! Output predefined signal-dictionary record */
void QS_sig_dict_pre_(enum_t const sig, void const * const obj,
                      char_t const *name);

/*! Output predefined object-dictionary record */
void QS_obj_dict_pre_(void const * const obj,
                      char_t const *name);

/*! Output predefined function-dictionary record */
void QS_fun_dict_pre_(void (* const fun)(void),
                      char_t const *name);

/*! Output predefined user-dictionary record */
void QS_usr_dict_pre_(enum_t const rec,
                      char_t const * const name);


/****************************************************************************/
/* Miscellaneous pre-formatted trace records used in applications */

/*! Output the assertion failure trace record */
/**
* @description
* This trace record is intended to use from the Q_onAssert() callback.
*/
void QS_ASSERTION(char_t const * const module,
                  int_t const loc,
                  uint32_t delay);

/*! Output the critical section entry */
void QF_QS_CRIT_ENTRY(void);

/*! Output the critical section exit */
void QF_QS_CRIT_EXIT(void);

/*! Output the interrupt entry record */
void QF_QS_ISR_ENTRY(uint8_t const isrnest, uint8_t const prio);

/*! Output the interrupt exit record */
void QF_QS_ISR_EXIT(uint8_t const isrnest, uint8_t const prio);

/*! Execute an action that is only necessary for QS output */
#define QF_QS_ACTION(act_)    (act_)


/****************************************************************************/
/* QS private data (the TX channel) */
typedef uint_fast16_t QSCtr;  /*!< QS ring buffer counter and offset type */

/*! Kinds of objects used in QS. */
enum QSpyObjKind {
    SM_OBJ,    /*!< state machine object */
    AO_OBJ,    /*!< active object */
    MP_OBJ,    /*!< event pool object */
    EQ_OBJ,    /*!< raw queue object */
    TE_OBJ,    /*!< time event object */
    AP_OBJ,    /*!< generic Application-specific object */
    MAX_OBJ
};

enum OSpyObjCombnation {
    SM_AO_OBJ = (enum_t)MAX_OBJ /*!< combination of SM and AO */
};

/*! Private QS attributes to keep track of the filters and the trace buffer */
typedef struct {
    uint8_t glbFilter[16]; /*!< global on/off QS filter */
    uint8_t locFilter[16]; /*!< local QS filters */
    void const *locFilter_AP; /*!< deprecated local QS filter */
    uint8_t *buf;         /*!< pointer to the start of the ring buffer */
    QSCtr    end;         /*!< offset of the end of the ring buffer */
    QSCtr    head;        /*!< offset to where next byte will be inserted */
    QSCtr    tail;        /*!< offset of where next byte will be extracted */
    QSCtr    used;        /*!< number of bytes currently in the ring buffer */
    uint8_t  seq;         /*!< the record sequence number */
    uint8_t  chksum;      /*!< the checksum of the current record */

    uint8_t  critNest;    /*!< critical section nesting level */
} QSPrivAttr;

extern QSPrivAttr QS_priv_;


/****************************************************************************/
/* QS receive channel */

/*! Enumeration for the received Qs record types (RX channel). */
enum QSpyRxRecords {
    QS_RX_INFO,           /*!< query Target info (ver, config, tstamp) */
    QS_RX_COMMAND,        /*!< execute a user-defined command in the Target */
    QS_RX_RESET,          /*!< reset the Target */
    QS_RX_TICK,           /*!< call QF_TICK_X() in the Target */
    QS_RX_PEEK,           /*!< peek Target memory */
    QS_RX_POKE,           /*!< poke Target memory */
    QS_RX_FILL,           /*!< fill Target memory */
    QS_RX_TEST_SETUP,     /*!< test setup */
    QS_RX_TEST_TEARDOWN,  /*!< test teardown */
    QS_RX_TEST_PROBE,     /*!< set a Test-Probe in the Target */
    QS_RX_GLB_FILTER,     /*!< set global filters in the Target */
    QS_RX_LOC_FILTER,     /*!< set local  filters in the Target */
    QS_RX_AO_FILTER,      /*!< set local AO filter in the Target */
    QS_RX_CURR_OBJ,       /*!< set the "current-object" in the Target */
    QS_RX_TEST_CONTINUE,  /*!< continue a test after QS_TEST_PAUSE() */
    QS_RX_QUERY_CURR,     /*!< query the "current object" in the Target */
    QS_RX_EVENT           /*!< inject an event to the Target */
};

/*! Initialize the QS RX data buffer. */
void QS_rxInitBuf(uint8_t sto[], uint16_t stoSize);

/*! Parse all bytes present in the QS RX data buffer */
void QS_rxParse(void);

/*! Put one byte into the QS RX lock-free buffer */
bool QS_RX_PUT(uint8_t const b);

/*! Obtain the number of free bytes in the QS RX data buffer */
uint16_t QS_rxGetNfree(void);

/*! Set the "current object" in the Target */
void QS_setCurrObj(uint8_t obj_kind, void *obj_ptr);

/*! Query the "current object" in the Target */
void QS_queryCurrObj(uint8_t obj_kind);

/*! Callback function to reset the Target (to be implemented in the BSP) */
void QS_onReset(void);

/*! Callback function to execute user commands (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,   uint32_t param1,
                  uint32_t param2, uint32_t param3);

/*! Macro to handle the QS output from the application
* NOTE: if this macro is used, the application must define QS_output().
*/
#define QS_OUTPUT()   (QS_output())

/*! Macro to handle the QS-RX input to the application
* NOTE: if this macro is used, the application must define QS_rx_input().
*/
#define QS_RX_INPUT() (QS_rx_input())

/****************************************************************************/
/* Facilities for use in QUTest only */
#ifdef Q_UTEST
    /*! callback to setup a unit test inside the Target */
    void QS_onTestSetup(void);

    /*! callback to teardown after a unit test inside the Target */
    void QS_onTestTeardown(void);

    /*! callback to run the test loop */
    void QS_onTestLoop(void);

    /*! callback to "massage" the test event before dispatching/posting it */
    void QS_onTestEvt(QEvt *e);

    /*! callback to examine an event that is about to be posted */
    void QS_onTestPost(void const *sender, QActive *recipient,
                       QEvt const *e, bool status);

    /*! QS internal function to process posted events during test */
    void QS_processTestEvts_(void);

    /*! internal function to process armed time events during test */
    void QS_tickX_(uint_fast8_t const tickRate, void const * const sender);

    /*! QS internal function to get the Test-Probe for a given API */
    uint32_t QS_getTestProbe_(void (* const api)(void));

    /*! QS macro to define the Test-Probe for a given @p fun_ */
    #define QS_TEST_PROBE_DEF(fun_) \
        uint32_t const qs_tp_ = QS_getTestProbe_((void (*)(void))(fun_));

    /*! QS macro to apply a Test-Probe */
    #define QS_TEST_PROBE(code_) \
        if (qs_tp_ != 0U) { code_ }

    /*! QS macro to apply a Test-Probe */
    #define QS_TEST_PROBE_ID(id_, code_) \
        if (qs_tp_ == (uint32_t)(id_)) { code_ }

    /*! QS macro to pause test execution and enter the test event loop */
    #define QS_TEST_PAUSE() do {                    \
        QS_beginRec_((uint_fast8_t)QS_TEST_PAUSED); \
        QS_endRec_();                               \
        QS_onTestLoop();                            \
    } while (false)

    enum QUTestUserRecords {
        QUTEST_ON_POST = 124
    };

    /* interrupt nesting up-down counter */
    extern uint8_t volatile QF_intNest;

    /************************************************************************/
    /*! QActiveDummy Object class */
    /**
    * @description
    * QActiveDummy is a test double for the role of collaborating active
    * objects in QUTest unit testing.
    */
    typedef struct {
        QActive super; /*< inherit QActive */
    } QActiveDummy;

    /*! Constructor of the QActiveDummy Active Object class */
    void QActiveDummy_ctor(QActiveDummy * const me);

#else /* Q_UTEST not defined */

    /* dummy definitions when not building for QUTEST */
    #define QS_TEST_PROBE_DEF(fun_)
    #define QS_TEST_PROBE(code_)
    #define QS_TEST_PROBE_ID(id_, code_)
    #define QS_TEST_PAUSE()  ((void)0)

#endif /* Q_UTEST */

#endif /* QS_H  */

