#-------------------------------------------------
#
# Project created by QtCreator 2012-05-09T19:56:54
#
#-------------------------------------------------

QT      += core gui
TARGET   = qp
TEMPLATE = lib
CONFIG  += staticlib
DEFINES += QT_NO_STATEMACHINE

INCLUDEPATH += . \
    ../../../../include \
    ../../../../qep/source \
    ../../../../qf/source

HEADERS +=  \
    qep_port.h \
    qf_port.h \
    qp_app.h \
    pixellabel.h

SOURCES += \
    qf_port.cpp \
    pixellabel.cpp \
    ../../../../qep/source/qep.c \
    ../../../../qep/source/qhsm_dis.c \
    ../../../../qep/source/qhsm_ini.c \
    ../../../../qep/source/qhsm_in.c \
    ../../../../qep/source/qhsm_top.c \
    ../../../../qf/source/qa_defer.c \
    ../../../../qf/source/qa_sub.c \
    ../../../../qf/source/qa_usub.c \
    ../../../../qf/source/qa_usuba.c \
    ../../../../qf/source/qeq_fifo.c \
    ../../../../qf/source/qeq_get.c \
    ../../../../qf/source/qeq_init.c \
    ../../../../qf/source/qeq_lifo.c \
    ../../../../qf/source/qf_act.c \
    ../../../../qf/source/qf_gc.c \
    ../../../../qf/source/qf_log2.c \
    ../../../../qf/source/qf_new.c \
    ../../../../qf/source/qf_pool.c \
    ../../../../qf/source/qf_psini.c \
    ../../../../qf/source/qf_pspub.c \
    ../../../../qf/source/qf_pwr2.c \
    ../../../../qf/source/qf_tick.c \
    ../../../../qf/source/qmp_get.c \
    ../../../../qf/source/qmp_init.c \
    ../../../../qf/source/qmp_put.c \
    ../../../../qf/source/qte_arm.c \
    ../../../../qf/source/qte_ctor.c \
    ../../../../qf/source/qte_ctr.c \
    ../../../../qf/source/qte_darm.c \
    ../../../../qf/source/qte_rarm.c

CONFIG(debug, debug|release) {
    DEFINES += Q_SPY

    SOURCES += \
        ../../../../qs/source/qs.c \
        ../../../../qs/source/qs_.c \
        ../../../../qs/source/qs_blk.c \
        ../../../../qs/source/qs_byte.c \
        ../../../../qs/source/qs_f32.c \
        ../../../../qs/source/qs_f64.c \
        ../../../../qs/source/qs_mem.c \
        ../../../../qs/source/qs_str.c \
        ../../../../qs/source/qs_u64.c

} else {
    DEFINES += NDEBUG
}
