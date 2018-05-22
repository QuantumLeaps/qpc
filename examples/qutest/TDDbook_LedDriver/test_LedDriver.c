/*****************************************************************************
* Purpose: example 'LedDriver' QUTEST fixture (Chapters 3&4 from TDDfEC)
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-05-20
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"           /* for QUTEST */
#include "LedDriver.h"     /* CUT */

Q_DEFINE_THIS_FILE

/*--------------------------------------------------------------------------*/
static uint16_t virtualLeds;

enum {
    LEDDRIVER_CREATE  = QS_USER,
    LEDDRIVER_DESTROY,
    LEDDRIVER_TURNON,
    LEDDRIVER_TURNOFF,
    LEDDRIVER_TURNALLON,
    LEDDRIVER_TURNALLOFF,
    LEDDRIVER_ISON,
    LEDDRIVER_ISOFF,
};

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    QF_init();  /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* filter setup */
    QS_FILTER_ON(QS_UA_RECORDS); /* UA==user-all */

    /* dictionaries... */
    QS_OBJ_DICTIONARY(&virtualLeds);
    QS_USR_DICTIONARY(LEDDRIVER_CREATE);
    QS_USR_DICTIONARY(LEDDRIVER_DESTROY);
    QS_USR_DICTIONARY(LEDDRIVER_TURNON);
    QS_USR_DICTIONARY(LEDDRIVER_TURNOFF);
    QS_USR_DICTIONARY(LEDDRIVER_TURNALLON);
    QS_USR_DICTIONARY(LEDDRIVER_TURNALLOFF);
    QS_USR_DICTIONARY(LEDDRIVER_ISON);
    QS_USR_DICTIONARY(LEDDRIVER_ISOFF);

    return QF_run(); /* run the tests */
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
    LedDriver_Create(&virtualLeds);
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
    LedDriver_Destroy();
}
/*..........................................................................*/
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param2;
    (void)param3;
    BOOL ret;

    //printf("<TARGET> Command Id=%d,Par1=%ld,Par2=%ld,Par3=%ld\n",
    //       (int)cmdId, (long)param1, (long)param2, (long)param3);
    switch (cmdId) {
        case LEDDRIVER_CREATE: {
            LedDriver_Create(&virtualLeds);
            QS_BEGIN(LEDDRIVER_CREATE, (void *)0) /* user-specific record */
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_DESTROY: {
            LedDriver_Destroy();
            QS_BEGIN(LEDDRIVER_DESTROY, (void *)0) /* user-specific record */
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_TURNON: {
            LedDriver_TurnOn((int)param1);
            QS_BEGIN(LEDDRIVER_TURNON, (void *)0) /* user-specific record */
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_TURNOFF: {
            LedDriver_TurnOff((int)param1);
            QS_BEGIN(LEDDRIVER_TURNOFF, (void *)0)
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_TURNALLON: {
            LedDriver_TurnAllOn();
            QS_BEGIN(LEDDRIVER_TURNALLON, (void *)0)
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_TURNALLOFF: {
            LedDriver_TurnAllOff();
            QS_BEGIN(LEDDRIVER_TURNALLOFF, (void *)0)
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_ISON: {
            ret = LedDriver_IsOn((int)param1);
            QS_BEGIN(LEDDRIVER_ISON, (void *)0)
               QS_U8(1, ret);
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        case LEDDRIVER_ISOFF: {
            ret = LedDriver_IsOff((int)param1);
            QS_BEGIN(LEDDRIVER_ISOFF, (void *)0)
               QS_U8(1, ret);
               QS_U32_HEX(4, (uint32_t)virtualLeds);
            QS_END()
            break;
        }
        default:
            break;
    }
}
/*..........................................................................*/
/*! callback function to "massage" the injected QP events (not used here) */
void QS_onTestEvt(QEvt *e) {
    (void)e;
}
