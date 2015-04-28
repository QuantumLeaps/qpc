About this Example
==================
The dpp_stm32f4-discovery directory contains the DPP example for
the embOS RTOS.

The sub-directories contain code and project files, which are
specific to the particular ARM toolchains, such as IAR-ARM.


Support Code for STM32F4-Discovery
----------------------------------
The directory qpc\3rd_party\stm32f4-discovery contains the CMSIS-
compliant device code for the STM32Fxx MCUs and the ST driver library.
Please see the README file in this folder for more details.


QS Software Tracing Instrumentation
-----------------------------------
The DPP example provides the "Spy" build configuration, which outputs
the QS (Quantum Spy) software tracing data through USART2. To get the
data out ouf the board, you need to connect the TTL/RS232 converter
as follows:

STM32F4-Discovery  | TTL/RS232 Converter
-------------------+------------------------
PA2                | TX
PA3                | RX (currently not used)
VDD                | VCC
GND                | GND

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from STM32F4-Discovery:

qspy -cCOM1

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.

