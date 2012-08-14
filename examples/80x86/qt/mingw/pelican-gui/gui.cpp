//////////////////////////////////////////////////////////////////////////////
// Product: QP/C, PELICAN example
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Jul 10, 2012
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
#include "gui.h"
//-----------------
extern "C" {
    #include "qp_port.h"
    #include "pelican.h"
#include "bsp.h"
}

Q_DEFINE_THIS_FILE

//............................................................................
static Gui *l_instance;

//............................................................................
Gui::Gui(QWidget *parent)
    : QDialog(parent)
{
    l_instance = this;   // initialize the instance (Singleton)
    setupUi(this);
    setWindowTitle(tr("PELICAN crossing"));
}
//............................................................................
Gui *Gui::instance() {                                               // static
    return l_instance;
}
//............................................................................
void Gui::onPedsPressed() {                                            // slot
    static QEvt const e = { PEDS_WAITING_SIG, 0U, 0U };
    QACTIVE_POST(AO_Pelican, &e, (void *)0);
    m_pedsButton1->setIcon(QPixmap(":/res/BTN_DWN.png"));
    m_pedsButton2->setIcon(QPixmap(":/res/BTN_DWN.png"));
}
//............................................................................
void Gui::onPedsReleased() {                                           // slot
    m_pedsButton1->setIcon(QPixmap(":/res/BTN_UP.png"));
    m_pedsButton2->setIcon(QPixmap(":/res/BTN_UP.png"));
}
//............................................................................
void Gui::onOnPressed() {                                              // slot
    static QEvt const e = { OFF_SIG, 0U, 0U };
    m_onOffButton->setText("OFF");
    QACTIVE_POST(AO_Pelican, &e, (void *)0);
    qDebug("onOnPressed");
}
//............................................................................
void Gui::onOnReleased() {                                             // slot
    static QEvt const e = { ON_SIG, 0U, 0U };
    m_onOffButton->setText("ON");
    QACTIVE_POST(AO_Pelican, &e, (void *)0);
    qDebug("onOnReleased");
}
//............................................................................
void Gui::onQuit() {                                                   // slot
    static QEvt const e = { TERMINATE_SIG, 0U, 0U };
    QF_PUBLISH(&e, (void *)0);
    qDebug("onQuit");
}

