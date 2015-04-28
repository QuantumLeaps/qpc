The examples in the win32 directory can be built either with the
GNU comiler (MinGW GCC C/C++) or Microsoft Visual C++ (Express 2013).

The MinGW toolset is supported by the Makefiles provided for every 
example project. The Makefile requires the GNU make uitility for
Windows. Both the MinGW compiler and the make utility are are included
in the Qtools collection. Qtools\bin directory should be added to the
system PATH, as described below.

The Visual C++ toolset is supported by the VC++ solution and project
files files (.sln, and .vcxproj files) also provided with some
example projects. 


The QP Libraries
----------------
All projects in this directory link to the QP/C framework, which is
provided in pre-built libraries for the three supported build
configurations (dbg, rel, and spy). The pre-compiled libraries
for are located in the qpc\ports\windows\mingw and
qpc\ports\windows\vc for MinGW and VC++ compilers, respectively.
Typically these libraries can be used "as is" for your projects

Howver, if you want to change the QP settings, or if you want to use
different compiler options, you need to re-bulid the QP libraries from
sources. The QP port directory qpc\ports\windows\mingw contains the
Makefile for re-building the QP libraries for the MinGW toolset.
Similarly, the QP port directory qpc\ports\windows\vc constains the
VC++ solution file to re-build QP libraries for the Visual C++ toolset. 

