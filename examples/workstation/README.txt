ABOUT THE "Workstation" EXAMPLES
================================
The examples in the "workstation" directory can be built on any workstation
(Windows, Linux, and MacOS). The provided Makefiles support the
GNU GCC toolchain.

*** NOTE ***
For Windows, the make utility and the GNU GCC toolchain (MinGW) are provided
in the QTools collection, which is available for a separate download from:

http://sourceforge.net/projects/qpc/files/QTools/

*** NOTE ***
The code can be also built with other tools as well, such as the
Microsoft Visual Studio 2013 and newer.


The QP/C Code
=============
For Windows, the QP/C framework used in the examples comes pre-compiled
into a library.

For POSIX platforms (Linux and MacOS), the QP/C framework is built from
sources in every provided project.


Single-Threaded and Multi-Threaded QP/C Ports
---------------------------------------------
Each of the examples can be linked to either the single-threaded QP/C ports
(win32-qv or posix-qv) or multi-threaded ports (win32 or posix). The choice
is made in the Makefiles, by editing the line, which defines the
QP_PORT_DIR symbol. For example, the following lines select the win32-qv
port and leave the win32 port commented-out:

QP_PORT_DIR := $(QPC)/ports/win32-qv
#QP_PORT_DIR := $(QPC)/ports/win32

To reverse the selection, you need to move the comment '#' character.

*** NOTE ***
The single-threaded QP/C ports (win32-qv and posix-qv) are recommended for
emulating software intended for deeply-embedded targets.

The multi-threaded QP/C ports (win32 and posix) are intended for the
actual applications that run on (embedded)Windows or (embedded)Linux.
 

Debug, Release, and Spy Build Configurations
============================================
The Makefiles for the examples generally support the following three build
configurations:

Debug Configuration
-------------------
This is the default build configuration, with full debugging information and
minimal optimization. To build this configuration, type:

make

To clean this build, type

make clean

The object files and the executable is located in the 'build' sub-directory.


Release Configuration
---------------------
This configuration is built with no debugging information and high
optimization. Single-stepping and debugging might be difficult due
to the lack of debugging information and optimized code. To build
this configuration, type:

make CONF=rel

To clean this build, type

make CONF=rel clean

The object files and the executable is located in the 'build_rel' directory.

Spy Configuration
-----------------
This configuration is built with the QP's Q-SPY trace functionality.
The QP/Spy output is performed by a TCP/IP socket and requires launching
the QSPY host application with the -t option. To build this configuration,
type:

make CONF=spy

To clean this build, type

make CONF=spy clean

The object files and the executable are located in the 'build_spy' directory.

*** NOTE ***
Only specific examples support the Spy build configuration. The examples
that don't support it, will report an error or will fail the linking stage.


Support and Contact Information
===============================
https://sourceforge.net/p/qpc/discussion/668726/
https://www.state-machine.com
info@state-machine.com
