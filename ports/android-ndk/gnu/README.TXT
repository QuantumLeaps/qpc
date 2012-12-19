Include this module to your native application by adding

$(call import-module,qpc/ports/arm/android-ndk/gnu)

to end of your project's Android.mk file.

You also need to refer to libraries you want to use.

For example:

LOCAL_STATIC_LIBRARIES := qpc

or (for QSpy support):

LOCAL_STATIC_LIBRARIES := qpc-qspy

For more information, please see the Application note for QP/C Android-NDK
