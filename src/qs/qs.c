/**
* @file
* @brief QS software tracing services
* @ingroup qs
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-03-04
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
* <www.state-machine.com>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qs_port.h"      /* QS port */
#include "qs_pkg.h"       /* QS package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qs")

/****************************************************************************/
QSPrivAttr QS_priv_;  /* QS private attributes */

/****************************************************************************/
/**
* @description
* This function should be called from QS_onStartup() to provide QS with
* the data buffer. The first parameter @p sto[] is the address of the memory
* block, and the second parameter @p stoSize is the size of this block
* in bytes. Currently the size of the QS buffer cannot exceed 64KB.
*
* @note QS can work with quite small data buffers, but you will start losing
* data if the buffer is too small for the bursts of tracing activity.
* The right size of the buffer depends on the data production rate and
* the data output rate. QS offers flexible filtering to reduce the data
* production rate.
*
* @note If the data output rate cannot keep up with the production rate,
* QS will start overwriting the older data with newer data. This is
* consistent with the "last-is-best" QS policy. The record sequence counters
* and check sums on each record allow the QSPY host uitiliy to easily detect
* any data loss.
*
* @note
* This function initializes all the internal QS variables, so that the
* tracing can start correctly even if the startup code fails to clear
* any uninitialized data (as is required by the C Standard).
*/
void QS_initBuf(uint8_t sto[], uint_fast16_t stoSize) {
    /* the provided buffer must be at least 8 bytes long */
    Q_REQUIRE_ID(100, stoSize > 8U);

    /* all maskable filters off */
    QS_filterOff_((uint_fast8_t)QS_ALL_RECORDS);

    QS_priv_.locFilter[SM_OBJ] = (void *)0;
    QS_priv_.locFilter[AO_OBJ] = (void *)0;
    QS_priv_.locFilter[MP_OBJ] = (void *)0;
    QS_priv_.locFilter[EQ_OBJ] = (void *)0;
    QS_priv_.locFilter[TE_OBJ] = (void *)0;
    QS_priv_.locFilter[AP_OBJ] = (void *)0;

    QS_priv_.buf      = &sto[0];
    QS_priv_.end      = (QSCtr)stoSize;
    QS_priv_.head     = 0U;
    QS_priv_.tail     = 0U;
    QS_priv_.used     = 0U;
    QS_priv_.seq      = 0U;
    QS_priv_.chksum   = 0U;
    QS_priv_.critNest = 0U;

    /* produce an empty record to "flush" the QS trace buffer */
    QS_beginRec_((uint_fast8_t)QS_EMPTY);
    QS_endRec_();

    /* produce the reset record to inform QSPY of a new session */
    QS_target_info_pre_(0xFFU); /* send Reset and Target info */

    /* wait with flushing after successfull initialization (see QS_INIT()) */
}

/****************************************************************************/
/**
* @description
* This function sets up the QS filter to enable the record type @p rec.
* The argument @a #QS_ALL_RECORDS specifies to filter-in all records.
* This function should be called indirectly through the macro #QS_FILTER_ON.
*
* @note Filtering based on the record-type is only the first layer of
* filtering. The second layer is based on the object-type. Both filter
* layers must be enabled for the QS record to be inserted into the QS buffer.
* @sa QS_filterOff_(), QS_FILTER_SM_OBJ, QS_FILTER_AO_OBJ, QS_FILTER_MP_OBJ,
* QS_FILTER_EQ_OBJ, and QS_FILTER_TE_OBJ.
*/
void QS_filterOn_(uint_fast8_t rec) {
    if (rec == (uint_fast8_t)QS_ALL_RECORDS) {
        uint_fast8_t i;
        for (i = 0U; i < 15U; ++i) {
            QS_priv_.glbFilter[i] = 0xFFU; /* set all bits */
        }
        /* never turn the last 3 records on (0x7D, 0x7E, 0x7F) */
        QS_priv_.glbFilter[sizeof(QS_priv_.glbFilter) - 1U] = 0x1FU;
    }
    else if (rec == (uint_fast8_t)QS_SM_RECORDS) {
        QS_priv_.glbFilter[0] |= 0xFEU;
        QS_priv_.glbFilter[1] |= 0x03U;
        QS_priv_.glbFilter[6] |= 0x80U;
        QS_priv_.glbFilter[7] |= 0x03U;
    }
    else if (rec == (uint_fast8_t)QS_AO_RECORDS) {
        QS_priv_.glbFilter[1] |= 0xFCU;
        QS_priv_.glbFilter[2] |= 0x07U;
        QS_priv_.glbFilter[5] |= 0x20U;
    }
    else if (rec == (uint_fast8_t)QS_EQ_RECORDS) {
        QS_priv_.glbFilter[2] |= 0x78U;
        QS_priv_.glbFilter[5] |= 0x40U;
    }
    else if (rec == (uint_fast8_t)QS_MP_RECORDS) {
        QS_priv_.glbFilter[3] |= 0x03U;
        QS_priv_.glbFilter[5] |= 0x80U;
    }
    else if (rec == (uint_fast8_t)QS_QF_RECORDS) {
        QS_priv_.glbFilter[3] |= 0xFCU;
        QS_priv_.glbFilter[4] |= 0xC0U;
        QS_priv_.glbFilter[5] |= 0x1FU;
    }
    else if (rec == (uint_fast8_t)QS_TE_RECORDS) {
        QS_priv_.glbFilter[4] |= 0x7FU;
    }
    else if (rec == (uint_fast8_t)QS_SC_RECORDS) {
        QS_priv_.glbFilter[6] |= 0x7FU;
    }
    else if (rec == (uint_fast8_t)QS_U0_RECORDS) {
        QS_priv_.glbFilter[12] |= 0xF0U;
        QS_priv_.glbFilter[13] |= 0x01U;
    }
    else if (rec == (uint_fast8_t)QS_U1_RECORDS) {
        QS_priv_.glbFilter[13] |= 0x1EU;
    }
    else if (rec == (uint_fast8_t)QS_U2_RECORDS) {
        QS_priv_.glbFilter[13] |= 0xE0U;
        QS_priv_.glbFilter[14] |= 0x03U;
    }
    else if (rec == (uint_fast8_t)QS_U3_RECORDS) {
        QS_priv_.glbFilter[14] |= 0xF8U;
    }
    else if (rec == (uint_fast8_t)QS_U4_RECORDS) {
        QS_priv_.glbFilter[15] |= 0x1FU;
    }
    else if (rec == (uint_fast8_t)QS_UA_RECORDS) {
        QS_priv_.glbFilter[12] |= 0xF0U;
        QS_priv_.glbFilter[13] |= 0xFFU;
        QS_priv_.glbFilter[14] |= 0xFFU;
        QS_priv_.glbFilter[15] |= 0x1FU;
    }
    else {
        /* record numbers can't exceed QS_ESC, so they don't need escaping */
        Q_ASSERT_ID(210, rec < QS_ESC);
        QS_priv_.glbFilter[rec >> 3] |= (uint8_t)(1U << (rec & 7U));
    }
}

/****************************************************************************/
/**
* @description
* This function sets up the QS filter to disable the record type @p rec.
* The argument @a #QS_ALL_RECORDS specifies to suppress all records.
* This function should be called indirectly through the macro #QS_FILTER_OFF.
*
* @note Filtering records based on the record-type is only the first layer of
* filtering (global filter). The second layer is based on the object-type
* (local filter). Both filter layers must be enabled for the QS record to be
* inserted into the QS buffer.
*/
void QS_filterOff_(uint_fast8_t rec) {
    uint8_t tmp;

    if (rec == (uint_fast8_t)QS_ALL_RECORDS) {
        /* first clear all global filters */
        for (tmp = 15U; tmp > 0U; --tmp) {
            QS_priv_.glbFilter[tmp] = 0x00U;
        }
        /* next leave the specific filters enabled  */
        QS_priv_.glbFilter[0] = 0x01U;
        QS_priv_.glbFilter[7] = 0xFCU;
        QS_priv_.glbFilter[8] = 0x3FU;
    }
    else if (rec == (uint_fast8_t)QS_SM_RECORDS) {
        QS_priv_.glbFilter[0] &= ~(uint8_t)0xFEU;
        QS_priv_.glbFilter[1] &= ~(uint8_t)0x03U;
        QS_priv_.glbFilter[6] &= ~(uint8_t)0x80U;
        QS_priv_.glbFilter[7] &= ~(uint8_t)0x03U;
    }
    else if (rec == (uint_fast8_t)QS_AO_RECORDS) {
        QS_priv_.glbFilter[1] &= ~(uint8_t)0xFCU;
        QS_priv_.glbFilter[2] &= ~(uint8_t)0x07U;
        QS_priv_.glbFilter[5] &= ~(uint8_t)0x20U;
    }
    else if (rec == (uint_fast8_t)QS_EQ_RECORDS) {
        QS_priv_.glbFilter[2] &= ~(uint8_t)0x78U;
        QS_priv_.glbFilter[5] &= ~(uint8_t)0x40U;
    }
    else if (rec == (uint_fast8_t)QS_MP_RECORDS) {
        QS_priv_.glbFilter[3] &= ~(uint8_t)0x03U;
        QS_priv_.glbFilter[5] &= ~(uint8_t)0x80U;
    }
    else if (rec == (uint_fast8_t)QS_QF_RECORDS) {
        QS_priv_.glbFilter[3] &= ~(uint8_t)0xFCU;
        QS_priv_.glbFilter[4] &= ~(uint8_t)0xC0U;
        QS_priv_.glbFilter[5] &= ~(uint8_t)0x1FU;
    }
    else if (rec == (uint_fast8_t)QS_TE_RECORDS) {
        QS_priv_.glbFilter[4] &= ~(uint8_t)0x7FU;
    }
    else if (rec == (uint_fast8_t)QS_SC_RECORDS) {
        QS_priv_.glbFilter[6] &= ~(uint8_t)0x7FU;
    }
    else if (rec == (uint_fast8_t)QS_U0_RECORDS) {
        QS_priv_.glbFilter[12] &= ~(uint8_t)0xF0U;
        QS_priv_.glbFilter[13] &= ~(uint8_t)0x01U;
    }
    else if (rec == (uint_fast8_t)QS_U1_RECORDS) {
        QS_priv_.glbFilter[13] &= ~(uint8_t)0x1EU;
    }
    else if (rec == (uint_fast8_t)QS_U2_RECORDS) {
        QS_priv_.glbFilter[13] &= ~(uint8_t)0xE0U;
        QS_priv_.glbFilter[14] &= ~(uint8_t)0x03U;
    }
    else if (rec == (uint_fast8_t)QS_U3_RECORDS) {
        QS_priv_.glbFilter[14] &= ~(uint8_t)0xF8U;
    }
    else if (rec == (uint_fast8_t)QS_U4_RECORDS) {
        QS_priv_.glbFilter[15] &= 0x1FU;
    }
    else if (rec == (uint_fast8_t)QS_UA_RECORDS) {
        QS_priv_.glbFilter[12] &= ~(uint8_t)0xF0U;
        QS_priv_.glbFilter[13] = 0U;
        QS_priv_.glbFilter[14] = 0U;
        QS_priv_.glbFilter[15] &= ~(uint8_t)0x1FU;
    }
    else {
        /* record IDs can't exceed QS_ESC, so they don't need escaping */
        Q_ASSERT_ID(310, rec < (uint_fast8_t)QS_ESC);
        tmp =  (uint8_t)(1U << (rec & 0x07U));
        tmp ^= 0xFFU; /* invert all bits */
        QS_priv_.glbFilter[rec >> 3] &= tmp;
    }
}

/****************************************************************************/
/**
* @description
* This function must be called at the beginning of each QS record.
* This function should be called indirectly through the macro #QS_BEGIN,
* or #QS_BEGIN_NOCRIT, depending if it's called in a normal code or from
* a critical section.
*/
void QS_beginRec_(uint_fast8_t rec) {
    uint8_t b      = (uint8_t)(QS_priv_.seq + 1U);
    uint8_t chksum = 0U;              /* reset the checksum */
    uint8_t *buf   = QS_priv_.buf;    /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;   /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;    /* put in a temporary (register) */

    QS_priv_.seq = b; /* store the incremented sequence num */
    QS_priv_.used += 2U; /* 2 bytes about to be added */

    QS_INSERT_ESC_BYTE_(b)

    chksum = (uint8_t)(chksum + rec); /* update checksum */
    QS_INSERT_BYTE_((uint8_t)rec) /* rec byte does not need escaping */

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/**
* @description
* This function must be called at the end of each QS record.
* This function should be called indirectly through the macro #QS_END,
* or #QS_END_NOCRIT, depending if it's called in a normal code or from
* a critical section.
*/
void QS_endRec_(void) {
    uint8_t *buf = QS_priv_.buf;  /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;
    QSCtr   end  = QS_priv_.end;
    uint8_t b = QS_priv_.chksum;
    b ^= 0xFFU;   /* invert the bits in the checksum */

    QS_priv_.used += 2U; /* 2 bytes about to be added */

    if ((b != QS_FRAME) && (b != QS_ESC)) {
        QS_INSERT_BYTE_(b)
    }
    else {
        QS_INSERT_BYTE_(QS_ESC)
        QS_INSERT_BYTE_(b ^ QS_ESC_XOR)
        ++QS_priv_.used; /* account for the ESC byte */
    }

    QS_INSERT_BYTE_(QS_FRAME) /* do not escape this QS_FRAME */

    QS_priv_.head = head; /* save the head */

    /* overrun over the old data? */
    if (QS_priv_.used > end) {
        QS_priv_.used = end;   /* the whole buffer is used */
        QS_priv_.tail = head;  /* shift the tail to the old data */
    }
}

/****************************************************************************/
void QS_target_info_pre_(uint8_t isReset) {
    static uint8_t const ZERO = (uint8_t)'0';
    static uint8_t const * const TIME = (uint8_t const *)&Q_BUILD_TIME[0];
    static uint8_t const * const DATE = (uint8_t const *)&Q_BUILD_DATE[0];
    uint8_t b;

    QS_beginRec_((uint_fast8_t)QS_TARGET_INFO);
        QS_U8_PRE_(isReset);
        QS_U16_PRE_(QP_VERSION); /* two-byte version number */

        /* send the object sizes... */
        QS_U8_PRE_(Q_SIGNAL_SIZE | (QF_EVENT_SIZ_SIZE << 4));

#ifdef QF_EQUEUE_CTR_SIZE
        QS_U8_PRE_(QF_EQUEUE_CTR_SIZE | (QF_TIMEEVT_CTR_SIZE << 4));
#else
        QS_U8_PRE_(QF_TIMEEVT_CTR_SIZE << 4);
#endif /* QF_EQUEUE_CTR_SIZE */

#ifdef QF_MPOOL_CTR_SIZE
        QS_U8_PRE_(QF_MPOOL_SIZ_SIZE | (QF_MPOOL_CTR_SIZE << 4));
#else
        QS_U8_PRE_(0U);
#endif /* QF_MPOOL_CTR_SIZE */

        QS_U8_PRE_(QS_OBJ_PTR_SIZE | (QS_FUN_PTR_SIZE << 4));
        QS_U8_PRE_(QS_TIME_SIZE);

        /* send the limits... */
        QS_U8_PRE_(QF_MAX_ACTIVE);
        QS_U8_PRE_(QF_MAX_EPOOL | (QF_MAX_TICK_RATE << 4));

        // send the build time in three bytes (sec, min, hour)...
        QS_U8_PRE_((10U * (uint8_t)(TIME[6] - ZERO))
                   + (uint8_t)(TIME[7] - ZERO));
        QS_U8_PRE_((10U * (uint8_t)(TIME[3] - ZERO))
                   + (uint8_t)(TIME[4] - ZERO));
        if (Q_BUILD_TIME[0] == ' ') {
            QS_U8_PRE_(TIME[1] - ZERO);
        }
        else {
            QS_U8_PRE_((10U * (uint8_t)(TIME[0] - ZERO))
                      + (uint8_t)(TIME[1] - ZERO));
        }

        // send the build date in three bytes (day, month, year) ...
        if (Q_BUILD_DATE[4] == ' ') {
            QS_U8_PRE_(DATE[5] - ZERO);
        }
        else {
            QS_U8_PRE_((10U * (uint8_t)(DATE[4] - ZERO))
                       + (uint8_t)(DATE[5] - ZERO));
        }
        // convert the 3-letter month to a number 1-12 ...
        switch (DATE[0] + DATE[1] + DATE[2]) {
            case (int_t)'J' + (int_t)'a' + (int_t)'n':
                b = 1U;
                break;
            case (int_t)'F' + (int_t)'e' + (int_t)'b':
                b = 2U;
                break;
            case (int_t)'M' + (int_t)'a' + (int_t)'r':
                b = 3U;
                break;
            case (int_t)'A' + (int_t)'p' + (int_t)'r':
                b = 4U;
                break;
            case (int_t)'M' + (int_t)'a' + (int_t)'y':
                b = 5U;
                break;
            case (int_t)'J' + (int_t)'u' + (int_t)'n':
                b = 6U;
                break;
            case (int_t)'J' + (int_t)'u' + (int_t)'l':
                b = 7U;
                break;
            case (int_t)'A' + (int_t)'u' + (int_t)'g':
                b = 8U;
                break;
            case (int_t)'S' + (int_t)'e' + (int_t)'p':
                b = 9U;
                break;
            case (int_t)'O' + (int_t)'c' + (int_t)'t':
                b = 10U;
                break;
            case (int_t)'N' + (int_t)'o' + (int_t)'v':
                b = 11U;
                break;
            case (int_t)'D' + (int_t)'e' + (int_t)'c':
                b = 12U;
                break;
            default:
                b = 0U;
                break;
        }
        QS_U8_PRE_(b); // store the month
        QS_U8_PRE_((10U * (uint8_t)(DATE[9] - ZERO))
                   + (uint8_t)(DATE[10] - ZERO));
    QS_endRec_();
}

/****************************************************************************/
/**
* @description
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u8_fmt_(uint8_t format, uint8_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;    /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;   /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;    /* put in a temporary (register) */

    QS_priv_.used += 2U; /* 2 bytes about to be added */

    QS_INSERT_ESC_BYTE_(format)
    QS_INSERT_ESC_BYTE_(d)

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/**
* @description
* This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u16_fmt_(uint8_t format, uint16_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;    /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;   /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;    /* put in a temporary (register) */
    uint8_t b = (uint8_t)d;

    QS_priv_.used += 3U; /* 3 bytes about to be added */

    QS_INSERT_ESC_BYTE_(format)
    QS_INSERT_ESC_BYTE_(b)
    b = (uint8_t)(d >> 8);
    QS_INSERT_ESC_BYTE_(b)

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/**
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u32_fmt_(uint8_t format, uint32_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;    /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;   /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;    /* put in a temporary (register) */
    uint32_t x = d;
    uint_fast8_t i;

    QS_priv_.used += 5U; /* 5 bytes about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* insert 4 bytes... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)x)
        x >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/*! output uint8_t data element without format information */
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u8_raw_(uint8_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */

    QS_priv_.used += 1U; /* 1 byte about to be added */
    QS_INSERT_ESC_BYTE_(d)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_2u8_raw_(uint8_t d1, uint8_t d2) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */

    QS_priv_.used += 2U; /* 2 bytes are about to be added */
    QS_INSERT_ESC_BYTE_(d1)
    QS_INSERT_ESC_BYTE_(d2)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/**
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u16_raw_(uint16_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    uint16_t x   = d;

    QS_priv_.used += 2U; /* 2 bytes are about to be added */

    QS_INSERT_ESC_BYTE_((uint8_t)x)
    x >>= 8;
    QS_INSERT_ESC_BYTE_((uint8_t)x)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u32_raw_(uint32_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    uint32_t x = d;
    uint_fast8_t i;

    QS_priv_.used += 4U; /* 4 bytes are about to be added */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)x)
        x >>= 8;
    }

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_obj_raw_(void const * const obj) {
#if (QS_OBJ_PTR_SIZE == 1U)
    QS_u8_raw_((uint8_t)obj);
#elif (QS_OBJ_PTR_SIZE == 2U)
    QS_u16_raw_((uint16_t)obj);
#elif (QS_OBJ_PTR_SIZE == 4U)
    QS_u32_raw_((uint32_t)obj);
#elif (QS_OBJ_PTR_SIZE == 8U)
    QS_u64_raw_((uint64_t)obj);
#else
    QS_u32_raw_((uint32_t)obj);
#endif
}

/****************************************************************************/
/**
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_str_raw_(char_t const *str) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    QSCtr   used = QS_priv_.used;     /* put in a temporary (register) */
    char_t const *s;

    for (s = str; *s != '\0'; ++s) {
        chksum += (uint8_t)*s; /* update checksum */
        QS_INSERT_BYTE_((uint8_t)*s)  /* ASCII char doesn't need escaping */
        ++used;
    }
    QS_INSERT_BYTE_((uint8_t)'\0')  /* zero-terminate the string */
    ++used;

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
    QS_priv_.used   = used;   /* save # of used buffer space */
}

/****************************************************************************/
/**
* @description
* This function delivers one byte at a time from the QS data buffer.
*
* @returns the byte in the least-significant 8-bits of the 16-bit return
* value if the byte is available. If no more data is available at the time,
* the function returns ::QS_EOD (End-Of-Data).
*
* @note QS_getByte() is __not__ protected with a critical section.
*/
uint16_t QS_getByte(void) {
    uint16_t ret;
    if (QS_priv_.used == 0U) {
        ret = QS_EOD; /* set End-Of-Data */
    }
    else {
        uint8_t const *buf = QS_priv_.buf;  /* put in a temporary */
        QSCtr tail   = QS_priv_.tail; /* put in a temporary (register) */
        ret = (uint16_t)buf[tail];    /* set the byte to return */
        ++tail; /* advance the tail */
        if (tail == QS_priv_.end) { /* tail wrap around? */
            tail = 0U;
        }
        QS_priv_.tail = tail; /* update the tail */
        --QS_priv_.used;      /* one less byte used */
    }
    return ret; /* return the byte or EOD */
}

/****************************************************************************/
/**
* @description
* This function delivers a contiguous block of data from the QS data buffer.
* The function returns the pointer to the beginning of the block, and writes
* the number of bytes in the block to the location pointed to by @p pNbytes.
* The parameter @p pNbytes is also used as input to provide the maximum size
* of the data block that the caller can accept.
*
* @returns if data is available, the function returns pointer to the
* contiguous block of data and sets the value pointed to by @p pNbytes
* to the # available bytes. If data is available at the time the function is
* called, the function returns NULL pointer and sets the value pointed to by
* @p pNbytes to zero.
*
* @note Only the NULL return from QS_getBlock() indicates that the QS buffer
* is empty at the time of the call. The non-NULL return often means that
* the block is at the end of the buffer and you need to call QS_getBlock()
* again to obtain the rest of the data that "wrapped around" to the
* beginning of the QS data buffer.
*
* @note QS_getBlock() is NOT protected with a critical section.
*/
uint8_t const *QS_getBlock(uint16_t *pNbytes) {
    QSCtr used = QS_priv_.used; /* put in a temporary (register) */
    uint8_t const *buf;

    /* any bytes used in the ring buffer? */
    if (used != 0U) {
        QSCtr tail = QS_priv_.tail;  /* put in a temporary (register) */
        QSCtr end  = QS_priv_.end;   /* put in a temporary (register) */
        QSCtr n = (QSCtr)(end - tail);
        if (n > used) {
            n = used;
        }
        if (n > (QSCtr)(*pNbytes)) {
            n = (QSCtr)(*pNbytes);
        }
        *pNbytes = (uint16_t)n;    /* n-bytes available */
        buf = &QS_priv_.buf[tail]; /* the bytes are at the tail */

        QS_priv_.used = (QSCtr)(used - n);
        tail += n;
        if (tail == end) {
            tail = 0U;
        }
        QS_priv_.tail = tail;
    }

    else { /* no bytes available */
        *pNbytes = 0U;      /* no bytes available right now */
        buf = (uint8_t *)0; /* no bytes available right now */
    }
    return buf;
}

/****************************************************************************/
/** @note This function is only to be used through macro QS_SIG_DICTIONARY()
*/
void QS_sig_dict_pre_(enum_t const sig, void const * const obj,
                      char_t const *name)
{
    QS_CRIT_STAT_

    QS_CRIT_ENTRY_();
    QS_beginRec_((uint_fast8_t)QS_SIG_DICT);
    QS_SIG_PRE_(sig);
    QS_OBJ_PRE_(obj);
    QS_str_raw_((*name == (char_t)'&') ? &name[1] : name);
    QS_endRec_();
    QS_CRIT_EXIT_();
    QS_onFlush();
}

/****************************************************************************/
/** @note This function is only to be used through macro QS_OBJ_DICTIONARY()
*/
void QS_obj_dict_pre_(void const * const obj,
                      char_t const *name)
{
    QS_CRIT_STAT_

    QS_CRIT_ENTRY_();
    QS_beginRec_((uint_fast8_t)QS_OBJ_DICT);
    QS_OBJ_PRE_(obj);
    QS_str_raw_((*name == (char_t)'&') ? &name[1] : name);
    QS_endRec_();
    QS_CRIT_EXIT_();
    QS_onFlush();
}

/****************************************************************************/
/** @note This function is only to be used through macro QS_FUN_DICTIONARY()
*/
void QS_fun_dict_pre_(void (* const fun)(void), char_t const *name) {
    QS_CRIT_STAT_

    QS_CRIT_ENTRY_();
    QS_beginRec_((uint_fast8_t)QS_FUN_DICT);
    QS_FUN_PRE_(fun);
    QS_str_raw_((*name == (char_t)'&') ? &name[1] : name);
    QS_endRec_();
    QS_CRIT_EXIT_();
    QS_onFlush();
}

/****************************************************************************/
/** @note This function is only to be used through macro QS_USR_DICTIONARY()
*/
void QS_usr_dict_pre_(enum_t const rec,
                      char_t const * const name)
{
    QS_CRIT_STAT_

    QS_CRIT_ENTRY_();
    QS_beginRec_((uint_fast8_t)QS_USR_DICT);
    QS_u8_raw_((uint8_t)rec);
    QS_str_raw_(name);
    QS_endRec_();
    QS_CRIT_EXIT_();
    QS_onFlush();
}

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_mem_fmt_(uint8_t const *blk, uint8_t size) {
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;  /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head; /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;  /* put in a temporary (register) */
    uint8_t const *pb = blk;
    uint8_t len;

    QS_priv_.used += ((QSCtr)size + 2U); /* size+2 bytes to be added */

    QS_INSERT_BYTE_((uint8_t)QS_MEM_T)
    chksum += (uint8_t)QS_MEM_T;

    QS_INSERT_ESC_BYTE_(size)
    /* output the 'size' number of bytes */
    for (len = size; len > 0U; --len) {
        QS_INSERT_ESC_BYTE_(*pb)
        ++pb;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/**
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_str_fmt_(char_t const *str) {
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;  /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head; /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;  /* put in a temporary (register) */
    QSCtr   used   = QS_priv_.used; /* put in a temporary (register) */
    char_t const *s;

    used += 2U; /* account for the format byte and the terminating-0 */
    QS_INSERT_BYTE_((uint8_t)QS_STR_T)
    chksum += (uint8_t)QS_STR_T;

    for (s = str; *s != '\0'; ++s) {
        QS_INSERT_BYTE_((uint8_t)*s) /* ASCII char doesn't need escaping */
        chksum += (uint8_t)*s; /* update checksum */
        ++used;
    }
    QS_INSERT_BYTE_(0U) /* zero-terminate the string */

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
    QS_priv_.used   = used;    /* save # of used buffer space */
}

/****************************************************************************/
/*! Output the assertion failure trace record */
void QS_ASSERTION(char_t const * const module,
                  int_t const loc,
                  uint32_t delay)
{
    uint32_t volatile delay_ctr;

    QS_BEGIN_NOCRIT_PRE_(QS_ASSERT_FAIL, (void *)0, (void *)0)
        QS_TIME_PRE_();
        QS_U16_PRE_(loc);
        QS_STR_PRE_((module != (char_t *)0) ? module : "?");
    QS_END_NOCRIT_PRE_()
    QS_onFlush();
    for (delay_ctr = delay; delay_ctr > 0U; --delay_ctr) {
    }
    QS_onCleanup();
}

/****************************************************************************/
/* Output the critical section entry/exit
*/
void QF_QS_CRIT_ENTRY(void) {
    QS_BEGIN_NOCRIT_PRE_(QS_QF_CRIT_ENTRY, (void *)0, (void *)0)
        QS_TIME_PRE_();
        ++QS_priv_.critNest;
        QS_u8_raw_(QS_priv_.critNest);
    QS_END_NOCRIT_PRE_()
}

void QF_QS_CRIT_EXIT(void) {
    QS_BEGIN_NOCRIT_PRE_(QS_QF_CRIT_EXIT, (void *)0, (void *)0)
        QS_TIME_PRE_();
        QS_u8_raw_(QS_priv_.critNest);
        --QS_priv_.critNest;
    QS_END_NOCRIT_PRE_()
}

/****************************************************************************/
/* Output the ISR entry/exit
*/
void QF_QS_ISR_ENTRY(uint8_t const isrnest, uint8_t const prio) {
    QS_BEGIN_NOCRIT_PRE_(QS_QF_ISR_ENTRY, (void *)0, (void *)0)
        QS_TIME_PRE_();
        QS_2u8_raw_(isrnest, prio);
    QS_END_NOCRIT_PRE_()
}

void QF_QS_ISR_EXIT(uint8_t const isrnest, uint8_t const prio) {
    QS_BEGIN_NOCRIT_PRE_(QS_QF_ISR_EXIT, (void *)0, (void *)0)
        QS_TIME_PRE_();
        QS_2u8_raw_(isrnest, prio);
    QS_END_NOCRIT_PRE_()
}

