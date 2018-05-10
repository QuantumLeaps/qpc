::============================================================================
:: Batch file to load the DPP program to the flash of STM32 boards
:: 
:: NOTE: requires the ST-LINK utlity from ST, see:
:: http://www.st.com/en/embedded-software/stsw-link004.html
:: 
@echo off
setlocal

@echo Load the program to the flash of STM32 board
@echo usage: flash
@echo usage: flash rel
@echo usage: flash spy

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: STLINK utility on your machine 
::
set STLINK="C:\tools\ST\ST-LINK\ST-LINK Utility\ST-LINK_CLI.exe"

:: set the build directory depending on the first parameter %1
set BUILD_DIR=dbg
if [%1] NEQ [] set BUILD_DIR=%1
@echo on

%STLINK% -P %BUILD_DIR%\dpp-qv.bin 0x08000000

@echo.
@echo.
@echo Reset the target to start running your program!

@echo off
endlocal
