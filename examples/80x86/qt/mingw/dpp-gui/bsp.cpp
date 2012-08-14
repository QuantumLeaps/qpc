//////////////////////////////////////////////////////////////////////////////
// Product: BSP for DPP example
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Jul 06, 2012
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include <QtGui>
#include "qp_app.h"
#include "gui.h"
//-----------------
extern "C" {

#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };
    static uint8_t const l_time_tick = 0U;                           // for QS
#endif

Q_DEFINE_THIS_FILE

//............................................................................
static uint32_t l_rnd;                                          // random seed

//............................................................................
void QPApp::onClockTick(void) {                                        // slot
    QF_TICK(&l_time_tick);
}
//............................................................................
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC);
    QS_OBJ_DICTIONARY(&l_time_tick);
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void BSP_init(void) {
    Q_ALLEGE(QS_INIT((char *)0));
    QS_OBJ_DICTIONARY(&l_time_tick);
    QS_USR_DICTIONARY(PHILO_STAT);

    BSP_randomSeed(1234U);
}
//............................................................................
void BSP_terminate(int result) {
    qDebug("terminate");
    QF_stop();                                     // stop the QF_run() thread
    qApp->quit();  // quit the Qt application *after* the QF_run() has stopped
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {

    static QLabel * const philoLabel[] = {
        Gui::instance()->m_philoLabel_0,
        Gui::instance()->m_philoLabel_1,
        Gui::instance()->m_philoLabel_2,
        Gui::instance()->m_philoLabel_3,
        Gui::instance()->m_philoLabel_4
    };
    static QPixmap thinking(":/res/thinking.png");
    static QPixmap hungry  (":/res/hungry.png");
    static QPixmap eating  (":/res/eating.png");

    Q_REQUIRE(n < Q_DIM(philoLabel));

    switch (stat[0]) {
    case 't':
        philoLabel[n]->setPixmap(thinking);
        break;
    case 'h':
        philoLabel[n]->setPixmap(hungry);
        break;
    case 'e':
        philoLabel[n]->setPixmap(eating);
        break;
    }

    qDebug("philo[%d] is %s", n, stat);
}
//............................................................................
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        qDebug("PAUSED");
        Gui::instance()->m_pauseButton->setText("PAUSED");
    }
    else {
        qDebug("SERVING");
        Gui::instance()->m_pauseButton->setText("SERVING");
    }
}
//............................................................................
uint32_t BSP_random(void) {     // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    l_rnd = l_rnd * (3*7*11*13*23);
    return l_rnd >> 8;
}
//............................................................................
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
//............................................................................
void Q_onAssert(char_t const * const file, int line) {
    QMessageBox::critical(0, "PROBLEM",
        QString("<p>Assertion failed in module <b>%1</b>,"
                "line <b>%2</b></p>")
            .arg(file)
            .arg(line));
    QS_ASSERTION(file, line);       // send the assertion info to the QS trace
    qFatal("Assertion failed in module %s, line %d", file, line);
}

//////////////////////////////////////////////////////////////////////////////
#ifdef Q_SPY

#include "qspy.h"

static QTime l_time;

//............................................................................
static int custParserFun(QSpyRecord * const qrec) {
    int ret = 1;                           /* perform standard QSPY parsing */
    switch (qrec->rec) {
        case QS_QF_MPOOL_GET: {                  /* example record to parse */
            int nFree;
            (void)QSpyRecord_getUint32(qrec, QS_TIME_SIZE);
            (void)QSpyRecord_getUint64(qrec, QS_OBJ_PTR_SIZE);
            nFree = (int)QSpyRecord_getUint32(qrec, QF_MPOOL_CTR_SIZE);
            (void)QSpyRecord_getUint32(qrec, QF_MPOOL_CTR_SIZE);    /* nMin */
            if (QSpyRecord_OK(qrec)) {
                Gui::instance()->m_epoolLabel->setText(QString::number(nFree));
                ret = 0;             /* don't perform standard QSPY parsing */
            }
            break;
        }
    }
    return ret;
}
//............................................................................
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[4*1024];                 // 4K buffer for Quantum Spy
    QS_initBuf(qsBuf, sizeof(qsBuf));

    QSPY_config((QP_VERSION >> 8),  // version
                QS_OBJ_PTR_SIZE,    // objPtrSize
                QS_FUN_PTR_SIZE,    // funPtrSize
                QS_TIME_SIZE,       // tstampSize
                Q_SIGNAL_SIZE,      // sigSize,
                QF_EVENT_SIZ_SIZE,  // evtSize
                QF_EQUEUE_CTR_SIZE, // queueCtrSize
                QF_MPOOL_CTR_SIZE,  // poolCtrSize
                QF_MPOOL_SIZ_SIZE,  // poolBlkSize
                QF_TIMEEVT_CTR_SIZE,// tevtCtrSize
                (void *)0,          // matFile,
                (void *)0,
                &custParserFun);    // customized parser function

    l_time.start();                 // start the time stamp

    // setup the QS filters...
    //QS_FILTER_OFF(QS_ALL_RECORDS);
    QS_FILTER_ON(QS_ALL_RECORDS);

    //QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
    //QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
    //QS_FILTER_OFF(QS_QEP_STATE_EXIT);
    //QS_FILTER_OFF(QS_QEP_STATE_INIT);
    //QS_FILTER_OFF(QS_QEP_INIT_TRAN);
    //QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
    //QS_FILTER_OFF(QS_QEP_TRAN);
    //QS_FILTER_OFF(QS_QEP_IGNORED);
    //QS_FILTER_OFF(QS_QEP_UNHANDLED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
    //QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    //QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
    //QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    //QS_FILTER_OFF(QS_QF_MPOOL_INIT);
    //QS_FILTER_OFF(QS_QF_MPOOL_GET);
    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
    //QS_FILTER_OFF(QS_QF_PUBLISH);
    QS_FILTER_OFF(QS_QF_NEW);
    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
    QS_FILTER_OFF(QS_QF_GC);
    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return (uint8_t)1;                                              // success
}
//............................................................................
void QS_onCleanup(void) {
    QSPY_stop();
}
//............................................................................
void QS_onFlush(void) {
    uint16_t nBytes = 1024;
    uint8_t const *block;
    QF_CRIT_ENTRY(dummy);
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QF_CRIT_EXIT(dummy);
        QSPY_parse(block, nBytes);
        nBytes = 1024;
    }
}
//............................................................................
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)l_time.elapsed();
}

//............................................................................
void QS_onEvent(void) {
    uint16_t nBytes = 1024;
    uint8_t const *block;
    QF_CRIT_ENTRY(dummy);
    if ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QF_CRIT_EXIT(dummy);
        QSPY_parse(block, nBytes);
    }
}
//............................................................................
void QSPY_onPrintLn(void) {
    qDebug(QSPY_line);
}

#endif                                                                // Q_SPY

}
