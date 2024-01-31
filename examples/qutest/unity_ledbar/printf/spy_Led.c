#include <stdint.h>
#include <stdio.h> // for printf()

#include "Led.h"  // original interface

static uint32_t led_power[MAX_LED] = {
    10, 20, 10, 20, 10
};

//----------------------------------------------------------------------------
// turns a given LED off
void Led_off(uint8_t index) {
    printf("Led_off %d\n", index);
}

// turns a given LED on and returns the power drawn by it in [uW]
uint32_t Led_on(uint8_t index) {
    uint32_t ret = led_power[index];
    printf("Led_on %d %d\n", ret, index);
    return ret;
}

