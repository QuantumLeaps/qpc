This directory contains the support code for the EK-LM3S811 board.

The sub-directories contain code that is specific to the particular
ARM toolchains, such as ARM (MDK-ARM), GCC, and IAR.


Adjusting the CPU Clock Speed
=============================
The current setting is to run at 20MHz from the PLL, but the CPU
clock speed can be modified by editing the "Clock Configuration"
section in the file system_LM3S811.c. 

