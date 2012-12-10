The Qt project files (.pro files) provided with this project
assume that the environment variable QPC is defined and it
points to the location of the QP/C framework. For example, 
assuming that you have installed QP/C into the directory
C:\qp\qpc, you should define the environment variable QPC to
C:\qp\qpc.


The QP Libraries
----------------
All projects in this directory link to the QP/C framework,
which is provided in pre-built libraries for the two
supported build configurations (debug, release). The pre-
compiled libraries are located in the %QPC%\ports\qt\mingw
directory. Howver, if you want to change the QP settings, or
if you want to use different compiler options, you need to
re-bulid the QP libraries from sources. The QP library
directory contains the makefile for re-building the libraries.


Editing the Environment Variables on Windows
-----------------------------------------------------
Open the Windows Explorer and right-click on the Computer
icon. Then select Properties from the pop-up menu. This opens
a window with the basic information about your computer. In the
left panel of this window, click on "Advanced system settings".
This opens the dialog "System Properties". Click the
"Environment Variables..." button.

To add a new environment variable, click the "New..." button
in the System variables section and provide the variable name
(e.g., QPC) and value (e.g., C:\qp\qpc).


****
NOTE: After updating envrinment variables, you logout and login
again so that all applications can pick up the changes.
****

