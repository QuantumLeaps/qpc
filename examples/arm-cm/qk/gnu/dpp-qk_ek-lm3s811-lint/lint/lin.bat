@echo off
:: ===========================================================================
:: Product: QP/C build script for PC-Lint(TM)
:: Last updated for version 5.3.0
:: Last updated on  2014-02-22
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) Quantum Leaps, www.state-machine.com.
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
:: Web:   www.state-machine.com
:: Email: info@state-machine.com
:: ===========================================================================
setlocal

:: Options for calling lin.bat
::
:: -DQ_SPY              // for linting the Spy configuration (default for QS)
:: up to 4 other options, if needed

:: NOTE: adjust to for your installation directory of PC-Lint
:: 
set PC_LINT_DIR=C:\tools\lint

set LINTFLAGS=-i%PC_LINT_DIR%\lnt %QPC%\include\std.lnt options.lnt %1 %2 %3 %4

set PROJ_DIR=..

:: linting -------------------------------------------------------------------
%PC_LINT_DIR%\lint-nt -os(lint_out.txt) %LINTFLAGS% %PROJ_DIR%\philo.c %PROJ_DIR%\table.c

endlocal
