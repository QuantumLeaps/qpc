/*****************************************************************************
* Purpose: example QUTEST "spy" test double implementation
* Last Updated for Version: 6.3.6
* Date of the Last Update:  2018-10-11
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"  /* QUTest interface */
#include "Led.h"  /* original interface */

//Q_DEFINE_THIS_FILE

enum {
    LED_MOD = QS_USER1 /* QS app-specific record for the LED module */
};

static uint32_t led_power[MAX_LED] = {
    10, 20, 10, 20, 10
};

/*--------------------------------------------------------------------------*/
void Led_DICTIONARY(void) {
    QS_FUN_DICTIONARY(&Led_on);
    QS_FUN_DICTIONARY(&Led_off);
    QS_USR_DICTIONARY(LED_MOD);
    QS_OBJ_DICTIONARY(&led_power);
}

/*--------------------------------------------------------------------------*/
/* turns a given LED off */
void Led_off(uint8_t index) {
    QS_BEGIN_ID(LED_MOD, 0U) /* app-specific record */
        QS_FUN(&Led_off); /* function called */
        QS_U8 (0, index); /* parameter */
    QS_END()
}

/* turns a given LED on and retruns the power drawn by it in uW */
uint32_t Led_on(uint8_t index) {
    QS_TEST_PROBE_DEF(&Led_on)
    uint32_t ret = led_power[index]; /* assume  */

    /* tweak the returned power draw from the test probe */
    QS_TEST_PROBE(
        ret = (uint32_t)qs_tp_;
    )

    QS_BEGIN_ID(LED_MOD, 0U) /* app-specific record */
        QS_FUN(&Led_on);  /* function called */
        QS_U32(0, ret);   /* value returned */
        QS_U8 (0, index); /* parameter */
    QS_END()

    return ret;
}

