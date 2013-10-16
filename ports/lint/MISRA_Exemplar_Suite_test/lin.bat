@echo off
:: ===========================================================================
:: Product: batch for testing QP/C against MISRA Exemplar Suite with PC-Lint
:: Last Updated for Version: 4.5.04
:: Date of the Last Update:  Feb 01, 2013
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
:: Quantum Leaps Web sites: http://www.quantum-leaps.com
::                          http://www.state-machine.com
:: e-mail:                  info@quantum-leaps.com
:: ===========================================================================
setlocal

:: NOTE: adjust to for your installation directory of PC-Lint
:: 
set PC_LINT_DIR=C:\tools\lint

set LINTFLAGS=%QPC%\include\std.lnt options.lnt %1 %2 %3 %4

::  MISRA exemplar suite -----------------------------------------------------
%PC_LINT_DIR%\lint-nt -os(lint_MES.txt) %LINTFLAGS% *.c

endlocal
