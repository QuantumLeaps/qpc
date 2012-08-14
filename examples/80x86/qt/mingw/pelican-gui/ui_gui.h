/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created: Tue Jul 10 17:42:32 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QPushButton *m_pedsButton1;
    QPushButton *m_quitButton;
    QLabel *label;
    QLabel *m_epoolLabel;
    QLabel *m_crossing;
    QLabel *m_carsLabel;
    QLabel *m_pedsLabel;
    QPushButton *m_onOffButton;
    QLabel *m_stateLabel;
    QPushButton *m_pedsButton2;

    void setupUi(QDialog *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QString::fromUtf8("Gui"));
        Gui->resize(528, 279);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
        Gui->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8("res/qp.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Gui->setWindowIcon(icon);
        m_pedsButton1 = new QPushButton(Gui);
        m_pedsButton1->setObjectName(QString::fromUtf8("m_pedsButton1"));
        m_pedsButton1->setGeometry(QRect(470, 130, 41, 41));
        m_pedsButton1->setCursor(QCursor(Qt::PointingHandCursor));
        m_pedsButton1->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/BTN_UP.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_pedsButton1->setIcon(icon1);
        m_pedsButton1->setIconSize(QSize(43, 45));
        m_quitButton = new QPushButton(Gui);
        m_quitButton->setObjectName(QString::fromUtf8("m_quitButton"));
        m_quitButton->setGeometry(QRect(410, 240, 101, 31));
        label = new QLabel(Gui);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 240, 51, 16));
        m_epoolLabel = new QLabel(Gui);
        m_epoolLabel->setObjectName(QString::fromUtf8("m_epoolLabel"));
        m_epoolLabel->setGeometry(QRect(60, 240, 51, 16));
        m_crossing = new QLabel(Gui);
        m_crossing->setObjectName(QString::fromUtf8("m_crossing"));
        m_crossing->setGeometry(QRect(10, 10, 498, 220));
        m_crossing->setPixmap(QPixmap(QString::fromUtf8(":/res/crossing.png")));
        m_carsLabel = new QLabel(Gui);
        m_carsLabel->setObjectName(QString::fromUtf8("m_carsLabel"));
        m_carsLabel->setGeometry(QRect(470, 20, 32, 80));
        m_carsLabel->setPixmap(QPixmap(QString::fromUtf8(":/res/cars_red.png")));
        m_pedsLabel = new QLabel(Gui);
        m_pedsLabel->setObjectName(QString::fromUtf8("m_pedsLabel"));
        m_pedsLabel->setGeometry(QRect(20, 50, 32, 32));
        m_pedsLabel->setPixmap(QPixmap(QString::fromUtf8(":/res/peds_blank.png")));
        m_onOffButton = new QPushButton(Gui);
        m_onOffButton->setObjectName(QString::fromUtf8("m_onOffButton"));
        m_onOffButton->setGeometry(QRect(380, 20, 75, 31));
        m_stateLabel = new QLabel(Gui);
        m_stateLabel->setObjectName(QString::fromUtf8("m_stateLabel"));
        m_stateLabel->setGeometry(QRect(200, 240, 151, 20));
        m_pedsButton2 = new QPushButton(Gui);
        m_pedsButton2->setObjectName(QString::fromUtf8("m_pedsButton2"));
        m_pedsButton2->setGeometry(QRect(8, 120, 41, 41));
        m_pedsButton2->setCursor(QCursor(Qt::PointingHandCursor));
        m_pedsButton2->setAutoFillBackground(false);
        m_pedsButton2->setIcon(icon1);
        m_pedsButton2->setIconSize(QSize(43, 45));
        m_crossing->raise();
        m_pedsButton1->raise();
        m_quitButton->raise();
        label->raise();
        m_epoolLabel->raise();
        m_carsLabel->raise();
        m_pedsLabel->raise();
        m_onOffButton->raise();
        m_stateLabel->raise();
        m_pedsButton2->raise();
        QWidget::setTabOrder(m_pedsButton1, m_quitButton);

        retranslateUi(Gui);
        QObject::connect(m_quitButton, SIGNAL(clicked()), Gui, SLOT(onQuit()));
        QObject::connect(m_pedsButton1, SIGNAL(pressed()), Gui, SLOT(onPedsPressed()));
        QObject::connect(Gui, SIGNAL(finished(int)), Gui, SLOT(onQuit()));
        QObject::connect(m_onOffButton, SIGNAL(pressed()), Gui, SLOT(onOnPressed()));
        QObject::connect(m_onOffButton, SIGNAL(released()), Gui, SLOT(onOnReleased()));
        QObject::connect(m_pedsButton2, SIGNAL(pressed()), Gui, SLOT(onPedsPressed()));
        QObject::connect(m_pedsButton1, SIGNAL(released()), Gui, SLOT(onPedsReleased()));
        QObject::connect(m_pedsButton2, SIGNAL(released()), Gui, SLOT(onPedsReleased()));

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QDialog *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "Dialog", 0, QApplication::UnicodeUTF8));
        m_pedsButton1->setText(QString());
        m_quitButton->setText(QApplication::translate("Gui", "Quit", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Gui", "MPool[0]:", 0, QApplication::UnicodeUTF8));
        m_epoolLabel->setText(QApplication::translate("Gui", "?", 0, QApplication::UnicodeUTF8));
        m_crossing->setText(QString());
        m_carsLabel->setText(QString());
        m_pedsLabel->setText(QString());
        m_onOffButton->setText(QApplication::translate("Gui", "Offline", 0, QApplication::UnicodeUTF8));
        m_stateLabel->setText(QApplication::translate("Gui", "state", 0, QApplication::UnicodeUTF8));
        m_pedsButton2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
