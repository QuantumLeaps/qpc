![QTools Collection](https://www.state-machine.com/img/qwin_banner.jpg)

# About this Directory
This directory contains examples that illustrate the "Dual Targeting"
approach to developing embedded software. Specifically, the examples
run on Windows GUI, but large portions of the code are actually for
deeply embedded boards.


# What is "Dual Targeting"?
When developing embedded code for devices with non-trivial user interfaces,
it often pays off to build a prototype (virtual prototype) of the embedded
system on a PC. The strategy is called "dual targeting", because you develop
software on one machine (e.g., Windows PC) and run it on a deeply embedded
target, as well as on the PC. Dual targeting is the main strategy for
avoiding the "target system bottleneck" in the agile embedded software
development.

> **NOTE**
*Please note that dual targeting does not mean that the embedded device has
anything to do with the PC or Windows. Neither it means that the simulation
must be cycle-exact with the embedded target CPU. Instead, dual targeting
simply means that from day one, your embedded code (typically in C or C++)
is designed to run on at least two platforms: the final target hardware and
your PC. All you really need for this is two C/C++ compilers: one for the PC
and another for the embedded device.*


# About QWin™ GUI Prototyping Toolkit
QWin™ is a free GUI toolkit for prototyping embedded systems on Windows in the
C or C++ programming language, including building realistic embedded front panels
consisting of LCD displays (both graphic and segmented), LEDs, buttons, knobs,
sliders, etc. The implementation is based on the raw Win32 API to provide simple
direct mapping to C/C++ for easy integration with your embedded code.


> **NOTE**
_The problem of building GUI prototypes of embedded devices on the desktop
is very common, yet it seems that most developers try to use C#, .NET,
VisualBasic or other such environments to build prototypes of their embedded
devices. The main shortcoming of all such solutions is that they don't provide
direct binding to C/C++, which complicates the build process and the debugging
of the C/C++ code on the host._


# QWin™ Code
QWin™ is included in the QP/C and QP/C++ frameworks (in the `<qp>\ports\win32`
and `<qp>\ports\win32-qv` directories), where `<qp>` stands for the
installation directory of the qpc or the qpcpp framework.
 from the QTools Collection._


# QWin™ Features
Currently QWin™ provides the following facilities:

* Graphic displays (pixel-addressable) such as graphical LCDs, OLEDs, etc.
  with up to 24-bit color
* Segmented displays such as segment LCDs, and segment LEDs with generic,
  custom bitmaps for the segments.
* Owner-drawn buttons with custom “depressed” and “released” bitmaps and capable
  of generating separate events when depressed and when released.

Additionally, the provided code shows how to handle input sources:
* Keyboard events
* Mouse move events and mouse-wheel events


# QWin™ Documentation
QWin™ GUI Prototyping Toolkit is described in the pplication Note:
[QWIN GUI Kit for Prototyping Embedded Systems on Windows](https://www.state-machine.com/doc/AN_QWin-GUI.pdf).

[![AppNote: QWIN GUI Kit for Prototyping Embedded Systems](https://www.state-machine.com/img/qwin_an.jpg)](https://www.state-machine.com/doc/AN_QWin-GUI.pdf)


# Licensing
The QWin is licensed under the permissive MIT license
(see top-level comments in the files `qwin_gui.h` and `qwin_gui.c`)


# Contact information:
- https://www.state-machine.com
- info@state-machine.com
