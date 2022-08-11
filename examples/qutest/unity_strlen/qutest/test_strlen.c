/*****************************************************************************
* Purpose: example QUTEST fixture for the mock example from Unity
* Last updated for: @ref qpc_7_0_0
* Date of the Last Update:  2022-03-12
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
#include "my_strlen.h" /* CUT interface */

Q_DEFINE_THIS_FILE

/*--------------------------------------------------------------------------*/
static char string[128];

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    QF_init();  /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* dictionaries... */
    QS_FUN_DICTIONARY(&my_strlen);
    QS_OBJ_DICTIONARY(string);

    /* filter setup... */
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_setCurrObj(AP_OBJ, string);

    return QF_run(); /* run the tests */
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
    switch (cmdId) {
        case 0: { /* call the CUT: my_strlen(string) */
            uint16_t ret = my_strlen(string);
            QS_BEGIN_ID(QS_USER + cmdId, 0U) /* app-specific record */
                QS_FUN(&my_strlen); /* function called */
                QS_U16(0, ret); /* value returned */
                QS_STR(string); /* the current string */
            QS_END()
            break;
        }
        default:
            break;
    }

    /* unused parameters... */
    (void)param1;
    (void)param2;
    (void)param3;
}
/*..........................................................................*/
/* host callback function to "massage" the event, if necessary */
void QS_onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  /* is this test compiled for a desktop Host computer? */
#else /* this test is compiled for an embedded Target system */
#endif
}
/*..........................................................................*/
/*! callback function to output the posted QP events (not used here) */
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    (void)sender;
    (void)recipient;
    (void)e;
    (void)status;
}
