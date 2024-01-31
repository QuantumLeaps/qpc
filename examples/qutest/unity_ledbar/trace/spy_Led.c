#include "qpc.h"  // QP/Spy
#include "Led.h"  // original interface

static uint32_t led_power[MAX_LED] = {
    10, 20, 10, 20, 10
};

//----------------------------------------------------------------------------
enum {
    LED_MOD = QS_USER1 // QS app-specific record for the LED module
};

void Led_DICTIONARY(void) {
    QS_FUN_DICTIONARY(&Led_on);
    QS_FUN_DICTIONARY(&Led_off);
    QS_USR_DICTIONARY(LED_MOD);
    QS_OBJ_DICTIONARY(&led_power);
}

//----------------------------------------------------------------------------
// turns a given LED off
void Led_off(uint8_t index) {
    //printf("Led_off %d\n", index);
    QS_BEGIN_ID(LED_MOD, 0U) // app-specific record
        QS_FUN(&Led_off); // function called
        QS_U8 (0, index); // parameter
    QS_END()
}

// turns a given LED on and returns the power drawn by it in [uW]
uint32_t Led_on(uint8_t index) {
    uint32_t ret = led_power[index]; // assume
    //printf("Led_on %d %d\n", ret, index);
    QS_BEGIN_ID(LED_MOD, 0U) // app-specific record
        QS_FUN(&Led_on);  // function called
        QS_U32(0, ret);   // value returned
        QS_U8 (0, index); // parameter
    QS_END()

    return ret;
}

