/*****************************************************************************
* Purpose: Fixture for QUTEST self-test
* Last Updated for Version: 7.2.0
* Date of the Last Update:  2022-12-07
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

enum UsrEnum {
    FIXTURE_SETUP = QS_USER,
    FIXTURE_TEARDOWN,
    COMMAND,
};

enum CmdEnum {
    CMD_A,
    CMD_B,
    CMD_C,
    CMD_X,
    CMD_Y,
    CMD_Z
};

enum UsrSig {
    MYSIG_SIG = Q_USER_SIG,
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
    QS_SIG_DICTIONARY(MYSIG_SIG, (void *)0);

    QS_USR_DICTIONARY(FIXTURE_SETUP);
    QS_USR_DICTIONARY(FIXTURE_TEARDOWN);
    QS_USR_DICTIONARY(COMMAND);

    QS_ENUM_DICTIONARY(CMD_A, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_B, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_C, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_X, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_Y, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_Z, QS_CMD);

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
    switch (cmdId) {
        case CMD_A: {
            Q_ASSERT_ID(100, param1 != 0U);
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, param1);
            QS_END()
            break;
        }
        case CMD_B: {
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_ENUM(QS_CMD, cmdId);
                QS_U8(0, param1);
                QS_STR("BAR");
                QS_U16(0, param2);
                QS_STR("FOO");
                QS_U32(0, param3);
                QS_F64(param1, -6.02214076E23);
            QS_END()
            break;
        }
        case CMD_C: {
            /* all supported data elements */
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_ENUM(QS_CMD, cmdId);
                QS_I8(10,  param1);
                QS_U8(10,  param1);
                QS_I16(10, param2);
                QS_U16(10, param2);
                QS_I32(10, param3);
                QS_U32(10, param3);
            QS_END()
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_I64(10, param3);
                QS_U64(10, param3);
                QS_F32(param1, -6.02214076E23);
                QS_F64(param1, -6.02214076E23);
            QS_END()
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_OBJ(buffer);
                QS_FUN(&myFun);
                QS_SIG(MYSIG_SIG, (void *)0);
                QS_MEM(&buffer[6], 16);
            QS_END()
            break;
        }
        case CMD_X: {
            uint32_t x = myFun();
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, x);
                /* ... */
            QS_END()
            break;
        }
        case CMD_Y: {
            QS_BEGIN_ID(COMMAND, 0U) /* application-specific record */
                QS_ENUM(QS_CMD, cmdId);
                QS_FUN(&myFun);
                QS_MEM(buffer, param1);
                QS_STR((char const *)&buffer[33]);
            QS_END()
            break;
        }
        case CMD_Z: {
            float32_t f32 = (float32_t)((int32_t)param2/(float32_t)param3);
            float64_t f64 = -6.02214076E23;
            QS_BEGIN_ID(COMMAND, 0U) /* app-specific record */
                QS_ENUM(QS_CMD, cmdId);
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
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(recipient);
    Q_UNUSED_PAR(e);
    Q_UNUSED_PAR(status);
}

/*--------------------------------------------------------------------------*/
static uint32_t myFun(void) {
    QS_TEST_PROBE_DEF(&myFun)
    QS_TEST_PROBE(
        return qs_tp_;
    )
    return 0;
}

