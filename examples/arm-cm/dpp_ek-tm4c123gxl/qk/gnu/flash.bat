::============================================================================
:: Batch file to load the DPP program to the flash of EK-TM4C123GXL
:: (equivalent to EK-LM4F120XL)
:: 
:: NOTE: requires the LMFlash programmer from Texas Instruments, see:
:: http://www.ti.com/tool/LMFLASHPROGRAMMER
:: 
@echo off
setlocal

@echo Load the program to the flash of EK-TM4C123GXL
@echo usage: flash
@echo usage: flash rel
@echo usage: flash spy

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: LMFlash utility on your machine 
::
set LMFLASH=%QTOOLS%\..\LM_Flash_Programmer\LMFlash.exe

:: set the build directory depending on the first parameter %1
set BUILD_DIR=dbg
if [%1] NEQ [] set BUILD_DIR=%1
@echo on

%LMFLASH% -q ek-tm4c123gxl %BUILD_DIR%\dpp-qk.bin

@echo.
@echo.
@echo Reset the target to start running your program!

@echo off
endlocal