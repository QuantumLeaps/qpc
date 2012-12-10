1 Introduction
..............................................................................

This Application Note describes how to use the QP/C™ event-driven platform version 4.5 or higher with the Android Native Development kit (NDK).

To focus the discussion, the Application Note uses an OpenGL based Android NDK version of the Dining Philosopher Problem (DPP) test application running on Android ARM/MIPS/X86 emulators and Android based cellphones and devices (see the Application Note [QL ANDPP 08] “Application Note: Dining Philosophers Application”). 

NOTE: This Application Note pertains only the C version of the QP™ state machine 
frameworks. 

1.1 About QP
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨
1.2 About QM
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨


1.3 About the QP™ Port to Android
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

In this port, a QP application runs as a single process inside Android NativeActivity class, with each QP active object executing in a separate lightweight POSIX thread (Pthread). The port uses a Pthread mutex to implement the QP critical section and the Pthread condition variables to provide the blocking mechanism for event queues of active objects.

The Android NDK does not implement the standard C library fully. Instead it has it's own library called Bionic libc. The Bionic libc is designed to be small, fast and to use BSD licensing.

The general assumption underlying the QP port to Android NDK is that the application is going to be real-time or perhaps “soft real-time”. This means that the port is trying to use the real-time features available in the Bionic PThread API as much as possible. These features are tested against the Android Emulator software. 

The QP port to Android NDK uses one dedicated Pthread to periodically call the QF_tick() function to handle the armed time events. At startup, QP attempts to set the priority of this “ticker” thread to the maximum, so that the system clock tick occurs in the timely manner. 

For QSPY tracing, an additional logger PThread is started. This thread works as a gateway for reading the QSPY logging messages and feeding them through standard Android logging mechanism.


1.4 Licensing QP and QP port to Android
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

1.5 Licensing QM
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨


2. Directories and Files
..............................................................................

The code for the QP port to Android NDK is part of the standard QP Generally Available (GA)distribution, which also contains example applications. The GA distribution is available in a platform-independent ZIP file that can be unzipped into an arbitrary root directory. The QP Root Directory chosen for the installation will be henceforth referred to as <qp>.
The code of Android NDK port is organized according to the Application Note: “QP_Directory_Structure”. 

Specifically, for this port the files are placed in the following directories:

<qp>/                          - QP-root directory for Quantum Platform (QP)
|
+-include/                     - QP public include files
| +-qassert.h                  – QP assertions public include file
| +-qevent.h                   – QEvent declaration
| +-qep.h                      – QEP platform-independent public include
| +-qf.h                       – QF platform-independent public include
| +-qk.h                       – QK platform-independent public include
| +-qs.h                       – QS platform-independent public include
| +-qs_dummy.h                 – QS “dummy” public include
| +-qequeue.h                  – native QF event queue include
| +-qmpool.h                   – native QF memory pool include
| +-qpset.h                    – native QF priority set include
|                              
+-ports/                       - QP ports
| +-arm/                       - ARM ports
| | +-android/                 - POSIX port
| | | +-gnu/                   - GNU compiler
| | | | +-Android.mk           – ndk-build makefile for QP libraries
| | | | +-AndroidManifest.xml  – Manifest for the QP libraries
| | | | +-qep_port.h           – QEP platform-dependent public include
| | | | +-qf_port.h            – QF platform-dependent public include
| | | | +-qs_port.h            – QS platform-dependent public include
| | | | +-src/                 – directory for port source files
| | | | | +-qf_port.c          – QF plaform-dependent implementation
| | | | | +-qs_port.c          – QSPY plaform-dependent implementation
|                              
+-examples/                    - subdirectory containing the QP example files
| +-arm/                       - ARM ports
| | +-android/                 - POSIX examples
| | | +-gnu/                   - GNU compiler
| | | | +-dpp/                 - Dining Philosopher Problem application
| | | | | +-jni/               – Java native interface compatible files
| | | | | | +-Android.mk       – ndk-build makefile for application
| | | | | | +-Application.mk   – ndk-build app build config 
| | | | | | +-bsp.c            - Board Support Package for Android
| | | | | | +-bsp.h            - BSP header file
| | | | | | +-display.c        - OpenGL display routines
| | | | | | +-display.h        - OpenGL display routine API
| | | | | | +-main.c           - the main function
| | | | | | +-philo.c          - the Philosopher active object
| | | | | | +-table.c          - the Table active object
| | | | | | +-dpp.h            - the DPP header file
| | | | | | +-dpp.qm           - the DPP model file
| | | | | |                    
| | | | | +-libs/              – JNI ndk library accessed by NativeActivity
| | | | | | +-armeabi/         – JNI lib for armeabi architecture
| | | | | | | +-libdpp.so      – JNI Shared lib QP/C Android NDK DPP      
| | | | | | | +-libdpp-qspy.so – JNI Shared lib QP/C Android NDK DPP QSPY
| | | | | | +-armeabi-v7a/     – JNI lib for armeabi-v7a architecture
| | | | | | | +-libdpp.so      – JNI Shared lib QP/C Android NDK DPP     
| | | | | | | +-libdpp-qspy.so – JNI Shared lib QP/C Android NDK DPP QSPY
| | | | | | +-mips/            – JNI lib for mips architecture
| | | | | | | +-libdpp.so      – JNI Shared lib QP/C Android NDK DPP     
| | | | | | | +-libdpp-qspy.so – JNI Shared lib QP/C Android NDK DPP QSPY
| | | | | | +-x86/             – JNI lib for x86 architecture
| | | | | | | +-libdpp.so      – JNI Shared lib QP/C Android NDK DPP     
| | | | | | | +-libdpp-qspy.so – JNI Shared lib QP/C Android NDK DPP QSPY
| | | | | |
| | | | | +-obj/                       – various object files
| | | | | | +- convert_dependecies.sh  – ndk-build auto-generated script
| | | | | | +- local                   – local object files
| | | | | | |  +-armeabi/              – object files for armeabi
| | | | | | |  +-armeabi-v7a/          – object files for armeabi-v7a 
| | | | | | |  +-mips/                 – object files for mips
| | | | | | |  +-x86/                  – object files for x86
| | | | | |                            
| | | | | +-bin/                       – Packaged application files
| | | | | | +-DiningPhilosophersProblem-debug.apk – debug version
| | | | | +-res/                       – resource files for application
| | | | | | +-drawable-ldpi/           – low res images for application
| | | | | | | +-application_icon.png   – low res application icon
| | | | | | +-drawable-hdpi/           – high res images for application
| | | | | | | +-application_icon.png   – high res application icon
| | | | | | +-drawable-mdpi/           – medium res images for application
| | | | | | | +-application_icon.png   – meduim res application icon
| | | | | | +-drawable-xhdpi/          – xtra high res images for application
| | | | | | | +-application_icon.png   – xtra high res application icon
| | | | | |                             
| | | | | +-AndroidManifest.xml        – Manifest for the QP libraries
| | | | | +-build.xml                  – Ant build script for buiding the app

Even though the examples are placed under ARM directory, they don't have any ARM specific code and they are build also to MIPS or x86 architectures.

2.1 Prerequisites for building / debugging / testing QP Android NDK applications
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

- Android NDK r8 
- Cygwin + Devel package (for Windows builds)
- Android SDK
- Apache Ant
- Java runtime (needed for Apache Ant)
- QTools (needed for the QSpy build)

Install all of the above requisites to your computer. 



Setup the paths by modifying cygwin.bat, add following lines

set PATH=<ANDROID_SDK_DIR>/android-sdk/tools;<ANDROID_SDK_DIR>/android-sdk/platform-tools;<ANDROID_NDK_DIR>;<JAVA_BIN_DIR>;<ANT_BIN_DIR>
set JAVA_HOME=
set ANDROID_NDK_ROOT=<ANDROID_NDK_DIR>
set NDK_MODULE_PATH=<one dir above QPC>
set QTOOLS=<QTOOLS_INSTALL_DIR>
set QPC=<QPC_INSTALL_DIR>

real life example:

set PATH=/cygdrive/c/Users/caprica/AppData/Local/Android/android-sdk/tools;/cygdrive/c/Users/caprica/AppData/Local/Android/android-sdk/platform-tools;/cygdrive/d/projects/android-ndk-r8b;/cygdrive/C/Program Files/Java/jdk1.6.0_37/bin/;/cygdrive/d/projects/ant/bin/
set JAVA_HOME=/cygdrive/C/Program Files/Java/jdk1.6.0_37
set ANDROID_NDK_ROOT=/cygdrive/d/projects/android-ndk-r8b
set NDK_MODULE_PATH=/cygdrive/d/projects
set QTOOLS=/cygdrive/d/projects/qtools
set QPC=/cygdrive/d/projects/qpc

If using Linux or Mac, adjust accordingly.

See that you can succesfully start:
- android.bat (from regular Windows command line) 
- "ant" (from Cygwin or regular Windows command line)
- "ndk-build" and adb" (from Cygwin).



2.2 Building the QP Libraries
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

The Android NDK build system is called ndk-build, which itself is a wrapper around GNU Make. It has a concept of "modules". Each module is defined in a Android.mk file. In this file, the compiler flags and dependencies to other modules are declared. Most of the time, the modules map 1:1 to specific libraries.

As QP libraries for Android are defined as modules, you don't need to build them separately. Instead they are automatically pulled (and build) to the application through the module dependency system.


2.3 Building the QP Applications
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

As shown in Listing 1, the DPP application example for Android NDK is located in the directory <qp>/examples/arm/android/gnu/dpp/. This directory contains the Android.mk file and build.xml file to build the example. 

The provided Android.mk makefile automatically builds two configurations of the DPP example: with and without QSPY tracing.

The build is started using the command (inside cygwin for Windows):

ndk-build

or 

ndk-build NDK_DEBUG=1

The latter command builds all libraries with debug symbols, and the first command without debug symbols.

The following listing shows a snippet from the console output from the build:

$ ndk-build
Cygwin         : Generating dependency file converter script
Compile thumb  : dpp <= main.c
Compile thumb  : dpp <= bsp.c
Compile thumb  : dpp <= display.c
Compile thumb  : dpp <= table.c
Compile thumb  : dpp <= philo.c
Compile thumb  : android_native_app_glue <= android_native_app_glue.c
StaticLibrary  : libandroid_native_app_glue.a
Compile thumb  : qf <= qa_defer.c
Compile thumb  : qf <= qa_fifo.c
Compile thumb  : qf <= qa_lifo.c
Compile thumb  : qf <= qa_get_.c
Compile thumb  : qf <= qa_sub.c
Compile thumb  : qf <= qa_usub.c
Compile thumb  : qf <= qa_usuba.c
Compile thumb  : qf <= qeq_init.c
Compile thumb  : qf <= qeq_fifo.c
Compile thumb  : qf <= qeq_lifo.c
Compile thumb  : qf <= qeq_get.c
Compile thumb  : qf <= qf_act.c
Compile thumb  : qf <= qf_gc.c
Compile thumb  : qf <= qf_log2.c
Compile thumb  : qf <= qf_new.c
Compile thumb  : qf <= qf_pool.c
Compile thumb  : qf <= qf_psini.c
Compile thumb  : qf <= qf_pspub.c
Compile thumb  : qf <= qf_pwr2.c
Compile thumb  : qf <= qf_tick.c
Compile thumb  : qf <= qmp_get.c
Compile thumb  : qf <= qmp_init.c
Compile thumb  : qf <= qmp_put.c
Compile thumb  : qf <= qte_ctor.c
Compile thumb  : qf <= qte_arm.c
Compile thumb  : qf <= qte_darm.c
Compile thumb  : qf <= qte_rarm.c
Compile thumb  : qf <= qte_ctr.c
Compile thumb  : qf <= qf_port.c
StaticLibrary  : libqf.a
Compile thumb  : qep <= qep.c
Compile thumb  : qep <= qfsm_ini.c
Compile thumb  : qep <= qfsm_dis.c
Compile thumb  : qep <= qhsm_ini.c
Compile thumb  : qep <= qhsm_dis.c
Compile thumb  : qep <= qhsm_top.c
Compile thumb  : qep <= qhsm_in.c
StaticLibrary  : libqep.a
SharedLibrary  : libdpp.so
Install        : libdpp.so => libs/armeabi/libdpp.so
Compile thumb  : dpp-qspy <= main.c
...



Additionally, after building the native shared library, you need to build and package the Android apk-distributable file. The Android uses Ant tool to do this. 
The Ant reads its configuration for the build from build.xml-file, which in turn includes the local.properties. These files need to be regenerated once to mach your configuration. This is done using following command (in the dpp-directory):

android.bat update project -p .

After this, the build is done with following command:

ant debug

or 

ant release


2.4 Executing the Example
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

The DPP example is Android OpenGL application that needs to be transferred to the Android emulator or a real device.

The transferring is done using a tool called "adb".

To install the apk package to the emulator or a real device, issue following command:

adb install -r bin\DiningPhilosophersProblem-debug.apk

The emulator must be started or the real device connected with USB cable before the command can be run.

Example output of upload command:
D:\projects\qpc\examples\arm\android\gnu\dpp>adb install -r bin\DiningPhilosophersProblem-debug.apk
74 KB/s (240197 bytes in 3.160s)
        pkg: /data/local/tmp/DiningPhilosophersProblem-debug.apk
Success


After succesfull upload, you can start the application by clicking the application icon on the emulator or on the device. The DPP screen should appear and start running immediately. While running, you can tap the screen to pause the application (send the PAUSE_SIG). While paused, another tap resumes the operation. The application presents a white table surrounded by 5 seated philosophers. The philosophers are color coded with following codes:
 - red = hungry
 - green = eating
 - blue = thinking
Additionally, when the application is paused the table changes color from white to grey.

2.5 Using QS Software Tracing
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

The QP port to Android provides particularly easy access to the QS (Quantum Spy) software tracing information. (See [Q_SPY-Ref] and Chapter 11 of [PSiCC2] book for more information about the QS software tracing system). In the Android NDK port, you can choose to have the QS data converted on-the-fly from the compressed binary to the human readable format for direct output to the android system log. This on-the-fly formatting of the binary QS data is achieved by incorporating code normally used in the QSPY host application into the application.

To select which version of the libdpp is packaged to the .apk-file, you need to modify the following line in AndroidManifest.xml file:

<meta-data android:name="android.app.lib_name" android:value="dpp" />

to

<meta-data android:name="android.app.lib_name" android:value="dpp-qspy" />

After this modification, you need to rerun the "ant debug" or "ant release" command and load the application to the emulator or device with "adb install -r".
The QS data can be read using the "adb" tool while the qspy-enabled version of the application is running. To show QSPY-information, use following command:

adb logcat -s "dpp"

The parameter -s "dpp" instructs to show only messages with "dpp" tag.

NOTE: This QS implementation requires access (during the build) to the QSPY host application code, which resides in the Qtools collection. Therefore, Qtools needs to be installed in your system.


2.5.1 Example QSPY output
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

adb logcat -s "dpp"

I/dpp     ( 4010):            ==>UnHd: Obj=l_philo[2] Sig=EAT_SIG Active=Philo_hungry
I/dpp     ( 4010): 3252764559 Ignored: Obj=l_philo[2] Sig=EAT_SIG Active=Philo_hungry
I/dpp     ( 4010): 3252764571 Disp==>: Obj=l_philo[2] Sig=EAT_SIG Active=Philo_hungry
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[2] State=Philo_eating
I/dpp     ( 4010): 3252764585 ==>Tran: Obj=l_philo[2] Sig=EAT_SIG Source=Philo_hungry New=Philo_eating
I/dpp     ( 4010): 3252764597 Disp==>: Obj=l_philo[2] Sig=EAT_SIG Active=Philo_eating
I/dpp     ( 4010): 3252764607 Intern : Obj=l_philo[2] Sig=EAT_SIG Source=Philo_eating
I/dpp     ( 4010): 3252764651 Disp==>: Obj=l_philo[1] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764662 Intern : Obj=l_philo[1] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764674 Disp==>: Obj=l_philo[1] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764684 Intern : Obj=l_philo[1] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764695 Disp==>: Obj=l_philo[1] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764706 Intern : Obj=l_philo[1] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764748 Disp==>: Obj=l_philo[5] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764758 Intern : Obj=l_philo[5] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764771 Disp==>: Obj=l_philo[5] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764781 Intern : Obj=l_philo[5] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764792 Disp==>: Obj=l_philo[5] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764802 Intern : Obj=l_philo[5] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764845 Disp==>: Obj=l_philo[3] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764855 Intern : Obj=l_philo[3] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252764868 Disp==>: Obj=l_philo[3] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3252764878 Intern : Obj=l_philo[3] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3252866042 Disp==>: Obj=l_philo[5] Sig=TIMEOUT_SIG Active=Philo_thinking
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[5] State=Philo_hungry
I/dpp     ( 4010): 3252866120 ==>Tran: Obj=l_philo[5] Sig=TIMEOUT_SIG Source=Philo_thinking New=Philo_hungry
I/dpp     ( 4010): 3252866195 Disp==>: Obj=l_table Sig=HUNGRY_SIG Active=Table_serving
I/dpp     ( 4010): 3252866208 User000: 5 hungry
I/dpp     ( 4010): 3252866222 Intern : Obj=l_table Sig=HUNGRY_SIG Source=Table_serving
I/dpp     ( 4010): 3252866273 Disp==>: Obj=l_philo[3] Sig=TIMEOUT_SIG Active=Philo_thinking
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[3] State=Philo_hungry
I/dpp     ( 4010): 3252866317 ==>Tran: Obj=l_philo[3] Sig=TIMEOUT_SIG Source=Philo_thinking New=Philo_hungry
I/dpp     ( 4010): 3252866365 Disp==>: Obj=l_table Sig=HUNGRY_SIG Active=Table_serving
I/dpp     ( 4010): 3252866377 User000: 3 hungry
I/dpp     ( 4010): 3252866389 Intern : Obj=l_table Sig=HUNGRY_SIG Source=Table_serving
I/dpp     ( 4010): 3252866436 Disp==>: Obj=l_philo[1] Sig=TIMEOUT_SIG Active=Philo_thinking
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[1] State=Philo_hungry
I/dpp     ( 4010): 3252866481 ==>Tran: Obj=l_philo[1] Sig=TIMEOUT_SIG Source=Philo_thinking New=Philo_hungry
I/dpp     ( 4010): 3252866530 Disp==>: Obj=l_table Sig=HUNGRY_SIG Active=Table_serving
I/dpp     ( 4010): 3252866542 User000: 1 hungry
I/dpp     ( 4010): 3252866553 Intern : Obj=l_table Sig=HUNGRY_SIG Source=Table_serving
I/dpp     ( 4010): 3253004401 Disp==>: Obj=l_philo[0] Sig=TIMEOUT_SIG Active=Philo_eating
I/dpp     ( 4010):            Q_EXIT : Obj=l_philo[0] State=Philo_eating
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[0] State=Philo_thinking
I/dpp     ( 4010): 3253004463 ==>Tran: Obj=l_philo[0] Sig=TIMEOUT_SIG Source=Philo_eating New=Philo_thinking
I/dpp     ( 4010): 3253004523 Disp==>: Obj=l_table Sig=DONE_SIG Active=Table_serving
I/dpp     ( 4010): 3253004534 User000: 0 thinking
I/dpp     ( 4010): 3253004549 Intern : Obj=l_table Sig=DONE_SIG Source=Table_serving
I/dpp     ( 4010): 3253004594 Disp==>: Obj=l_philo[2] Sig=TIMEOUT_SIG Active=Philo_eating
I/dpp     ( 4010):            Q_EXIT : Obj=l_philo[2] State=Philo_eating
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[2] State=Philo_thinking
I/dpp     ( 4010): 3253004641 ==>Tran: Obj=l_philo[2] Sig=TIMEOUT_SIG Source=Philo_eating New=Philo_thinking
I/dpp     ( 4010): 3253004685 Disp==>: Obj=l_table Sig=DONE_SIG Active=Table_serving
I/dpp     ( 4010): 3253004696 User000: 2 thinking
I/dpp     ( 4010): 3253004820 User000: 1 eating
I/dpp     ( 4010): 3253004830 Intern : Obj=l_table Sig=DONE_SIG Source=Table_serving
I/dpp     ( 4010): 3253004874 Disp==>: Obj=l_philo[0] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3253004885 Intern : Obj=l_philo[0] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3253004929 Disp==>: Obj=l_philo[4] Sig=TIMEOUT_SIG Active=Philo_eating
I/dpp     ( 4010):            Q_EXIT : Obj=l_philo[4] State=Philo_eating
I/dpp     ( 4010):            Q_ENTRY: Obj=l_philo[4] State=Philo_thinking
I/dpp     ( 4010): 3253004973 ==>Tran: Obj=l_philo[4] Sig=TIMEOUT_SIG Source=Philo_eating New=Philo_thinking
I/dpp     ( 4010): 3253004984 Disp==>: Obj=l_philo[4] Sig=EAT_SIG Active=Philo_thinking
I/dpp     ( 4010): 3253004994 Intern : Obj=l_philo[4] Sig=EAT_SIG Source=Philo_thinking
I/dpp     ( 4010): 3253005036 Disp==>: Obj=l_table Sig=DONE_SIG Active=Table_serving

3. The QP Port to Android NDK
..............................................................................

3.1 The qep_port.h Header File
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨
<nothing special?>

3.2 The qs_port.h Header File
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨
<nothing special?>

3.3 The qf_port.h Header File
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

typedef struct PThreadTypeTag {                       /* Thread type struct */
    pthread_t thread;
    bool running;
} PThreadType;

Special thread type is defined to allow joining the pthread on exit. Without this, the Android process lifecycle might leave dangling threads.


3.4 The qf_port.c Source File
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨
QActive_start()

SCHED_FIFO scheduling policy is used for PThreads. This allows setting separate priorities for active objects.


Q_ALLEGE(pthread_attr_setdetachstate(&attr, 
                                         PTHREAD_CREATE_JOINABLE) == 0);

The threads for Active Objects are created as JOINABLE, so that the main thread can ensure termination of all pthreads before exiting. This remedies the problems caused by Android process lifecycle.

3.5 The qs_port.c Source File
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

QSTimeCtr QS_onGetTime(void)

clock_gettime() is used to achieve 1 usec accuracy with large enough roll over value.

3.6 DPP example bsp.c file
¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

Q_onAssert()

Assertion is implemented using __android_log_assert() command. This command causes signal 11 to be sent to the process for termination.

void BSP_displayPhilStat(uint8_t n, char const *stat)

BSP display-callbacks only modify the values that are drawn on next animation thread frame draw.

QF_run()

Main loop blocks while the animation is stopped, waiting for events to arrive. When animation is running, frames are constantly re-drawn.

android_main()

This is the entry point to the native application from "native application glue" component, originating from the Java NativeActivity class via JNI. Android makes sure that the .bss variables are initialized when the library is first loaded. On concecutive start-ups the variables are not cleared. Because of this, the .bss is manually cleared on every startup.

QSPY_onPrintLn()

The QSPY records are forwarded to android logging facility via
__android_log_print() call.


4 Related Documents and References
..............................................................................
