The Makefile provided with this project assumes that the
MinGW\bin directory is added to the PATH.

For example, assuming that you have installed MinGW into
the C:\MinGW directory, please make sure that your PATH
contains C:\MinGW\bin.

Editing the PATH variable on Windows 7
--------------------------------------
Open the Windows Explorer and right-click on the Computer
icon. Then select Properties from the pop-up menu. This opens
a window with the basic information about your computer. In the
left panel of this window, click on "Advanced system settings".
This opens the dialog "System Properties". Click the
"Environment Variables..." button. In the bottom pannel "System
Variables" scroll down to find Path. Click on it to select it.
Then press "Edit..." button. Click on the Variable value edit
field. At the end of the string append ;C:\MinGW\bin (assuming
that you have installed MinGW in C:\MinGW). Make sure that there
is a semicolon ';' between the last entry and the added path.
After this, you don't need to reboot. Just open a new console
window and type path. The updated path should be displayed.
