About this Example
==================
This example can be built from the command prompt with the provided
Makefile. The example can also be imported as a Makefile-based
project into Eclipse-based IDEs.


The Makefile
============
The provided Makefile should be easy to adapt for your own projects.
It contains three build configurations: Debug (default), Release, and
Spy.

Also, the Makefile has been specifically designed to work as an external
Makefile with the Eclipse CDT.

The various build configurations are built as follows:

make
make CONF=rel
make CONF=spy

make clean
make CONF=rel clean
make CONF=spy clean

***
NOTE:
The installation folder of the GNU-ARM toolset on YOUR machine needs
to be adjusted in the provided Makefile, by editing the symbol: GNU_ARM.
As described in the comment for this symbol, the GNU-ARM toolset is taken
from: http://gnutoolchains.com/arm-eabi

It is highly recommened to use the same GNU-ARM distribution, especially
for ARM Cortex-M4F projects, due to the support for the hardware FPU
(float-abi=hard).
***


Adjusting Stack and Heap Sizes
==============================
The stack and heap sizes are determined in this project by the GCC linker
script (.ld file), which provides a template of the recommended GCC linker
script for QP applications.


Selecting QXK Exception
=======================
The QXK kernel needs a dedicated exception to return to the thread
context after preemption by a basic thread. The default is to use
the NMI exception for that purpose. However, in case NMI is needed
for some other purpose, the QXK port allows you to select a any,
otherwise unused IRQ for that purpose. To choose a given IRQ, you
need to define the macros QXK_USE_IRQ_NUM and QXK_USE_IRQ_HANDLER.
These macros can be provided on the command-line to the compiler.

For example, for the EFM32 MCU, you might dedicate the IRQ
"CRYPTO_IRQHandler" (see the vector table), with IRQ number 25,
as follows:

QXK_USE_IRQ_HANDLER=CRYPTO_IRQHandler
QXK_USE_IRQ_NUM=25


Startup Code
============
The startup code for the EFM32PG1B200F256GM48 MCU used in this project
is located in the "3rd_party" folder in the following location:

qpc\3rd_party\efm32pg1b\gnu\startup_efm32pg1b.c

The file startup_efm32pg1b.c provides a template of the recommended
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

