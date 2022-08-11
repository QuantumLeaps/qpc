About uC-OS2 for ARM Cortex-M Examples
=======================================
This folder contains examples for uC-OS2 V2.93 running on ARM Cortex-M.
This uC-OS2 version has been released under the permissive Apache License,
Version 2.0 (see GitHub https://github.com/SiliconLabs/uC-OS2 ).


The Blinky Example
------------------
The sub-folder blinky_ek-tm4c123gxl contains a simple "Blinky" example for
the EK-TM4C123GLX board (Cortex-M4F). This example shows a bare-bones
application coded by hand and without many features. The example contains
projects for the ARM (MDK-ARM) compiler and the IAR compiler.


The DPP Examples
----------------
The sub-folders dpp_ek-tm4c123gxl and dpp_nucleo-l152re demonstrate
the Dining Philosopher Problem (DPP) for EK-TM4C123GLX board (Cortex-M4F)
and the STM32 NUCLEO-L152RE (Cortex-M3) boards, respectively. The DPP
example showcases most of the QP features and much of its code has been
generated automatically by the QM graphical modeling tool. The DPP examples
contain projects for the ARM (MDK-ARM) compiler and the IAR compiler.


uC-OS2 Source and QP-Port
=========================
The QP-Port to uC-OS2 is located in qpc\ports\uC-OS2

The uC-OS2 source code and ports are located in 3rd_party\uC-OS2.
Please make sure to read the README.md file found in that directory.
