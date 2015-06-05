The examples in the win32 directory can be built either with the
GNU comiler (MinGW) or Microsoft Visual C++ (Express 2013).

The Visual C++ toolset is supported by the VC++ solution and project
files files (.sln, and .vcxproj files) also provided with some
example projects. 

The MinGW toolset is supported by the Makefiles provided for every 
example project. The Makefile requires the GNU make uitility for
Windows. Both the MinGW compiler and the make utility are are included
in the Qtools collection, which is available for a separate download
from:

http://sourceforge.net/projects/qpc/files/Qtools/

The <your-choice>\qtools\bin directory should be added to the system
PATH, and the QTOOLS environment variable should be defined to point
to <your-choice>\qtools.


The QP Libraries
================
All projects in this directory link to the QP/C framework, which is
provided in pre-built libraries for the three supported build
configurations (dbg, rel, and spy). The pre-compiled libraries
for are located in the following directoires:

qpc\ports\win32\dbg\libqp.a    - MinGW, Debug build configuration
qpc\ports\win32\rel\libqp.a    - MinGW, Release build configuration
qpc\ports\win32\spy\libqp.a    - MinGW, Spy build configuration

qpc\ports\win32\Debug\qp.lib   - VC++, Debug build configuration
qpc\ports\win32\Release\qp.lib - VC++, Release build configuration
qpc\ports\win32\QSpy\qp.lib    - VC++, Spy build configuration

Typically these libraries can be used "as is" for your projects

Howver, if you want to change the QP settings, or if you want to use
different compiler options, you need to re-bulid the QP libraries from
sources. The QP port directory qpc\ports\win32 contains the
Makefile for re-building the QP libraries with MinGW as well as the
VC++ solution file to re-build QP libraries with Visual C++. 

