#-------------------------------------------------
#
# Project created by QtCreator 2012-06-11T14:58:33
#
#-------------------------------------------------

TEMPLATE = app
QT      += core gui
DEFINES += QT_NO_STATEMACHINE

TARGET   = game-gui

INCLUDEPATH = . \
    $(QPC)/include \
    $(QPC)/ports/qt/mingw \

SOURCES += \
    main.cpp \
    gui.cpp \
    bsp.cpp \
    missile.c \
    mine2.c \
    mine1.c \
    tunnel.c \
    ship.c \
    game.c

HEADERS  += \
    game.h \
    bsp.h \
    gui.h

FORMS += gui.ui

CONFIG(debug, debug|release) {
    DEFINES += Q_SPY
    INCLUDEPATH += $(QTOOLS)/qspy/include
    SOURCES += $(QTOOLS)/qspy/source/qspy.c
    HEADERS += qs_port.h
    LIBS += -L$(QPC)/ports/qt/mingw/debug
} else {
    LIBS += -L$(QPC)/ports/qt/mingw/release
}

LIBS += -lqp

RESOURCES = gui.qrc

win32:RC_FILE = gui.rc

