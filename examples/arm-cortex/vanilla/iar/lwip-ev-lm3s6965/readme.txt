About this example
------------------
This example application demonstrates the lightweight TCP/IP stack called lwIP
(version 1.4.0-rc2) running on top of the QP state machine framework, which are
both briefly described below.

The example is currently configured to use DHCP to obtain the IP address, so
it assumes the presence of a DHCP server, such as a typical Ethernet router.
By modifying the lwipopts.h header file, the application can also be
configured to optain the IP address via AUTOIP (as described in RFC 3927), or
to use a static IP address.

The example provides an HTTP server with simple Server Side Include (SSI) and
Common Gateway Interface (CGI) facility capability, so that dynamic web pages
can be served and the pages can send data or commands to the target. Internal
file system for web pages and a utility (qfsgen) for generating the HTML-files
and images is also provided.

The example also demonstrates UDP. A UDP test application for a Windows or
Linux hosts is provided (qudp).

The QP-lwIP example code assume that you are using the EV-LM3S6965 board
Revision C or higher (please check the back of your board). At board Revision
C Luminary Micro changed the graphical OLED display from OSRAM to RITEK.

NOTE: The provided RITEK driver won't work for the boards with the OSRAM
display. 


About QP-lwIP Integration
-------------------------
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


About QP State Machine Frameworks
---------------------------------
QP™ is a family of very lightweight, open source, state machine-based
frameworks for embedded microprocessors, microcontrollers, and DSPs. QP
enables developing well-structured embedded applications as a set of
concurrently executing hierarchical state machines (UML statecharts). With QP,
coding of modern state machines directly in C or C++ is a non-issue. No big
design automation tools are needed. 
 
Current versions of QP are: QP/C™ and QP/C++™, which require about 4KB of code
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

QP is licensed under the GPL version 2 open source license as well as under
commercial closed-source licenses, which are expressly designed for users, who
wish to retain the proprietary character of their code. More information about
QP can be found at the QP homepage at http://www.state-machine.com. 


About lwIP TCP/IP Stack
-----------------------
lwIP is an open source implementation of the TCP/IP
protocol suite that was originally written by Adam Dunkels of the Swedish
Institute of Computer Science (SICS) but now is being actively developed by a
team of developers distributed world-wide. Since it's release, lwIP has
spurred a lot of interest and has been ported to several platforms and
operating systems. lwIP can be used either with or without an underlying RTOS.

The focus of the lwIP TCP/IP implementation is to reduce the RAM usage while
still having a full scale TCP. This makes lwIP suitable for use in embedded
systems with tens of kilobytes of free RAM and room for around 40 kilobytes of
code ROM. 

lwIP is licensed under the BSD open source license. More information about
lwIP can be found at the lwIP homepage at http://www.sics.se/~adam/lwip/. 


Contact information
---------------------
Quantum Leaps, LLC
103 Cobble Ridge Drive
Chapel Hill, NC 27516
USA

info@quantum-leaps.com
www.state-machine.com


Copyright information
---------------------
The QP state machine frameworks and most of this example are:
Copyright (c) 2002-2010 Quantum Leaps, LLC. All rights reserved.

The lwIP TCP/IP stack is:
Copyright (c) 2001-2004 Swedish Institute of Computer Science.

Parts of the device driver for the Stellaris MCUs are:
Copyright (c) 2007-2009 Luminary Micro, Inc. All rights reserved.
