@echo off
:: ==========================================================================
:: Product: batch script for generating Doxygen documentation
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
:: ==========================================================================
@setlocal

@echo usage:
@echo make
@echo make -CHM
@echo make ...

:: Doxygen tool (adjust to your system) ......................................
@set DOXYGEN=doxygen

:: HTML Help tool (needed only with the -CHM option, (adjust to your system) .
@set HHC="C:\tools\HTML Help Workshop\hhc.exe"

:: Simple complexity metrics tool  (adjust to your system) ...................
@set LIZARD=lizard

:: QP/C directory ............................................................
@set QPC=..

:: HTML outut directory ......................................................
@set HTML_OUT=%QPC%\html

:: Generate metrics.dox file...
@set METRICS_INP=%QPC%\include %QPC%\src -x %QPC%\src\qs\*
@set METRICS_OUT=metrics.dox

@echo /** @page metrics Code Metrics > %METRICS_OUT%
@echo.>> %METRICS_OUT%
@echo @code{cpp} >> %METRICS_OUT%
@echo                    Code Metrics >> %METRICS_OUT%

%LIZARD% -m -L500 -a10 -C20 -V %METRICS_INP% >> %METRICS_OUT%

@echo @endcode >> %METRICS_OUT%
@echo */ >> %METRICS_OUT%

:: Generate Doxygen Documentation...
if "%1"=="-CHM" (
    @echo Generating HTML...
    %DOXYGEN% Doxyfile-CHM

    @echo Adding custom images...
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
    rmdir /S /Q  %HTML_OUT%

    @echo Generating HTML...
    %DOXYGEN% Doxyfile%1

    @echo Adding custom images...
    xcopy img %HTML_OUT%\img\
    xcopy /Y ..\..\ql-doxygen\jquery.js %HTML_OUT%
    rem @qclean %HTML_OUT%
)

@endlocal
