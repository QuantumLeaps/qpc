/*****************************************************************************
* Product: "Fly'n'Shoot" game, BSP for Qt (EK-LM3S811 simulation)
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jun 14, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include <QtGui>
#include "pixellabel.h"
#include "qp_app.h"
#include "gui.h"
//-----------------
extern "C" {

#include "qp_port.h"
#include "game.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

//............................................................................
static quint8 const c_onColor[] = { 255U, 255U,   0U };
static uint8_t l_ship_pos = GAME_SHIP_Y;

#ifdef Q_SPY
    enum QSUserRecords {
        PLAYER_TRIGGER = QS_USER
    };

    static uint8_t const l_clock_tick = 0U;
    static uint8_t const l_bsp = 0U;
#endif

//............................................................................
void QPApp::onClockTick(void) {                                        // slot
    static QEvt const tickEvt = { TIME_TICK_SIG, 0U, 0U };
    QF_TICK(&l_clock_tick);            // perform the QF clock tick processing
    QF_PUBLISH(&tickEvt, &l_clock_tick);             // publish the tick event
}
//............................................................................
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC);
    QS_OBJ_DICTIONARY(&l_clock_tick);
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void BSP_init() {
    Q_ALLEGE(QS_INIT((char *)0));
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_OBJ_DICTIONARY(&l_bsp);
    QS_USR_DICTIONARY(PLAYER_TRIGGER);
}
//............................................................................
void BSP_terminate(int16_t result) {
    (void)result;
    qDebug("terminate");
    QF_stop();                                     // stop the QF_run() thread
    qApp->quit();  // quit the Qt application *after* the QF_run() has stopped
}
//............................................................................
void BSP_drawBitmap(uint8_t const *bitmap) {
    PixelLabel *display = Gui::instance->m_display;
    for (unsigned y = 0U; y < BSP_SCREEN_HEIGHT; ++y) {
        for (unsigned x = 0U; x < BSP_SCREEN_WIDTH; ++x) {
            uint8_t bits = bitmap[x + (y/8)*BSP_SCREEN_WIDTH];
            if ((bits & (1U << (y & 0x07U))) != 0U) {
                display->setPixel(x, y, c_onColor);
            }
            else {
                display->clearPixel(x, y);
            }
        }
    }

    display->redraw();
}
//............................................................................
void BSP_drawNString(uint8_t x, uint8_t y, char const *str) {
    static uint8_t const font5x7[95][5] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00 },                            /* ' ' */
        { 0x00, 0x00, 0x4F, 0x00, 0x00 },                              /* ! */
        { 0x00, 0x07, 0x00, 0x07, 0x00 },                              /* " */
        { 0x14, 0x7F, 0x14, 0x7F, 0x14 },                              /* # */
        { 0x24, 0x2A, 0x7F, 0x2A, 0x12 },                              /* $ */
        { 0x23, 0x13, 0x08, 0x64, 0x62 },                              /* % */
        { 0x36, 0x49, 0x55, 0x22, 0x50 },                              /* & */
        { 0x00, 0x05, 0x03, 0x00, 0x00 },                              /* ' */
        { 0x00, 0x1C, 0x22, 0x41, 0x00 },                              /* ( */
        { 0x00, 0x41, 0x22, 0x1C, 0x00 },                              /* ) */
        { 0x14, 0x08, 0x3E, 0x08, 0x14 },                              /* * */
        { 0x08, 0x08, 0x3E, 0x08, 0x08 },                              /* + */
        { 0x00, 0x50, 0x30, 0x00, 0x00 },                              /* , */
        { 0x08, 0x08, 0x08, 0x08, 0x08 },                              /* - */
        { 0x00, 0x60, 0x60, 0x00, 0x00 },                              /* . */
        { 0x20, 0x10, 0x08, 0x04, 0x02 },                              /* / */
        { 0x3E, 0x51, 0x49, 0x45, 0x3E },                              /* 0 */
        { 0x00, 0x42, 0x7F, 0x40, 0x00 },                              /* 1 */
        { 0x42, 0x61, 0x51, 0x49, 0x46 },                              /* 2 */
        { 0x21, 0x41, 0x45, 0x4B, 0x31 },                              /* 3 */
        { 0x18, 0x14, 0x12, 0x7F, 0x10 },                              /* 4 */
        { 0x27, 0x45, 0x45, 0x45, 0x39 },                              /* 5 */
        { 0x3C, 0x4A, 0x49, 0x49, 0x30 },                              /* 6 */
        { 0x01, 0x71, 0x09, 0x05, 0x03 },                              /* 7 */
        { 0x36, 0x49, 0x49, 0x49, 0x36 },                              /* 8 */
        { 0x06, 0x49, 0x49, 0x29, 0x1E },                              /* 9 */
        { 0x00, 0x36, 0x36, 0x00, 0x00 },                              /* : */
        { 0x00, 0x56, 0x36, 0x00, 0x00 },                              /* ; */
        { 0x08, 0x14, 0x22, 0x41, 0x00 },                              /* < */
        { 0x14, 0x14, 0x14, 0x14, 0x14 },                              /* = */
        { 0x00, 0x41, 0x22, 0x14, 0x08 },                              /* > */
        { 0x02, 0x01, 0x51, 0x09, 0x06 },                              /* ? */
        { 0x32, 0x49, 0x79, 0x41, 0x3E },                              /* @ */
        { 0x7E, 0x11, 0x11, 0x11, 0x7E },                              /* A */
        { 0x7F, 0x49, 0x49, 0x49, 0x36 },                              /* B */
        { 0x3E, 0x41, 0x41, 0x41, 0x22 },                              /* C */
        { 0x7F, 0x41, 0x41, 0x22, 0x1C },                              /* D */
        { 0x7F, 0x49, 0x49, 0x49, 0x41 },                              /* E */
        { 0x7F, 0x09, 0x09, 0x09, 0x01 },                              /* F */
        { 0x3E, 0x41, 0x49, 0x49, 0x7A },                              /* G */
        { 0x7F, 0x08, 0x08, 0x08, 0x7F },                              /* H */
        { 0x00, 0x41, 0x7F, 0x41, 0x00 },                              /* I */
        { 0x20, 0x40, 0x41, 0x3F, 0x01 },                              /* J */
        { 0x7F, 0x08, 0x14, 0x22, 0x41 },                              /* K */
        { 0x7F, 0x40, 0x40, 0x40, 0x40 },                              /* L */
        { 0x7F, 0x02, 0x0C, 0x02, 0x7F },                              /* M */
        { 0x7F, 0x04, 0x08, 0x10, 0x7F },                              /* N */
        { 0x3E, 0x41, 0x41, 0x41, 0x3E },                              /* O */
        { 0x7F, 0x09, 0x09, 0x09, 0x06 },                              /* P */
        { 0x3E, 0x41, 0x51, 0x21, 0x5E },                              /* Q */
        { 0x7F, 0x09, 0x19, 0x29, 0x46 },                              /* R */
        { 0x46, 0x49, 0x49, 0x49, 0x31 },                              /* S */
        { 0x01, 0x01, 0x7F, 0x01, 0x01 },                              /* T */
        { 0x3F, 0x40, 0x40, 0x40, 0x3F },                              /* U */
        { 0x1F, 0x20, 0x40, 0x20, 0x1F },                              /* V */
        { 0x3F, 0x40, 0x38, 0x40, 0x3F },                              /* W */
        { 0x63, 0x14, 0x08, 0x14, 0x63 },                              /* X */
        { 0x07, 0x08, 0x70, 0x08, 0x07 },                              /* Y */
        { 0x61, 0x51, 0x49, 0x45, 0x43 },                              /* Z */
        { 0x00, 0x7F, 0x41, 0x41, 0x00 },                              /* [ */
        { 0x02, 0x04, 0x08, 0x10, 0x20 },                              /* \ */
        { 0x00, 0x41, 0x41, 0x7F, 0x00 },                              /* ] */
        { 0x04, 0x02, 0x01, 0x02, 0x04 },                              /* ^ */
        { 0x40, 0x40, 0x40, 0x40, 0x40 },                              /* _ */
        { 0x00, 0x01, 0x02, 0x04, 0x00 },                              /* ` */
        { 0x20, 0x54, 0x54, 0x54, 0x78 },                              /* a */
        { 0x7F, 0x48, 0x44, 0x44, 0x38 },                              /* b */
        { 0x38, 0x44, 0x44, 0x44, 0x20 },                              /* c */
        { 0x38, 0x44, 0x44, 0x48, 0x7F },                              /* d */
        { 0x38, 0x54, 0x54, 0x54, 0x18 },                              /* e */
        { 0x08, 0x7E, 0x09, 0x01, 0x02 },                              /* f */
        { 0x0C, 0x52, 0x52, 0x52, 0x3E },                              /* g */
        { 0x7F, 0x08, 0x04, 0x04, 0x78 },                              /* h */
        { 0x00, 0x44, 0x7D, 0x40, 0x00 },                              /* i */
        { 0x20, 0x40, 0x44, 0x3D, 0x00 },                              /* j */
        { 0x7F, 0x10, 0x28, 0x44, 0x00 },                              /* k */
        { 0x00, 0x41, 0x7F, 0x40, 0x00 },                              /* l */
        { 0x7C, 0x04, 0x18, 0x04, 0x78 },                              /* m */
        { 0x7C, 0x08, 0x04, 0x04, 0x78 },                              /* n */
        { 0x38, 0x44, 0x44, 0x44, 0x38 },                              /* o */
        { 0x7C, 0x14, 0x14, 0x14, 0x08 },                              /* p */
        { 0x08, 0x14, 0x14, 0x18, 0x7C },                              /* q */
        { 0x7C, 0x08, 0x04, 0x04, 0x08 },                              /* r */
        { 0x48, 0x54, 0x54, 0x54, 0x20 },                              /* s */
        { 0x04, 0x3F, 0x44, 0x40, 0x20 },                              /* t */
        { 0x3C, 0x40, 0x40, 0x20, 0x7C },                              /* u */
        { 0x1C, 0x20, 0x40, 0x20, 0x1C },                              /* v */
        { 0x3C, 0x40, 0x30, 0x40, 0x3C },                              /* w */
        { 0x44, 0x28, 0x10, 0x28, 0x44 },                              /* x */
        { 0x0C, 0x50, 0x50, 0x50, 0x3C },                              /* y */
        { 0x44, 0x64, 0x54, 0x4C, 0x44 },                              /* z */
        { 0x00, 0x08, 0x36, 0x41, 0x00 },                              /* { */
        { 0x00, 0x00, 0x7F, 0x00, 0x00 },                              /* | */
        { 0x00, 0x41, 0x36, 0x08, 0x00 },                              /* } */
        { 0x02, 0x01, 0x02, 0x04, 0x02 },                              /* ~ */
    };

    PixelLabel *display = Gui::instance->m_display;
    while (*str != '\0') {
        uint8_t const *ch = &font5x7[*str - ' '][0];
        for (int dx = 0; dx < 5; ++dx) {
            for (int dy = 0; dy < 8; ++dy) {
                if ((ch[dx] & (1U << dy)) != 0U) {
                    display->setPixel(x + dx, y*8 + dy, c_onColor);
                }
                else {
                    display->clearPixel(x + dx, y*8 + dy);
                }
            }
        }
        ++str;
        x += 6;
    }
    display->redraw();
}
//............................................................................
void BSP_updateScore(uint16_t score) {
    Gui::instance->m_score->display((int)score);
}
//............................................................................
void BSP_displayOn(void) {
    Gui::instance->m_LED->setPixmap(QPixmap(":/res/LED_OFF.png"));
}
//............................................................................
void BSP_displayOff(void) {
    Gui::instance->m_display->clear();
    Gui::instance->m_display->redraw();
    Gui::instance->m_LED->setPixmap(QPixmap(":/res/LED_ON.png"));
}
//............................................................................
void BSP_moveShipUp(void) {
    if (l_ship_pos > 0U) {
        --l_ship_pos;
    }
    QACTIVE_POST(AO_Ship,
                 (QEvt *)Q_NEW(ObjectPosEvt, PLAYER_SHIP_MOVE_SIG,
                               (uint8_t)GAME_SHIP_X, (uint8_t)l_ship_pos),
                 &l_bsp);
}
//............................................................................
void BSP_moveShipDown(void) {
    if (l_ship_pos < (GAME_SCREEN_HEIGHT - 3U)) {
        ++l_ship_pos;
    }
    QACTIVE_POST(AO_Ship,
                 (QEvt *)Q_NEW(ObjectPosEvt, PLAYER_SHIP_MOVE_SIG,
                               (uint8_t)GAME_SHIP_X, (uint8_t)l_ship_pos),
                 &l_bsp);
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

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#include "qspy.h"

static QTime l_time;

//............................................................................
static int custParserFun(QSpyRecord * const qrec) {
    int ret = 0;                     /* don't perform standard QSPY parsing */
    switch (qrec->rec) {
        case QS_QF_MPOOL_GET: {                  /* example record to parse */
            int nFree;
            (void)QSpyRecord_getUint32(qrec, QS_TIME_SIZE);
            (void)QSpyRecord_getUint64(qrec, QS_OBJ_PTR_SIZE);
            nFree = (int)QSpyRecord_getUint32(qrec, QF_MPOOL_CTR_SIZE);
            (void)QSpyRecord_getUint32(qrec, QF_MPOOL_CTR_SIZE);    /* nMin */
            if (QSpyRecord_OK(qrec)) {
                Gui::instance->m_epoolLabel->setText(QString::number(nFree));
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
    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    //QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
    //QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
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

}                                                                // extern "C"



