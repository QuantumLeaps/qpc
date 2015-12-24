/*! @mainpage Getting Started

@tableofcontents

@htmlonly
<img style="float:right; margin:0 8px 8px 15px; clear:right;" src="img/logo_qp.jpg" title="QP">
@endhtmlonly

@section about About QP/C
<p><a class="extern" target="_blank" href="http://www.state-machine.com/qp"><strong>QP/C&trade; (Quantum Platform in C)</strong></a> is a lightweight, open source <a class="extern" target="_blank" href="http://www.state-machine.com/doc/concepts.html#Framework">active object (actor) framework</a> for building responsive and modular real-time embedded applications as systems of cooperating, event-driven <a class="extern" target="_blank" href="http://www.state-machine.com/doc/concepts.html#Active">active objects</a> (<a href="http://en.wikipedia.org/wiki/Actor_model">actors</a>). The QP/C&trade; framework is a member of a larger family consisting of QP/C, <a href="http://www.state-machine.com/qpcpp" target="_blank" class="extern">QP/C++</a>, and <a href="http://www.state-machine.com/qpcn" target="_blank" class="extern">QP-nano</a> frameworks, which are all strictly quality controlled, thoroughly documented, and <a href="http://www.state-machine.com/licensing" target="_blank" class="extern">commercially licensable</a>.
</p>

All QP&trade; frameworks can run on @ref exa_native "bare-metal single-chip microcontrollers", completely replacing a traditional Real-Time Operating System (RTOS). The frameworks contain a selection of built-in real-time kernels, such as the cooperative @ref comp_qv "QV kernel", the preemptive non-blocking @ref comp_qk "QK kernel", and the preemptive, blocking @ref comp_qxk "QXK kernel", <span class="highlight">which provides all the features you might expect from a traditional <strong>RTOS kernel</strong></span>. @ref ports_native "Native QP ports" and ready-to-use @ref exa_native "examples" are provided for major @ref exa_ref_mcu "CPU families".

QP/C can also work with many traditional @ref exa_rtos "RTOSes" and @ref exa_rtos "desktop OSes" (such as Windows and Linux).

The behavior of active objects is specified in QP by means of
<a class="extern" target="_blank" href="http://www.state-machine.com/doc/concepts.html#HSM">hierarchical state machines (UML statecharts)</a>. The frameworks support manual coding of UML state machines in C or C++ as well as fully automatic code generation by means of the free graphical <a class="extern" target="_blank" href="http://www.state-machine.com/qm">QM&trade; modeling tool</a>.

The QP frameworks are used in <a href="http://www.state-machine.com/about/customers.html" target="_blank" class="extern">millions of products worldwide</a> in aerospace, medical devices, consumer electronics, wired and wireless telecommunications, industrial automation, transportation, robotics, and many more. The QP frameworks and the <a class="extern" target="_blank" href="http://www.state-machine.com/qm">QM modeling tool</a> receive over <a class="extern" target="_blank" href="http://sourceforge.net/projects/qpc/files/stats/timeline?dates=2015-01-01+to+2015-12-31">40,000 downloads a year</a>. The book, <a class="extern" target="_blank" href="http://www.state-machine.com/psicc2" >Practical UML Statecharts in C/C++, 2nd Edition</a> provides a detailed design study of the QP frameworks and explains all the related concepts.


------------------------------------------------------------------------------
@section start Downloading and Installing QP/C
QP/C is available for download from <a class="extern" target="_blank" href="http://sourceforge.net/projects/qpc/files/QP-C">SourceForge.net</a>--the world's biggest open source repository. The summary of available downloads is also available from <a class="extern" target="_blank" href="http://www.state-machine.com/downloads">Quantum Leaps download page</a>. 

The Quantum Leaps Application Note <a class="extern" target="_blank" href="http://www.state-machine.com/doc/AN_Getting_Started_with_QPC.pdf"><strong>Getting Started with QP/C</strong></a> provides step-by-step instructions on how to download, install, and get started with QP/C quickly.

@htmlonly
<div class="image">
<a target="_blank" href="http://www.state-machine.com/doc/AN_Getting_Started_with_QPC.pdf"><img border="0" src="img/AN_Getting_Started_with_QPC.jpg" title="Download PDF"></a>
<div class="caption">
Application Note: Getting Started with QP/C
</div>
</div>
@endhtmlonly

The standard QP/C distribution contains also many @ref exa "Example Projects", which are specifically designed to help you learn to use QP/C and to serve you as starting points for your own projects.


------------------------------------------------------------------------------
@section qpc_files Directories and Files

The following annotated directory tree lists the top-level directories provided in the standard QP/C distribution.

<ul class="tag">
  <li><span class="img folder">qpc</span>
  </li>
  <ul class="tag">
    <li><span class="img folder">3rd_party</span> &mdash; Third-Party code used in the QP/C @ref ports "ports" and @ref exa "examples"
    </li>
    <li><span class="img folder">examples</span> &mdash; @ref exa "QP/C Examples"
    </li>
    <li><span class="img folder">ports</span> &mdash; @ref ports "QP/C Ports"
    </li>
    <li><span class="img folder">include</span> &mdash; Platform-independent QP/C API (see <a href="dir_d44c64559bbebec7f509842c48db8b23.html"><strong>include</strong></a>)
    </li>
    <li><span class="img folder">source</span> &mdash; Platform-independent QP/C source code (see @ref <a href="dir_b2f33c71d4aa5e7af42a1ca61ff5af1b.html"><strong>source</strong></a>)
    </li>
  </ul>
</ul>


------------------------------------------------------------------------------
@section support Help and Support
Please post any **technical questions** to the <a class="extern" target="_blank" href="http://sourceforge.net/p/qpc/discussion/668726"><strong>Free Support Forum</strong></a> hosted on SourceForge.net. Posts to this forum benefit the whole community and are typically answered the same day.

Direct **Commercial Support** is available to the commercial licensees. Every commercial license includes one year of Technical Support for the licensed software. The support term can be extended annually.

Training and consulting services are also available from Quantum Leaps. Please refer to the <a class="extern" target="_blank" href="http://www.state-machine.com/contact.html">Contact web-page</a> for more information.


------------------------------------------------------------------------------
@section licensing Licensing QP/C

QP/C is licensed under the increasingly popular <a class="extern" target="_blank" href="http://www.state-machine.com/licensing">dual licensing model</a>, in which both the open source software distribution mechanism and traditional closed source software distribution models are combined.

@note If your company has a policy forbidding open source in your product, all QP frameworks can be licensed commercially, in which case you don't use any open source license and you do not violate your policy.

Open Source Projects:
---------------------
If you are developing and distributing open source applications under the GNU General Public License (GPL), as published by the Free Software Foundation, then you are free to use the Quantum Leaps software under the <a class="extern" target="_blank" href="http://www.gnu.org/copyleft/gpl.html">GPL version 3</a> of the License, or (at your option) any later version. Please note that GPL requires that all modifications to the original code as well as your application code (Derivative Works as defined in the Copyright Law) must also be released under the terms of the GPL open source license.

Closed Source Projects:
-----------------------
If you are developing and distributing traditional closed source applications, you can purchase one of <a class="extern" target="_blank" href="http://www.state-machine.com/licensing/index.html#Commercial">Quantum Leaps commercial licenses</a>, which are specifically designed for users interested in retaining the proprietary status of their code. All Quantum Leaps commercial licenses expressly supersede the GPL open source license. This means that when you license Quantum Leaps software under a commercial license, you specifically do not use the software under the open source license and therefore you are not subject to any of its terms. 


------------------------------------------------------------------------------
@section contact Contact Information

- Quantum Leaps Web site:   <a class="extern" target="_blank" href="http://www.state-machine.com">www.state-machine.com</a>

- Quantum Leaps licensing:  <a class="extern" target="_blank" href="http://www.state-machine.com">www.state-machine.com/licensing</a>

- QP/QM on SourceForge.net: <a class="extern" target="_blank" href="http://sourceforge.net/projects/qpc">sourceforge.net/projects/qpc</a>

- e-mail: <a class="extern" target="_blank" href="mailto:info@state-machine.com">info@state-machine.com</a>

@image html logo_ql_TM.jpg
Copyright &copy; 2002-2015 Quantum Leaps, LLC. All Rights Reserved.

@next{struct}
*/

