/*! @page srs_toc Specification

<h1>Software Requirements Specification (SRS)</h1>
<p>This document specifies the requirements for the **QP/C active object framework**. This document describes the intended features of the QP/C framework, as well as the interfaces to other software, hardware, and various other technical dependencies. The quick links to the main sections of this SRS are given below:
</p>

- @subpage srs_intro
  + @ref srs_purpose
  + @ref srs_conv
  + @ref srs_scope
  + @ref srs_audience
  + @ref srs_refs
- @subpage srs_descr
  + @ref srs_prod
  + @ref srs_funct
  + @ref srs_user
  + @ref srs_assume
- @subpage srs
  + @ref srs_fun
    - @ref srs_qf
    - @ref srs_qep
    - @ref srs_qv
    - @ref srs_qk
    - @ref srs_qxk
  + @ref srs_non-fun
  + @ref srs_constr
- @subpage glossary

@note
This SRS document is included as an integral part of the "QP/C Reference Manual" for bi-directional @termref{traceabilty, traceabilty} of the requirements to and from the design, the code, and the tests. Specifically, the electronic format of all these documents enables creating *hyper-links*, so it possible, for example, to simply click on a hyper-link to a requirement to get to its description.

@next{srs_intro}
*/
/*##########################################################################*/
/*! @page srs_intro Introduction

@section srs_purpose Purpose
<strong>QP/C</strong> (Quantum Platform in C) is a lightweight software @termref{framework, framework} for building responsive and modular real-time embedded applications as systems of cooperating, event-driven  @termref{ao, active objects} (@termref{ao, actors}).


------------------------------------------------------------------------------
@section srs_conv Document Conventions

<div class="separate"></div>
@subsection srs_conv_num Numbering of Requirements


<div class="separate"></div>
@subsection srs_conv_shall Use of "Shall" and "Should"
Requirement definitions use consistent terminology to indicate whether something is mandatory or desirable. _Shall_ is used to denote **mandatory** behavior. _Should_ is used to denote a **desirable** behavior that should typically take place, but might not happen all the time or might be optional in uncommon cases.


------------------------------------------------------------------------------
@section srs_scope Project Scope


------------------------------------------------------------------------------
@section srs_audience Intended Audience

This SRS document is primarily intended for **embedded software engineers**, who develop applications based on the QP/C framework.

This SRS can be also of interest to test engineers, software architects, system engineers, quality-assurance engineers, hardware engineers, as well as managers overseeing the software development.


------------------------------------------------------------------------------
@section srs_refs References



@next{srs_descr}
*/
/*##########################################################################*/
/*! @page srs_descr Overall Description

------------------------------------------------------------------------------
@section srs_prod Product Perspective


------------------------------------------------------------------------------
@section srs_funct Product Functions

- @reqref{RQPC101}
- @reqref{RQPC102}


------------------------------------------------------------------------------
@section srs_user User Characteristics
The main users of the QP/C framework are **embedded software engineers**, who develop applications based on the QP/C framework.



------------------------------------------------------------------------------
@section srs_assume Assumptions and Dependencies

@next{srs}
*/
/*##########################################################################*/
/*! @page srs Requirements


------------------------------------------------------------------------------
@section srs_fun Functional Requirements

<div class="separate"></div>
@subsection srs_qf Active Object Framework
The software shall support hierarchical state nesting

<div class="separate"></div>
@subsection srs_qep State Machine Event Processor


@reqdef{RQPC101, The software shall support hierarchical state nesting}
@par Amplification
State hierarchy is the primary mechanism of behavioral reuse in state machines.
@par Rationale
This is a useful thing to have

@reqdef{RQPC102, The software shall support manual coding}


<div class="separate"></div>
@subsection srs_qv Cooperative Run-to-Completion Kernel


<div class="separate"></div>
@subsection srs_qk Preemptive Run-to-Completion Kernel


<div class="separate"></div>
@subsection srs_qxk Preemptive Blocking Kernel

------------------------------------------------------------------------------
@section srs_non-fun Non-functional Requirements

<div class="separate"></div>
@subsection srs_quality Software Quality Attributes


<div class="separate"></div>
@subsection srs_perform Performance Requirements


<div class="separate"></div>
@subsection srs_port Portability Requirements


<div class="separate"></div>
@subsection srs_safe Safety Requirements


<div class="separate"></div>
@subsection srs_secure Security Requirements


------------------------------------------------------------------------------
@section srs_constr Constraints

<div class="separate"></div>
@subsection srs_standards Compliance with Standards

*/
/*##########################################################################*/
/*! @page glossary Glossary

@termdef{ao, active object (actor)}
Active objects (Actors) are event-driven, strictly encapsulated software objects running in their own threads of control that communicate with one another asynchronously by exchanging events. The UML specification further proposes the UML variant of hierarchical state machines (UML statecharts) with which to model the behavior of event-driven active objects.


@termdef{framework, framework}
A software framework is a universal, reusable software architecture for development of specific class of software (e.g., real-time embedded control systems). A framework typically provides a set of abstract classes and defines their responsibilities and collaborations. Developers customize a framework to a particular software application by subclassing and composing the framework classes. The most important characteristics of this process is that code provided by the application developers is being called by the framework, which results in **inversion of control** compared to using a library such as a traditional operating system.

@termdef{traceabilty, traceabilty}
Traceability is the ability to verify the history, location, or application of an item by means of documented recorded identification.

*/
