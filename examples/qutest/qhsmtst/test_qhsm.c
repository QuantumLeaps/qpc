/*****************************************************************************
* Purpose: Fixture for QUTEST
* Last Updated for Version: 5.9.0
* Date of the Last Update:  2017-05-15
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2005-2017 Quantum Leaps, LLC. All rights reserved.
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
#include "qhsmtst.h"

Q_DEFINE_THIS_FILE

enum {
    BSP_DISPLAY = QS_USER,
};

/*--------------------------------------------------------------------------*/
int main() {
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; /* small pool */

    QHsmTst_ctor();   /* instantiate the QHsmTst object */

    QF_init();    /* initialize the framework and the underlying RT kernel */

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    Q_ALLEGE(QS_INIT((void *)0)); /* initialize the QS software tracing */

    /* dictionaries... */
    QS_OBJ_DICTIONARY(the_hsm);
    QS_USR_DICTIONARY(BSP_DISPLAY);

    return QF_run();
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
}
/*..........................................................................*/
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;

    //printf("<TARGET> Command id=%d param=%d\n", (int)cmdId, (int)param);
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }
}

/*..........................................................................*/
#ifdef Q_HOST  /* is this test compiled for a desktop Host computer? */

/*! host callback function to "massage" the event, if necessary */
void QS_onTestEvt(QEvt *e) {
    (void)e;
}

#else /* this test is compiled for an embedded Target system */

void QS_onTestEvt(QEvt *e) {
    (void)e;
}

#endif

/*--------------------------------------------------------------------------*/
void BSP_display(char const *msg) {
    QS_BEGIN(BSP_DISPLAY, (void *)0) /* application-specific record */
        QS_STR(msg);
    QS_END()
}

/*..........................................................................*/
void BSP_exit(void) {
}

