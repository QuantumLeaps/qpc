/*! @defgroup qep QEP

@brief
Hierarchical Event Processor

QEP is a universal, UML-compliant event processor that enables developers to code UML state machines in highly readable ANSI-C, in which every state machine element is mapped to code precisely, unambiguously, and exactly once (traceability). QEP fully supports hierarchical state nesting, which is the fundamental mechanism for reusing behavior across many states instead of repeating the same actions and transitions over and over again.
*/
/*###########################################################################*/
/*!
@defgroup qf QF

@brief
Active Object (Actor) Framework

QF is a portable, event-driven, real-time framework for execution of active objects (concurrent state machines) specifically designed for real-time embedded (RTE) systems.
*/
/*###########################################################################*/
/*!
@defgroup qk QK

@brief
Preemptive Non-Blocking Kernel

QK is a tiny **preemptive**, priority-based, non-blocking kernel designed specifically for executing active objects. QK meets all the requirement of the Rate Monotonic Scheduling (a.k.a. Rate Monotonic Analysis — RMA) and can be used in hard real-time systems.
*/
/*###########################################################################*/
/*!
@defgroup qv QV

@brief
Cooperative Run-to-Completion Kernel

QV is a simple **cooperative** kernel (previously called "Vanilla" kernel). This kernel executes active objects one at a time, with priority-based scheduling performed before processing of each event. Due to naturally short duration of event processing in state machines, the simple QV kernel is often adequate for many real-time systems. 
*/
/*###########################################################################*/
/*! @dir ../include

Platform-independent QP/C API

@note
The QP/C <span class="img folder">include</span> directory needs to be added to the compiler's include path in the applications using QP/C. 
*/
/*###########################################################################*/
/*! @dir ../source

Platform-independent QP/C implementation

Files from this directory need to be added to the project, to build the QP/C framework from source code.

@attention
Not all QP/C source files should be added to every project. For example, native QP/C ports to the preemptive QK kernel should **not** contain the file <span class="img file_c">qv.c</span> and conversely, QP/C ports to the cooperative QV kernel should not contain the files <span class="img file_c">qk.c</span> and <span class="img file_c">qk_mutex.c</span>.

@note
The QP/C <span class="img folder">source</span> directory needs to be added to the compiler's include path in the applications using QP/C, because QP/C is built from sources. 

*/