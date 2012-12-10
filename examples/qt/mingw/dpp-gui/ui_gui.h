/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created: Tue Aug 7 13:06:31 2012
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
    QPushButton *m_pauseButton;
    QPushButton *m_quitButton;
    QLabel *m_philoLabel_0;
    QLabel *m_philoLabel_1;
    QLabel *m_philoLabel_2;
    QLabel *m_philoLabel_3;
    QLabel *m_philoLabel_4;
    QLabel *label;
    QLabel *m_epoolLabel;

    void setupUi(QDialog *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QString::fromUtf8("Gui"));
        Gui->resize(338, 360);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
        Gui->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8("res/qp.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Gui->setWindowIcon(icon);
        m_pauseButton = new QPushButton(Gui);
        m_pauseButton->setObjectName(QString::fromUtf8("m_pauseButton"));
        m_pauseButton->setGeometry(QRect(130, 150, 81, 41));
        m_pauseButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_pauseButton->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/EK-BTN_UP.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_pauseButton->setIcon(icon1);
        m_pauseButton->setIconSize(QSize(43, 45));
        m_quitButton = new QPushButton(Gui);
        m_quitButton->setObjectName(QString::fromUtf8("m_quitButton"));
        m_quitButton->setGeometry(QRect(230, 320, 101, 31));
        m_philoLabel_0 = new QLabel(Gui);
        m_philoLabel_0->setObjectName(QString::fromUtf8("m_philoLabel_0"));
        m_philoLabel_0->setGeometry(QRect(130, 20, 80, 74));
        m_philoLabel_0->setPixmap(QPixmap(QString::fromUtf8(":/res/thinking.png")));
        m_philoLabel_1 = new QLabel(Gui);
        m_philoLabel_1->setObjectName(QString::fromUtf8("m_philoLabel_1"));
        m_philoLabel_1->setGeometry(QRect(240, 100, 80, 74));
        m_philoLabel_1->setPixmap(QPixmap(QString::fromUtf8(":/res/thinking.png")));
        m_philoLabel_2 = new QLabel(Gui);
        m_philoLabel_2->setObjectName(QString::fromUtf8("m_philoLabel_2"));
        m_philoLabel_2->setGeometry(QRect(190, 230, 80, 74));
        m_philoLabel_2->setPixmap(QPixmap(QString::fromUtf8(":/res/thinking.png")));
        m_philoLabel_3 = new QLabel(Gui);
        m_philoLabel_3->setObjectName(QString::fromUtf8("m_philoLabel_3"));
        m_philoLabel_3->setGeometry(QRect(60, 230, 80, 74));
        m_philoLabel_3->setPixmap(QPixmap(QString::fromUtf8(":/res/thinking.png")));
        m_philoLabel_4 = new QLabel(Gui);
        m_philoLabel_4->setObjectName(QString::fromUtf8("m_philoLabel_4"));
        m_philoLabel_4->setGeometry(QRect(10, 100, 80, 74));
        m_philoLabel_4->setPixmap(QPixmap(QString::fromUtf8(":/res/thinking.png")));
        label = new QLabel(Gui);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 320, 51, 16));
        m_epoolLabel = new QLabel(Gui);
        m_epoolLabel->setObjectName(QString::fromUtf8("m_epoolLabel"));
        m_epoolLabel->setGeometry(QRect(70, 320, 51, 16));
        QWidget::setTabOrder(m_pauseButton, m_quitButton);

        retranslateUi(Gui);
        QObject::connect(m_quitButton, SIGNAL(clicked()), Gui, SLOT(onQuit()));
        QObject::connect(m_pauseButton, SIGNAL(pressed()), Gui, SLOT(onPausePressed()));
        QObject::connect(m_pauseButton, SIGNAL(released()), Gui, SLOT(onPauseReleased()));
        QObject::connect(Gui, SIGNAL(finished(int)), Gui, SLOT(onQuit()));

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QDialog *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "Dialog", 0, QApplication::UnicodeUTF8));
        m_pauseButton->setText(QApplication::translate("Gui", "SERVING", 0, QApplication::UnicodeUTF8));
        m_quitButton->setText(QApplication::translate("Gui", "Quit", 0, QApplication::UnicodeUTF8));
        m_philoLabel_0->setText(QString());
        m_philoLabel_1->setText(QString());
        m_philoLabel_2->setText(QString());
        m_philoLabel_3->setText(QString());
        m_philoLabel_4->setText(QString());
        label->setText(QApplication::translate("Gui", "MPool[0]:", 0, QApplication::UnicodeUTF8));
        m_epoolLabel->setText(QApplication::translate("Gui", "?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
