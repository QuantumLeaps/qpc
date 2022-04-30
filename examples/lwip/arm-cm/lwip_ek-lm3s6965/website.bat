:: batch file to re-generate the fsdata.h header file from
:: the contents of the "website" directory.
::
:: The qfsgen utility is provided in the Qtools collection
:: (see http://sourceforge.net/projects/qpc/files/Qtools/).
:: The qfsgen utility is assumed to be in the PATH.

qfsgen.exe website %1 %2 %3