![QP Framework](https://www.state-machine.com/img/qp_banner.jpg)

# What's New?

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/QuantumLeaps/qpc)](https://github.com/QuantumLeaps/qpc/releases/latest)

View QP/C Revision History at: https://www.state-machine.com/qpc/history.html

> **NOTE:** If you're interested in the latest QP/C version from GitHub,
it is recommended that you clone this repo like that:

```
git clone https://github.com/QuantumLeaps/qpc --recurse-submodules --depth 1
```

Alternatively, you can also download one of the stable
[QP/C Releases][QP-Rel].


# Getting Started with QP/C
The most recommended way of obtaining QP/C is by downloading the
[QP-bundle](https://www.state-machine.com/#Downloads), which includes QP/C
as well as the [QM modeling tool][QM] and the [QTools collection][QTools].
The main advantage of obtaining QP/C bundled together like that is
that you get all components, tools and examples ready to go.

### Getting Started Resources
- ["QP/C Tutorial"][Tutorial]
describes a series of progressively advanced QP/C example applications.

- [Video: "Getting Started with QP Real-Time Embedded Frameworks"][Video]
provides instructions on how to download, install, and get started with QP.

- [AppNote: "Getting Started with QP Real-Time Embedded Frameworks"][AN]
contains also a tutorial, in which you build a simple "Blinky" application.

## Licensing
The QP frameworks (QP/C and QP/C++) are licensed under the
[dual licensing model](https://www.state-machine.com/licensing), with
the following licensing options:

1. [Open-source licensing](https://www.state-machine.com/licensing#Open) under the
[GNU General Public License (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.en.html).

> NOTE: GPL requires that all modifications to the original code
as well as your application code (Derivative Works as defined in the
Copyright Law) must also be released under the terms of the GPL
open source license.

2. [Closed-source licensing](https://www.state-machine.com/licensing#Closed) under one of
[Quantum Leaps commercial licenses](https://www.state-machine.com/licensing#Commercial),
which are specifically designed for users interested in retaining the
proprietary status of their code.

> NOTE: If your company has a policy forbidding open source in your product,
all QP frameworks can be licensed commercially, in which case you don't use
any open source license and you do not violate your policy.


# About QP/C
QP/C (Quantum Platform in C) is a lightweight, open source
[Real-Time Embedded Framework (RTEF)][RTEF] for building modern embedded
software as systems of asynchronous, event-driven [Active Objects][Active]
(actors). The [QP/C] framework is a member of a [QP] family consisting of
[QP/C] and [QP/C++] frameworks, which are strictly quality controlled,
thoroughly documented, and [commercially licensable][Lic].

## Safer Model of Concurrency
The [QP] framework family implements the
[Active Object model of computation][AO_model], which is **inherently safer**
than the traditional "shared state concurrency" based on explicit mutual
exclusion and managing RTOS threads by blocking. The Active Object model
supports and automatically enforces the following best practices
of concurrent programming:

- Keep data isolated and bound to Active Objects' threads. Threads should
hide (**encapsulate**) their private data and other resources, and not
share them with the rest of the system.

- Communicate among Active Object threads **asynchronously** via [Event
objects][Event]. Using asynchronous events keeps the threads running truly
independently, **without blocking** on each other.

- Active Object threads should spend their lifetime responding to incoming
events, so their mainline should consist of an **event-loop** that handles
events one at a time (to completion), thus avoiding any concurrency hazards
within an Active Object thread itself.

This architecture also provides higher level of abstraction and the *correct*
abstractions to effectively apply [Hierarchical State Machines][HSM],
**modeling** and **code generation** to deeply embedded real-time systems.

## Hierarchical State Machines
The behavior of Active Objects is specified in QP/C by means of
[Hierarchical State Machines][HSM] (UML statecharts). The framework
supports manual coding of UML state machines in C as well as automatic
**code generation** by means of the free [QM modeling tool][QM].

## Built-in Real-Time Kernels
The QP/C framework can run on standalone on single-chip microcontrollers,
without any traditional RTOS. The framework contains a selection of
**built-in real-time kernels**, such as the [non-preemptive QV kernel][QV],
the [preemptive non-blocking QK kernel][QK], and the preemptive,
[dual-mode QXK kernel][QXK] that provides all the features you might expect
from a traditional RTOS. Native QP ports and ready-to-use examples are provided
for major CPUs, such as ARM Cortex-M (M0/M0+/M3/M4/M7/M23/M33/M85).

## Traditional RTOS/OS
QP/C can also work with a traditional RTOS, such as ThreadX, embOS, FreeRTOS,
uC/OS-II and Zephyr, as well as with (embedded) Linux (POSIX) and Windows.

## Popularity and Maturity
With 20 years of continuous development, [400+ commercial licensees][Cust],
and many times more open source users worldwide, the QP frameworks are the
most popular such offering on the market. They power countless electronic
products ranging from implantable medical devices to complex weapon systems.


# QP/C Documentation
The online HTML documentation for the **latest** version of QP/C is located
at: https://www.state-machine.com/qpc

The offline HTML documentation for **this** particular version of QP/C
is located in the sub-folder [html](html) (included in the [QP/C releases][QP-Rel]).
To view the offline documentation, open the file [html/index.html](html/index.html)
in your web browser.


# How to Get Help?
- [Free Support Forum](https://sourceforge.net/p/qpc/discussion/668726)
- [Bug Reports](https://sourceforge.net/p/qpc/bugs/)
- [Feature Requests](https://sourceforge.net/p/qpc/feature-requests/)
- [Quantum Leaps website](https://www.state-machine.com)
- [Quantum Leaps licensing](https://www.state-machine.com/licensing)
- [info@state-machine.com](mailto:info@state-machine.com)


# How to Help this Project?
If you like this project, please give it a star (in the upper-right corner of your browser window):

![GitHub star](https://www.state-machine.com/img/github-star.jpg)

   [RTEF]:   <https://www.state-machine.com/rtef>
   [QP]:     <https://www.state-machine.com/products/qp>
   [QP/C]:   <https://github.com/QuantumLeaps/qpc>
   [QP/C++]: <https://github.com/QuantumLeaps/qpcpp>
   [QM]:     <https://github.com/QuantumLeaps/qm>
   [QTools]: <https://github.com/QuantumLeaps/qtools>
   [QP-Rel]: <https://github.com/QuantumLeaps/qpc/releases>
   [Active]: <https://www.state-machine.com/qpc/srs-qp_ao.html>
   [AO_model]: <https://www.state-machine.com/qpc/srs-qp_ao.html#srs-qp_ao-model>
   [Event]:  <https://www.state-machine.com/qpc/srs-qp_evt.html>
   [HSM]:    <https://www.state-machine.com/qpc/srs-qp_sm.html>
   [Lic]:    <https://www.state-machine.com/licensing>
   [Cust]:   <https://www.state-machine.com/customers>
   [AN]:     <https://www.state-machine.com/doc/AN_Getting_Started_with_QP.pdf>
   [Tutorial]: <https://www.state-machine.com/qpc/gs_tut.html>
   [Video]:  <https://youtu.be/O7ER6_VqIH0>
