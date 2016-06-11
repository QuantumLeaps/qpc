This directory contains the support code for the EFM32-SLSTK3401A
board (Pearl Gecko Starter Kit).

The sub-directories contain code that is specific to the particular
ARM toolchains, such as ARM (MDK-ARM), GCC, and IAR.


CMSIS-Compliant Device Files
============================
The code also includes the CMSIS-compliant interface to the
EFM32PG1B200F256GM48 MCU files:

efm32pg1b200f256gm48.h
system_efm32pg1b.h
system_efm32pg1b.c
arm\startup_efm32pg1b.s
gcc\startup_efm32pg1b.c
iar\startup_efm32pg1b.s


Adjusting the CPU Clock Speed
-----------------------------
The current setting is to run at 50MHz from PLL, but the CPU clock speed
can be modified by editing the file system_efm32pg1b.c.
