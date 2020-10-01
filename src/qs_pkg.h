/**
* @file
* @ingroup qs
* @brief Internal (package scope) QS/C interface.
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-15
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
#ifndef QS_PKG_H
#define QS_PKG_H

/****************************************************************************/
/*! Internal QS macro to begin a predefined QS record with
* entering critical section. */
/**
* @note This macro is intended to use only inside QP components and NOT
* at the application level.
* @sa QS_BEGIN_ID()
*/
#define QS_BEGIN_PRE_(rec_, qs_id_)                     \
    if (QS_GLB_CHECK_(rec_) && QS_LOC_CHECK_(qs_id_)) { \
        QS_CRIT_E_();                               \
        QS_beginRec_((uint_fast8_t)(rec_));

/*!  Internal QS macro to end a predefined QS record with
* exiting critical section. */
/**
* @note This macro is intended to use only inside QP components and NOT
* at the application level.
* @sa QS_END()
*/
#define QS_END_PRE_()    \
        QS_endRec_();    \
        QS_CRIT_X_(); \
    }

/*! Internal macro to begin a predefined QS record without
* entering critical section. */
/**
* @note This macro is intended to use only inside QP components and NOT
* at the application level.
* @sa QS_BEGIN_NOCRIT()
*/
#define QS_BEGIN_NOCRIT_PRE_(rec_, qs_id_)              \
    if (QS_GLB_CHECK_(rec_) && QS_LOC_CHECK_(qs_id_)) { \
        QS_beginRec_((uint_fast8_t)(rec_));

/*! Internal QS macro to end a predefined QS record without
* exiting critical section. */
/**
* @note This macro is intended to use only inside QP components and NOT
* at the application level. @sa #QS_END_NOCRIT
*/
#define QS_END_NOCRIT_PRE_()    QS_endRec_(); }

/*! Internal QS macro to output a predefined uint8_t data element */
#define QS_U8_PRE_(data_)       (QS_u8_raw_((uint8_t)(data_)))

/*! Internal QS macro to output 2 predefined uint8_t data elements */
#define QS_2U8_PRE_(data1_, data2_) \
    (QS_2u8_raw_((uint8_t)(data1_), (uint8_t)(data2_)))

/*! Internal QS macro to output an predefined uint16_t data element */
#define QS_U16_PRE_(data_)      (QS_u16_raw_((uint16_t)(data_)))

/*! Internal QS macro to output a predefined uint32_t data element */
#define QS_U32_PRE_(data_)      (QS_u32_raw_((uint32_t)(data_)))

/*! Internal QS macro to output a predefined zero-terminated string element */
#define QS_STR_PRE_(msg_)       (QS_str_raw_((msg_)))


#if (Q_SIGNAL_SIZE == 1U)
    /*! Internal macro to output an unformatted event signal data element */
    /**
    * @note the size of the pointer depends on the macro #Q_SIGNAL_SIZE.
    */
    #define QS_SIG_PRE_(sig_)   (QS_u8_raw_((uint8_t)sig_))
#elif (Q_SIGNAL_SIZE == 2U)
    #define QS_SIG_PRE_(sig_)   (QS_u16_raw_((uint16_t)sig_))
#elif (Q_SIGNAL_SIZE == 4U)
    #define QS_SIG_PRE_(sig_)   (QS_u32_raw_((uint32_t)sig_))
#endif

#define QS_OBJ_PRE_(obj_)       (QS_obj_raw_(obj_))

#if (QS_FUN_PTR_SIZE == 1U)
    #define QS_FUN_PRE_(fun_)   (QS_u8_raw_((uint8_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 2U)
    #define QS_FUN_PRE_(fun_)   (QS_u16_raw_((uint16_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 4U)
    #define QS_FUN_PRE_(fun_)   (QS_u32_raw_((uint32_t)(fun_)))
#elif (QS_FUN_PTR_SIZE == 8U)
    #define QS_FUN_PRE_(fun_)   (QS_u64_raw_((uint64_t)(fun_)))
#else
    /*! Internal macro to output an unformatted function pointer */
    /** @note the size of the pointer depends on the macro #QS_FUN_PTR_SIZE.
    * If the size is not defined the size of pointer is assumed 4-bytes.
    */
    #define QS_FUN_PRE_(fun_)   (QS_u32_raw_((uint32_t)(fun_)))
#endif


/****************************************************************************/
#if (QF_EQUEUE_CTR_SIZE == 1U)

    /*! Internal QS macro to output an unformatted event queue counter
    * data element. */
    /**
    * @note the counter size depends on the macro #QF_EQUEUE_CTR_SIZE.
    */
    #define QS_EQC_PRE_(ctr_)       QS_u8_raw_((uint8_t)(ctr_))
#elif (QF_EQUEUE_CTR_SIZE == 2U)
    #define QS_EQC_PRE_(ctr_)       QS_u16_raw_((uint16_t)(ctr_))
#elif (QF_EQUEUE_CTR_SIZE == 4U)
    #define QS_EQC_PRE_(ctr_)       QS_u32_raw_((uint32_t)(ctr_))
#endif


#if (QF_EVENT_SIZ_SIZE == 1U)

    /*! Internal QS macro to output an unformatted event size
    * data element. */
    /**
    * @note the event size depends on the macro #QF_EVENT_SIZ_SIZE.
    */
    #define QS_EVS_PRE_(size_)      QS_u8_raw_((uint8_t)(size_))
#elif (QF_EVENT_SIZ_SIZE == 2U)
    #define QS_EVS_PRE_(size_)      QS_u16_raw_((uint16_t)(size_))
#elif (QF_EVENT_SIZ_SIZE == 4U)
    #define QS_EVS_PRE_(size_)      QS_u32_raw_((uint32_t)(size_))
#endif


#if (QF_MPOOL_SIZ_SIZE == 1U)

    /*! Internal QS macro to output an unformatted memory pool
    * block-size data element */
    /**
    * @note the block-size depends on the macro #QF_MPOOL_SIZ_SIZE.
    */
    #define QS_MPS_PRE_(size_)      QS_u8_raw_((uint8_t)(size_))
#elif (QF_MPOOL_SIZ_SIZE == 2U)
    #define QS_MPS_PRE_(size_)      QS_u16_raw_((uint16_t)(size_))
#elif (QF_MPOOL_SIZ_SIZE == 4U)
    #define QS_MPS_PRE_(size_)      QS_u32_raw_((uint32_t)(size_))
#endif

#if (QF_MPOOL_CTR_SIZE == 1U)

    /*! Internal QS macro to output an unformatted memory pool
    * block-counter data element. */
    /**
    * @note the counter size depends on the macro #QF_MPOOL_CTR_SIZE.
    */
    #define QS_MPC_PRE_(ctr_)       QS_u8_raw_((uint8_t)(ctr_))
#elif (QF_MPOOL_CTR_SIZE == 2U)
    #define QS_MPC_PRE_(ctr_)       QS_u16_raw_((uint16_t)(ctr_))
#elif (QF_MPOOL_CTR_SIZE == 4U)
    #define QS_MPC_PRE_(ctr_)       QS_u32_raw_((uint16_t)(ctr_))
#endif


#if (QF_TIMEEVT_CTR_SIZE == 1U)

    /*! Internal QS macro to output an unformatted time event
    * tick-counter data element */
    /**
    * @note the counter size depends on the macro #QF_TIMEEVT_CTR_SIZE.
    */
    #define QS_TEC_PRE_(ctr_)       QS_u8_raw_((uint8_t)(ctr_))
#elif (QF_TIMEEVT_CTR_SIZE == 2U)
    #define QS_TEC_PRE_(ctr_)       QS_u16_raw_((uint16_t)(ctr_))
#elif (QF_TIMEEVT_CTR_SIZE == 4U)
    #define QS_TEC_PRE_(ctr_)       QS_u32_raw_((uint32_t)(ctr_))
#endif


/****************************************************************************/
/*! Internal QS macro to insert an un-escaped byte into the QS buffer */
#define QS_INSERT_BYTE_(b_) \
    buf[head] = (b_);       \
    ++head;                 \
    if (head == end) {      \
        head = 0U;          \
    }

/****************************************************************************/
/*! Internal QS macro to insert an escaped byte into the QS buffer */
#define QS_INSERT_ESC_BYTE_(b_)                      \
    chksum = (uint8_t)(chksum + (b_));               \
    if (((b_) != QS_FRAME) && ((b_) != QS_ESC)) {    \
        QS_INSERT_BYTE_(b_)                          \
    }                                                \
    else {                                           \
        QS_INSERT_BYTE_(QS_ESC)                      \
        QS_INSERT_BYTE_((uint8_t)((b_) ^ QS_ESC_XOR))\
        ++QS_priv_.used;                             \
    }

/****************************************************************************/
/*! Frame character of the QS output protocol */
#define QS_FRAME    (0x7EU)

/****************************************************************************/
/*! Escape character of the QS output protocol */
#define QS_ESC      (0x7DU)

/****************************************************************************/
/*! The expected checksum value over an uncorrupted QS record */
#define QS_GOOD_CHKSUM (0xFFU)

/****************************************************************************/
/*! Escape modifier of the QS output protocol */
/**
* @description
* The escaped byte is XOR-ed with the escape modifier before it is inserted
* into the QS buffer.
*/
#define QS_ESC_XOR  (0x20U)

/****************************************************************************/
/*! send the predefined target info trace record
 * (object sizes, build time-stamp, QP version) */
void QS_target_info_pre_(uint8_t isReset);

/****************************************************************************/
/*! Private QS-RX attributes to keep track of the current objects and
* the lock-free RX buffer
*/
typedef struct {
    void     *currObj[MAX_OBJ]; /*!< current objects */
    uint8_t  *buf;        /*!< pointer to the start of the ring buffer */
    QSCtr     end;        /*!< offset of the end of the ring buffer */
    QSCtr     head;       /*!< offset to where next byte will be inserted */
    QSCtr     tail;       /*!< offset of where next byte will be extracted */
#ifdef Q_UTEST
    QPSet     readySet;   /*!< QUTEST ready-set of active objects */
    bool      inTestLoop; /*!< QUTEST event loop is running */
#endif
} QSrxPrivAttr;

extern QSrxPrivAttr QS_rxPriv_; /* QS-RX private attributes */

#endif  /* QS_PKG_H */

