The Makefile provided with this project assumes that the
QP/C framework has been istalled on your machine
and the environment variable QPC has been defined.

Additionally, to build the SPY configuration, the Qtools
collectin must be also istalled on your machine and the
environment variable QTOOLS has been defined.


Editing the Environment Variables on Linux
------------------------------------------
You need to edit the ~/.bash_profile file to add the
following line:

export QPC=~/qpc
export QTOOLS=~/qtools

assuming that you have installed QP/C into ~/qpc and
Qtools into ~/qtools.


Editing the Environment Variables on Mac OS X
---------------------------------------------
You need to edit the ~/.profile file to add the
following line:

export QPC=~/qpc
export QTOOLS=~/qtools

assuming that you have installed QP/C into ~/qpc and
Qtools into ~/qtools.

****
NOTE: After updating envrinment variables, you should log off
your account and then log on again so that all applications can
pick up the changes.
****

