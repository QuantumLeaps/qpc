/*****************************************************************************
* Purpose: example 'FlashDriver' QUTEST fixture (Chapter 10 from TDDfEC)
* Last Updated for Version: 5.9.0
* Date of the Last Update:  2017-05-15
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"           /* for QUTEST */
#include "Flash.h"         /* CUT */
#include "FakeMicroTime.h" /* test double */

Q_DEFINE_THIS_FILE

/* mock initialization -----------------------------------------------------*/
enum {
    FLASH_WRITE = QS_USER0,
    FAKE_MICROTIME_INIT,
};

void MockIO_Init(void);

/*--------------------------------------------------------------------------*/
int main() {
    QF_init();  /* initialize the framework and the underlying RT kernel */
    Q_ALLEGE(QS_INIT((void *)0)); /* initialize QS tracing system */

    /* filter setup */
    QS_FILTER_ON(QS_UA_RECORDS); /* UA==user-all */

    /* dictionaries... */
    QS_USR_DICTIONARY(FLASH_WRITE);
    QS_USR_DICTIONARY(FAKE_MICROTIME_INIT);

    QS_FUN_DICTIONARY(&Flash_Create);
    QS_FUN_DICTIONARY(&Flash_Destroy);
    QS_FUN_DICTIONARY(&Flash_Write);

    /* initialize the mocks... */
    MockIO_Init();

    return QF_run(); /* run the tests */
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
    Flash_Create();
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
    Flash_Destroy();
}
/*..........................................................................*/
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param2;
    (void)param3;

    //printf("<TARGET> Command Id=%d,Par1=%ld,Par2=%ld,Par3=%ld\n",
    //       (int)cmdId, (long)param1, (long)param2, (long)param3);
    switch (cmdId) {
        case FLASH_WRITE: {
            int ret = Flash_Write((ioAddress)param1, (ioData)param2);
            QS_BEGIN(FLASH_WRITE, (void *)0) /* user-specific record */
               QS_I32(0, ret);
            QS_END()
            break;
        }
        case FAKE_MICROTIME_INIT: {
            FakeMicroTime_Init(param1, param2);
            QS_BEGIN(FAKE_MICROTIME_INIT, (void *)0) /* user-specific record */
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
