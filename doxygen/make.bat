@echo off
:: ==========================================================================
:: Product: QP/C script for generating Doxygen documentation
:: Last Updated for Version: 6.9.0
:: Date of the Last Update:  2020-08-24
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
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: <www.state-machine.com/licensing>
:: <info@state-machine.com>
:: ==========================================================================
@setlocal

@echo usage:
@echo make
@echo make -CHM

:: Doxygen tool (adjust to your system) ......................................
@set DOXYGEN=doxygen

:: HTML Help tool (needed only with the -CHM option, (adjust to your system) .
@set HHC="C:\tools\HTML Help Workshop\hhc.exe"

:: Simple complexity metrics tool  (adjust to your system) ...................
@set LIZARD=lizard

:: Generate metrics.dox file...
@set METRICS_INP=..\include ..\src -x ..\src\qs\*
@set METRICS_OUT=metrics.dox

@echo /** @page metrics Code Metrics > %METRICS_OUT%
@echo.>> %METRICS_OUT%
@echo @code{cpp} >> %METRICS_OUT%
@echo                    Code Metrics for QP/C >> %METRICS_OUT%

%LIZARD% -m -L500 -a10 -C20 -V %METRICS_INP% >> %METRICS_OUT%

@echo @endcode >> %METRICS_OUT%
@echo */ >> %METRICS_OUT%

:: Generate Doxygen Documentation...
if "%1"=="-CHM" (
    @echo Generating HTML...
    %DOXYGEN% Doxyfile-CHM
    
    @echo Adding custom images...
    xcopy preview.js tmp\
    xcopy img tmp\img\
    @echo img\img.htm >> tmp\index.hhp

    @echo Generating CHM...
    %HHC% tmp\index.hhp
    
    @echo.
    @echo Cleanup...
    @rmdir /S /Q  tmp
    @echo CHM file generated

) else (
    @echo.
    @echo Cleanup...
    rmdir /S /Q  ..\html
    
    @echo Adding custom images...
    xcopy preview.js ..\html\
    xcopy img ..\html\img\
    copy images\favicon.ico ..\html

    @echo Generating HTML...
    %DOXYGEN% Doxyfile
    @qclean ..\html
)

@endlocal
