About this Example
==================
This example demonstrates how to use the uVision IDE together with
the MDK-ARM toolchain.

***
NOTE: This example requires installing the following Software Pack
in the Keil uVision: Keil::STM32L0xx_DFP.
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
  

Startup Code
============
The startup code for the STM32L1xx MCU used in this project is
located in the "3rd_party" folder in the following location:

3rd_party\nucleo-l053r8\arm\startup_stm32l053xx.s

The file startup_stm32l053xx.s provides a template of the recommended
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

  