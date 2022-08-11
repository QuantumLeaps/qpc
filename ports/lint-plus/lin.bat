@echo off
:: ===========================================================================
:: Batch script for linting QP/C with PC-Lint-Plus
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
@echo Usage: lin [16bit] [-d...]
@echo examples:
@echo lin -uQ_SPY -uQ_UTEST : use 32bit CPU (default) and undefine Q_SPY/Q_UTEST
@echo lin 16bit -uQ_SPY : use 16bit CPU includes and undefine Q_SPY
@echo.

:: NOTE: adjust to for your installation directory of PC-Lint-Plus
@set PCLP=C:\tools\lint-plus\windows\pclp64.exe

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
