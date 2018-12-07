/*****************************************************************************
* Purpose: Fixture for QUTEST self-test
* Last Updated for Version: 6.3.5
* Date of the Last Update:  2018-09-22
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h" /* for QUTEST */

Q_DEFINE_THIS_FILE

/*--------------------------------------------------------------------------*/
static uint8_t buffer[100];
static uint32_t myFun(void);

enum {
    FIXTURE_SETUP = QS_USER,
    FIXTURE_TEARDOWN,
    COMMAND_X,
    MY_RECORD,
};

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    QF_init();  /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* global filter */
    QS_FILTER_ON(QS_ALL_RECORDS); /* enable all QS records */

    /* dictionaries... */
    QS_OBJ_DICTIONARY(buffer);
    QS_FUN_DICTIONARY(&myFun);

    QS_USR_DICTIONARY(FIXTURE_SETUP);
    QS_USR_DICTIONARY(FIXTURE_TEARDOWN);
    QS_USR_DICTIONARY(COMMAND_X);
    QS_USR_DICTIONARY(MY_RECORD);

    return QF_run(); /* run the tests */
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
    QS_BEGIN(FIXTURE_SETUP, (void *)0)
    QS_END()
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
    QS_BEGIN(FIXTURE_TEARDOWN, (void *)0)
    QS_END()
}

/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,   uint32_t param1,
                  uint32_t param2, uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
        case COMMAND_X: {
            uint32_t x = myFun();
            QS_BEGIN(COMMAND_X, (void *)0) /* application-specific record */
                QS_U32(0, x);
                /* ... */
            QS_END()
            break;
        }
        default:
            break;
    }
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

/*--------------------------------------------------------------------------*/
static uint32_t myFun(void) {
    QS_TEST_PROBE_DEF(&myFun)
    QS_TEST_PROBE(
        return qs_tp_;
    )
    return 0;
}

