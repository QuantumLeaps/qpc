::============================================================================
:: Batch file to load the Game program to the flash of EFM32-SLSTK3401A board
:: 
:: NOTE: requires the J-Link commander (JLink.exe) from SEGGER, see:
:: https://www.segger.com/j-link-commander.html
:: 
setlocal

@echo off
@echo Load the program to the flash of EFM32-SLSTK3401A 
@echo usage: flash
@echo usage: flash rel
@echo usage: flash spy

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: JLink utility on your machine 
::
set JLINK=C:\tools\SEGGER\JLink\Jlink.exe

:: set the Jlink command file depending on the first parameter %1
set CMD_FILE=flash_dbg.jlink
if [%1] NEQ [] set CMD_FILE=flash_%1.jlink
@echo on

%JLINK% -device EFM32PG1B200F256GM48 %CMD_FILE%

endlocal