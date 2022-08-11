::============================================================================
:: Batch file to program the flash of EFM32-SLSTK3401A board
:: 
:: NOTE: requires the J-Link commander (JLink.exe) from SEGGER, see:
:: https://www.segger.com/j-link-commander.html
:: 
setlocal

@echo off
@echo Load a given binary file to the flash of EFM32-SLSTK3401A 
@echo usage:   flash bin-file
@echo example: flash dbg\blinky-qk.bin

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: JLink utility on your machine
::
if [%JLINK%] EQU [] set JLINK=%QTOOLS%\..\JLink
if not exist "%JLINK%\JLink.exe" (
    @echo The JLink tool not found. Please adjust flash.bat 
    @goto end
)

if ["%~1"]==[""] (
    @echo The binary file missing
    @goto end
)
if not exist %~s1 (
    @echo The binary file '%1' does not exist
    @goto end
)

:: generate the Jlink command file depending on the first parameter %1
@echo si 1 >flash.jlink
@echo speed 4000 >>flash.jlink
@echo r >>flash.jlink
@echo h >>flash.jlink
@echo loadbin %1, 0 >>flash.jlink
@echo exit >>flash.jlink

@echo on
%JLINK%\JLink.exe -device EFM32PG1B200F256GM48 flash.jlink
@echo off

@del flash.jlink

:end

endlocal