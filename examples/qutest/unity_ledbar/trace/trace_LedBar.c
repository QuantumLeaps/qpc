#include "qpc.h"  // QP/Spy
#include "LedBar.h" // CUT interface

Q_DEFINE_THIS_FILE

//----------------------------------------------------------------------------
void Led_DICTIONARY(void); // dictionaries for the Led "spy " test double

static void trace_LedBar_percent(uint8_t percent) {
    uint32_t ret = LedBar_setPercent(percent);
    //printf("LedBar_setPercent %d %d\n", ret, percent);
    QS_BEGIN_ID(QS_USER, 0U) // app-specific record
        QS_FUN(&LedBar_setPercent); // function called
        QS_U32(0, ret); // value returned
        QS_U8 (0, percent); // parameter
    QS_END()
    QS_FLUSH(); // output trace buffer to the host
}

//----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    QF_init();  // initialize the framework

    // initialize the QS software tracing
    if (QS_INIT((argc > 1) ? argv[1] : (void *)0) == 0U) {
        Q_ERROR();
    }

    // produce dictionaries...
    Led_DICTIONARY();
    QS_FUN_DICTIONARY(&LedBar_setPercent);

    // filter setup...
    QS_GLB_FILTER(QS_ALL_RECORDS);

    trace_LedBar_percent(100U);
    trace_LedBar_percent(19U);
    trace_LedBar_percent(20U);
    trace_LedBar_percent(50U);
    trace_LedBar_percent(99U);
    trace_LedBar_percent(200U);

    return 0;
}

//----------------------------------------------------------------------------
// callback functions for QP/Spy...
void QS_onCommand(uint8_t cmdId, uint32_t param1,
                  uint32_t param2, uint32_t param3)
{
    // unused parametrers...
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}
//............................................................................
void QF_onStartup(void) {
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QF_onClockTick(void) {
}
//............................................................................
#include <stdlib.h> // for exit()

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    //printf("ERROR in %s:%d\n", module, id);
    QS_ASSERTION(module, id, 1000U); // report assertion to QS
    QF_onCleanup();
    QS_EXIT();
    exit(-1);
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}
