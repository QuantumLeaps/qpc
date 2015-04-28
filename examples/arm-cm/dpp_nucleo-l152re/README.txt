About this Example
==================
This directory contains the "Dining Philosophers Problem" (DPP) example
for CMSIS-RTOS RTX application running on the STM32 NUCLEO-L152RE board
(ARM Cortex-M3). This directory contains portable code that should
compile with any C compiler for ARM Cortex-M.

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM) and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Support Code for NUCLEO-L152RE Board 
====================================
The directory qpc\3rd_party\nucleo-l152re contains the CMSIS-compliant
device code for the STM32L152xx MCUs (ARM Cortex-M3). Please see the
README.txt file in this folder for more details.


Adjusting the CPU Clock Speed
=============================
The current setting is to run at 2MHz from the MSI (internal oscillator),
but the CPU clock speed can be modified by editing the file
system_stm32l1xx.c. Ther file system_stm32l1xx_pll.c provides an example
of clock setting using the PLL driven from the MSE.

***
NOTE:
The NUCLEO boards have a wide range of possible clock selections, depending
on the solder bridge configuration. Please see Chapter 5.7 "OSC clock" in
the STM32 NUCLEO Boards User Manual (ST document UM1724) for more information. 
***


QS Software Tracing Instrumentation
===================================
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART2, which is connected to
the virtual COM port of the ST-Link debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from NUCLEO-L152RE:

qspy -cCOM20

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.


