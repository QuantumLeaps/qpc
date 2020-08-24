#include <stdint.h>
#include "LedBar.h"
#include "Led.h"

/*
 Example sequence diagram for the LedBar_setPercent() implementation:

 +----------+      +------+  +------+  +------+  +------+  +------+
 |  LedBar  |      |LED[0]|  |LED[1]|  |LED[2]|  |LED[3]|  |LED[4]|
 +----------+      +------+  +------+  +------+  +------+  +------+
       |               |         |         |         |         |
      +-+ Led_on()     |         |         |         |         |
      | |------------->|         |         |         |         |
      | |              |         |         |         |         |
      | | Led_on()     |         |         |         |         |
      | |----------------------->|         |         |         |
      | |              |         |         |         |         |
      | | Led_on()     |         |         |         |         |
      | |--------------------------------->|         |         |
      | |              |         |         |         |         |
      | | Led_off()    |         |         |         |         |
      | |------------------------------------------->|         |
      | |              |         |         |         |         |
      | | Led_off()    |         |         |         |         |
      | |----------------------------------------------------->|
      | |              |         |         |         |         |
      +-+              |         |         |         |         |
       |               |         |         |         |         |
*/
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
