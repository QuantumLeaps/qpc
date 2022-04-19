@echo off
:: ===========================================================================
:: Batch script for running PC-Lint-Plus
:: Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
::
:: SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
::
:: This software is dual-licensed under the terms of the open source GNU
:: General Public License version 3 (or any later version), or alternatively,
:: under the terms of one of the closed source Quantum Leaps commercial
:: licenses.
::
:: The terms of the open source GNU General Public License version 3
:: can be found at: <www.gnu.org/licenses/gpl-3.0>
::
:: The terms of the closed source Quantum Leaps commercial licenses
:: can be found at: <www.state-machine.com/licensing>
::
:: Redistributions in source code must retain this top-level comment block.
:: Plagiarizing this software to sidestep the license obligations is illegal.
::
:: Contact information:
:: <www.state-machine.com>
:: <info@state-machine.com>
:: ===========================================================================
@setlocal

:: usage of lin.bat
@echo Usage: lin [16bit] [-d...] [files...]
@echo examples:
@echo lin ..\table.c               : use 32bit CPU (default) for linting ..\table.c
@echo lin 16bit -dQ_SPY ..\philo.c : use 16bit CPU and define Q_SPY for linting ..\philo.c
@echo lin ..\philo.c ..\table.c    : use 32bit CPU for linting ..\philo.c and ..\table.c
@echo.

:: NOTE: adjust to for your installation directory of PC-Lint-Plus
@set PCLP=C:\tools\lint-plus\windows\pclp32.exe

if NOT exist "%PCLP%" (
    @echo The PC-Lint-Plus toolset not found. Please adjust lin.bat
    @goto end
)

:: set the QP/C directory
set QPC=..\..\..\..
set QPC_LINT=%QPC%\ports\lint-plus

if "%1"=="16bit" (
    set LINTFLAGS=%QPC_LINT%\std.lnt -i%QPC_LINT%\16bit options.lnt %2 %3 %4 %5 %6 %7 %8
    @echo 16bit CPU
) else (
    set LINTFLAGS=%QPC_LINT%\std.lnt -i%QPC_LINT%\32bit options.lnt %1 %2 %3 %4 %6 %7 %8
    @echo 32bit CPU (default)
)

:: cleanup
@del *.log


:: linting -------------------------------------------------------------------
%PCLP% -os(lint_out.log) %LINTFLAGS%

:end
@endlocal
