About this Example
==================
This directory contains the facilities for "linting" the application with
PC-Lint-Plus.

To "lint" the application, invoke the batch "lin.bat"  without parameters.
Optionally, you can supply the parameter -dQ_SPY to include the QS
instrumentation in the "linting" process. 

The output from the "linting" is saved in the fle "lin_out.log".

***
NOTE: You need to adjust the symbol PCLP at the top of the lin.bat
file, to point to the directory where PC-Lint-Plus is installed on your
machine.
***


The PC-Lint-Plus Options File
=============================
The "options.lnt" file specifies the linting options. You might need
to modify this file to suit your needs.


****
NOTE: The purpose of this example is NOT to find problems with the
supporting 3rd-party code, which every non-trivial embedded system needs.
Rather, the purpose of this particular example is to check YOUR application
level code, such as your state machines.
****
