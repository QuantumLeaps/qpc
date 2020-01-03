@echo off
:: ==========================================================================
:: Product: QP/C script for generating Doxygen documentation
:: Last Updated for Version: 6.7.0
:: Date of the Last Update:  2019-12-27
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
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: <www.state-machine.com>
:: <info@state-machine.com>
:: ==========================================================================
@setlocal

@echo usage:
@echo make
@echo make -CHM

@set VERSION=6.7.0

@set DOXHOME="C:\tools\doxygen\bin"

:: Generate Resource Standard Metrics for QP/C ............................... 
@set RCMHOME="C:\tools\MSquared\M2 RSM"

@set RSM_OUTPUT=metrics.dox
@set RSM_INPUT=..\include\*.h ..\src\*.h ..\src\qf\*.c ..\src\qv\*.c  ..\src\qk\*.c  ..\src\qxk\*.c

@echo /** @page metrics Code Metrics > %RSM_OUTPUT%
@echo.>> %RSM_OUTPUT%
@echo @code{c} >> %RSM_OUTPUT%
@echo                    Standard Code Metrics for QP/C %VERSION% >> %RSM_OUTPUT%

%RCMHOME%\rsm.exe -fd -xNOCOMMAND -xNOCONFIG -u"File cfg rsm_qpc.cfg" %RSM_INPUT% >> %RSM_OUTPUT%

@echo @endcode >> %RSM_OUTPUT%
@echo */ >> %RSM_OUTPUT%

:: Generate Doxygen Documentation ........................................... 
if "%1"=="-CHM" (
    @echo Generating HTML...
    %DOXHOME%\doxygen.exe Doxyfile-CHM
    
    @echo Adding custom images...
    xcopy preview.js tmp\
    xcopy img tmp\img\
    @echo img\img.htm >> tmp\index.hhp

    @echo Generating CHM...
    "C:\tools\HTML Help Workshop\hhc.exe" tmp\index.hhp
    
    @echo.
    @echo Cleanup...
    @rmdir /S /Q  tmp
    @echo CHM file generated in qpc.chm

) else (
    @echo.
    @echo Cleanup...
    rmdir /S /Q  C:\qp_lab\qpc\html
    
    @echo Adding custom images...
    xcopy preview.js C:\qp_lab\qpc\html\
    xcopy img C:\qp_lab\qpc\html\img\
    copy images\favicon.ico C:\qp_lab\qpc\html

    @echo Generating HTML...
    %DOXHOME%\doxygen.exe Doxyfile
    @qclean C:\qp_lab\qpc\html
)

@endlocal
