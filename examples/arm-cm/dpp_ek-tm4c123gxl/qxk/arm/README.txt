About this Example
==================
This example demonstrates how to use the uVision IDE together with
the MDK-ARM toolchain.

***
NOTE: This example requires installing the following Software Pack
in the Keil uVision: Keil::TM4C_DFP.
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
numerical value of the heap size (for most embedded projects
yyy should be 0, as the using the heap is not recommended).
   

Startup Code
============
The startup code for the TM4C123GH6PM MCU used in this project is
located in the "3rd_party" folder in the following location:

3rd_party\ek-tm4c123gxl\arm\startup_TM4C123GH6PM.s

The file startup_TM4C123GH6PM.s provides a template of the recommended
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
  