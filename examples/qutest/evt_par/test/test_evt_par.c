/*****************************************************************************
* Purpose: example event QUTEST fixture
* Last Updated for Version: 6.3.6
* Date of the Last Update:  2018-10-04
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
#include "qpc.h"    /* QUTest interface */
#include "my_app.h" /* My Application */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
int main(int argc, char *argv[]) {
    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(MyEvt3) smlPoolSto[10];
    static QEvt const *myAoQueueSto[10];

    QF_init();   /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(AO_MyAO);

    QS_TEST_PAUSE();

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start active objects... */
    MyAO_ctor(); /* instantiate the MyAO active object */
    QACTIVE_START(AO_MyAO,              /* AO to start */
                  (uint_fast8_t)1,      /* QP priority of the AO */
                  myAoQueueSto,         /* event queue storage */
                  Q_DIM(myAoQueueSto),  /* queue length [events] */
                  (void *)0,            /* stack storage (not used) */
                  0U,                   /* size of the stack [bytes] */
                  (QEvt *)0);           /* initialization event */

    return QF_run(); /* run the QF application */
}

/*--------------------------------------------------------------------------*/
enum AppSpecRecords {
    MY_EVT0 = QS_USER,
    MY_EVT1,
    MY_EVT2,
    MY_EVT3
};

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
    QS_USR_DICTIONARY(MY_EVT0);
    QS_USR_DICTIONARY(MY_EVT1);
    QS_USR_DICTIONARY(MY_EVT2);
    QS_USR_DICTIONARY(MY_EVT3);
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
}

/*..........................................................................*/
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param1; /* unused parameter */
    (void)param2; /* unused parameter */
    (void)param3; /* unused parameter */

    switch (cmdId) {
        case 0: {
            break;
        }
        default:
            break;
    }
}
/*..........................................................................*/
/*! callback function to "massage" the injected QP events (not used here) */
void QS_onTestEvt(QEvt *e) {

    switch (e->sig) {
        case MY_EVT0_SIG:
            QS_BEGIN_ID(MY_EVT0, 0U) /* app-specific record */
            QS_END()
            break;
        case MY_EVT1_SIG:
            QS_BEGIN_ID(MY_EVT1, 0U) /* app-specific record */
                QS_U32(0, Q_EVT_CAST(MyEvt1)->u32);
            QS_END()
            break;
        case MY_EVT2_SIG:
            QS_BEGIN_ID(MY_EVT2, 0U) /* app-specific record */
                QS_U32(0, Q_EVT_CAST(MyEvt2)->u32);
                QS_U32(0, Q_EVT_CAST(MyEvt2)->u16);
            QS_END()
            break;
        case MY_EVT3_SIG:
            QS_BEGIN_ID(MY_EVT3, 0U) /* app-specific record */
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u32);
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u16);
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u8);
            QS_END()
            break;
    }

#ifdef Q_HOST  /* is this test compiled for a desktop Host computer? */
#else /* embedded Target */
#endif /* embedded Target */
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
