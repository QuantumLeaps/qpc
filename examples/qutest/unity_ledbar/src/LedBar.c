#include <stdint.h>
#include "LedBar.h"
#include "Led.h"

#include "qsafe.h"        // QP Functional Safety (FuSa) System

Q_DEFINE_THIS_MODULE("LedBar")

//
// Example sequence diagram for the LedBar_setPercent() implementation:
//
// +----------+       +-----+
// |  LedBar  |       | LED |
// +----------+       +-----+
//     |               |
//    +-+ Led_on(0)    |
//    | |------------->|
//    | |              |
//    | | Led_on(1)    |
//    | |------------->|
//    | |              |
//    | | Led_on(2)    |
//    | |------------->|
//    | |              |
//    | | Led_off(3)   |
//    | |------------->|
//    | |              |
//    | | Led_off(4)   |
//    | |------------->|
//    | |              |
//    +-+              |
//     |               |
//
uint32_t LedBar_setPercent(uint8_t const percent) {
    uint8_t const n_on = (uint8_t)((percent * MAX_LED) / 100U);

    //! @pre percent must not exceed 100
    Q_REQUIRE_ID(100, n_on <= MAX_LED);

    uint32_t p = 0U; // power draw in [uW]
    uint8_t i;
    for (i = 0U; i < n_on; ++i) {
        p += Led_on(i);
    }
    for (i = n_on; i < MAX_LED; ++i) {
        Led_off(i);
    }
    return p;
}
