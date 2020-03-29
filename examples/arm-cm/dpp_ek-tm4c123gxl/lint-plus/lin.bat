@echo off
:: ===========================================================================
:: Product: QP/C build script for PC-Lint-Plus
:: Last Updated for Version: 6.8.0
:: Date of the Last Update:  2020-01-26
::
::                    Q u a n t u m  L e a P s
::                    ------------------------
::                    Modern Embedded Software
::
:: Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 3 of the License, or
:: (at your option) any later version.
::
:: Alternatively, this program may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GNU General Public License and are specifically designed for
:: licensees interested in retaining the proprietary status of their code.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program. If not, see <www.gnu.org/licenses/>.
::
:: Contact information:
:: <www.state-machine.com/licensing>
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
@set PCLP=C:\tools\lint-plus\pclp32.exe

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
