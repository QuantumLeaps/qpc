About this Example
==================
This directory contains the "Fly 'n' Shoot" game example application for
the EFM32-SLSTK3401A board (Pearl Gecko evaluation kit) board. This
directory contains portable code that should compile with any C compiler
for ARM Cortex-M.

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM), GCC, and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Support Code for EFM32-SLSTK3401A Board 
=======================================
The directory qpc\3rd_party\efm32pg1b\ contains the CMSIS-compliant
device code for the LM3S811 MCU. Please see the README file in this
folder for more details.


QS Software Tracing Instrumentation
-----------------------------------
The Blinky example provides the "Spy" build configuration, which
outputs the QS (Quantum Spy) software tracing data through UART0,
which is connected to the virtual COM port of the TI Stellaris debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from EFM32-SLSTK3401A:

qspy -cCOM1

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.


Win32 Emulations
================
The sub-directorie win32 and win32-qv provide the emulations of the example
on Windows GUI (with regular Win32 threads and with cooperative QV scheduler,
respectively. These sub-directories contain the Makefiles for the MinGW
toolset and Visual Studio solution files (game-gui.sln) for Visual C++.

The Win32 emulations use exactly the same code as the embedded board and
differ only in the Board Support Package (bsp.c). This examples demonstrate
the "dual targeting" development approach, where most of the embedded code
is developed on the desktop machine (Windows), but is intended for a deeply
embedded target (EFM32-SLSTK3401A here).
