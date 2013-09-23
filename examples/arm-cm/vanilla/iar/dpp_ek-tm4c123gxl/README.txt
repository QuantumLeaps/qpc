The project file provided with this example assumes that
the environment variable QPC is defined and it points to the
location of the QP/C framework. For example, assuming that
you have installed QP/C into the directory C:\software\qpc,
you should define the environment variable QPC to 
C:\software\qpc.
 
Editing the Environment Variables on Windows
--------------------------------------------
Open the Windows Explorer and right-click on the Computer
icon. Then select Properties from the pop-up menu. This opens
a window with the basic information about your computer. In the
left panel of this window, click on "Advanced system settings".
This opens the dialog "System Properties". Click the
"Environment Variables..." button.

To add a new environment variable, click the "New..." button
in the System variables section and provide the variable name
(e.g., QPC) and value (e.g., C:\software\qpc).

****
NOTE: After updating envrinment variables, you shold reboot your
computer so that all applications, such as IAR Embedded Workbench
can pick up the changes.
****