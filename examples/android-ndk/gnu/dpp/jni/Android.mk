#
# Build script for DPP example for QP/C for Android NDK
# Please NDK_MODULE_PATH enviroment variable to point to one diretory 
# above qpc root dir.
QP_INCDIR      := $(QPC)/include
QP_PORT_INCDIR := $(QPC)/ports/android-ndk/gnu
QSPY_INCDIR    := $(QTOOLS)/qspy/include

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := dpp
LOCAL_SRC_FILES := main.c bsp.c display.c table.c philo.c
LOCAL_C_INCLUDES += $(QP_INCDIR) $(QP_PORT_INCDIR)  
LOCAL_CFLAGS += -Wall
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue qpc
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := dpp-qspy
LOCAL_SRC_FILES := main.c bsp.c display.c table.c philo.c
LOCAL_C_INCLUDES += $(QP_INCDIR) $(QP_PORT_INCDIR) $(QSPY_INCDIR)
LOCAL_CFLAGS += -Wall
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue qpc-qspy
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,qpc/ports/android-ndk/gnu)
