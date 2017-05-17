/*****************************************************************************
* Product: QUTEST fixture for the DPP components
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
#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

//#include <stdio.h>

/*..........................................................................*/
void QS_onTestSetup(void) {
    //printf("QS_onTestSetup\n");
}
void QS_onTestTeardown(void) {
    //printf("QS_onTestTeardown\n");
}

/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
       case 0U: {
           QEvt const e = { PAUSE_SIG, 0U, 0U };
           QHSM_DISPATCH(&AO_Table->super, &e);
           break;
       }
       case 1U: {
           QEvt const e = { SERVE_SIG, 0U, 0U };
           QHSM_DISPATCH(&AO_Table->super, &e);
           break;
       }
       default:
           break;
    }
}

/****************************************************************************/
#ifdef Q_HOST  /* is this test compiled for a desktop Host computer? */

    /*! Host callback function to "massage" the event, if necessary */
    void QS_onTestEvt(QEvt *e) {
        (void)e;
    }

#else /* embedded Target */

    /*! Target callback function to "massage" the event, if necessary */
    void QS_onTestEvt(QEvt *e) {
        (void)e;
    }

#endif /* embedded Target */
