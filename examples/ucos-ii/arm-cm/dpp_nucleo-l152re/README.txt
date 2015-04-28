About this Example
==================
This directory contains the "Dining Philosophers Problem" (DPP) example
for uC/OS-II application running on the STM32 NUCLEO-L152RE board
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

Note About the Clock Speed
--------------------------
With the default setting of 8MHz external clock (HSE_VALUE=8000000),
the SystemCoreClockUpdate() routine in system_stm32l1xx.c sets the
SystemCoreClock variable to 32000000 (32MHz). However, the NUCLEO-L152RE
board appears to run actually only at 16MHz. As a quick fix, the
DPP project directory set the HSE_VALUE to only 4MHz (HSE_VALUE=4000000)
in the command-line options to the compiler, to match the reporeted
SystemCoreClock to the actual speed of the clock (16MHz). At this point
it is not clear why the CPU clock is set to only half of the expected
speed.


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


