Purpose
=======
This directory contains facilities for testing MISRA-C:2004 rule
coverage against the MISRA-C Exemplar Suite (MES) with PC-Lint 9.x.

NOTE: This Readme assumes that you have installed PC-Lint 9.x.


Preparing the MES code
======================
First, you need to open the file lin.bat in this folder and adjust
the symbol PC_LINT_DIR at the top of the file to the PC-Lint directory
on *your* system.

Next, you need to download the MISRA-C Exemplar Suite, which requires
registration.

Point your browser to the MISRA Bulletin Board:

http://www.misra.org.uk/forum: 

Next click on the "Register" button at the top-right corner
(or Login if you are already registered)

After registration, scroll down the Resources Section. Click on the
MISRA C Resources, which opens a new page. On this page, click on the
"MISRA C Exemplar Suite" topic and download the file:

MISRA-C_exemplar_suite_2007_07_17.zip 


After the download, you need to unzip the archive into *this* directory.

NOTE: while unziping, you should be asked if you want to replace the
already existing files mc2_1902.c and mc2_1903.c. Say "NO" or "Skip"
(so that you do NOT overwrite the existing files).


Running the test
================
To run PC-Lint over the MES code, you open a command prompt window,
change to this directory and type "lin.bat". The  PC-Lint output is
saved in the file lint_MES.txt.

You can adjust the PC-Lint options by editing the file options.lnt.
Initially, this file activates only the au-misra2.lnt option file.
But you can also uncomment the QP/C options file lib-qpc.lnt. This
way you can observe and compare what difference does this option file
introduce.

To test your own option file(s) you can add them as command-line
parameter to the lin.bat file, for example:

lin.bat my_lint_options.lnt

or you can add your option file to the options.lnt file.


Contact information
===================
Quantum Leaps, LLC
Web      :  http://www.quantum-leaps.com
            http://www.state-machine.com
e-mail   :  info@quantum-leaps.com
Toll-free:  1-866-450-LEAP (US Eastern Standard Time)

Last updated: 02-Feb-2013
 