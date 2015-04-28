About this Example
==================
This folder contains the simple "Blinky" QP example application.
This directory contains portable code that should compile with any C
compiler for ARM Cortex-M.

The sub-directories contain code and project files, which are
specific to the particular ARM toolchains, such as ARM-KEIL (MDK-ARM),
GNU, and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Support Code for STM32 NUCLEO-L053R8 Board
------------------------------------------
The directory qpc\3rd_party\nucleo-l053r8 contains the CMSIS-
compliant device code for the STM32L0xx MCUs (ARM Cortex-M0+). Please see
the README file in this folder for more details.


QS Software Tracing Instrumentation
===================================
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART2, which is connected to
the virtual COM port of the ST-Link V2 USB debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from NUCLEO board:

qspy -cCOM20

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.

