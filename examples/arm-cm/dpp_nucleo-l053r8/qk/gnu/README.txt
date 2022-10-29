About this Example
==================
This example can be built in two different ways:
- from the command prompt with the provided Makefile.
- from the Atollic TRUEStudio-ST32 with the provided project.


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
for ARM Cortex-M projects, due to the support for the hardware FPU
(float-abi=hard).
***


The TRUEStudio Project
======================
The project file for the Atollic TRUEStudio-STM32 is provided and can be
directly imported into the TRUEStudio Eclipse-based IDE. All three build
configurations are supported (Debug, Release and Spy).


Adjusting Stack and Heap Sizes
==============================
The stack and heap sizes are determined in this project by the GCC linker
script (.ld file), which provides a template of the recommended GCC linker
script for QP applications.


Startup Code
============
The startup code for the STM32L0xx MCU used in this project is
located in the "3rd_party" folder in the following location:

3rd_party\nucleo-l053r8\gnu\startup_stm32l053xx.c

The file startup_stm32l053xx.c provides a template of the recommended
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


Adjusting the CPU Clock Speed
=============================
The current setting is to run at 2MHz from the MSI (internal oscillator),
but the CPU clock speed can be modified by editing the file
system_stm32l1xx.c. Ther file system_stm32l0xx.c.pll provides an example
of clock setting using the PLL driven from the MSE.

***
NOTE:
The NUCLEO boards have a wide range of possible clock selections, depending
on the solder bridge configuration. Please see Chapter 5.7 "OSC clock" in
the STM32 NUCLEO Boards User Manual (ST document UM1724) for more information.
***

