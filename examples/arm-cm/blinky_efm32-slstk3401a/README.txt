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
The directory qpc\3rd_party\efm32pg1b contains the CMSIS-compliant
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

qspy -cCOM1

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.
