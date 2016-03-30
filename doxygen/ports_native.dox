/*##########################################################################*/
/*! @page ports_native Native (Bare-Metal) Ports

- @subpage lint (generic C compiler)
- @subpage arm-cm (Cortex-M0/M0+/M3/M4/M4F)
    - @ref arm-cm_qv (ARM-KEIL, GNU-ARM, IAR-ARM toolsets)
    - @ref arm-cm_qk (ARM-KEIL, GNU-ARM, IAR-ARM toolsets)
    - @ref arm-cm_qxk (ARM-KEIL, GNU-ARM, IAR-ARM toolsets)
- @subpage arm7-9 ("classic" ARM)
    - @ref arm7-9_qv (GNU-ARM, IAR-ARM toolsets)
    - @ref arm7-9_qk (GNU-ARM, IAR-ARM toolsets)
- @subpage msp430 ("classic" MSP430 and "extended" MSP430X)
    - @ref msp430_qv (CCS, IAR toolsets)
    - @ref msp430_qk (CCS, IAR toolsets)

*/
/*##########################################################################*/
/*! @page lint PC-Lint

@image html under_construction.jpg

*/
/*##########################################################################*/
/*! @page arm7-9 ARM7/ARM9

@image html under_construction.jpg

@section arm7-9_qk Preemptive QK Kernel 

\includelineno ports/arm7-9/qk/iar/qk_port.s

@section arm7-9_qv Cooperative QV Kernel

*/
/*##########################################################################*/
/*! @page arm-cm ARM Cortex-M

------------------------------------------------------------------------------
@section arm-cm_qv Cooperative QV Kernel
The QV port to ARM Cortex-M has been described in detail in Section 3 of the Quantum Leaps Application Note: <a href="http://www.state-machine.com/doc/AN_QP_and_ARM-Cortex-M-ARM-KEIL.pdf" target="_blank" class="extern" title="Read PDF">QP and ARM Cortex-M with ARM-KEIL</a>. The Application Note focuses on the ARM-KEIL toolset, but the general principles apply to all supported toolsets, such as IAR-ARM, GNU-ARM, and TI-ARM.


------------------------------------------------------------------------------
@section arm-cm_qk Preemptive Non-Blocking QK Kernel
The QK port to ARM Cortex-M has been described in detail in Section 4 of the Quantum Leaps Application Note: <a href="http://www.state-machine.com/doc/AN_QP_and_ARM-Cortex-M-ARM-KEIL.pdf" target="_blank" class="extern" title="Read PDF">QP and ARM Cortex-M with ARM-KEIL</a>. The Application Note focuses on the ARM-KEIL toolset, but the general principles apply to all supported toolsets, such as IAR-ARM, GNU-ARM, and TI-ARM.


------------------------------------------------------------------------------
@section arm-cm_qxk Preemptive Blocking QXK Kernel

The preemptive, blocking QXK kernel works on ARM Cortex-M as follows:

1. The ARM Cortex-M processor executes application code in the Privileged Thread mode, which is exactly the mode entered out of reset. The exceptions (including all interrupts) are always processed in the Privileged Handler mode.

2. QXK uses the Process Stack Pointer (PSP) for handling threads (both active object threads and "naked" blocking-threads) and the Main Stack Pointer (MSP) for handling interrupts and exceptions (such as the PendSV exception).

3. The QXK port uses the @c PendSV (exception number 14) to perform thread-to-thread context switch. The application code (your code) must initialize the Interrupt Vector Table with the addresses of the @c PendSV_Handler exception handler. Additionally, the interrupt table must be initialized with the @c SysTick_Handler exception handler.
@n
> NOTE: QXK uses only the CMSIS-compliant exception and interrupt names, such as @c PendSV_Handler, @c SysTick_Handler, etc.@n
> NOTE: The QXK port specifically does **not** use the SVC exception (Supervisor Call). This makes the QXK ports compatible with various "hypervisors" (such as mbed uVisor or Nordic SoftDevice), which use the SVC exception.

4. The application code (your code) must call the function QXK_init() to set up the stack of the *idle thread* of the QXK kernel before calling QF_run().
@n
> NOTE: Right before starting multitasking, the QXK kernel re-uses the main C-stack as the Main Stack (for interrupts and exceptions), and assigns the provided idle stack to the internal idle thread.

5. You need to explicitly **assign priorities of the all interrupts** used in your application, according to the Application Note: <a href="http://www.state-machine.com/doc/AN_ARM-Cortex-M_Interrupt-Priorities.pdf" target="_blank" class="extern" title="Read PDF">Setting ARM Cortex-M Interrupt Priorities in QP 5.1 and Higher</a>.

6. It is strongly recommended that you do not assign the lowest NVIC priority (0xFF) to any interrupt in your application, because it is used by the PendSV handler. For example, with 3 bits of priority implemented in the NVIC, this leaves the following 7 priority levels for you (listed from the lowest to the highest urgency): 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, and 0x00 (the highest priority).
@n
> NOTE: The prioritization of interrupts, including the PendSV exception, is performed entirely by the NVIC. Because the PendSV has the lowest priority in the system, the NVIC tail-chains to the PendSV exception only after exiting the last nested interrupt.

7. ISRs are written as regular C functions, but they need to call QXK_ISR_ENTRY() before using any QF services, and they must call QXK_ISR_EXIT() after using any of the QF services.

8. ARM Cortex-M enters interrupt context without disabling interrupts. Generally, you should not disable interrupts inside your ISRs. In particular, the QF services (such as QF_PUBLISH(), QF_TICK_X(), and QACTIVE_POST()) should be called with interrupts enabled, to avoid nesting of critical sections.
@n
> NOTE: If you don't wish an interrupt to be preempted by another interrupt, you can always prioritize that interrupt in the NVIC to a higher or equal level as other interrupts (use a lower numerical value of priority).

9. In compliance with the ARM Application Procedure Call Standard (AAPCS), the QXK kernel always preserves the 8-byte alignment of the stack.


<div class="separate"></div>
@subsection arm-com_qxk_vfp Using the VFP
If you have the Cortex-M4F/M7 CPU and your application is compiled with the VFP present, the QXK kernel will enable the VFP along with the VFP automatic state preservation and lazy stacking features. This will case any thread that uses the VFP, to automatically use the Cortex-M exception stack frame with VFP (with 18 VFP registers S0-S15 plus VFP status and stack "aligner"). Also, the QXK context switch will add to this the rest of the VFP registers (S16-S31).

@note
A QXK thread (both an active object thread and a "naked" blocking thread) that uses the VFP will use 136 more bytes of its private stack space than a thread that does not use the VFP. Also, a thread that uses the VFP will take longer to perform the context switch.

*/
/*##########################################################################*/
/*! @page msp430 MSP430

@image html under_construction.jpg

@section msp430_qk Preemptive QK Kernel 

@section msp430_qv Cooperative QV Kernel

*/
