::============================================================================
:: Batch file to load the DPP program to the flash of EK-LM3S811
:: 
:: NOTE: requires the LMFlash programmer from Texas Instruments, see:
:: http://www.ti.com/tool/LMFLASHPROGRAMMER
:: 
setlocal

@echo off
@echo Load the program to the flash of EK-LM3S811
@echo usage: flash
@echo usage: flash rel
@echo usage: flash spy

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: LMFlash utility on your machine 
::
set LMFLASH=C:\tools\TI\LM_Flash_Programmer\LMFlash.exe

:: set the build directory depending on the first parameter %1
set BUILD_DIR=dbg
if [%1] NEQ [] set BUILD_DIR=%1
@echo on

%LMFLASH% -q ek-lm3s811 %BUILD_DIR%\game-qk.bin

endlocal