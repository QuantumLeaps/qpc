/*##########################################################################*/
/*! @page ports_rtos Ports to Third-Party RTOS

The main purpose of integrating QP/C with conventional RTOSes is to enable you to incorporate various communication stacks (TCP/IP, USB, CAN, etc.) as well as other middleware, which requires the ability to **block** the task code.   

- @subpage cmsis-rtx
- @subpage embos
- @subpage freertos
- @subpage threadx
- @subpage ucos-ii

@note
You do **not** need to use a traditional RTOS just to achieve preemptive multitasking with QP. The @ref comp_qk "preemptive QK kernel", available as part of the QP package, supports preemptive priority-based multitasking and is fully compatible with Rate Monotonic Scheduling to achieve guaranteed, hard real-time performance. The preemptive, run-to-completion QK kernel perfectly matches the run-to-completion execution semantics of active objects, yet it is simpler, faster, and more efficient than any traditional blocking kernel.

*/
/*##########################################################################*/
/*! @page cmsis-rtx CMSIS-RTOS RTX

@image html under_construction.jpg

*/
/*##########################################################################*/
/*! @page embos embOS

@image html under_construction.jpg

*/
/*##########################################################################*/
/*! @page freertos FreeRTOS

@image html under_construction.jpg

*/
/*##########################################################################*/
/*! @page threadx ThreadX

@image html under_construction.jpg

*/
/*##########################################################################*/
/*! @page ucos-ii uC/OS-II

@section ucos-ii_about About the QP Port to uC/OS-II
This directory contains a generic platform-independent QP/C port to uC/OS-II V2.92.

Typically, you should not need to change the files in this directory to adapt the QP-uC/OS port on any CPU/Compiler to which uC/OS-II has been ported, because all the CPU and compiler specifics are handled by the uC/OS-II RTOS.

@note
Currently, the port has been tested only on ARM Cortex-M3 and M4F.


@section ucos-ii_source uC/OS-II Source and ARM Cortex-M3/M4 Ports
The uC/OS-II V2.92 source code and ports are located in 3rd_party@uCOS-II. Please make sure to read about uC/OS-II licensing in the README file found in this directory.

@note
The official Micrium ARM-Cortex-M ports have been modified by Quantum Leaps to remove the dependencies on the Micrium's uC-CPU and uC-LIB components, and instead to inter-operate with the Cortex Microcontroller Software Interface Standard (CMSIS).


------------------------------------------------------------------------------
@section ucos-ii_using Using this QP Port to uC/OS-II
The example projects for this port are located in @ref exa_ucos-ii "examples/ucos-ii/arm-cm/dpp_ek-tm4c123gxl".

Currently, ARM-KEIL and IAR-ARM toolsets are supported (in the <span class="img folder">arm</span> and <span class="img folder">iar</span> sub-directories within this example project).

The example projects use this port by directly including the QP source code (and this port) in the application projects. There is no QP library to build.

However, this port can also be used as a library, in which case you need to build the QP library yourself and include in your project.


<div class="separate"></div>
@subsection ucos-ii_build QP Source Files Needed in this QP Port
Whether you use this QP port as source files or as a library, it is important to note that not all QP soruce files should be included. Here is the list of QP source files needed:

@verbatim
qpc
+-source
| +-qep_hsm.c
| +-qep_msm.c
| +-qf_act.c
| +-qf_defer.c
| +-qf_dyn.c
| +-qf_ps.c
| +-qf_qeq.c
| +-qf_qmact.c
| +-qf_time.c
| |
| +-qs.c        - included only in the Spy build configuration
| +-qs_fp.c     - included only in the Spy build configuration
|
+-ports
| +-ucos-ii
| | +-qf_port.c
@endverbatim

@note
Specifically, the QP source files qf_actq.c and qf_mem.c must **NOT** be included in the build, because this functionality is taken from uC/OS-II.

@image html under_construction.jpg

*/
