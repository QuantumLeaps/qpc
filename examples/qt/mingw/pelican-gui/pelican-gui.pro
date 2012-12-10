#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T10:48:40
#
#-------------------------------------------------
TEMPLATE = app

QT      += core gui
TARGET   = pelican-gui
DEFINES += QT_NO_STATEMACHINE

INCLUDEPATH = . \
    $(QPC)/include \
    $(QPC)/ports/qt/mingw \

SOURCES += \
    main.cpp \
    gui.cpp \
    bsp.cpp \
    pelican.c \

HEADERS += \
    gui.h \
    bsp.h \
    pelican.h

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
