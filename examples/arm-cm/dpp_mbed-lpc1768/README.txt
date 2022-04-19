About this Example
==================
This directory contains the "Dining Philosophers Problem" (DPP) example
for the preemptive QK kernel running on the NXP mbed-LPC1768 board
(ARM Cortex-M3). This directory contains portable code that should
compile with any C compiler for ARM Cortex-M.

The sub-directory "gnu" contains the Makefile for a generic GNU-ARM
toolset (e.g., see http://gnutoolchains.com/arm-eabi/). Thus, this
project provides a way of building mbed applications locally with
free and unrestricted tools.

***
NOTE: To build the code on Windows, you need to download and install
the GNU make utility. The Qtools collection from Quantum Leaps contains
GNU make and other UNIX-style utilites for Windows (native Windows,
without the need to install CygWin).
***

After building the code with the provided Makefile, you can simply
copy the binary image to the mbed folder for execution.

The whole build process and loading the image to the mbed board can
be executed directly from the QM modeling (by means of external tools).
The provided QM model (dpp.qm) comes pre-configured with tools
setup to build (via make) and copy the code to the mbed board.


Support Code for NXP mbed-LPC1768 Board
=======================================
The directory 3rd_party\mbed-lpc1768 contains the CMSIS-compliant
device code for the NXP LPC176xx MCUs (ARM Cortex-M3). Please see the
README.txt file in this folder for more details.


Note About the Board Support Package for NXP mbed-LPC1768
=========================================================
The provided Board Support Package (see bsp.c) is minimal for the
application at hand and is completely **standalone**, meaning that
it does NOT use the extensive mbed driver library. The use of the
mbed-library is possible, but not necessary (but requires C++).


QS Software Tracing Instrumentation
===================================
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART0 of the mbed-LPC1768 board,
which is connected to the virtual COM port of the mbed USB connection.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from mbed-LPC1768 :

qspy -cCOM20

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.


