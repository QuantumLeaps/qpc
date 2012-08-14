@echo off
:: ===========================================================================
:: SLS example, 80x86, Vanilla port, Open Watcom compiler
:: Last Updated for Version: 4.5.00
:: Date of the Last Update:  May 24, 2012
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
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

:: If you have defined the WATCOM environment variable, the following line has
:: no effect and your definition is used. However, if the varible WATCOM is
:: not defined, the following default is assuemed: 
if "%WATCOM%"=="" set WATCOM=c:\tools\WATCOM

path %WATCOM%\binw
set INCLUDE=%WATCOM%\H
set CC=wcc.exe
set AS=wasm.exe
set LD=wlink.exe

if not "%1"=="rel" goto dbg
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-d0 -ml -3 -fpi87 -ot -dNDEBUG
    set LDFLAGS=@link_rel.rsp
goto compile
:dbg
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-d2 -ml -3 -fpi87
    set LDFLAGS=@link_dbg.rsp

:: compile -------------------------------------------------------------------
:compile

mkdir %BINDIR%
set SRCDIR=.
set CCINC=@inc_qp.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\bsp.obj      %SRCDIR%\bsp.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\main.obj     %SRCDIR%\main.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\num_ent.obj  %SRCDIR%\num_ent.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\ui.obj       %SRCDIR%\ui.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\ui_help.obj  %SRCDIR%\ui_help.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\ui_nu_lr.obj %SRCDIR%\ui_nu_lr.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\ui_num.obj   %SRCDIR%\ui_num.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\video.obj    %SRCDIR%\video.c
@echo off


:: link ----------------------------------------------------------------------
@echo on
%LD% %LDFLAGS%
@echo off
