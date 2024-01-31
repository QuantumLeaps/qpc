#include "et.h"       // Embedded Test (ET)

// includes for the CUT...
#include "qp_port.h"      // QP port
#include "qsafe.h"        // QP Functional Safety (FuSa) System
#ifdef Q_SPY // software tracing enabled?
#include "qs_port.h"      // QS/C port from the port directory
#else
#include "qs_dummy.h"     // QS/C dummy (inactive) interface
#endif

enum { QUEUE_SIZE = 10 };


void setup(void) {
}

void teardown(void) {
}

//static QEvt const *queBuf[QUEUE_SIZE];
static QEQueue queue;

static QEvt const evt1 = QEVT_INITIALIZER(1);
static QEvt const evt2 = QEVT_INITIALIZER(2);

// test group --------------------------------------------------------------
TEST_GROUP("QEQueue") {

TEST("new no-buffer queue has 1 free") {
    QEQueue_init(&queue, (QEvt const **)0, 0U);
    VERIFY(1U == QEQueue_getNFree(&queue));
}

TEST("no-buffer queue can hold at least 1") {
    QEQueue_post(&queue, &evt1, QF_NO_MARGIN, 0U);
    VERIFY(0U == QEQueue_getNFree(&queue));
    QEvt const *e = QEQueue_get(&queue, 0U);
    VERIFY(&evt1 == e);
    VERIFY(1U == QEQueue_getNFree(&queue));
}

TEST("no-buffer queue can hold only 1 (expected assertion)") {
    QEQueue_post(&queue, &evt1, QF_NO_MARGIN, 0U);
    ET_expect_assert("qf_qeq", 210);
    VERIFY(false == QEQueue_post(&queue, &evt2, QF_NO_MARGIN, 0U));
    VERIFY(0U == QEQueue_getNFree(&queue));
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
    VERIFY_ASSERT(module, location);
    for (;;) { // explicitly make it "noreturn"
    }
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
