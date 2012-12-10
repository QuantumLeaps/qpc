/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created: Tue Aug 7 13:02:23 2012
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
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include "pixellabel.h"

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QLabel *m_background;
    QPushButton *m_button;
    QPushButton *m_quit;
    QLCDNumber *m_score;
    PixelLabel *m_display;
    QLabel *m_LED;
    QLabel *label;
    QLabel *m_epoolLabel;

    void setupUi(QDialog *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QString::fromUtf8("Gui"));
        Gui->resize(801, 369);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
        Gui->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8("res/qp.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Gui->setWindowIcon(icon);
        m_background = new QLabel(Gui);
        m_background->setObjectName(QString::fromUtf8("m_background"));
        m_background->setGeometry(QRect(0, 10, 791, 291));
        m_background->setPixmap(QPixmap(QString::fromUtf8(":/res/EK-LM3S811.png")));
        m_button = new QPushButton(Gui);
        m_button->setObjectName(QString::fromUtf8("m_button"));
        m_button->setGeometry(QRect(550, 217, 41, 41));
        m_button->setCursor(QCursor(Qt::PointingHandCursor));
        m_button->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/EK-BTN_UP.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_button->setIcon(icon1);
        m_button->setIconSize(QSize(43, 45));
        m_quit = new QPushButton(Gui);
        m_quit->setObjectName(QString::fromUtf8("m_quit"));
        m_quit->setGeometry(QRect(690, 330, 101, 31));
        m_score = new QLCDNumber(Gui);
        m_score->setObjectName(QString::fromUtf8("m_score"));
        m_score->setGeometry(QRect(10, 300, 131, 61));
        m_score->setNumDigits(4);
        m_display = new PixelLabel(Gui);
        m_display->setObjectName(QString::fromUtf8("m_display"));
        m_display->setGeometry(QRect(420, 135, 192, 32));
        m_LED = new QLabel(Gui);
        m_LED->setObjectName(QString::fromUtf8("m_LED"));
        m_LED->setGeometry(QRect(611, 55, 29, 30));
        m_LED->setPixmap(QPixmap(QString::fromUtf8("res/LED_OFF.png")));
        label = new QLabel(Gui);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 330, 46, 13));
        m_epoolLabel = new QLabel(Gui);
        m_epoolLabel->setObjectName(QString::fromUtf8("m_epoolLabel"));
        m_epoolLabel->setGeometry(QRect(240, 330, 46, 13));
        QWidget::setTabOrder(m_button, m_quit);

        retranslateUi(Gui);
        QObject::connect(m_quit, SIGNAL(clicked()), Gui, SLOT(onQuit()));
        QObject::connect(m_button, SIGNAL(pressed()), Gui, SLOT(onBtnPressed()));
        QObject::connect(m_button, SIGNAL(released()), Gui, SLOT(onBtnReleased()));
        QObject::connect(Gui, SIGNAL(finished(int)), Gui, SLOT(onQuit()));

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QDialog *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "\"Fly 'n' Shoot\" game on EK-LM3S811", 0, QApplication::UnicodeUTF8));
        m_background->setText(QString());
        m_button->setText(QString());
        m_quit->setText(QApplication::translate("Gui", "Quit", 0, QApplication::UnicodeUTF8));
        m_display->setText(QApplication::translate("Gui", "TextLabel", 0, QApplication::UnicodeUTF8));
        m_LED->setText(QString());
        label->setText(QApplication::translate("Gui", "MPool[0]:", 0, QApplication::UnicodeUTF8));
        m_epoolLabel->setText(QApplication::translate("Gui", "?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
