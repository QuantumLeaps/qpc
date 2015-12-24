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
/*###########################################################################*/ /*!
@defgroup qv QV

@brief
Cooperative Kernel

QV is a simple **cooperative** kernel (previously called "Vanilla" kernel). This kernel executes active objects one at a time, with priority-based scheduling performed before processing of each event. Due to naturally short duration of event processing in state machines, the simple QV kernel is often adequate for many real-time systems. 
*/
/*###########################################################################*/
/*!
@defgroup qk QK

@brief
Preemptive Non-Blocking Kernel

QK is a tiny **preemptive**, priority-based, non-blocking kernel designed specifically for executing active objects. QK meets all the requirement of the Rate Monotonic Scheduling (a.k.a. Rate Monotonic Analysis — RMA) and can be used in hard real-time systems.
*/

/*###########################################################################*//*!
@defgroup qxk QXK

@brief
Preemptive Blocking RTOS Kernel

QXK is a small, preemptive, priority-based, **blocking** kernel that provides typical services of a conventional __RTOS__ (Real-Time Operating System). QXK has been designed specifically for mixing event-driven active objects with traditional blocking code, such as commercial middleware (TCP/IP stacks, UDP stacks, embedded file systems, etc.) or legacy software.

Currently, the QXK kernel has been ported to the following CPUs:

- @ref arm-cm_qxk "Cortex-M (M0/M0+/M1/M3/M4/M4F/M7)"@n
Supported toolchains include: ARM-KEIL MDK, IAR-ARM, GNU-ARM, TI-ARM.

Currently, the QXK kernel is illustrated by the following examples:

- @ref arm-cm_dpp_ek-tm4c123gxl@n
Example illustrates: 6 active objects plus one "naked" thread, QXK blocking delay, QXK semaphore, QXK mutex, QXK blocking message queue.

- @ref arm-cm_dpp_nucleo-l053r8@n
Example illustrates: 6 active objects plus one "naked" thread, QXK blocking delay, QXK semaphore, QXK mutex, QXK blocking message queue.


------------------------------------------------------------------------------
\section qxk_overview QXK Overview

QXK works as most conventional RTOS kernels, by frequently switching the CPU from one thread to another in the process called *context switching*. As shown in the diagram below, each thread of execution (a.k.a. task) in QXK is assigned its own private **stack** area in RAM and its own data structure (often called the *thread control block*). The use of multiple stacks allows the kernel to **block** (suspend) threads at any moment, because the top of the stack of each thread is always available.

@image html qxk_ctx-switch.gif "Context-switching among multiple threads with private stacks"
@n

<div class="separate"></div>
\subsection qxk_classes Classes in QXK
The figure below shows the main classes introduced in the QXK kernel and their relation to the classes of the QP framework.

@image html qxk_classes.gif "Classes of the QXK blocking kernel"

<ul class="tag">
  <li><span class="tag">0</span> The abstract ::QMActive class represents active objects in QP. This class contains the @c thread object of the underlying kernel (QXK thread-control-block in this case) as well as the event queue and the unique priority of the active object.
  </li>

  <li><span class="tag">1</span> The ::QXThread class represents the "naked" blocking threads of the QXK kernel. It inherits ::QMActive, so that "naked" threads can be treated as active objects internally in the framework. However, the "naked" threads do not implement state machines. Instead, the data fields used for storing the current state in active objects are re-used to store the address of the QXK primitive that blocks the "naked" thread. The ::QXThread class also contains the @c timeEvt object (see ::QTimeEvt) for generating timeouts when the "naked" thread is blocked.
  </li>

  <li><span class="tag">2</span> The ::QXMutex class represents the @ref qxk_mutex "priority-ceiling mutex" of the QXK kernel. The mutex can be used by both the "naked" threads and active object threads (in case they share resources that need to be protected). However, using any blocking mechanism inside active objects is not recommended, because it delays run-to-completion event processing.
  </li>

  <li><span class="tag">3</span> The ::QXSemaphore class represents the @ref qxk_sema "counting semaphore" of the QXK kernel. The semaphore can be waited on only in the "naked" threads and the QXK kernel would assert if an active object thread would attempt to wait on a semaphore. On the other hand, a semaphore can be signaled from anywhere in the code, including active objects and ISRs.
  </li>
</ul>

@note
The main takeaway from the QXK class diagram is QXK's **optimal, tight integration** with the QP/C framework. The QXK kernel reuses all mechanisms already provided in QP, thus avoiding any code duplication, inefficient layers of indirection, and additional licensing costs, which are inevitable when using @ref ports_rtos "3rd-party RTOS kernels" to run QP/C applications.


------------------------------------------------------------------------------
\section qxk_features QXK Features
As you can see in the list below, <span class="highlight">QXK provides most features you might expect of a traditional blocking **RTOS** kernel and is <strong>recommended</strong> as the preferred RTOS kernel for QP/C applications</span> that need to mix active objects with traditional blocking code.

<ul class="tag">

<li><span class="bullet">&gt;</span>Preemptive, priority-based scheduling of up to 64 threads. Each thread must be provided with its own private stack space and must be assigned its own unique priority (1..::QF_MAX_ACTIVE);
</li>

> NOTE: QXK always executes the highest-priority thread that is ready to run (is not blocked). The scheduling algorithm used in QXK meets all the requirement of the Rate Monotonic Scheduling (a.k.a. Rate Monotonic Analysis — RMA) and can be used in hard real-time systems.

<li><span class="bullet">&gt;</span>All threads in QXK are capable of **blocking**. However, QXK distinguishes between two types of threads:
</li>

- active object threads that can block only while waiting on events from the private event queue and should not block otherwise (e.g., inside state machines). QXK asserts when an active object thread attempts to use a timed-delay or attempts to block on a semaphore. This is because active objects have non-blocking alternatives for these operations (time-events and event posting, respectively).

- "naked" threads, that can block anywhere in their thread-handler function, whereas QXK provides a typical assortments of blocking primitives for "naked" threads, such as time-delay, message queue, semaphore, or a mutex;

<li><span class="bullet">&gt;</span>Tightly integrated mechanisms for communication between event-driven active objects and "naked" blocking threads:
</li>

- Active objects can signal semaphores and send messages to "naked" threads.

- "Naked" threads can post or publish events to active objects;

<li><span class="bullet">&gt;</span>Priority-Ceiling Mutexes with optional timeout;
</li>

> NOTE: Priority-ceiling protocol implemented in QXK is immune to priority-inversions.

<li><span class="bullet">&gt;</span>Counting Semaphores with optional timeout that can block multiple "naked" threads;
</li>

<li><span class="bullet">&gt;</span>Blocking "zero-copy" message queue with optional timeout bound to each "naked" thread;
</li>

<li><span class="bullet">&gt;</span>Deterministic fixed-size memory pools for dynamic memory management available both to "naked" threads and active objects;
</li>

<li><span class="bullet">&gt;</span>Interrupt management, including "zero-latency", kernel-unaware interrupts that are never disabled;
</li>

> NOTE: This feature is only supported on CPUs that allow selective interrupt disabling, such as ARM Cortex-M3/M4 (but not ARM Cortex-M0).

</ul>


<div class="separate"></div>
\subsection qxk_kernel Kernel Initialization and Control
- QXK_init()
- QF_run()
- QXK_onIdle()
- QXK_getVersion()

<div class="separate"></div>
\subsection qxk_thread Thread Management
- ::QXThread structure (Thread Control Block)
- QXThread_ctor()
- QXTHREAD_START()
- QXThread_block()
- QXThread_unblock()
- QXThread_delay()
- QXThread_delayCancel()

<div class="separate"></div>
\subsection qxk_isr Interrupt Management
- QXK_ISR_ENTRY()
- QXK_ISR_EXIT()

<div class="separate"></div>
\subsection qxk_mutex Mutexes
- ::QXMutex structure (Mutex Control Block)
- QXMutex_init()
- QXMutex_lock()
- QXMutex_unlock()

<div class="separate"></div>
\subsection qxk_queue Message Queues
- QXTHREAD_POST_X() - posting messages to blocking threads
- QACTIVE_POST_X()  - posting events to Active Objects
- QXThread_queueGet() - waiting (blocking) on message queue

<div class="separate"></div>
\subsection qxk_sema Semaphores
- ::QXSemaphore structure (Semaphore Control Block)
- QXSemaphore_init()
- QXSemaphore_wait()
- QXSemaphore_signal()

<div class="separate"></div>
\subsection qxk_mem Memory Pools
- ::QMPool structure
- QMPool_init()
- QMPool_get()
- QMPool_put()

*/

/*###########################################################################*//*! @dir ../include

Platform-independent QP/C API

@note
The QP/C <span class="img folder">include</span> directory needs to be added to the compiler's include path in the applications using QP/C.
*/

/*###########################################################################*//*! @dir ../source

Platform-independent QP/C implementation

Files from this directory need to be added to the project, to build the QP/C framework from source code.

@attention
Not all QP/C source files should be added to every project. For example, native QP/C ports to the preemptive QK kernel should **not** contain the file <span class="img file_c">qv.c</span> and conversely, QP/C ports to the cooperative QV kernel should not contain the files <span class="img file_c">qk.c</span> and <span class="img file_c">qk_mutex.c</span>.

@note
The QP/C <span class="img folder">source</span> directory needs to be added to the compiler's include path in the applications using QP/C, because QP/C is built from sources.

*/