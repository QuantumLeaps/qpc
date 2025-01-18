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

# About QP/C Real-Time Embedded Framework
QP/C real-time embedded framework (RTEF) is a lightweight implementation of
the [Active Object (a.k.a. Actor) model of computation][AOmod] specifically
tailored for deeply embedded real-time systems, such as microcontrollers (MCUs).
QP/C is both a software infrastructure for building applications consisting
of Active Objects (Actors) and a runtime environment for executing the Active
Objects in a deterministic, real-time fashion. Additionally, QP/C Framework
supports Hierarchical State Machines with which to specify the behavior of
Active Objects [UML 2.5], [Sutter:10], [ROOM:94]. The QP/C Framework can be
viewed as a modern, asynchronous, and truly event driven real-time operating
system (RTOS).

## QP Framework Family
QP/C framework is part of the larger QP family consisting of the following
QP editions:

|QP Edition |  Language   |      API         | Safety Functions   |Certification Artifacts| Licensing
|:----------|:-----------:|:-----------------|:-------------------|:----------------|:---------
| QP/C      | C (C11)     |same as SafeQP/C  |Selected Assertions |Req/Arch/Design  | [dual][Lic]
| SafeQP/C  | C (C11)     |same as QP/C      |All Safety Functions|Certification Kit| [commercial][Com]
| QP/C++    | C++ (C++17) |same as SafeQP/C++|Selected Assertions |Req/Arch/Design  | [dual][Lic]
| SafeQP/C++| C++ (C++17) |same as QP/C++    |All Safety Functions|Certification Kit| [commercial][Com]

[The documentation](#documentation) of all QP editions includes the
[Requirements][SRS], [Architecture][SAS], and [Design Specifications][SDS],
which are the best source of information about the underlying concepts,
functionality, architecture, and design of the QP Frameworks and the QP
Applications based on the frameworks.

> **NOTE:** The **SafeQP** frameworks additionally contain **Safety Functions**
required to achieve the higher safety integrity levels and come with much more
extensive [Certification Kits][Cert].


# Getting Started with QP/C
The most recommended way of obtaining QP/C is by downloading the
[QP-bundle](https://www.state-machine.com/#Downloads), which includes QP/C
as well as the [QM modeling tool][QM] and the [QTools collection][QTools].
The main advantage of obtaining QP/C bundled together like that is
that you get all components, tools and examples ready to go.

### Getting Started Resources
- ["QP/C Tutorial"][Tut]
describes a series of progressively advanced QP/C example applications.

- [Video: "Getting Started with QP Real-Time Embedded Frameworks"][Video]
provides instructions on how to download, install, and get started with QP.

- [AppNote: "Getting Started with QP Real-Time Embedded Frameworks"][AN]
contains also a tutorial, in which you build a simple "Blinky" application.

# Licensing
The QP/C real-time embedded framework is licensed under the
[dual licensing model](https://www.state-machine.com/licensing), with
the following licensing options:

1. [Open-source licensing](https://www.state-machine.com/licensing#Open) under the
[GNU General Public License (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.en.html).

> NOTE: The GPL requires that all modifications to the original code
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

## Files Removed from the QP/C Open Source GPL Distribution
Due to the widespread non-compliance with the GPL, as well as infringement on the
[dual-licensing model of QP frameworks][Lic], the following QP/C components
have been **removed from the open-source GPL distribution**:
- QS target-resident software tracing component
- QXK dual-mode kernel

> NOTE: These components are available to the [commercial licensees][Cust] with
the active Support Term. Please contact [Quantum Leaps technical support][Sup]
to get the complete QP/C framework distribution.

> **NOTE:** To request **evaluation** of the complete QP/C framework, please contact
Quantum Leaps at: https://www.state-machine.com/contact


# Documentation
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

<p align="center">
<img src="https://www.state-machine.com/img/github-star.jpg"/><br>
</p>

   [RTEF]:   <https://www.state-machine.com/rtef>
   [QP]:     <https://www.state-machine.com/products/qp>
   [QP/C]:   <https://github.com/QuantumLeaps/qpc>
   [QP/C++]: <https://github.com/QuantumLeaps/qpcpp>
   [Cert]:   <https://www.state-machine.com/products/qp#CERT>
   [QM]:     <https://github.com/QuantumLeaps/qm>
   [QTools]: <https://github.com/QuantumLeaps/qtools>
   [Lic]:    <https://www.state-machine.com/licensing>
   [Com]:    <https://www.state-machine.com/licensing#Commercial>
   [Cust]:   <https://www.state-machine.com/customers>
   [Sup]:    <mailto:support@state-machine.com>
   [AN]:     <https://www.state-machine.com/doc/AN_Getting_Started_with_QP.pdf>
   [Video]:  <https://youtu.be/O7ER6_VqIH0>
   [QS]:     <https://www.state-machine.com/qpc/srs-qp_qs.html>
   [QV]:     <https://www.state-machine.com/qpc/srs-qp_qv.html>
   [QK]:     <https://www.state-machine.com/qpc/srs-qp_qk.html>
   [QXK]:    <https://www.state-machine.com/qpc/srs-qp_qxk.html>
   [SRS]:    <https://www.state-machine.com/qpc/srs-qp.html>
   [SAS]:    <https://www.state-machine.com/qpc/sas-qp.html>
   [SDS]:    <https://www.state-machine.com/qpc/sds-qp.html>
   [Active]: <https://www.state-machine.com/qpc/srs-qp_ao.html>
   [AOmod]:  <https://www.state-machine.com/qpc/srs-qp_ao.html#srs-qp_ao-model>
   [Event]:  <https://www.state-machine.com/qpc/srs-qp_evt.html>
   [HSM]:    <https://www.state-machine.com/qpc/srs-qp_sm.html>
   [QP-Rel]: <https://github.com/QuantumLeaps/qpc/releases>
   [Tut]:    <https://www.state-machine.com/qpc/gs_tut.html>
