::============================================================================
:: Batch file to load the DPP program to the flash of STM32 boards
:: 
:: NOTE: requires the ST-LINK utlity from ST, see:
:: http://www.st.com/en/embedded-software/stsw-link004.html
:: 
@echo off
setlocal

@echo Load the program to the flash of STM32 board
@echo usage: flash dbg\dpp.bin

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: STLINK utility on your machine 
::
set STLINK="C:\tools\ST\ST-LINK\ST-LINK Utility\ST-LINK_CLI.exe"

@echo on
%STLINK% -P %1 0x08000000

@echo off
endlocal
