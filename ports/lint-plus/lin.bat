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
@echo Usage: lin [16bit] [-d...]
@echo examples:
@echo lin -dQ_SPY -dQ_UTEST : use 32bit CPU (default) and define Q_SPY and Q_UTEST
@echo lin 16bit -dQ_SPY     : use 16bit CPU includes and define Q_SPY
@echo.

:: NOTE: adjust to for your installation directory of PC-Lint-Plus
@set PCLP=C:\tools\lint-plus\pclp32.exe

if NOT exist "%PCLP%" (
    @echo The PC-Lint-Plus toolset not found. Please adjust lin.bat
    @goto end
)

:: set the QP/C directory
@set QPC=..\..

if "%1"=="16bit" (
    set LINTFLAGS=std.lnt -i16bit options.lnt %2 %3 %4
    @echo 16bit CPU
) else (
    set LINTFLAGS=std.lnt -i32bit options.lnt %1 %2 %3 %4
    @echo 32bit CPU default
)

:: cleanup
@del *.log

:: linting -------------------------------------------------------------------
%PCLP% -os(lint_qf.log)  %LINTFLAGS% -iqv  ..\..\src\qf\*.c

%PCLP% -os(lint_qv.log)  %LINTFLAGS% -iqv  ..\..\src\qv\*.c

%PCLP% -os(lint_qk.log)  %LINTFLAGS% -iqk  ..\..\src\qk\*.c

%PCLP% -os(lint_qxk.log) %LINTFLAGS% -iqxk ..\..\src\qxk\*.c

%PCLP% -os(lint_qs.log)  %LINTFLAGS% -iqv  ..\..\src\qs\*.c

:end
@endlocal
