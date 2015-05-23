This directory contains the CMSIS-compliant support code for the
NXT mbed-LPC1768 board.

The sub-directory "gnu" contains code that is specific to the particular
ARM toolchain, such GNU-ARM. Support for other toolchains can be added,
if needed. 


Adjusting the CPU Clock Speed
=============================
The current setting is to run at 96MHz from PLL0, but the CPU
clock speed can be modified by editing the "Clock Configuration"
section in the file system_LPC17xx.c.
 