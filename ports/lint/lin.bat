@echo off
:: ===========================================================================
:: Product: QP/C build script for PC-Lint(TM), Standard C compiler
:: Last Updated for Version: 5.7.2
:: Date of the Last Update:  2016-09-28
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
:: http://www.state-machine.com
:: mailto:info@state-machine.com
:: ===========================================================================
setlocal

:: Options for calling lin.bat
::
:: -DQ_SPY  // for linting the Spy configuration (default for QS)
:: up to 4 other options, if needed

:: NOTE: adjust to for your installation directory of PC-Lint
:: 
set PC_LINT=C:\tools\lint

if NOT exist "%PC_LINT%" (
    @echo The PC_LINT toolset not found. Please adjust lin.bat 
    goto end
)

set LINTFLAGS=..\..\include\std.lnt options.lnt %1 %2 %3 %4

:: do the linting...
%PC_LINT%\lint-nt -os(lint_qep.log) %LINTFLAGS% -iqv ..\..\source\qep*.c

%PC_LINT%\lint-nt -os(lint_qf.log)  %LINTFLAGS% -iqv ..\..\source\qf*.c 

%PC_LINT%\lint-nt -os(lint_qv.log)  %LINTFLAGS% -iqv ..\..\source\qv*.c

%PC_LINT%\lint-nt -os(lint_qk.log)  %LINTFLAGS% -iqk ..\..\source\qk*.c

%PC_LINT%\lint-nt -os(lint_qxk.log) %LINTFLAGS% -iqxk ..\..\source\qxk*.c

%PC_LINT%\lint-nt -os(lint_qs.log)  %LINTFLAGS% -iqv -DQ_SPY ..\..\source\qs*.c

:end
endlocal
