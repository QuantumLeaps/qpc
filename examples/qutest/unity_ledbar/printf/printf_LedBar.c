#include <stdint.h>
#include <stdio.h> // for printf()

#include "LedBar.h" // CUT interface

//----------------------------------------------------------------------------
static void trace_LedBar_percent(uint8_t percent) {
    uint32_t ret = LedBar_setPercent(percent);
    printf("LedBar_setPercent %d %d\n", ret, percent);
}

//----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    (void)argc; // unused parameter
    (void)argv; // unused parameter

    trace_LedBar_percent(100U);
    trace_LedBar_percent(19U);
    trace_LedBar_percent(20U);
    trace_LedBar_percent(50U);
    trace_LedBar_percent(99U);
    trace_LedBar_percent(200U);

    return 0;
}
//............................................................................
#include <stdlib.h> // for exit()

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    printf("ERROR in %s:%d\n", module, id);
    exit(-1);
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}
