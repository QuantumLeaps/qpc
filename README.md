![QP framework](https://state-machine.com/img/qp_banner.jpg)

> **NOTE:** If your company has a policy forbidding open source in your
product, all QP frameworks can be
[licensed commercially](https://state-machine.com/licensing),
in which case you don't use any open source license and you do not violate
your policy.

# What's New?
View QP/C Revision History at: https://state-machine.com/qpc/history.html

---------------------------------------------------------------------------
# About QP/C
QP/C (Quantum Platform in C) is a lightweight, open source software
framework/RTOS for building reactvie real-time embedded applications
as systems of cooperating, event-driven active objects (actors). The
QP/C framework is a member of a larger [QP] family consisting of
QP/C, QP/C++, and QP-nano frameworks, which are all strictly quality
controlled, thoroughly documented, and commercially licensable.

## Safer Model of Concurrency
The [QP] framework family is based on the [Active Object] (**actor**)
design pattern, which inherently supports and automatically enforces the
following best practices of concurrent programming:

- Keep data isolated and bound to active objects' threads. Threads should
hide (**encapsulate**) their private data and other resources, and not
share them with the rest of the system.

- Communicate among active object threads **asynchronously** via event
objects. Using asynchronous events keeps the threads running truly
independently, **without blocking** on each other.

- Active object threads should spend their lifetime responding to incoming
events, so their mainline should consist of an **event-loop** that handles
events one at a time (to completion), thus avoiding any concurrency hazards
within an active object thread itself.

This architecture is generally **safer**, more responsive and easier to
understand and maintain than the shared-state concurrency of a conventional
RTOS. It also provides higher level of abstraction and the right
abstractions to effectively apply **modeling** and **code generation** to
deeply embedded real-time systems.

## Hierarchical State Machines
The behavior of active objects is specified in QP/C by means of
[Hierarchical State Machines] (UML statecharts). The framework
supports manual coding of UML state machines in C as well as automatic
**code generation** by means of the free [QM] modeling tool.

## Built-in Real-Time Kernels
The QP/C framework can run on bare-metal single-chip microcontrollers,
completely replacing a traditional RTOS. The framework contains a selection
of **built-in real-time kernels**, such as the cooperative QV kernel, the
preemptive non-blocking QK kernel, and the preemptive, blocking QXK kernel
that provides all the features you might expect from a traditional RTOS.
Native QP ports and ready-to-use examples are provided for major CPUs, such
as ARM Cortex-M (M0/M0+/M3/M4/M7).

## Traditional RTOS/OS
QP/C can also work with a traditional RTOS, such as ThreadX, embOS,
uC/OS-II and TI-RTOS, as well as with (embedded) Linux (POSIX) and Windows.

## Maturity
With 60,000 downloads a year, the [QP] family is the most popular such
solution on the embedded software market. It provides a modern, reusable
architecture for embedded applications, which combines the active-object
model of concurrency with hierarchical state machines.

---------------------------------------------------------------------------
# Getting Started with QP/C
The following "Getting Started with QP/C" guide provides step-by-step
instructions on how to download, install, and get started with QP/C quickly:
https://state-machine.com/doc/AN_Getting_Started_with_QPC.pdf

The guide also contains a tutorial, in which you build a simple "Blinky"
application.

> **NOTE:** QP/C can be unzipped anywhere in your file system, but the
recommended location is `C:\qp\qpc` on Windows and `~/qp/qpc`
on Linux/MacOS.

---------------------------------------------------------------------------
# QP/C Licensing
QP/C is licensed under the increasingly popular
[dual licensing model](https://state-machine.com/licensing), in which both
the open source software distribution mechanism and traditional closed
source software distribution models are combined.

> **NOTE:** If your company has a policy forbidding open source in your
product, all QP frameworks can be
[licensed commercially](https://state-machine.com/licensing), in which case
you don't use any open source license and you do not violate your policy.

---------------------------------------------------------------------------
# QP/C Documentation
The **QP/C Manual** is located online at: https://state-machine.com/qpc

---------------------------------------------------------------------------
# How to get help?
- [Free Support Forum](https://sourceforge.net/p/qpc/discussion/668726)
- [Bug Reports](https://sourceforge.net/p/qpc/bugs/)
- [Feature Requests](https://sourceforge.net/p/qpc/feature-requests/)
- [Quantum Leaps website](https://state-machine.com)
- [Quantum Leaps licensing](https://state-machine.com/licensing)
- [info@state-machine.com](mailto:info@state-machine.com)

   [QP]: <https://state-machine.com/products/#QP>
   [QM]: <https://state-machine.com/qm>
   [Active Object]: <https://state-machine.com/doc/concepts#Active>
   [Hierarchical State Machines]: <https://state-machine.com/doc/concepts#HSM>
