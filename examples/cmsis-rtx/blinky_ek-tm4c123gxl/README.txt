About this Example==================This directory contains the simple "Blinky" QP-RTX example application
for the EK-TM4C123GXL board, a.k.a. TivaC LauchPad (ARM Cortex-M4F). This
directory contains portable code that should compile with any C compiler
for ARM Cortex-M.

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM) and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


Limitations of the Blinky Demo
------------------------------
The "Blinky" demo is specifically kept small and simple to demonstrate
the basic bare-bones application quickly.

However, to achieve this simplicity, the demo intentionally omits 
many features of QP, such as: multiple active objects, dynamic events
(event pools), publish-subscribe event delivery, or QS software tracing.

Please refer to more complete examples (such as the "Dining Philosophers
Problem" (DPP) for demonstration of the aforementioned features.


Support Code for EK-TM4C123GXL Board 
====================================
The directory qpc\3rd_party\ek-tm4c123gxl contains the CMSIS-compliant
device code for the TM4C123GH6PM MCU (ARM Cortex-M4F). Please see the
README.txt file in this folder for more details.


