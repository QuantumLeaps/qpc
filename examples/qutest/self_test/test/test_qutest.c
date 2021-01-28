/*****************************************************************************
* Purpose: Fixture for QUTEST self-test
* Last Updated for Version: 6.9.2a
* Date of the Last Update:  2021-01-28
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h" /* for QUTEST */

Q_DEFINE_THIS_MODULE("test_qutest")

/*--------------------------------------------------------------------------*/
static uint8_t buffer[100];
static uint32_t myFun(void);

enum {
    FIXTURE_SETUP = QS_USER,
    FIXTURE_TEARDOWN,
    COMMAND_A,
    COMMAND_X,
    COMMAND_Y,
    COMMAND_Z,
    MY_RECORD,
};

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    QF_init();  /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* global filter */
    QS_GLB_FILTER(QS_ALL_RECORDS); /* enable all QS records */

    /* dictionaries... */
    QS_OBJ_DICTIONARY(buffer);
    QS_FUN_DICTIONARY(&myFun);

    QS_USR_DICTIONARY(FIXTURE_SETUP);
    QS_USR_DICTIONARY(FIXTURE_TEARDOWN);
    QS_USR_DICTIONARY(COMMAND_A);
    QS_USR_DICTIONARY(COMMAND_X);
    QS_USR_DICTIONARY(COMMAND_Y);
    QS_USR_DICTIONARY(COMMAND_Z);
    QS_USR_DICTIONARY(MY_RECORD);

    return QF_run(); /* run the tests */
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
    QS_BEGIN_ID(FIXTURE_SETUP, 0U)
    QS_END()
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
    QS_BEGIN_ID(FIXTURE_TEARDOWN, 0U)
    QS_END()
}

/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
        case COMMAND_A: {
            Q_ASSERT_ID(100, param1 != 0U);
            QS_BEGIN_ID(COMMAND_A, 0U) /* app-specific record */
                QS_U32(0, param1);
            QS_END()
            break;
        }
        case COMMAND_X: {
            uint32_t x = myFun();
            QS_BEGIN_ID(COMMAND_X, 0U) /* app-specific record */
                QS_U32(0, x);
                /* ... */
            QS_END()
            break;
        }
        case COMMAND_Y: {
            QS_BEGIN_ID(COMMAND_Y, 0U) /* application-specific record */
                QS_FUN(&myFun);
                QS_MEM(buffer, param1);
                QS_STR((char const *)&buffer[33]);
            QS_END()
            break;
        }
        case COMMAND_Z: {
            float32_t f32 = (float32_t)((int32_t)param2/(float32_t)param3);
            float64_t f64 = -6.02214076E23;
            QS_BEGIN_ID(COMMAND_Z, 0U) /* app-specific record */
                QS_F32(param1, f32);
                QS_F64(param1, f64);
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

