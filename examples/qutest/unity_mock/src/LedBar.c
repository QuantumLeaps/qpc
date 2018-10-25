#include <stdint.h>
#include "LedBar.h"
#include "Led.h"

uint32_t LedBar_setPercent(uint8_t percent) {
    uint8_t n = (uint8_t)((percent * MAX_LED) / 100);
    uint8_t i;
    uint32_t p = 0U; /* power draw in uW */
    for (i = 0U; i < n; ++i) {
        p += Led_on(i);
    }
    for (i = n; i < MAX_LED; ++i) {
        Led_off(i);
    }
    return p;
}
