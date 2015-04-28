@echo off
::============================================================================
:: Batch file to load the Blinky program to the flash of Arduino-UNO
:: 
:: NOTE: requires the AVRDUDE uitlity which is included in the Qtools
:: collection
:: 
setlocal

@echo off
@echo Load the program to the flash of Arduino-UNO
@echo usage: flash 
@echo usage: flash rel
@echo usage: flash spy
@echo usage: flash dbg COM5
@echo usage: flash rel COM5
@echo usage: flash spy COM5

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: AVRDUDE utility on your machine 
::
set AVRDUDE=avrdude.exe

set BIN_DIR=dbg
if [%1] NEQ [] set BIN_DIR=%1

set COM_PORT=COM5
if [%2] NEQ [] set COM_PORT=%2

%AVRDUDE% -p atmega328p -c arduino -P %COM_PORT% -b 115200 -D -v -U flash:w:%BIN_DIR%\blinky-qk.hex:i

endlocal