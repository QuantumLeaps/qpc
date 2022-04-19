#include <stdint.h>
#include "LedBar2.h"
#include "Led.h"
#include "Spi.h"

#include "qassert.h" /* for embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("LedBar2")

/*
 Example sequence diagram for the LedBar2_setPercent() implementation:

 +----------+       +-----+   +------+
 |  LedBar2 |       | LED |   | SPI  |
 +----------+       +-----+   +------+
       |               |          |
      +-+ SPI_beginTransaction()  |
      | |------------------------>|
      | |              |          |
      | | Led_on(0)    |          |
      | |------------->|          |
      | | SPI_transfer(pwr)       |
      | |------------------------>|
      | |              |          |
      | | Led_on(1)    |          |
      | |------------->|          |
      | |              |          |
      | | SPI_transfer(pwr)       |
      | |------------------------>|
      | | SPI_endTransaction()    |
      | |------------------------>|
      | |              |          |
      | | Led_off(2)   |          |
      | |------------->|          |
      | |              |          |
      | | Led_off(3)   |          |
      | |------------->|          |
      | |              |          |
      | | Led_off(4)   |          |
      | |------------->|          |
      +-+              |          |
       |               |          |
*/
uint32_t LedBar2_setPercent(uint8_t const percent) {
    uint8_t const n_on = (uint8_t)((percent * MAX_LED) / 100U);

    /*! @pre percent must not exceed 100 */
    Q_REQUIRE_ID(100, n_on <= MAX_LED);

    uint32_t p = 0U; /* power draw in [uW] */
    uint8_t i;
    if (n_on > 0U) {
        SPI_beginTransaction();
    }
    for (i = 0U; i < n_on; ++i) {
        uint32_t p1 = Led_on(i);
        p += p1;
        SPI_transfer(p1);
    }
    if (n_on > 0U) {
        SPI_endTransaction();
    }
    for (i = n_on; i < MAX_LED; ++i) {
        Led_off(i);
    }
    return p;
}
