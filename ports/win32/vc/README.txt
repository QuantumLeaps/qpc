The projects in the vc directory use the Microsoft Visual C++
Express 2010 compiler. This version can be downloaded for
free from Microsoft. To develop plain C/C++ applications,
you additionally need the Microsoft Platform SDK and a resource
editor, such as ResEdit for graphical applications.

The provided Visual C++ projects files assume that the environment
variable QPC is defined and it points to the location of the QP/C
framework. For example, assuming that you have installed QP/C into
the directory C:\qp\qpc, you should define the environment variable
QPC to C:\qp\qpc, as described below.


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

