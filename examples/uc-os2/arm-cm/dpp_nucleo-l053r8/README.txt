About this Example
==================
This directory contains the "Dining Philosophers Problem" (DPP) example
for uC/OS-II application running on the NUCLEO-L053R8 board (ARM Cortex-M0+).
This directory contains portable code that should compile with any C
compiler for ARM Cortex-M.

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM) and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Support Code for NUCLEO-L053R8 Board
====================================
The directory qpc\3rd_party\nucleo-l053r8 contains the CMSIS-compliant
device code for the STM32L053 MCU (ARM Cortex-M0+). Please see the
README.txt file in this folder for more details.


QS Software Tracing Instrumentation
===================================
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART0, which is connected to
the virtual COM port of the ST-LINK debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from NUCLEO-L053R8:

qspy -cCOM2

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.
