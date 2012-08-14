#include <QtGui>
#include "gui.h"
//-----------------
extern "C" {
    #include "qp_port.h"
    #include "game.h"
    #include "bsp.h"
}

Q_DEFINE_THIS_FILE

//............................................................................
Gui *Gui::instance;

//............................................................................
Gui::Gui(QWidget *parent)
  : QDialog(parent)
{
    instance = this;  // only one instance for the Gui

    setupUi(this);

    static quint8 const c_offColor[] = {  15U,  15U,  15U };
    m_display->init(BSP_SCREEN_WIDTH, 2U,
                    BSP_SCREEN_HEIGHT, 2U,
                    c_offColor);
}
//............................................................................
void Gui::onBtnPressed() {                                             // slot
    m_button->setIcon(QPixmap(":/res/EK-BTN_DWN.png"));
    static QEvt const fireEvt = { PLAYER_TRIGGER_SIG, 0, 0 };
    QF_PUBLISH(&fireEvt, (void*)0);
}
//............................................................................
void Gui::onBtnReleased() {                                            // slot
    m_button->setIcon(QPixmap(":/res/EK-BTN_UP.png"));
}
//............................................................................
void Gui::onQuit() {                                                   // slot
    BSP_terminate(0);
}
//............................................................................
void Gui::wheelEvent(QWheelEvent *e) {
    if (e->delta() >= 0) {
        BSP_moveShipUp();
    }
    else {
        BSP_moveShipDown();
    }
}
