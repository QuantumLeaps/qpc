@echo off
:: ===========================================================================
:: Product: batch for testing QP/C against MISRA Exemplar Suite with PC-Lint
:: Last Updated for Version: 4.4.00
:: Date of the Last Update:  Jan 23, 2012
:: 
::                     Q u a n t u m     L e a P s
::                     ---------------------------
::                     innovating embedded systems
:: 
:: Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
:: 
:: This software may be distributed and modified under the terms of the GNU
:: General Public License version 2 (GPL) as published by the Free Software
:: Foundation and appearing in the file GPL.TXT included in the packaging of
:: this file. Please note that GPL Section 2[b] requires that all works based
:: on this software must also be made publicly available under the terms of
:: the GPL ("Copyleft").
:: 
:: Alternatively, this software may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GPL and are specifically designed for licensees interested in
:: retaining the proprietary status of their code.
:: 
:: Contact information:
:: Quantum Leaps Web site:  http://www.quantum-leaps.com
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
