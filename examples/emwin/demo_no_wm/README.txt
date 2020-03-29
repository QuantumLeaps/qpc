About this Example
==================
This directory contains QP example applications for the emWin
GUI library from SEGGER, which is also identical to the uC/GUI
product from Micrium. The demos run on Windows as emWin GUI
simulation supplied by SEGGER.

NOTE: The emWin example provided in examples\emwin has been built and
tested against emWin 6.10 (SeggerEval_WIN32_MSVC_MinGW_GUI_V610.zip).


Executable Examples
===================
For your convenience the demo comes with pre-compiled executables,
which you can run directly on your PC. The executables are located
in the following sub-directories:

examples\
 +-emWin\
 | +-demo_no_wm\
 | | +-Debug\
 | | | +-demo_no_win.exe   - Windows executable
 | |
 | +-demo_with_wm\
 | | +-Debug\
 | | | +-demo_with_win.exe - Windows executable


Building the QP Examples
========================
In order to re-build the examples from source files, you will need
to first download and install the "emWin Simulation & Trial version"
directly from SEGGER (https://www.segger.com/downloads/emwin)  

***
SEGGER expressly forbids re-distribution of the emWin code and
libraries, so these essential elements could not be provided with
this QP distribution.
*** 

The "emWin Simulation & Trial version" should be installed in
the 3rd_part\emWin directory. Please see the README.txt file
in this directory for more details.