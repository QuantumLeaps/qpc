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
@echo make -LATEX

:: tools (adjust to your system)---------------------------------------------
:: Doxygen tool
@set DOXYGEN=doxygen

:: Simple complexity metrics tool  (adjust to your system)
@set METRICS=lizard -m -L500 -a10 -C20 -V

:: HTML Help tool (needed only with the -CHM option) .
@set HHC="C:\tools\HTML Help Workshop\hhc.exe"

:: QP directory .............................................................
@set QP=..

:: Outut directories ........................................................
@set HTML_OUT=%QP%\html
IF "%1"=="-CERT" (
    @set HTML_OUT=%QP%\cert-pack
)
@set LATEX_OUT=%QP%\latex

:: Generate metrics.dox file-------------------------------------------------
@set METRICS_INP=%QP%\include %QP%\src -x %QP%\src\qs\*
@set METRICS_OUT=gen\metrics.txt

@echo @code{.c}> %METRICS_OUT%
%METRICS% %METRICS_INP% >> %METRICS_OUT%
@echo @endcode>> %METRICS_OUT%

:: Generate Doxygen Documentation -------------------------------------------
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

) else if "%1"=="-LATEX" (

    @echo.
    @echo Cleanup...
    rmdir /S /Q  %LATEX_OUT%

    @echo Generating LATEX...
    %DOXYGEN% Doxyfile-LATEX

    @echo Adding custom files...
    xcopy img %LATEX_OUT%\img\

    @cd %LATEX_OUT%
    @call make.bat

) else (

    @echo.
    @echo Cleanup...
    rmdir /S /Q  %HTML_OUT%

    @echo Generating HTML...
    %DOXYGEN% Doxyfile%1

    @echo Adding custom files...
    xcopy img %HTML_OUT%\img\
    xcopy /Y ..\..\ql-doxygen\jquery.js %HTML_OUT%
    rem @qclean %HTML_OUT%
)

@endlocal
