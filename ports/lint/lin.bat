@echo off
:: ===========================================================================
:: Product: QP/C build script for PC-Lint(TM), Standard C compiler
:: Last Updated for Version: 6.2.0
:: Date of the Last Update:  2018-03-16
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: https://www.state-machine.com
:: mailto:info@state-machine.com
:: ===========================================================================
setlocal

:: Options for calling lin.bat

:: NOTE: adjust to for your installation directory of PC-Lint
set PC_LINT=C:\tools\lint

if NOT exist "%PC_LINT%" (
    @echo The PC_LINT toolset not found. Please adjust lin.bat 
    goto end
)

set LINTFLAGS=..\..\include\std.lnt options.lnt -DQ_SPY %1 %2

:: do the linting...
%PC_LINT%\lint-nt -os(lint_qf.log)  %LINTFLAGS% -iqv  ..\..\src\qf\*.c 

%PC_LINT%\lint-nt -os(lint_qv.log)  %LINTFLAGS% -iqv  ..\..\src\qv\*.c

%PC_LINT%\lint-nt -os(lint_qk.log)  %LINTFLAGS% -iqk  ..\..\src\qk\*.c

%PC_LINT%\lint-nt -os(lint_qxk.log) %LINTFLAGS% -iqxk ..\..\src\qxk\*.c

%PC_LINT%\lint-nt -os(lint_qs.log)  %LINTFLAGS% -iqv  ..\..\src\qs\*.c

:end
endlocal
