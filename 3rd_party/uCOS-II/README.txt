About uCOS-II
=============
uCOS-II (a.k.a. uC/OS-II or MicroC/OS-II) is a a Real Time Operating
System (RTOS) kernel developed and licensed by Micriµm, Inc.


uCOS-II Source Code
===================
The folder "Source" contains complete source code of uCOS-II V2.92.10.


uCOS-II Ports
=============
The folder "Ports" contains the generic uCOS-II Ports to ARM-Cortex-M3
and ARM-Cortex-M4.
 
***
NOTE: The ports have been modified by Quantum Leaps to remove the
dependencies on the Micrium's uC-CPU and uC-LIB components, and instead
to inter-operate with the Cortex Microcontroller Software Interface
Standard (CMSIS).
***

Modifications for CMSIS
-----------------------
Specifically, the names of the standard exception handlers PendSV and SysTick
used by the uC/OS-II ports have been renamed to be compliant with the CMSIS
naming convention.

Additionally, the Cortex-M4F ports have been modified to include the FPU
configuration that is expected by the uC/OS-II port to Cortex-M4F, which
was different from the FPU configuration performed by the CMSIS-compliant
startup code. Specifically, the explicit FPU configuration has been
OSInitHookEnd() function in the file os_cpu_c.c.


Licensing Information
=====================
uCOS-II is a product of Micriµm, Inc. and is licensed as follows:

uC/OS-II is provided in source form for FREE evaluation, for educational
use or for peaceful research. If you plan on using uC/OS-II  in a
commercial product you need to contact Micrium to properly license
its use in your product. We provide ALL the source code for your
convenience and to help you experience uC/OS-II. The fact that the source
is provided does  NOT  mean that you can use it without  paying a
licensing fee.

Please refer to the Micriµm website at: 

http://www.micrium.com

for more information regarding the licensing of uC/OS-II.
