//////////////////////////////////////////////////////////////////////////////
// Product: QF/C port to Qt-1T (single thread)
// Last Updated for Version: 4.5.01
// Date of the Last Update:  Jun 14, 2012
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
#include <QTimer>
#include <QTime>
#include "qp_app.h"
//-----------------
extern "C" {
    #include "qf_pkg.h"
    #include "qassert.h"
}

Q_DEFINE_THIS_MODULE("qf_port")

static QEvent::Type qp_event_type = QEvent::MaxUser;
static QTimer       *qp_tick_timer;


//////////////////////////////////////////////////////////////////////////////
class QP_Event : public QEvent {
public:
    QP_Event(QEvt const *e, QActive *a)
      : QEvent(qp_event_type),
        m_qpe(e),
        m_act(a)
    {}

private:
    QEvt const *m_qpe;                                     // QP event pointer
    QActive    *m_act;             // QP active object pointer (the recipient)

    friend class QPApp;
};

//............................................................................
QPApp::QPApp(int &argc, char **argv)
  : QApplication(argc, argv)
{
    qp_event_type =
        static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::MaxUser));
}
//............................................................................
bool QPApp::event(QEvent *e) {
    bool ret;
    if (e->type() == qp_event_type) {

        QEvt const *qpe = (static_cast<QP_Event *>(e))->m_qpe;
        QF_ACTIVE_DISPATCH_(&(static_cast<QP_Event *>(e))->m_act->super, qpe);
        QF_gc(qpe);      // determine if event is garbage and collect it if so
#ifdef Q_SPY
        QS_onEvent();
#endif
        ret = true;                            // event recognized and handled
    }
    else {
        ret = QApplication::event(e);            // delegate to the superclass
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////
extern "C" {

static int qp_tick_interval_ms = 10;             // default 10ms tick interval

//............................................................................
void QF_init(void) {
}
//............................................................................
int16_t QF_run(void) {
    QF_onStartup();                             // invoke the startup callback
    qp_tick_timer = new QTimer(qApp);
    QObject::connect(qp_tick_timer, SIGNAL(timeout()),
                     qApp,          SLOT(onClockTick()));
    qp_tick_timer->setSingleShot(false);                      // periodic timer
    qp_tick_timer->setInterval(qp_tick_interval_ms);   // set system clock tick
    qp_tick_timer->start();

    return static_cast<int16_t>(qApp->exec());        // run the Qt event loop
}
//............................................................................
void QF_stop(void) {
    qp_tick_timer->stop();
    delete qp_tick_timer;
}
//............................................................................
void QF_setTickRate(uint32_t ticksPerSec) {
    qp_tick_interval_ms = 1000 / ticksPerSec;           // tick interval in ms
}
//............................................................................
void QActive_start(QActive * const me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    Q_REQUIRE((qSto == static_cast<QEvt const **>(0))     /* no event queue */
              && (qLen == 0U)                             /* no event queue */
              && (stkSto == static_cast<void *>(0))    /* no per-task stack */
              && (stkSize == 0U));                        // no per-task stack

    me->prio = prio;
    QF_add_(me);                        // make QF aware of this active object
    QF_ACTIVE_INIT_(&me->super, ie);         // execute the initial transition

    QS_FLUSH();                          // flush the trace buffer to the host
}
//............................................................................
void QActive_stop(QActive * const me) {
    QF_remove_(me);
}
//............................................................................
#ifndef Q_SPY
void QActive_postFIFO(QActive * const me, QEvt const * const e)
#else
void QActive_postFIFO(QActive * const me, QEvt const * const e,
                      void const * const sender)
#endif
{
    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_aoObj_, me)
        QS_TIME_();                                               // timestamp
        QS_OBJ_(sender);                                  // the sender object
        QS_SIG_(e->sig);                            // the signal of the event
        QS_OBJ_(me);                                     // this active object
        QS_U8_(QF_EVT_POOL_ID_(e));                // the pool Id of the event
        QS_U8_(QF_EVT_REF_CTR_(e));              // the ref count of the event
        QS_EQC_(0);                       // number of free entries (not used)
        QS_EQC_(0);                   // min number of free entries (not used)
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != static_cast<uint8_t>(0)) {     // dynamic event?
        QF_EVT_REF_CTR_INC_(e);             // increment the reference counter
    }
    QCoreApplication::postEvent(qApp, new QP_Event(e, me), me->prio);
}
//............................................................................
void QActive_postLIFO(QActive * const me, QEvt const * const e) {

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_aoObj_, me)
        QS_TIME_();                                               // timestamp
        QS_SIG_(e->sig);                           // the signal of this event
        QS_OBJ_(me);                                     // this active object
        QS_U8_(QF_EVT_POOL_ID_(e));                // the pool Id of the event
        QS_U8_(QF_EVT_REF_CTR_(e));              // the ref count of the event
        QS_EQC_(0);                       // number of free entries (not used)
        QS_EQC_(0);                   // min number of free entries (not used)
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != static_cast<uint8_t>(0)) {     // dynamic event?
        QF_EVT_REF_CTR_INC_(e);             // increment the reference counter
    }
    QCoreApplication::postEvent(qApp, new QP_Event(e, me),
                                QF_MAX_ACTIVE + 1);
}

}                                                                // extern "C"
