About this Example
==================
This directory contains a QP example application for the
EFM32-SLSTK3401A board (Pearl Gecko Starter Kit). This directory
contains portable code that should compile with any C compiler for
ARM Cortex-M.

The sub-directories contain code and project files, which are specific
to the particular ARM toolchains, such as ARM (MDK-ARM), GNU, and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Support Code for EFM32-SLSTK3401A Board 
=======================================
The directory 3rd_party\efm32pg1b contains the CMSIS-compliant
device code for the EFM32PG1B200F256GM48 MCU. Please see the README.txt
file in this folder for more details.


QS Software Tracing Instrumentation
-----------------------------------
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART0, which is connected to
the virtual COM port of the TI Stellaris debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from EFM32-SLSTK3401A:

qspy -u -c COM1             (Windows)

qspy -u -c /dev/ttyS1       (POSIX)

The actual serial-port number might be different on your workstation.
Please check the Device Manager to find the COM port number.


Win32-GUI Emulation
===================
The sub-directory "win32-gui" provides the emulation of the example
on Windows GUI, either single-threaded (win32-qv) or multithreded (win32).
This sub-directory contains the Makefile for the GNU-GCC toolset (MinGW)
and Visual Studio solution file (dpp-gui.sln) for Visual C++.

The Win32-GUI emulation is based on the QWin™ GUI Prototyping Toolkit, see:

https://www.state-machine.com/qtools/qwin.html

The emulation uses exactly the same code as the embedded board and differs
only in the Board Support Package (bsp.c). This example demonstrates the
"dual targeting" development approach, where most of the embedded code is
developed on the workstation (Windows), but is intended for a deeply
embedded target (EFM32-SLSTK3401A here).


QS Software Tracing Instrumentation
-----------------------------------
The "win32-gui" emulation also supports the "Spy" build configuration,
in which case it attempts to connect to the QSPY host application via
a TCP/IP socket. This requires launching the QSPY host application with
the command-line parameter -t

qspy -u -t


QSpyView Visualization
======================
The sub-directory "qspyview" provides the QSpyView Visualization
example for the DPP application, see:

https://www.state-machine.com/qtools/qspyview.html

This QSpyView Visualization works with all versions of the software
running in the Spy build configuration, including the code for the
EFM32-SLSTK3401A board and the Win32-GUI emulation.
