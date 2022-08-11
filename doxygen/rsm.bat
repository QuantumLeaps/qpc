@echo off
::  ==========================================================================
::  Product: QP/C script for running MSquared Resource Standard Metrics (RSM)
::  Last Updated for Version: 5.5.0
::  Date of the Last Update:  2015-09-01
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
:: https://state-machine.com
:: mailto:info@state-machine.com
::  ==========================================================================
setlocal

set RCMHOME="C:\tools\MSquared\M2 RSM"

set RSM_OUTPUT=qpc_metrics.txt
set RSM_INPUT=..\include\*.h ..\source\*.h ..\source\*.c

%RCMHOME%\rsm.exe -fd -n -xNOCOMMAND -xNOCONFIG -u"File cfg rsm_qpc.cfg" %RSM_INPUT% > %RSM_OUTPUT%

endlocal
