About this example
==================
This example application demonstrates the lightweight TCP/IP stack called lwIP
(version 1.4.1) running on top of the QP state machine framework, which are
both briefly described below.

The example provides an HTTP server with simple Server Side Include (SSI) and
Common Gateway Interface (CGI) facility capability, so that dynamic web pages
can be served and the pages can send data or commands to the target. Internal
file system for web pages and a utility (qfsgen) for generating the
HTML-files and images is also provided.

The example also demonstrates UDP. A UDP test application for a Windows or
Linux hosts is provided (qudp).


Building the QP-lwIP Example
----------------------------
The sub-directories "arm", "gnu", and "iar" contain ready-to-use projects
Makefile for the GNU-ARM, and the IAR EWARM workspace, respectively. Please
run the make utility on the Makefile, or open the IAR workspace in IAR EWARM
IDE.

***
NOTE: The build process supports three build configurations: Debug, Release,
and Spy. Please refer to the Makefile or the IAR project to select the
desired build configuration.
***

***
NOTE: This example pushes the limits of the free evaluation versions of the
development tools. The example will barely fit in the 64KB codesize limit
of the IAR EWARM KickStart (with the highest levels of optimization for
size), but unfortunately the Spy configuration will no longer link due to
exceeding the code size limit. Unfortunately also, the example won't compile
with the free evaluation MDK-ARM uVision, because it exceeds the 32KB limit
of this toolset.
***

***
NOTE:
The QP-lwIP example code assume that you are using the EK-LM3S6965 board
Revision C or higher (please check the back of your board). At board Revision
C Luminary Micro changed the graphical OLED display from OSRAM to RITEK.
The provided RITEK driver won't work for the boards with the OSRAM display.
***


Downloading the Code to the EK-LM3S6965 Board
---------------------------------------------
The "gnu" sub-folder contains the flash.bat file to download the binary image
to the board. This batch file requires installation of the free LMFlash utility
from Texas Instruments.

The IAR project in the "iar" sub-folder allows you to download and debug
the code on the board from the EWARM IDE.


Running the QP-lwIP Example
---------------------------
Connect the EK-LM3S6965 board to the wired Ehternet and power it up by
plugging in the USB cable. Immediately after the power-up, you should see
the startup screen on the OLED-display. The status of the Dining Philosophers
should be changing.

The example is currently configured to use DHCP to obtain the IP address,
so it assumes the presence of a DHCP server, such as a typical Ethernet
router. The IP address acquired from the DHCP will be displayed at the top
of the screen. You should type this IP address at your browser, for example:

http://192.168.1.77

This should open an example website served by the EK-LM3S6965 board. This
website contains a few menu item for testing SSI, CGI, and UDP. Please
refer to the instructions on the website to test these features.

NOTE: By modifying the lwipopts.h header file, the application can also be
configured to optain the IP address via AUTOIP (as described in RFC 3927),
or to use a static IP address.


QS Software Tracing Instrumentation
-----------------------------------
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART0 of the EK-LM3S6965 board,
which is connected to the virtual COM port of the mbed USB connection.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from EK-LM3S6965:

qspy -cCOM12

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.


About QP-lwIP Integration
=========================
The QP-lwIP integration has been carefully designed for hard real-time
control-type applications, in which the TCP/IP stack is used to monitor and
configure the device as well as to provide remote user interface (e.g., by
means of a web browser). In particular, The lwIP stack, which is not
preemptive, is strictly encapsulated inside a dedicated active object
(LwIP-Manager), so interrupt locking is unnecessary, which critical for low
interrupt latency. Also, the Ethernet interrupt service routine (ISR) runs
very fast without performing any lengthy copy operations. This means that
hard-real-time processing can be done at the task level, especially when you
use a preemptive QK™ kernel for executing your QP™ application.

The QP-lwIP integration has been also carefully designed for portability. All
hardware-specific code is clearly separated in the Ethernet/lwIP device driver
with the clean interface to the rest of the application.

Please refer to the Quantum Leaps Application Note: "QP and lwIP TCP/IP Stack"
for more information. The Application Note in PDF is provided in the QP-lwIP
example code.


Customizing the Website
-----------------------
The sub-directory "website" contains the website that you want to embed in
your microcontroller to be served by your lwIP Web-Server. You need to
put the files and directories of your website in there.

To be useful in the microcontroller, the content of the "webstie" directory
needs to be converted to the "fsdata.h" header file. The provided batch
file "website.bat" re-generates the website after you change any file in it.
The batch file calls internally the qfsgen.exe utility, which is described
below.


qfsgen Utility to Generate Websites
-----------------------------------
Quantum Leaps provides a cross-platform, command-line utility, called
"qfsgen", which converts all the files in a given folder (your website) into
a C header file that contains a bunch of constant byte arrays representing
your files and directories. This generated file (fsdata.h) is subsequently
included in the fs.c ROM-Based File System implementation.

The "qfsgen" utility (both compiled executable and source code) is
provided in the Qtools collection, available for download from:

https://sourceforge.net/projects/qpc/files/Qtools

***
NOTE: The qfsgen utility is a simple executable (versions are available
for Windows and Linux), which does not require any interpreters to run
(such as Python, etc.)
***

You use the qfsgen utility from command-prompt. First, change current
directory to examples\lwip\arm-cm\lwip_ek-lm3s6965 and from there
execute:

qfsgen website -h

This will recursively generate the file-system corresponding to the
directory website with HTTP headers (-h option).


Utilities to Test UDP
---------------------
Quantum Leaps provides a utility "qudp" to send and receive UDP packets
for testing. The "qudp" utility (both compiled executable and source code)
is provided in the Qtools collection, available for download from:

https://sourceforge.net/projects/qpc/files/Qtools



About QP Active Object Frameworks
=================================
QP is a family of very lightweight, open source, active-object (actor)
frameworks for embedded microcontrollers. QP enables developing well-
structured embedded applications as a set of concurrently executing
hierarchical state machines (UML statecharts). The QP frameworks support
manual coding of UML state machines in C or C++ as well as fully
automatic code generation by means of the free graphical QM modeling tool.

Current versions of QP are: QP/C and QP/C++, which require about 4KB of code
and a few hundred bytes of RAM, and the ultra-lightweight QP-nano, which
requires only 1-2KB of code and just several bytes of RAM.

QP can work with or without a traditional OS/RTOS. In the standalone
configuration, QP can completely replace a traditional RTOS. QP includes a
simple non-preemptive scheduler and a fully preemptive kernel (QK). The QP/C
and QP/C++ frameworks can also work with a traditional OS/RTOS to take
advantage of existing device drivers, communication stacks, and other
middleware.

All versions of QP are described in detail in the book Practical UML
Statecharts in C/C++, Second Edition: Event-Driven Programming for Embedded
Systems by Dr. Miro Samek, (ISBN: 978-0750687065). QP has a strong user
community and has been applied worldwide by some of the largest companies in
their respective industries, such as: consumer electronics, telecommunications
equipment, industrial automation, transportation systems, medical devices,
national defense, and many more.

QP is licensed under the GPL version 3 open source license as well as under
commercial closed-source licenses, which are expressly designed for users, who
wish to retain the proprietary character of their code. More information about
QP can be found at the QP homepage at http://www.state-machine.com.


About lwIP TCP/IP Stack
=======================
lwIP is an open source implementation of the TCP/IP protocol suite that was
originally written by Adam Dunkels of the Swedish Institute of Computer
Science (SICS) but now is being actively developed by a team of developers
distributed world-wide. Since it's release, lwIP has spurred a lot of interest
and has been ported to several platforms and operating systems. lwIP can be
used either with or without an underlying RTOS.

The focus of the lwIP TCP/IP implementation is to reduce the RAM usage while
still having a full scale TCP. This makes lwIP suitable for use in embedded
systems with tens of kilobytes of free RAM and room for around 40 kilobytes
of code ROM.

lwIP is licensed under the BSD open source license. More information about
lwIP can be found at the lwIP homepage at http://www.sics.se/~adam/lwip/.

The lwIP version 1.4.1 (the latest as of this writing) is included in the
qpc\3rd_party\lwip directory. Please refer to this directory for more
information about licensing lwIP and the source.


Contact information
===================
Web:   www.state-machine.com
Email: info@state-machine.com
