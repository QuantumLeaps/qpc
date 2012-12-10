
LOCAL_PATH := $(call my-dir)

QPC_PATH   := ../../..
QPC_ABS_PATH := $(abspath $(LOCAL_PATH)/$(QPC_PATH))
QP_INCDIR  := $(QPC_ABS_PATH)/include
QF_INCDIR  := $(QPC_ABS_PATH)/qf/source
QS_INCDIR  := $(QPC_ABS_PATH)/qs/source

QEP_SRCDIR := $(QPC_PATH)/qep/source
QF_SRCDIR  := $(QPC_PATH)/qf/source
QS_SRCDIR  := $(QPC_PATH)/qs/source
QSPY_SRCDIR := ../../../../qtools/qspy/source
QSPY_INCDIR := $(QTOOLS)/qspy/include


include $(CLEAR_VARS)
LOCAL_MODULE := qpc
LOCAL_C_INCLUDES += $(QP_INCDIR) $(QF_INCDIR) $(LOCAL_PATH)/..
LOCAL_CFLAGS += -Wall
LOCAL_SRC_FILES := $(QEP_SRCDIR)/qep.c \
$(QEP_SRCDIR)/qfsm_ini.c $(QEP_SRCDIR)/qfsm_dis.c \
$(QEP_SRCDIR)/qhsm_ini.c $(QEP_SRCDIR)/qhsm_dis.c \
$(QEP_SRCDIR)/qhsm_top.c $(QEP_SRCDIR)/qhsm_in.c \
$(QF_SRCDIR)/qa_defer.c \
$(QF_SRCDIR)/qa_fifo.c $(QF_SRCDIR)/qa_lifo.c \
$(QF_SRCDIR)/qa_get_.c $(QF_SRCDIR)/qa_sub.c \
$(QF_SRCDIR)/qa_usub.c $(QF_SRCDIR)/qa_usuba.c \
$(QF_SRCDIR)/qeq_init.c $(QF_SRCDIR)/qeq_fifo.c \
$(QF_SRCDIR)/qeq_lifo.c $(QF_SRCDIR)/qeq_get.c \
$(QF_SRCDIR)/qf_act.c $(QF_SRCDIR)/qf_gc.c \
$(QF_SRCDIR)/qf_log2.c $(QF_SRCDIR)/qf_new.c \
$(QF_SRCDIR)/qf_pool.c $(QF_SRCDIR)/qf_psini.c \
$(QF_SRCDIR)/qf_pspub.c $(QF_SRCDIR)/qf_pwr2.c \
$(QF_SRCDIR)/qf_tick.c $(QF_SRCDIR)/qmp_get.c \
$(QF_SRCDIR)/qmp_init.c $(QF_SRCDIR)/qmp_put.c \
$(QF_SRCDIR)/qte_ctor.c $(QF_SRCDIR)/qte_arm.c \
$(QF_SRCDIR)/qte_darm.c $(QF_SRCDIR)/qte_rarm.c \
$(QF_SRCDIR)/qte_ctr.c src/qf_port.c
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := qpc-qspy
LOCAL_CFLAGS  += -DQ_SPY -Wall
LOCAL_EXPORT_CFLAGS := -DQ_SPY
LOCAL_C_INCLUDES += $(QP_INCDIR) $(QF_INCDIR) $(QS_INCDIR) $(QSPY_INCDIR) $(LOCAL_PATH)/..
LOCAL_SRC_FILES := $(QEP_SRCDIR)/qep.c \
$(QEP_SRCDIR)/qfsm_ini.c $(QEP_SRCDIR)/qfsm_dis.c \
$(QEP_SRCDIR)/qhsm_ini.c $(QEP_SRCDIR)/qhsm_dis.c \
$(QEP_SRCDIR)/qhsm_top.c $(QEP_SRCDIR)/qhsm_in.c \
$(QF_SRCDIR)/qa_fifo.c $(QF_SRCDIR)/qa_lifo.c \
$(QF_SRCDIR)/qa_get_.c $(QF_SRCDIR)/qa_sub.c \
$(QF_SRCDIR)/qa_usub.c $(QF_SRCDIR)/qa_usuba.c \
$(QF_SRCDIR)/qeq_init.c $(QF_SRCDIR)/qeq_fifo.c \
$(QF_SRCDIR)/qeq_lifo.c $(QF_SRCDIR)/qeq_get.c \
$(QF_SRCDIR)/qf_act.c $(QF_SRCDIR)/qf_gc.c \
$(QF_SRCDIR)/qf_log2.c $(QF_SRCDIR)/qf_new.c \
$(QF_SRCDIR)/qf_pool.c $(QF_SRCDIR)/qf_psini.c \
$(QF_SRCDIR)/qf_pspub.c $(QF_SRCDIR)/qf_pwr2.c \
$(QF_SRCDIR)/qf_tick.c $(QF_SRCDIR)/qmp_get.c \
$(QF_SRCDIR)/qmp_init.c $(QF_SRCDIR)/qmp_put.c \
$(QF_SRCDIR)/qte_ctor.c $(QF_SRCDIR)/qte_arm.c \
$(QF_SRCDIR)/qte_darm.c $(QF_SRCDIR)/qte_rarm.c \
$(QF_SRCDIR)/qte_ctr.c src/qf_port.c \
$(QS_SRCDIR)/qs.c \
$(QS_SRCDIR)/qs_.c $(QS_SRCDIR)/qs_blk.c \
$(QS_SRCDIR)/qs_byte.c $(QS_SRCDIR)/qs_f32.c \
$(QS_SRCDIR)/qs_f64.c $(QS_SRCDIR)/qs_mem.c \
$(QS_SRCDIR)/qs_str.c src/qs_port.c $(QSPY_SRCDIR)/qspy.c
include $(BUILD_STATIC_LIBRARY)


