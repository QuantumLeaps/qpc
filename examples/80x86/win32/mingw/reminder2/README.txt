The Makefile provided with this project assumes that the 
environment variable QPC is defined and it points to the
location of the QP/C framework. For example, assuming that
you have installed QP/C into the directory C:\software\qpc,
you should define the environment variable QPC to 
C:\software\qpc.
 
Additionally, the provided Makefile assumes that the MinGW
bin directory is added to the PATH. For example, assuming
that you have installed MinGW into the C:\MinGW directory,
please make sure that your PATH contains C:\MinGW\bin.


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
(e.g., QPC) and value (e.g., C:\software\qpc).

To modify the Path variable, look in the bottom pannel "System
Variables" and scroll down to find the variable Path. Click on
it and then press "Edit..." button. Click on the Variable value
edit field. At the end of the string append ;C:\MinGW\bin
(assuming that you have installed MinGW in C:\MinGW). Make sure
that there is a semicolon ';' between the last entry and the
added path.


Editing the PATH variable on Windows
------------------------------------
In the bottom pannel "System Variables" scroll down to find PATH.
Click on it to select it. Then press "Edit..." button. Click on
the Variable value edit field. At the end of the string append
;C:\MinGW\bin (assuming that you have installed MinGW in C:\MinGW).
Make sure that there is a semicolon ';' between the last entry
and the added path. After this, you don't need to reboot. Just
open a new console window and type path. The updated path should
be displayed.

 
****
NOTE: After updating envrinment variables, you logout and login
again so that all applications can pick up the changes.
****

