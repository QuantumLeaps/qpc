/**
* @file
* @brief QS/C receive channel services
* @ingroup qs
* @cond
******************************************************************************
* Last updated for version 6.3.7
* Last updated on  2018-11-16
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qs_port.h"      /* QS port */
#include "qs_pkg.h"       /* QS package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qs_rx")

/****************************************************************************/
QSrxPriv QS_rxPriv_;      /* QS-RX private data */

/****************************************************************************/
#if (QS_OBJ_PTR_SIZE == 1)
    typedef uint8_t QSObj;
#elif (QS_OBJ_PTR_SIZE == 2)
    typedef uint16_t QSObj;
#elif (QS_OBJ_PTR_SIZE == 4)
    typedef uint32_t QSObj;
#elif (QS_OBJ_PTR_SIZE == 8)
    typedef uint64_t QSObj;
#endif

#if (QS_FUN_PTR_SIZE == 1)
    typedef uint8_t QSFun;
#elif (QS_FUN_PTR_SIZE == 2)
    typedef uint16_t QSFun;
#elif (QS_FUN_PTR_SIZE == 4)
    typedef uint32_t QSFun;
#elif (QS_FUN_PTR_SIZE == 8)
    typedef uint64_t QSFun;
#endif

/** @cond
* Exlcude the following internals from the Doxygen documentation
* Extended-state variables used for parsing various QS-RX Records
*/
typedef struct {
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint8_t  idx;
    uint8_t  cmdId;
} CmdVar;

typedef struct {
    uint_fast8_t rate;
} TickVar;

typedef struct {
    uint16_t offs;
    uint8_t  size;
    uint8_t  num;
    uint8_t  idx;
} PeekVar;

typedef struct {
    uint32_t data;
    uint16_t offs;
    uint8_t  size;
    uint8_t  num;
    uint8_t  idx;
    uint8_t  fill;
} PokeVar;

typedef struct {
    uint8_t data[16];
    uint8_t idx;
} GFltVar;

typedef struct {
    QSObj   addr;
    uint8_t idx;
    uint8_t kind; /* see qs.h, enum QSpyObjKind */
    uint8_t recId;
} ObjVar;

typedef struct {
    QSFun    addr;
    uint32_t data;
    uint8_t  idx;
} TPVar; /* Test-Probe */

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

/* extended-state variables for the current state */
static struct {
    union Variant {
        CmdVar   cmd;
        TickVar  tick;
        PeekVar  peek;
        PokeVar  poke;
        GFltVar  gFlt;
        AFltVar  aFlt;
        ObjVar   obj;
        EvtVar   evt;
        TPVar    tp;
    } var;
    uint8_t state;
    uint8_t esc;
    uint8_t seq;
    uint8_t chksum;
} l_rx;

enum {
    WAIT4_SEQ,
    WAIT4_REC,
    WAIT4_INFO_FRAME,
    WAIT4_CMD_ID,
    WAIT4_CMD_PARAM1,
    WAIT4_CMD_PARAM2,
    WAIT4_CMD_PARAM3,
    WAIT4_CMD_FRAME,
    WAIT4_RESET_FRAME,
    WAIT4_TICK_RATE,
    WAIT4_TICK_FRAME,
    WAIT4_PEEK_OFFS,
    WAIT4_PEEK_SIZE,
    WAIT4_PEEK_NUM,
    WAIT4_PEEK_FRAME,
    WAIT4_POKE_OFFS,
    WAIT4_POKE_SIZE,
    WAIT4_POKE_NUM,
    WAIT4_POKE_DATA,
    WAIT4_POKE_FRAME,
    WAIT4_FILL_DATA,
    WAIT4_FILL_FRAME,
    WAIT4_GLB_FILTER_LEN,
    WAIT4_GLB_FILTER_DATA,
    WAIT4_GLB_FILTER_FRAME,
    WAIT4_AO_FILTER_PRIO,
    WAIT4_AO_FILTER_FRAME,
    WAIT4_OBJ_KIND,
    WAIT4_OBJ_ADDR,
    WAIT4_OBJ_FRAME,
    WAIT4_QUERY_KIND,
    WAIT4_QUERY_FRAME,
    WAIT4_EVT_PRIO,
    WAIT4_EVT_SIG,
    WAIT4_EVT_LEN,
    WAIT4_EVT_PAR,
    WAIT4_EVT_FRAME,
    WAIT4_TEST_SETUP_FRAME,
    WAIT4_TEST_TEARDOWN_FRAME,
    WAIT4_TEST_PROBE_DATA,
    WAIT4_TEST_PROBE_ADDR,
    WAIT4_TEST_PROBE_FRAME,
    WAIT4_TEST_CONTINUE_FRAME,
    ERROR_STATE
};

#ifdef Q_UTEST
    static struct {
        TPVar     tpBuf[16]; /* buffer of Test-Probes received so far */
        uint8_t   tpNum;     /* current number of Test-Probes */
        QSTimeCtr testTime;  /* test time (tick counter)  */
    } l_testData;
#endif /* Q_UTEST */

/* static helper functions... */
static void QS_rxParseData_(uint8_t b);
static void QS_rxHandleGoodFrame_(uint8_t state);
static void QS_rxHandleBadFrame_(uint8_t state);
static void QS_rxReportAck_(enum QSpyRxRecords recId);
static void QS_rxReportError_(uint8_t code);
static void QS_rxReportDone_(enum QSpyRxRecords recId);
static void QS_rxPoke_(void);

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

/** @endcond */

/****************************************************************************/
/**
* @description
* This function should be called from QS_onStartup() to provide QS-RX with
* the receive data buffer.
*
* @param[in]  sto[]   the address of the memory block
* @param[in]  stoSize the size of this block [bytes]. The size of the
*                     QS RX buffer cannot exceed 64KB.
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
    QS_rxPriv_.buf  = &sto[0];
    QS_rxPriv_.end  = (QSCtr)stoSize - (QSCtr)1;
    QS_rxPriv_.head = (QSCtr)0;
    QS_rxPriv_.tail = (QSCtr)0;

    QS_rxPriv_.currObj[SM_OBJ] = (void *)0;
    QS_rxPriv_.currObj[AO_OBJ] = (void *)0;
    QS_rxPriv_.currObj[MP_OBJ] = (void *)0;
    QS_rxPriv_.currObj[EQ_OBJ] = (void *)0;
    QS_rxPriv_.currObj[TE_OBJ] = (void *)0;
    QS_rxPriv_.currObj[AP_OBJ] = (void *)0;

    QS_RX_TRAN_(WAIT4_SEQ);
    l_rx.esc    = (uint8_t)0U;
    l_rx.seq    = (uint8_t)0;
    l_rx.chksum = (uint8_t)0;

    QS_beginRec((uint_fast8_t)QS_OBJ_DICT);
        QS_OBJ_(&QS_rxPriv_);
        QS_STR_("QS_RX");
    QS_endRec();
    /* no QS_REC_DONE(), because QS is not running yet */

#ifdef Q_UTEST
    l_testData.tpNum    = (uint8_t)0;
    l_testData.testTime = (QSTimeCtr)0;
#endif /* Q_UTEST */
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
    while (QS_rxPriv_.head != QS_rxPriv_.tail) { /*QS-RX buffer not empty? */
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
            /* get ready for the next frame */
            b = l_rx.state; /* save the current state in b */
            l_rx.esc = (uint8_t)0;
            QS_RX_TRAN_(WAIT4_SEQ);

            if (l_rx.chksum == QS_GOOD_CHKSUM) {
                l_rx.chksum = (uint8_t)0;
                QS_rxHandleGoodFrame_(b);
            }
            else { /* bad checksum */
                l_rx.chksum = (uint8_t)0;
                QS_rxReportError_((uint8_t)0x41U);
                QS_rxHandleBadFrame_(b);
            }
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
            if (l_rx.seq != b) {
                QS_rxReportError_((uint8_t)0x42U);
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
                    if (QS_rxPriv_.currObj[AP_OBJ] != (void *)0) {
                        l_rx.var.peek.offs = (uint16_t)0;
                        l_rx.var.peek.idx  = (uint8_t)0;
                        QS_RX_TRAN_(WAIT4_PEEK_OFFS);
                    }
                    else {
                        QS_rxReportError_((uint8_t)QS_RX_PEEK);
                        QS_RX_TRAN_(ERROR_STATE);
                    }
                    break;
                case QS_RX_POKE:
                case QS_RX_FILL:
                    l_rx.var.poke.fill = (b == (uint8_t)QS_RX_FILL)
                                         ? (uint8_t)1
                                         : (uint8_t)0;
                    if (QS_rxPriv_.currObj[AP_OBJ] != (void *)0) {
                        l_rx.var.poke.offs = (uint16_t)0;
                        l_rx.var.poke.idx  = (uint8_t)0;
                        QS_RX_TRAN_(WAIT4_POKE_OFFS);
                    }
                    else {
                        QS_rxReportError_((l_rx.var.poke.fill != (uint8_t)0)
                                           ? (uint8_t)QS_RX_FILL
                                           : (uint8_t)QS_RX_FILL);
                        QS_RX_TRAN_(ERROR_STATE);
                    }
                    break;
                case QS_RX_GLB_FILTER:
                    QS_RX_TRAN_(WAIT4_GLB_FILTER_LEN);
                    break;
                case QS_RX_LOC_FILTER:
                    l_rx.var.obj.recId = (uint8_t)QS_RX_LOC_FILTER;
                    QS_RX_TRAN_(WAIT4_OBJ_KIND);
                    break;
                case QS_RX_AO_FILTER:
                    QS_RX_TRAN_(WAIT4_AO_FILTER_PRIO);
                    break;
                case QS_RX_CURR_OBJ:
                    l_rx.var.obj.recId = (uint8_t)QS_RX_CURR_OBJ;
                    QS_RX_TRAN_(WAIT4_OBJ_KIND);
                    break;
                case QS_RX_QUERY_CURR:
                    l_rx.var.obj.recId = (uint8_t)QS_RX_QUERY_CURR;
                    QS_RX_TRAN_(WAIT4_QUERY_KIND);
                    break;
                case QS_RX_EVENT:
                    QS_RX_TRAN_(WAIT4_EVT_PRIO);
                    break;

#ifdef Q_UTEST
                case QS_RX_TEST_SETUP:
                    QS_RX_TRAN_(WAIT4_TEST_SETUP_FRAME);
                    break;
                case QS_RX_TEST_TEARDOWN:
                    QS_RX_TRAN_(WAIT4_TEST_TEARDOWN_FRAME);
                    break;
                case QS_RX_TEST_CONTINUE:
                    QS_RX_TRAN_(WAIT4_TEST_CONTINUE_FRAME);
                    break;
                case QS_RX_TEST_PROBE:
                    if (l_testData.tpNum
                        < (uint8_t)(sizeof(l_testData.tpBuf)
                                    /sizeof(l_testData.tpBuf[0])))
                    {
                        l_rx.var.tp.data = (uint32_t)0;
                        l_rx.var.tp.idx  = (uint8_t)0;
                        QS_RX_TRAN_(WAIT4_TEST_PROBE_DATA);
                    }
                    else { /* the number of Test-Probes exceeded */
                        QS_rxReportError_((uint8_t)QS_RX_TEST_PROBE);
                        QS_RX_TRAN_(ERROR_STATE);
                    }
                    break;
#endif /* Q_UTEST */

                default:
                    QS_rxReportError_((uint8_t)0x43U);
                    QS_RX_TRAN_(ERROR_STATE);
                    break;
            }
            break;
        }
        case WAIT4_INFO_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_CMD_ID: {
            l_rx.var.cmd.cmdId  = b;
            l_rx.var.cmd.idx    = (uint8_t)0;
            l_rx.var.cmd.param1 = (uint32_t)0;
            l_rx.var.cmd.param2 = (uint32_t)0;
            l_rx.var.cmd.param3 = (uint32_t)0;
            QS_RX_TRAN_(WAIT4_CMD_PARAM1);
            break;
        }
        case WAIT4_CMD_PARAM1: {
            l_rx.var.cmd.param1 |= ((uint32_t)b << l_rx.var.cmd.idx);
            l_rx.var.cmd.idx    += (uint8_t)8;
            if (l_rx.var.cmd.idx == (uint8_t)(8*4)) {
                l_rx.var.cmd.idx = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_CMD_PARAM2);
            }
            break;
        }
        case WAIT4_CMD_PARAM2: {
            l_rx.var.cmd.param2 |= ((uint32_t)b << l_rx.var.cmd.idx);
            l_rx.var.cmd.idx    += (uint8_t)8;
            if (l_rx.var.cmd.idx == (uint8_t)(8*4)) {
                l_rx.var.cmd.idx = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_CMD_PARAM3);
            }
            break;
        }
        case WAIT4_CMD_PARAM3: {
            l_rx.var.cmd.param3 |= ((uint32_t)b << l_rx.var.cmd.idx);
            l_rx.var.cmd.idx    += (uint8_t)8;
            if (l_rx.var.cmd.idx == (uint8_t)(8*4)) {
                l_rx.var.cmd.idx = (uint8_t)0;
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
            l_rx.var.tick.rate = (uint_fast8_t)b;
            QS_RX_TRAN_(WAIT4_TICK_FRAME);
            break;
        }
        case WAIT4_TICK_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_PEEK_OFFS: {
            if (l_rx.var.peek.idx == (uint8_t)0) {
                l_rx.var.peek.offs = (uint16_t)b;
                l_rx.var.peek.idx += (uint8_t)8;
            }
            else {
                l_rx.var.peek.offs |= (uint16_t)((uint16_t)b << 8);
                QS_RX_TRAN_(WAIT4_PEEK_SIZE);
            }
            break;
        }
        case WAIT4_PEEK_SIZE: {
            if ((b == (uint8_t)1)
                || (b == (uint8_t)2)
                || (b == (uint8_t)4))
            {
                l_rx.var.peek.size = b;
                QS_RX_TRAN_(WAIT4_PEEK_NUM);
            }
            else {
                QS_rxReportError_((uint8_t)QS_RX_PEEK);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_PEEK_NUM: {
            l_rx.var.peek.num = b;
            QS_RX_TRAN_(WAIT4_PEEK_FRAME);
            break;
        }
        case WAIT4_PEEK_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_POKE_OFFS: {
            if (l_rx.var.poke.idx == (uint8_t)0) {
                l_rx.var.poke.offs = (uint16_t)b;
                l_rx.var.poke.idx  = (uint8_t)1;
            }
            else {
                l_rx.var.poke.offs |= (uint16_t)((uint16_t)b << 8);
                QS_RX_TRAN_(WAIT4_POKE_SIZE);
            }
            break;
        }
        case WAIT4_POKE_SIZE: {
            if ((b == (uint8_t)1)
                || (b == (uint8_t)2)
                || (b == (uint8_t)4))
            {
                l_rx.var.poke.size = b;
                QS_RX_TRAN_(WAIT4_POKE_NUM);
            }
            else {
                QS_rxReportError_((l_rx.var.poke.fill != (uint8_t)0)
                                  ? (uint8_t)QS_RX_FILL
                                  : (uint8_t)QS_RX_POKE);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_POKE_NUM: {
            if (b > (uint8_t)0) {
                l_rx.var.poke.num  = b;
                l_rx.var.poke.data = (uint32_t)0;
                l_rx.var.poke.idx  = (uint8_t)0;
                QS_RX_TRAN_((l_rx.var.poke.fill != (uint8_t)0)
                            ? WAIT4_FILL_DATA
                            : WAIT4_POKE_DATA);
            }
            else {
                QS_rxReportError_((l_rx.var.poke.fill != (uint8_t)0)
                                  ? (uint8_t)QS_RX_FILL
                                  : (uint8_t)QS_RX_POKE);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_FILL_DATA: {
            l_rx.var.poke.data |= ((uint32_t)b << l_rx.var.poke.idx);
            l_rx.var.poke.idx += (uint8_t)8;
            if ((l_rx.var.poke.idx >> 3) == l_rx.var.poke.size) {
                QS_RX_TRAN_(WAIT4_FILL_FRAME);
            }
            break;
        }
        case WAIT4_POKE_DATA: {
            l_rx.var.poke.data |= ((uint32_t)b << l_rx.var.poke.idx);
            l_rx.var.poke.idx += (uint8_t)8;
            if ((l_rx.var.poke.idx >> 3) == l_rx.var.poke.size) {
                QS_rxPoke_();
                --l_rx.var.poke.num;
                if (l_rx.var.poke.num == (uint8_t)0) {
                    QS_RX_TRAN_(WAIT4_POKE_FRAME);
                }
            }
            break;
        }
        case WAIT4_FILL_FRAME: {
            /* keep ignoring the data until a frame is collected */
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
                QS_rxReportError_((uint8_t)QS_RX_GLB_FILTER);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_GLB_FILTER_DATA: {
            l_rx.var.gFlt.data[l_rx.var.gFlt.idx] = b;
            ++l_rx.var.gFlt.idx;
            if (l_rx.var.gFlt.idx
                == (uint8_t)sizeof(l_rx.var.gFlt.data))
            {
                QS_RX_TRAN_(WAIT4_GLB_FILTER_FRAME);
            }
            break;
        }
        case WAIT4_GLB_FILTER_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_OBJ_KIND: {
            if (b <= (uint8_t)SM_AO_OBJ) {
                l_rx.var.obj.kind = b;
                l_rx.var.obj.addr = (QSObj)0;
                l_rx.var.obj.idx  = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_OBJ_ADDR);
            }
            else {
                QS_rxReportError_(l_rx.var.obj.recId);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_OBJ_ADDR: {
            l_rx.var.obj.addr |= ((uint32_t)b << l_rx.var.obj.idx);
            l_rx.var.obj.idx += (uint8_t)8;
            if (l_rx.var.obj.idx == (uint8_t)(8*QS_OBJ_PTR_SIZE)) {
                QS_RX_TRAN_(WAIT4_OBJ_FRAME);
            }
            break;
        }
        case WAIT4_OBJ_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_QUERY_KIND: {
            if (b < (uint8_t)MAX_OBJ) {
                l_rx.var.obj.kind = b;
                QS_RX_TRAN_(WAIT4_QUERY_FRAME);
            }
            else {
                QS_rxReportError_(l_rx.var.obj.recId);
                QS_RX_TRAN_(ERROR_STATE);
            }
            break;
        }
        case WAIT4_QUERY_FRAME: {
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
            l_rx.var.evt.idx  = (uint8_t)0;
            QS_RX_TRAN_(WAIT4_EVT_SIG);
            break;
        }
        case WAIT4_EVT_SIG: {
            l_rx.var.evt.sig |= (QSignal)((uint32_t)b << l_rx.var.evt.idx);
            l_rx.var.evt.idx += (uint8_t)8;
            if (l_rx.var.evt.idx == (uint8_t)(8*Q_SIGNAL_SIZE)) {
                l_rx.var.evt.len = (uint16_t)0;
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
                    /* report Ack before generating any other QS records */
                    QS_rxReportAck_(QS_RX_EVENT);

                    l_rx.var.evt.e = QF_newX_(
                        ((uint_fast16_t)l_rx.var.evt.len
                         + (uint_fast16_t)sizeof(QEvt)),
                        (uint_fast16_t)1, /* margin */
                        (enum_t)l_rx.var.evt.sig);
                    if (l_rx.var.evt.e != (QEvt *)0) { /* evt allocated? */
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
                        QS_rxReportError_((uint8_t)QS_RX_EVENT);
                        QS_RX_TRAN_(ERROR_STATE);
                    }
                }
                else {
                    QS_rxReportError_((uint8_t)QS_RX_EVENT);
                    QS_RX_TRAN_(ERROR_STATE);
                }
            }
            break;
        }
        case WAIT4_EVT_PAR: {  /* event parameters */
            *l_rx.var.evt.p = b;
            ++l_rx.var.evt.p;
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

#ifdef Q_UTEST

        case WAIT4_TEST_SETUP_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_TEST_TEARDOWN_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_TEST_CONTINUE_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
        case WAIT4_TEST_PROBE_DATA: {
            l_rx.var.tp.data |= ((uint32_t)b << l_rx.var.tp.idx);
            l_rx.var.tp.idx += (uint8_t)8;
            if (l_rx.var.tp.idx == (uint8_t)(8*sizeof(uint32_t))) {
                l_rx.var.tp.addr = (uint32_t)0;
                l_rx.var.tp.idx  = (uint8_t)0;
                QS_RX_TRAN_(WAIT4_TEST_PROBE_ADDR);
            }
            break;
        }
        case WAIT4_TEST_PROBE_ADDR: {
            l_rx.var.tp.addr |= ((uint32_t)b << l_rx.var.tp.idx);
            l_rx.var.tp.idx += (uint8_t)8;
            if (l_rx.var.tp.idx == (uint8_t)(8*QS_FUN_PTR_SIZE)) {
                QS_RX_TRAN_(WAIT4_TEST_PROBE_FRAME);
            }
            break;
        }
        case WAIT4_TEST_PROBE_FRAME: {
            /* keep ignoring the data until a frame is collected */
            break;
        }
#endif /* Q_UTEST */

        case ERROR_STATE: {
            /* keep ignoring the data until a good frame is collected */
            break;
        }
        default: {  /* unexpected or unimplemented state */
            QS_rxReportError_((uint8_t)0x45);
            QS_RX_TRAN_(ERROR_STATE);
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxHandleGoodFrame_(uint8_t state) {
    uint8_t i;
    uint8_t *ptr;

    switch (state) {
        case WAIT4_INFO_FRAME: {
            /* no need to report Ack or Done */
            QS_target_info_((uint8_t)0); /* send only Target info */
            break;
        }
        case WAIT4_RESET_FRAME: {
            /* no need to report Ack or Done, because Target resets */
            QS_onReset(); /* reset the Target */
            break;
        }
        case WAIT4_CMD_PARAM1: /* intentionally fall-through */
        case WAIT4_CMD_PARAM2: /* intentionally fall-through */
        case WAIT4_CMD_PARAM3: /* intentionally fall-through */
        case WAIT4_CMD_FRAME: {
            QS_rxReportAck_(QS_RX_COMMAND);
            QS_onCommand(l_rx.var.cmd.cmdId, l_rx.var.cmd.param1,
                         l_rx.var.cmd.param2, l_rx.var.cmd.param3);
            QS_rxReportDone_(QS_RX_COMMAND);
            break;
        }
        case WAIT4_TICK_FRAME: {
            QS_rxReportAck_(QS_RX_TICK);
#ifdef Q_UTEST
            QS_tickX_((uint_fast8_t)l_rx.var.tick.rate, &QS_rxPriv_);
            QS_processTestEvts_(); /* process all events produced */
#else
            QF_tickX_((uint_fast8_t)l_rx.var.tick.rate, &QS_rxPriv_);
#endif
            QS_rxReportDone_(QS_RX_TICK);
            break;
        }
        case WAIT4_PEEK_FRAME: {
            /* no need to report Ack or Done */
            QS_beginRec((uint_fast8_t)QS_PEEK_DATA);
                ptr = ((uint8_t *)QS_rxPriv_.currObj[AP_OBJ]
                       + l_rx.var.peek.offs);
                QS_TIME_();                  /* timestamp */
                QS_U16_(l_rx.var.peek.offs); /* data offset */
                QS_U8_(l_rx.var.peek.size);  /* data size */
                QS_U8_(l_rx.var.peek.num);   /* number of data items */
                for (i = (uint8_t)0; i < l_rx.var.peek.num; ++i) {
                    switch (l_rx.var.peek.size) {
                        case 1:
                            QS_U8_(*(ptr + i));
                            break;
                        case 2:
                            QS_U16_(*((uint16_t *)ptr + i));
                            break;
                        case 4:
                            QS_U32_(*((uint32_t *)ptr + i));
                            break;
                        default:
                            break;
                    }
                }
            QS_endRec();
            QS_REC_DONE();
            break;
        }
        case WAIT4_POKE_DATA: {
            /* received less than expected poke data items */
            QS_rxReportError_((uint8_t)QS_RX_POKE);
            break;
        }
        case WAIT4_POKE_FRAME: {
            QS_rxReportAck_(QS_RX_POKE);
            /* no need to report done */
            break;
        }
        case WAIT4_FILL_FRAME: {
            QS_rxReportAck_(QS_RX_FILL);
            ptr = ((uint8_t *)QS_rxPriv_.currObj[AP_OBJ]
                   + l_rx.var.poke.offs);
            for (i = (uint8_t)0; i < l_rx.var.poke.num; ++i) {
                switch (l_rx.var.poke.size) {
                    case 1:
                        *(ptr + i) = (uint8_t)l_rx.var.poke.data;
                        break;
                    case 2:
                        *((uint16_t *)ptr + i)
                            = (uint16_t)l_rx.var.poke.data;
                        break;
                    case 4:
                        *((uint32_t *)ptr + i) = l_rx.var.poke.data;
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case WAIT4_GLB_FILTER_FRAME: {
            QS_rxReportAck_(QS_RX_GLB_FILTER);

            /* never disable the non-maskable records */
            l_rx.var.gFlt.data[0] |= (uint8_t)0x01;
            l_rx.var.gFlt.data[7] |= (uint8_t)0xFC;
            l_rx.var.gFlt.data[8] |= (uint8_t)0x3F;

            /* never enable the last 3 records (0x7D, 0x7E, 0x7F) */
            l_rx.var.gFlt.data[15] &= (uint8_t)0x1F;

            for (i=(uint8_t)0; i < (uint8_t)sizeof(QS_priv_.glbFilter); ++i) {
                QS_priv_.glbFilter[i] = l_rx.var.gFlt.data[i];
            }
            /* no need to report Done */
            break;
        }
        case WAIT4_OBJ_FRAME: {
            i = l_rx.var.obj.kind;
            if (i < (uint8_t)MAX_OBJ) {
                if (l_rx.var.obj.recId == (uint8_t)QS_RX_LOC_FILTER) {
                    QS_priv_.locFilter[i] = (void *)l_rx.var.obj.addr;
                }
                else { /* set the current object */
                    QS_rxPriv_.currObj[i] = (void *)l_rx.var.obj.addr;
                }
                QS_rxReportAck_((enum QSpyRxRecords)l_rx.var.obj.recId);
            }
            /* both SM and AO */
            else if (i == (uint8_t)SM_AO_OBJ) {
                if (l_rx.var.obj.recId == (uint8_t)QS_RX_LOC_FILTER) {
                    QS_priv_.locFilter[SM_OBJ] = (void *)l_rx.var.obj.addr;
                    QS_priv_.locFilter[AO_OBJ] = (void *)l_rx.var.obj.addr;
                }
                else {
                    QS_rxPriv_.currObj[SM_OBJ] = (void *)l_rx.var.obj.addr;
                    QS_rxPriv_.currObj[AO_OBJ] = (void *)l_rx.var.obj.addr;
                }
                QS_rxReportAck_((enum QSpyRxRecords)l_rx.var.obj.recId);
            }
            else {
                QS_rxReportError_(l_rx.var.obj.recId);
            }
            break;
        }
        case WAIT4_QUERY_FRAME: {
            i = l_rx.var.obj.kind;
            ptr = (uint8_t *)QS_rxPriv_.currObj[i];
            if (ptr != (void *)0) {
                QS_beginRec((uint_fast8_t)QS_QUERY_DATA);
                    QS_TIME_(); /* timestamp */
                    QS_U8_(i);  /* object kind */
                    QS_OBJ_(ptr);
                    switch (i) {
                        case SM_OBJ:
                            QS_FUN_(((QHsm *)ptr)->state.fun);
                            break;

#ifdef Q_UTEST
                        case AO_OBJ:
                            QS_EQC_(((QActive *)ptr)->eQueue.nFree);
                            QS_EQC_(((QActive *)ptr)->eQueue.nMin);
                            break;
                        case MP_OBJ:
                            QS_MPC_(((QMPool *)ptr)->nFree);
                            QS_MPC_(((QMPool *)ptr)->nMin);
                            break;
                        case EQ_OBJ:
                            QS_EQC_(((QEQueue *)ptr)->nFree);
                            QS_EQC_(((QEQueue *)ptr)->nMin);
                            break;
                        case TE_OBJ:
                            QS_OBJ_(((QTimeEvt *)ptr)->act);
                            QS_TEC_(((QTimeEvt *)ptr)->ctr);
                            QS_TEC_(((QTimeEvt *)ptr)->interval);
                            QS_SIG_(((QTimeEvt *)ptr)->super.sig);
                            QS_U8_ (((QTimeEvt *)ptr)->super.refCtr_);
                            break;
#endif /* Q_UTEST */

                        default:
                            break;
                    }
                QS_endRec();
                QS_REC_DONE();
            }
            else {
                QS_rxReportError_((uint8_t)QS_RX_QUERY_CURR);
            }
            break;
        }
        case WAIT4_AO_FILTER_FRAME: {
            QS_rxReportAck_(QS_RX_AO_FILTER);
            if (l_rx.var.aFlt.prio <= (uint8_t)QF_MAX_ACTIVE) {
                QS_rxReportAck_(QS_RX_AO_FILTER);
                QS_priv_.locFilter[AO_OBJ] = QF_active_[l_rx.var.aFlt.prio];
                QS_priv_.locFilter[SM_OBJ] = QF_active_[l_rx.var.aFlt.prio];
            }
            else {
                QS_rxReportError_((uint8_t)QS_RX_AO_FILTER);
            }
            /* no need to report Done */
            break;
        }
        case WAIT4_EVT_FRAME: {
            /* NOTE: Ack was already reported in the WAIT4_EVT_LEN state */
#ifdef Q_UTEST
            QS_onTestEvt(l_rx.var.evt.e); /* adjust the event, if needed */
#endif /* Q_UTEST */
            i = (uint8_t)0; /* use 'i' as status, 0 == success,no-recycle */

            if (l_rx.var.evt.prio == (uint8_t)0) { /* publish */
                QF_PUBLISH(l_rx.var.evt.e, &QS_rxPriv_);
                QS_rxReportDone_(QS_RX_EVENT);
            }
            else if (l_rx.var.evt.prio < (uint8_t)QF_MAX_ACTIVE) {
                if (QACTIVE_POST_X(QF_active_[l_rx.var.evt.prio],
                               l_rx.var.evt.e,
                               (uint_fast16_t)1, /* margin */
                               &QS_rxPriv_) == false)
                {
                    /* failed QACTIVE_POST() recycles the event */
                    i = (uint8_t)0x80; /* failure status, no recycle */
                }
            }
            else if (l_rx.var.evt.prio == (uint8_t)255) { /* special prio */
                /* dispatch to the current SM object */
                if (QS_rxPriv_.currObj[SM_OBJ] != (void *)0) {
                    /* increment the ref-ctr to simulate the situation
                    * when the event is just retreived from a queue.
                    * This is expected for the following QF_gc() call.
                    */
                    ++l_rx.var.evt.e->refCtr_;

                    QHSM_DISPATCH((QHsm *)QS_rxPriv_.currObj[SM_OBJ],
                                  l_rx.var.evt.e);
                    i = (uint8_t)0x01;  /* success status, recycle needed */
                }
                else {
                    i = (uint8_t)0x81;  /* failure status, recycle needed */
                }
            }
            else if (l_rx.var.evt.prio == (uint8_t)254) { /* special prio */
                /* init the current SM object" */
                if (QS_rxPriv_.currObj[SM_OBJ] != (void *)0) {
                    /* increment the ref-ctr to simulate the situation
                    * when the event is just retreived from a queue.
                    * This is expected for the following QF_gc() call.
                    */
                    ++l_rx.var.evt.e->refCtr_;

                    QHSM_INIT((QHsm *)QS_rxPriv_.currObj[SM_OBJ],
                              l_rx.var.evt.e);
                    i = (uint8_t)0x01;  /* success status, recycle needed */
                }
                else {
                    i = (uint8_t)0x81;  /* failure status, recycle needed */
                }
            }
            else if (l_rx.var.evt.prio == (uint8_t)253) { /* special prio */
                /* post to the current AO */
                if (QS_rxPriv_.currObj[AO_OBJ] != (void *)0) {
                    if (QACTIVE_POST_X(
                            (QActive *)QS_rxPriv_.currObj[AO_OBJ],
                            l_rx.var.evt.e,
                            (uint_fast16_t)1, /* margin */
                            &QS_rxPriv_) == false)
                    {
                        /* failed QACTIVE_POST() recycles the event */
                        i = (uint8_t)0x80;  /* failure status, no recycle */
                    }
                }
                else {
                    i = (uint8_t)0x81;  /* failure status, recycle needed */
                }
            }
            else {
                i = (uint8_t)0x81;  /* failure status, recycle needed */
            }

            if ((i & (uint8_t)0x01) != (uint8_t)0) { /* recycle needed? */
                QF_gc(l_rx.var.evt.e);
            }

            if ((i & (uint8_t)0x80) != (uint8_t)0) { /* failure? */
                QS_rxReportError_((uint8_t)QS_RX_EVENT);
            }
            else {
#ifdef Q_UTEST
                QS_processTestEvts_(); /* process all events produced */
#endif
                QS_rxReportDone_(QS_RX_EVENT);
            }
            break;
        }

#ifdef Q_UTEST
        case WAIT4_TEST_SETUP_FRAME: {
            QS_rxReportAck_(QS_RX_TEST_SETUP);
            l_testData.tpNum    = (uint8_t)0;   /* clear the Test-Probes */
            l_testData.testTime = (QSTimeCtr)0; /* clear the time tick */
            /* don't clear current objects */
            QS_onTestSetup(); /* application-specific test setup */
            /* no need to report Done */
            break;
        }
        case WAIT4_TEST_TEARDOWN_FRAME: {
            QS_rxReportAck_(QS_RX_TEST_TEARDOWN);
            QS_onTestTeardown(); /* application-specific test teardown */
            /* no need to report Done */
            break;
        }
        case WAIT4_TEST_CONTINUE_FRAME: {
            QS_rxReportAck_(QS_RX_TEST_CONTINUE);
            QS_rxPriv_.inTestLoop = false; /* exit the QUTest loop */
            /* no need to report Done */
            break;
        }
        case WAIT4_TEST_PROBE_FRAME: {
            QS_rxReportAck_(QS_RX_TEST_PROBE);
            Q_ASSERT_ID(815, l_testData.tpNum
                     < (uint8_t)(sizeof(l_testData.tpBuf)
                                 /sizeof(l_testData.tpBuf[0])));
            l_testData.tpBuf[l_testData.tpNum] = l_rx.var.tp;
            ++l_testData.tpNum;
            /* no need to report Done */
            break;
        }
#endif /* Q_UTEST */

        case ERROR_STATE: {
            /* keep ignoring all bytes until new frame */
            break;
        }
        default: {
            QS_rxReportError_((uint8_t)0x47);
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxHandleBadFrame_(uint8_t state) {
    QS_rxReportError_((uint8_t)0x50); /* report error for all bad frames */
    switch (state) {
        case WAIT4_EVT_FRAME: {
            Q_ASSERT_ID(910, l_rx.var.evt.e != (QEvt *)0);
            QF_gc(l_rx.var.evt.e); /* don't leak an allocated event */
            break;
        }
        default: {
            break;
        }
    }
}

/****************************************************************************/
static void QS_rxReportAck_(enum QSpyRxRecords recId) {
    QS_beginRec((uint_fast8_t)QS_RX_STATUS);
        QS_U8_(recId); /* record ID */
    QS_endRec();
    QS_REC_DONE();
}

/****************************************************************************/
static void QS_rxReportError_(uint8_t code) {
    QS_beginRec((uint_fast8_t)QS_RX_STATUS);
        QS_U8_((uint8_t)((uint8_t)0x80 | code)); /* error code */
    QS_endRec();
    QS_REC_DONE();
}

/****************************************************************************/
static void QS_rxReportDone_(enum QSpyRxRecords recId) {
    QS_beginRec((uint_fast8_t)QS_TARGET_DONE);
        QS_TIME_();    /* timestamp */
        QS_U8_(recId); /* record ID */
    QS_endRec();
    QS_REC_DONE();
}

/****************************************************************************/
static void QS_rxPoke_(void) {
    uint8_t *ptr = ((uint8_t *)QS_rxPriv_.currObj[AP_OBJ]
                    + l_rx.var.poke.offs);
    switch (l_rx.var.poke.size) {
        case 1:
            *ptr = (uint8_t)l_rx.var.poke.data;
            break;
        case 2:
            *(uint16_t *)ptr = (uint16_t)l_rx.var.poke.data;
            break;
        case 4:
            *(uint32_t *)ptr = l_rx.var.poke.data;
            break;
        default:
            Q_ERROR_ID(900);
            break;
    }

    l_rx.var.poke.data = (uint32_t)0;
    l_rx.var.poke.idx  = (uint8_t)0;
    l_rx.var.poke.offs += (uint16_t)l_rx.var.poke.size;
}

/*==========================================================================*/
#ifdef Q_UTEST

/****************************************************************************/
/**
* @description
* This function obtains the Test-Probe for a given API.
*
* @param[in]  api_id  the API-ID that requests its Test-Probe
*
* @returs Test-Probe data that has been received for the given API
* from the Host (running qutest). For any ginve API, the function returns
* the Test-Probe data in the same order as it was received from the Host.
* If there is no Test-Probe for a ginve API, or no more Test-Probes for
* a given API, the function returns zero.
*/
uint32_t QS_getTestProbe_(void (* const api)(void)) {
    uint32_t data = (uint32_t)0;
    uint8_t i;
    for (i = (uint8_t)0; i < l_testData.tpNum; ++i) {
        if (l_testData.tpBuf[i].addr == (QSFun)api) {
            data = l_testData.tpBuf[i].data;
            --l_testData.tpNum;
            /* move all remaining entries in the buffer up by one */
            for (; i < l_testData.tpNum; ++i) {
                l_testData.tpBuf[i] = l_testData.tpBuf[i + (uint8_t)1];
            }
            /* i == l_testData.tpNum, which terminates the for loop */
            QS_beginRec((uint_fast8_t)QS_TEST_PROBE_GET);
                QS_TIME_();    /* timestamp */
                QS_FUN_(api);  /* the calling API */
                QS_U32_(data); /* the Test-Probe data */
            QS_endRec();
            QS_REC_DONE();
        }
    }
    return data;
}

/****************************************************************************/
QSTimeCtr QS_onGetTime(void) {
    return (++l_testData.testTime);
}

#endif /* Q_UTEST */

