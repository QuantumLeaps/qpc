@echo off
:: ===========================================================================
:: Product: QP/C build script for PC-Lint-Plus, "Standard" C compiler
:: Last Updated for Version: 6.7.0
:: Date of the Last Update:  2019-12-29
::
::                    Q u a n t u m  L e a P s
::                    ------------------------
::                    Modern Embedded Software
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
:: along with this program. If not, see <www.gnu.org/licenses/>.
::
:: Contact information:
:: <www.state-machine.com/licensing>
:: <info@state-machine.com>
:: ===========================================================================
setlocal

:: Options for calling lin.bat

:: NOTE: adjust to for your installation directory of PC-Lint-Plus
set PCLP_DIR=C:\tools\lint-plus
set PCLP=%PCLP_DIR%\pclp32.exe

if NOT exist "%PCLP%" (
    @echo The PC-Lint-Plus toolset not found. Please adjust lin.bat
    goto end
)

:: set the QP/C directory
set QPC=..\..\..\..
set QPC_LINT=%QPC%\ports\lint-plus

set LINTFLAGS=%QPC%\ports\lint-plus\std.lnt options.lnt %1 %2 %3 %4
set PROJ_DIR=..

:: linting -------------------------------------------------------------------
%PCLP% -os(lint_out.log) %LINTFLAGS% %PROJ_DIR%\calc1_sub.c

:end
endlocal
