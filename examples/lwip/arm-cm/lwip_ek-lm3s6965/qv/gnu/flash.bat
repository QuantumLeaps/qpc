::============================================================================
:: Batch file to load the program to the flash of EK-LM3S6965
:: 
:: NOTE: requires the LMFlash programmer from Texas Instruments, see:
:: http://www.ti.com/tool/LMFLASHPROGRAMMER
:: 
setlocal

@echo off
@echo Load the program to the flash of EK-LM3S6965
@echo usage: flash
@echo usage: flash rel
@echo usage: flash spy

::----------------------------------------------------------------------------
:: NOTE: Adjust the following symbol to the location of the
:: LMFlash utility on your machine 
::
set LMFLASH=%QTOOLS%\..\LM_Flash_Programmer\LMFlash.exe

if NOT exist "%LMFLASH%" (
    @echo The LMFLASH utility not found. Please adjust flash.bat 
    goto end
)

:: set the build directory depending on the first parameter %1
set BUILD_DIR=dbg
if [%1] NEQ [] set BUILD_DIR=%1
@echo on

%LMFLASH% -q ek-lm3s6965 %BUILD_DIR%\lwip-qv.bin

:end

endlocal