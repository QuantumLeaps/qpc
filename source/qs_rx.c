/**
* @file
* @brief QS/C receive channel services
* @ingroup qs
* @cond
******************************************************************************
* Last updated for version 5.5.0
* Last updated on  2015-08-28
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#include "qs_port.h"      /* QS port */
#include "qs_pkg.h"       /* QS package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qs_rx")

/****************************************************************************/
QSrxPriv QS_rxPriv_;      /* QS-RX private data */

/****************************************************************************/
#if (QS_OBJ_PTR_SIZE == 1)
    typedef uint8_t QSAddr;
#elif (QS_OBJ_PTR_SIZE == 2)
    typedef uint16_t QSAddr;
#elif (QS_OBJ_PTR_SIZE == 4)
    typedef uint32_t QSAddr;
#elif (QS_OBJ_PTR_SIZE == 8)
    typedef uint64_t QSAddr;
#endif

/* extended-state variables used for parsing various QS-RX Records... */
typedef struct {
    uint32_t param;
    uint8_t  idx;
    uint8_t  cmdId;
} CmdVar;

typedef struct {
    uint8_t rate;
} TickVar;

typedef struct {
    QSAddr  addr;
    uint8_t idx;
    uint8_t len;
} PeekVar;

typedef struct {
    uint8_t data[8];
    QSAddr  addr;
    uint8_t idx;
    uint8_t len;
} PokeVar;

typedef struct {
    uint8_t data[16];
    uint8_t idx;
} GFltVar;

typedef struct {
    QSAddr addr;
    uint8_t idx;
    uint8_t fltId;
} LFltVar;

typedef struct {
    uint8_t prio;
} AFltVar;

typedef struct {
    QEvt    *e;
    uint8_t *p;
    QSignal  sig;
    uint16_t len;
    uint8_t  prio;
    uint8_t  idx;
} EvtVar;

static struct {
    union QSRxExtState {
        CmdVar   cmd;
        TickVar  tick;
        PeekVar  peek;
        PokeVar  poke;
        GFltVar gFlt;
        LFltVar lFlt;
        AFltVar aFlt;
        EvtVar   evt;
    } var;   /* extended-state variables for the current state */
    uint8_t  state;
    uint8_t  esc;
    uint8_t  seq;
    uint8_t  chksum;
} l_rx;

enum {
    WAIT4_SEQ,
    WAIT4_REC,
    WAIT4_INFO_FRAME,
    WAIT4_CMD_ID,
    WAIT4_CMD_PARAM,
    WAIT4_CMD_FRAME,
    WAIT4_RESET_FRAME,
    WAIT4_TICK_RATE,
    WAIT4_TICK_FRAME,
    WAIT4_PEEK_ADDR,
    WAIT4_PEEK_LEN,
    WAIT4_PEEK_FRAME,
    WAIT4_POKE_ADDR,
    WAIT4_POKE_LEN,
    WAIT4_POKE_DATA,
    WAIT4_POKE_FRAME,
    WAIT4_GLB_FILTER_LEN,
    WAIT4_GLB_FILTER_DATA,
    WAIT4_GLB_FILTER_FRAME,
    WAIT4_LOC_FILTER_ID,
    WAIT4_LOC_FILTER_ADDR,
    WAIT4_LOC_FILTER_FRAME,
    WAIT4_AO_FILTER_PRIO,
    WAIT4_AO_FILTER_FRAME,
    WAIT4_EVT_PRIO,
    WAIT4_EVT_SIG,
    WAIT4_EVT_LEN,
    WAIT4_EVT_PAR,
    WAIT4_EVT_FRAME,
    ERROR
};

static void QS_rxParseData_(uint8_t b);
static void QS_rxHandleGoodFrame_(void);
static void QS_rxHandleBadFrame_(void);
static void QS_rxReportSuccess_(enum QSpyRxRecords recId);
static void QS_rxReportError_(uint8_t stateId);
static bool QS_rxAddr_(uint8_t b, QSAddr *addr, uint8_t *idx);

/*! Internal QS-RX macro to access the QS ring buffer */
/**
* @description
* The QS-RX buffer is allocated by the user and is accessed through the
* pointer QS_rxPriv_.buf, which violates the MISRA-C 2004 Rule 17.4(req),
* pointer arithmetic other than array indexing. Encapsulating this violation
* in a macro allows to selectively suppress this specific deviation.
*/
#define QS_RX_AT_(i_) (QS_rxPriv_.buf + (i_))

/*! Internal QS-RX macro to encapsulate transition in the QS-RX FSM */
#define QS_RX_TRAN_(target_) (l_rx.state = (uint8_t)(target_))

static uint8_t const l_QS_RX = (uint8_t)0; /* QS source ID */

/****************************************************************************/
/**
* @description
* This function should be called from QS_onStartup() to provide QS-RX with
* the receive data buffer. The first parameter @p sto[] is the address of
* the memory block, and the second parameter @p stoSize is the size of
* this block in bytes. The size of the QS RX buffer cannot exceed 64KB.
*
* @note QS-RX can work with quite small data buffers, but you will start
* losing data if the buffer is not drained fast enough in the idle task.
*
* @note If the data input rate exceeds the QS-RX processing rate, the data
* will be lost, but the QS protocol will notice that:
* (1) that the checksum in the incomplete QS records will fail; and
* (2) the sequence counter in QS records will show discontinuities.
*
* The QS-RX channel will report any data errors by sending the
* QS_RX_DATA_ERROR trace record.
*/
void QS_rxInitBuf(uint8_t sto[], uint16_t stoSize) {
    static char_t const Q_ROM s_rxObjDic[] = "QS_RX";
    QS_CRIT_STAT_

    QS_rxPriv_.buf   = &sto[0];
    QS_rxPriv_.end   = (QSCtr)stoSize - (QSCtr)1;
    QS_rxPriv_.head  = (QSCtr)0;
    QS_rxPriv_.tail  = (QSCtr)0;

    QS_RX_TRAN_(WAIT4_SEQ);
    l_rx.esc    = (uint8_t)0U;
    l_rx.seq    = (uint8_t)0;
    l_rx.chksum = (uint8_t)0;

    QS_CRIT_ENTRY_();
    QS_beginRec((uint_fast8_t)QS_OBJ_DICT);
    QS_OBJ_(&l_QS_RX);
    QS_STR_ROM_(s_rxObjDic);
    QS_endRec();
    QS_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function is intended to be called from the ISR that reads the QS-RX
* bytes from the QSPY host application. The function returns the conservative
* number of free bytes currently available in the buffer, assuming that
* the head pointer is not being moved concurrently. The tail pointer might
* be moving, meaning that bytes can be concurrently removed from the buffer.
*/
uint16_t QS_rxGetNfree(void) {
    uint16_t nFree;
    if (QS_rxPriv_.head == QS_rxPriv_.tail) {
        nFree = (uint16_t)QS_rxPriv_.end;
    }
    else if (QS_rxPriv_.head < QS_rxPriv_.tail) {
        nFree = (uint16_t)(QS_rxPriv_.tail - QS_rxPriv_.head);
    }
    else {
        nFree = (uint16_t)((QS_rxPriv_.tail + QS_rxPriv_.end)
                           - QS_rxPriv_.head);
    }
    return nFree;
}

/****************************************************************************/
void QS_rxParse(void) {
    while (QS_rxPriv_.head != QS_rxPriv_.tail) { /* QS-RX buffer not empty? */
        uint8_t b = *QS_RX_AT_(QS_rxPriv_.tail);

        if (QS_rxPriv_.tail != (QSCtr)0) {
            --QS_rxPriv_.tail;
        }
        else {
             QS_rxPriv_.tail = QS_rxPriv_.end;
        }

        if (l_rx.esc) {  /* escaped byte arrived? */
            l_rx.esc = (uint8_t)0;
            b ^= QS_ESC_XOR;

            l_rx.chksum += b;
            QS_rxParseData_(b);
        }
        else if (b == QS_ESC) {
            l_rx.esc = (uint8_t)1;
        }
        else if (b == QS_FRAME) {
            if (l_rx.chksum == QS_GOOD_CHKSUM) {
                QS_rxHandleGoodFrame_();
            }
            else { /* bad checksum */
                QS_rxReportError_((uint8_t)0x00);
                QS_rxHandleBadFrame_();
            }

            /* get ready for the next frame */
            l_rx.esc    = (uint8_t)0;
            l_rx.chksum = (uint8_t)0;
            QS_RX_TRAN_(WAIT4_SEQ);
        }
        else {
            l_rx.chksum += b;
            QS_rxParseData_(b);
        }
    }
}

/****************************************************************************/
static void QS_rxParseData_(uint8_t b) {
    switch (l_rx.state) {
        case WAIT4_SEQ: {
            ++l_rx.seq;
            if (l_rx.seq != b) { /* not the expected sequence? */
                QS_rxReportError_((uint8_t)(1U << 5));
                l_rx.seq = b; /* update the sequence */
            }
            QS_RX_TRAN_(WAIT4_REC);
            break;
        }
        case WAIT4_REC: {
            switch (b) {
                case QS_RX_INFO:
                    QS_RX_TRAN_(WAIT4_INFO_FRAME);
                    break;
                case QS_RX_COMMAND:
                    QS_RX_TRAN_(WAIT4_CMD_ID);
                    break;
                case QS_RX_RESET:
                    QS_RX_TRAN_(WAIT4_RESET_FRAME);
                    break;
                case QS_RX_TICK:
                    QS_RX_TRAN_(WAIT4_TICK_RATE);
                    break;
                case QS_RX_PEEK:
                    l_rx.var.peek.addr = (QSAddr)0;
                    l_rx.var.peek.idx = (uint8_t)0;
                    QS_RX_TRAN_(WAIT4_PEEK_ADDR);
                    break;
                case QS_RX_POKE:
                    l_rx.var.poke.addr = (QSAddr)0;
                    l_rx.var.poke.idx = (uint8_t)0;
                    QS_RX_TRAN_(WAIT4_POKE_ADDR);
                    break;
                case QS_RX_GLB_FILTER:
                    QS_RX_TRAN_(WAIT4_GLB_FILTER_LEN);
                    break;
                case QS_RX_LOC_FILTER:
                    QS_RX_TRAN_(WAIT4_LOC_FILTER_ID);
                    break;
                case QS_RX_AO_FILTER:
                    QS_RX_TRAN_(WAIT4_AO_FILTER_PRIO);
                    break;
                case QS_RX_EVENT:
                    QS_RX_TRAN_(WAIT4_EVT_PRIO);
                    break;
                default:
                    QS_rxReportError_((uint8_t)(1U << 5));
                    QS_RX_TRAN_(ERROR);
                    break;
            }
            break;
        }
        case WAIT4_INFO_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_CMD_ID: {
            l_rx.var.cmd.cmdId = b;
            l_rx.var.cmd.idx   = (uint8_t)0;
            l_rx.var.cmd.param = (uint32_t)0;
            QS_RX_TRAN_(WAIT4_CMD_PARAM);
            break;
        }
        case WAIT4_CMD_PARAM: {
            l_rx.var.cmd.param |= ((uint32_t)b << l_rx.var.cmd.idx);
            l_rx.var.cmd.idx   += (uint8_t)8;
            if (l_rx.var.cmd.idx == (uint8_t)(8*4)) {
                QS_RX_TRAN_(WAIT4_CMD_FRAME);
            }
            break;
        }
        case WAIT4_CMD_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_RESET_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_TICK_RATE: {
            l_rx.var.tick.rate = b;
            QS_RX_TRAN_(WAIT4_TICK_FRAME);
            break;
        }
        case WAIT4_TICK_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_PEEK_ADDR: {
            if (QS_rxAddr_(b, &l_rx.var.peek.addr, &l_rx.var.peek.idx)) {
                QS_RX_TRAN_(WAIT4_PEEK_LEN);
            }
            break;
        }
        case WAIT4_PEEK_LEN: {
            l_rx.var.peek.len = b;
            QS_RX_TRAN_(WAIT4_PEEK_FRAME);
            break;
        }
        case WAIT4_PEEK_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_POKE_ADDR: {
            if (QS_rxAddr_(b, &l_rx.var.poke.addr, &l_rx.var.poke.idx)) {
                QS_RX_TRAN_(WAIT4_POKE_LEN);
            }
            break;
        }
        case WAIT4_POKE_LEN: {
            if (b <= (uint8_t)sizeof(l_rx.var.poke.data)) {
                l_rx.var.poke.len = b;
                l_rx.var.poke.idx = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_POKE_DATA);
            }
            else {
                QS_rxReportError_((uint8_t)(1U << 5));
                QS_RX_TRAN_(ERROR);
            }
            break;
        }
        case WAIT4_POKE_DATA: {
            l_rx.var.poke.data[l_rx.var.poke.idx] = b;
            ++l_rx.var.poke.idx;
            if (l_rx.var.poke.idx == l_rx.var.poke.len) {
                QS_RX_TRAN_(WAIT4_POKE_FRAME);
            }
            break;
        }
        case WAIT4_POKE_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_GLB_FILTER_LEN: {
            if (b == (uint8_t)sizeof(l_rx.var.gFlt.data)) {
                l_rx.var.gFlt.idx = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_GLB_FILTER_DATA);
            }
            else {
                QS_rxReportError_((uint8_t)(1U << 5));
                QS_RX_TRAN_(ERROR);
            }
            break;
        }
        case WAIT4_GLB_FILTER_DATA: {
            l_rx.var.gFlt.data[l_rx.var.gFlt.idx] = b;
            ++l_rx.var.gFlt.idx;
            if (l_rx.var.gFlt.idx == (uint8_t)sizeof(l_rx.var.gFlt.data)) {
                QS_RX_TRAN_(WAIT4_GLB_FILTER_FRAME);
            }
            break;
        }
        case WAIT4_GLB_FILTER_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_LOC_FILTER_ID: {
            if (b < (uint8_t)6) {
                l_rx.var.lFlt.fltId = b;
                l_rx.var.lFlt.addr  = (QSAddr)0;
                l_rx.var.lFlt.idx  = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_LOC_FILTER_ADDR);
            }
            else {
                QS_rxReportError_((uint8_t)(1U << 5));
                QS_RX_TRAN_(ERROR);
            }
            break;
        }
        case WAIT4_LOC_FILTER_ADDR: {
            if (QS_rxAddr_(b, &l_rx.var.lFlt.addr, &l_rx.var.lFlt.idx)) {
                QS_RX_TRAN_(WAIT4_LOC_FILTER_FRAME);
            }
            break;
        }
        case WAIT4_LOC_FILTER_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_AO_FILTER_PRIO: {
            l_rx.var.aFlt.prio = b;
            QS_RX_TRAN_(WAIT4_AO_FILTER_FRAME);
            break;
        }
        case WAIT4_AO_FILTER_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_EVT_PRIO: {
            l_rx.var.evt.prio = b;
            l_rx.var.evt.sig  = (QSignal)0;
            l_rx.var.evt.idx = (uint8_t)0;
            QS_RX_TRAN_(WAIT4_EVT_SIG);
            break;
        }
        case WAIT4_EVT_SIG: {
            l_rx.var.evt.sig |= (QSignal)((uint32_t)b << l_rx.var.evt.idx);
            l_rx.var.evt.idx += (uint8_t)8;
            if (l_rx.var.evt.idx == (uint8_t)(8*Q_SIGNAL_SIZE)) {
                l_rx.var.evt.len  = (uint16_t)0;
                l_rx.var.evt.idx = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_EVT_LEN);
            }
            break;
        }
        case WAIT4_EVT_LEN: {
            l_rx.var.evt.len |= (uint16_t)((uint32_t)b << l_rx.var.evt.idx);
            l_rx.var.evt.idx += (uint8_t)8;
            if (l_rx.var.evt.idx == (uint8_t)(8*2)) {
                if ((l_rx.var.evt.len + (uint16_t)sizeof(QEvt)) <=
                    (uint16_t)QF_poolGetMaxBlockSize())
                {
                    l_rx.var.evt.e = QF_newX_(
                        ((uint_fast16_t)l_rx.var.evt.len
                         + (uint_fast16_t)sizeof(QEvt)),
                        (uint_fast16_t)1, /* margin */
                        (enum_t)l_rx.var.evt.sig);
                    if (l_rx.var.evt.e != (QEvt *)0) { /* event allocated? */
                        l_rx.var.evt.p = (uint8_t *)l_rx.var.evt.e;
                        l_rx.var.evt.p += sizeof(QEvt);
                        if (l_rx.var.evt.len > (uint16_t)0) {
                            QS_RX_TRAN_(WAIT4_EVT_PAR);
                        }
                        else {
                            QS_RX_TRAN_(WAIT4_EVT_FRAME);
                        }
                    }
                    else {
                        QS_rxReportError_((uint8_t)(1U << 5));
                        QS_RX_TRAN_(ERROR);
                    }
                }
                else {
                    QS_rxReportError_((uint8_t)(1U << 5));
                    QS_RX_TRAN_(ERROR);
                }
            }
            break;
        }
        case WAIT4_EVT_PAR: {  /* event parameters */
            *l_rx.var.evt.p++ = b;
            --l_rx.var.evt.len;
            if (l_rx.var.evt.len == (uint16_t)0) {
                QS_RX_TRAN_(WAIT4_EVT_FRAME);
            }
            break;
        }
        case WAIT4_EVT_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case ERROR: {
            /* keep ignoring the data until a good frame is collected */
            break;
        }
        default: {  /* unexpected or unimplemented state */
            QS_rxReportError_((uint8_t)(1U << 5));
            QS_RX_TRAN_(ERROR);
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxHandleGoodFrame_(void) {
    QS_CRIT_STAT_
    uint8_t i;
    uint8_t *buf;

    switch (l_rx.state) {
        case WAIT4_INFO_FRAME: {
            /* no need to report success, because a QS record is produced */
            QS_CRIT_ENTRY_();
            QS_target_info_((uint8_t)0x00); /* send only Target info */
            QS_CRIT_EXIT_();
            break;
        }
        case WAIT4_RESET_FRAME: {
            /* no need to report success, because the target resets */
            QS_onReset(); /* reset the Target */
            break;
        }
        case WAIT4_CMD_FRAME: {
            QS_rxReportSuccess_(QS_RX_COMMAND);
            QS_onCommand(l_rx.var.cmd.cmdId, l_rx.var.cmd.param);
            break;
        }
        case WAIT4_TICK_FRAME: {
            QS_rxReportSuccess_(QS_RX_TICK);
            QF_TICK_X((uint_fast8_t)l_rx.var.tick.rate, &l_QS_RX);
            break;
        }
        case WAIT4_PEEK_FRAME: {
            /* no need to report success, because a QS record is produced */
            QS_BEGIN_(QS_PEEK_DATA, (void *)0, (void *)0)
                QS_TIME_();                  /* timestamp */
                QS_OBJ_(l_rx.var.peek.addr); /* address */
                QS_U8_(l_rx.var.peek.len);   /* data length */
                buf = (uint8_t *)l_rx.var.peek.addr;
                for (i = (uint8_t)0; i < l_rx.var.peek.len; ++i) {
                    QS_U8_(*QS_PTR_AT_(i));  /* data bytes */
                }
            QS_END_()
            break;
        }
        case WAIT4_POKE_FRAME: {
            QS_rxReportSuccess_(QS_RX_POKE);
            buf = (uint8_t *)l_rx.var.poke.addr;
            QS_CRIT_ENTRY_(); /* poke the data within a critical section */
            for (i = (uint8_t)0; i < l_rx.var.poke.len; ++i) {
                *QS_PTR_AT_(i) = l_rx.var.poke.data[i];
            }
            QS_CRIT_EXIT_();
            break;
        }
        case WAIT4_GLB_FILTER_FRAME: {
            QS_rxReportSuccess_(QS_RX_GLB_FILTER);

            /* never disable the non-maskable records */
            l_rx.var.gFlt.data[0] |= (uint8_t)0x01;
            l_rx.var.gFlt.data[7] |= (uint8_t)0xF0;
            l_rx.var.gFlt.data[8] |= (uint8_t)0x3F;

            /* never turn the last 3 records on (0x7D, 0x7E, 0x7F) */
            l_rx.var.gFlt.data[15] &= (uint8_t)0xE0;

            for (i=(uint8_t)0; i < (uint8_t)sizeof(QS_priv_.glbFilter); ++i) {
                QS_priv_.glbFilter[i] = l_rx.var.gFlt.data[i];
            }
            break;
        }
        case WAIT4_LOC_FILTER_FRAME: {
            i = (uint8_t)1;
            switch (l_rx.var.lFlt.fltId) {
                case 0:
                    QS_priv_.smObjFilter = (void *)l_rx.var.lFlt.addr;
                   break;
                case 1:
                    QS_priv_.aoObjFilter = (void *)l_rx.var.lFlt.addr;
                    break;
                case 2:
                    QS_priv_.mpObjFilter = (void *)l_rx.var.lFlt.addr;
                    break;
                case 3:
                    QS_priv_.eqObjFilter = (void *)l_rx.var.lFlt.addr;
                    break;
                case 4:
                    QS_priv_.teObjFilter = (void *)l_rx.var.lFlt.addr;
                    break;
                case 5:
                    QS_priv_.apObjFilter = (void *)l_rx.var.lFlt.addr;
                    break;
                default:
                    QS_rxReportError_((uint8_t)(2U << 5));
                    i = (uint8_t)0;
                    break;
            }
            if (i != (uint8_t)0) {
                QS_rxReportSuccess_(QS_RX_LOC_FILTER);
            }
            break;
        }
        case WAIT4_AO_FILTER_FRAME: {
            if (l_rx.var.aFlt.prio <= (uint8_t)QF_MAX_ACTIVE) {
                QS_rxReportSuccess_(QS_RX_AO_FILTER);
                QS_priv_.aoObjFilter = QF_active_[l_rx.var.aFlt.prio];
                QS_priv_.smObjFilter = QF_active_[l_rx.var.aFlt.prio];
            }
            else {
                QS_rxReportError_((uint8_t)(2U << 5));
            }
            break;
        }
        case WAIT4_EVT_FRAME: {
            if (l_rx.var.evt.prio == (uint8_t)0) {
                QS_rxReportSuccess_(QS_RX_EVENT);
                QF_PUBLISH(l_rx.var.evt.e, &l_QS_RX);
            }
            else if (l_rx.var.evt.prio < (uint8_t)QF_MAX_ACTIVE) {
                QS_rxReportSuccess_(QS_RX_EVENT);
                (void)QACTIVE_POST_X(QF_active_[l_rx.var.evt.prio],
                               l_rx.var.evt.e,
                               (uint_fast16_t)1, /* margin */
                               &l_QS_RX);
            }
            else {
                QS_rxReportError_((uint8_t)(2U << 5));
                QF_gc(l_rx.var.evt.e);
            }
            break;
        }
        case ERROR: {
            /* keep ignoring all bytes until new frame */
            break;
        }
        default: {
            QS_rxReportError_((uint8_t)(2U << 5));
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxHandleBadFrame_(void) {
    switch (l_rx.state) {
        case WAIT4_EVT_FRAME: {
            Q_ASSERT_ID(910, l_rx.var.evt.e != (QEvt *)0);
            QF_gc(l_rx.var.evt.e);
            break;
        }
        default: {
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxReportSuccess_(enum QSpyRxRecords recId) {
    QS_CRIT_STAT_
    QS_BEGIN_(QS_RX_STATUS, (void *)0, (void *)0)
        QS_TIME_();           /* timestamp */
        QS_U8_(recId);        /* record ID */
    QS_END_()
}

/****************************************************************************/
static void QS_rxReportError_(uint8_t stateId) {
    QS_CRIT_STAT_
    QS_BEGIN_(QS_RX_STATUS, (void *)0, (void *)0)
        QS_TIME_();           /* timestamp */
        QS_U8_((uint8_t)0x80 | stateId | l_rx.state); /* error number */
    QS_END_()
}

/****************************************************************************/
static bool QS_rxAddr_(uint8_t b, QSAddr *addr, uint8_t *idx) {
    *addr |= ((uint32_t)b << *idx);
    *idx += (uint8_t)8;
    return (*idx == (uint8_t)(8*QS_OBJ_PTR_SIZE)) ? true : false;
}
