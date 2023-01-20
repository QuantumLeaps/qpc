About this Example
==================
This example demonstrates how to use the uVision IDE together with
the MDK-ARM toolchain.

***
NOTE: This example requires installing the following Software Pack
in the Keil uVision: Keil::STM32C0xx_DFP.
***

uVision Project File
====================
The MDK-ARM uVision project file provided with this example uses
relative paths to the QP/C framework location (includes, port, and
libraries. These relative paths must be modified when the project
is moved to different relative location.


Adjusting Stack and Heap Sizes
==============================
The stack and heap sizes are determined in this project by the
command-line options for the ARM assembler (see the Asm tab in
the "Options for Target" dialog box in uVision). Specifically,
you should define symbols: Stack_Size=xxx Heap_Size=yyy, where
xxx represents a numerical value of stack size and yyy the
numberical value of the heap size.


Selecting QK Exception
======================
The QK kernel needs a dedicated exception to return to the thread
context after preemption. The default is to use the NMI exception
for that purpose. However, in case NMI is needed for some other
purpose, the QK port allows you to select a any, otherwise unused
IRQ for that purpose. To choose a given IRQ, you need to define
the macros QK_USE_IRQ_NUM and QK_USE_IRQ_HANDLER. These macros can
be provided on the command-line to the compiler.

For example, (see the vector table), you can define the QK IRQ
as follows:

QK_USE_IRQ_HANDLER=Reserved31_IRQHandler
QK_USE_IRQ_NUM=31


Startup Code
============
The startup code for the STM32C0xx MCU used in this project is
located in the "3rd_party" folder in the following location:

3rd_party\nucleo-c031c6\arm\startup_stm32c031xx.s

The file startup_stm32c031xx.s provides a template of the recommended
startup for QP applications and should be easily customizable for other
ARM Cortex-M microcontrollers.

The startup file typically does not need to be modified or adapted for
applications. It calls the assert_failed() function, which must be
defined in the application to customize it for the application-specific
error-handling policy.


Adjusting the CPU Clock Speed
=============================
The current setting is to run at 12MHz from the MSI (internal oscillator),
but the CPU clock speed can be modified.

***
NOTE:
The NUCLEO boards have a wide range of possible clock selections, depending
on the solder bridge configuration. Please see Chapter 5.7 "OSC clock" in
the STM32 NUCLEO Boards User Manual (ST document UM1724) for more information.
***

