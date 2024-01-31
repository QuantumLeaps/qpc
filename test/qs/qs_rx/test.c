#include "et.h"       // ET: embedded test

// includes for the CUT...
#include "qp_port.h"      // QP port
#include "qsafe.h"        // QP Functional Safety (FuSa) System
#ifdef Q_SPY // software tracing enabled?
#include "qs_port.h"      // QS/C port from the port directory
#else
#include "qs_dummy.h"     // QS/C dummy (inactive) interface
#endif

enum { RX_SIZE = 8 };

static uint8_t qsBuf[100];       // buffer for QS-TX channel
static uint8_t qsRxBuf[RX_SIZE]; // buffer for QS-RX channel

void setup(void) {
}

void teardown(void) {
}

// test group --------------------------------------------------------------
TEST_GROUP("QS/RX") {

QS_initBuf(qsBuf, sizeof(qsBuf));

TEST("QS-RX initialization") {
    QS_rxInitBuf(qsRxBuf, RX_SIZE);
    VERIFY(RX_SIZE - 1 == QS_rxGetNfree());
}

TEST("QS-RX putting 3") {
    for (uint8_t n = 0U; n < 3U; ++n) {
        VERIFY(QS_RX_PUT(n + 1U));
    }
    VERIFY(RX_SIZE - 1 - 3 == QS_rxGetNfree());
}

TEST("QS-RX QS_rxParse") {
    QS_rxParse();
    VERIFY(RX_SIZE - 1 == QS_rxGetNfree());
}

TEST("QS-RX putting 6") {
    for (uint8_t n = 0U; n < 6U; ++n) {
        VERIFY(QS_RX_PUT(n + 1U));
    }
    VERIFY(RX_SIZE - 1 - 6 == QS_rxGetNfree());
}

TEST("QS-RX putting 3 more") {
    VERIFY(QS_RX_PUT(0x7U));
    VERIFY(false == QS_RX_PUT(0x8U));
    VERIFY(false == QS_RX_PUT(0x9U));
    VERIFY(0 == QS_rxGetNfree());
}

} // TEST_GROUP()

// =========================================================================
// dependencies for the CUT ...

//..........................................................................
void QF_poolInit(void * const poolSto, uint_fast32_t const poolSize,
    uint_fast16_t const evtSize)
{
    (void)poolSto;
    (void)poolSize;
    (void)evtSize;
}
//..........................................................................
uint_fast16_t QF_poolGetMaxBlockSize(void) {
    return 0U;
}
//..........................................................................
void QActive_publish_(QEvt const * const e,
                      void const * const sender, uint_fast8_t const qsId)
{
    (void)e;
    (void)sender;
    (void)qsId;
}
//..........................................................................
void QTimeEvt_tick_(uint_fast8_t const tickRate, void const * const sender) {
    (void)tickRate;
    (void)sender;
}
//..........................................................................
QEvt *QF_newX_(uint_fast16_t const evtSize,
    uint_fast16_t const margin, enum_t const sig)
{
    (void)evtSize;
    (void)margin;
    (void)sig;

    return (QEvt *)0;
}
//..........................................................................
//! @static @public @memberof QF
void QF_gc(QEvt const * const e) {
    (void)e;
}
//..........................................................................
//! @static @private @memberof QF
QActive *QActive_registry_[QF_MAX_ACTIVE + 1U];

//..........................................................................
Q_NORETURN Q_onError(char const * const module, int_t const location) {
    ET_fail("Q_onError", module, location);
    for (;;) {} // explicitly no-return
}

//--------------------------------------------------------------------------
#ifdef Q_SPY

void QS_onCleanup(void) {
}
//..........................................................................
void QS_onReset(void) {
}
//..........................................................................
void QS_onFlush(void) {
}
//..........................................................................
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)0U;
}
//..........................................................................
void QS_onCommand(uint8_t cmdId, uint32_t param1,
    uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif // Q_SPY
