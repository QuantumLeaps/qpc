About this Example
==================
This example demonstrates how to use the IAR EWARM IDE to build
a QP application.


IAR Project File
----------------
The IAR EWARM project file provided with this example uses relative paths
to the QP/C framework location (includes, port, and libraries. These
relative paths must be modified when the project is moved to different
relative location.


Stack Size and Heap Size
------------------------
In this project, the size of the C stack and heap are determined in
the linker script dpp-qk.icf (see the next section).


Linker Script
-------------
The IAR linker script provides a template of the recommended linker script
for QP applications. This file needs to be customized to set the
application-specific sizes of the Stack and Heap. This file can be edited
from the IAR EWARM IDE via the Project Options/Linker settings.


Selecting QK Exception
======================
The QK kernel needs a dedicated exception to return to the thread
context after preemption. The default is to use the NMI exception
for that purpose. However, in case NMI is needed for some other
purpose, the QK port allows you to select a any, otherwise unused
IRQ for that purpose. To choose a given IRQ, you need to define
the macros QK_USE_IRQ_NUM and QK_USE_IRQ_HANDLER. These macros can
be provided on the command-line to the compiler.

For example, for the EFM32 CPU, you might dedicate the IRQ
"CRYPTO_IRQHandler" (see the vector table), with IRQ number 25,
as follows:

QK_USE_IRQ_HANDLER=CRYPTO_IRQHandler
QK_USE_IRQ_NUM=25


Startup Code
============
The startup code for the TM4C123GH6PM MCU used in this project is
located in the "3rd_party" folder in the following location:

3rd_party\efm32pg1b\iar\startup_efm32pg1b.s

The file startup_efm32pg1b.s provides a template of the recommended
startup for QP applications and should be easily customizable for other
ARM Cortex-M microcontrollers.

The startup file typically does not need to be modified or adapted for
applications. It provides only weak definitions of all exception and
interrupt handlers, as well as the assert_failed() function.

The weak function assert_failed() defined in this file might be re-defined
in the application to customize it for the application-specific error-
handling policy.

***
NOTE: The function assert_failed() typically should NOT use the stack,
because stack might be corrupted by the time this function is called.
Also, assert_failed() is intended to handle catastrophic errors and
should NOT return.
***
