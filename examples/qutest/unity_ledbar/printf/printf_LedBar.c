#include <stdint.h>
#include <stdio.h> /* for printf() */

#include "LedBar.h" /* CUT interface */

/*--------------------------------------------------------------------------*/
static void trace_LedBar_percent(uint8_t percent) {
    uint32_t ret = LedBar_setPercent(percent);
    printf("LedBar_setPercent %d %d\n", ret, percent);
}

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    (void)argc; /* unused parameter */
    (void)argv; /* unused parameter */

    trace_LedBar_percent(100U);
    trace_LedBar_percent(19U);
    trace_LedBar_percent(20U);
    trace_LedBar_percent(50U);
    trace_LedBar_percent(99U);
    trace_LedBar_percent(200U);

    return 0;
}
/*..........................................................................*/
#include <stdlib.h> /* for exit() */

void Q_onAssert(char const * const module, int const loc) {
    printf("Assertion in %s:%d\n", module, loc);
    exit(-1);
}
