The projects in the MinGW directory use the MinGW GCC C/C++
compiler for Windows as well as the GNU make utility. The
compiler as well as GNU make are included in the Qtools
collection. Qtools\bin directory should be added to the
system PATH, as described below.

The Makefiles provided with this project assume that the
environment variable QPC is defined and it points to the
location of the QP/C framework. For example, assuming that
you have installed QP/C into the directory C:\qp\qpc, you
should define the environment variable QPC to C:\qp\qpc.


Editing the PATH variable on Windows
------------------------------------
To modify the Path variable, look in the bottom pannel "System
Variables" and scroll down to find the variable Path. Click on
it and then press "Edit..." button. Click on the Variable value
edit field. At the end of the string append ;C:\tools\qtools\bin
(assuming that you have installed Qtools in this directory).
Make sure that there is a semicolon ';' between the last entry
and the added path.


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

