About uCOS-II for ARM Cortex-M Examples
=======================================
This folder contains examples for uCOS-II V2.92 running on ARM Cortex-M.


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


uCOS-II Source and QP-Port
==========================
The QP-Port to uCOS-II V2.92 is located in qpc\ports\ucos-ii

The uCOS-II V2.92 source code and ports are located in 3rd_party\uCOS-II.
Please make sure to read about uCOS-II licensing in the README file
found in this directory.
