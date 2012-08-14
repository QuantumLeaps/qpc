@echo off
rem ==========================================================================
rem Product: QP/C library buld script for M32C/80 Series, QK, NC308
rem Last Updated for Version: 4.0.01
rem Date of the Last Update:  Oct 29, 2008
rem
rem                    Q u a n t u m     L e a P s
rem                    ---------------------------
rem                    innovating embedded systems
rem
rem Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
rem
rem This software may be distributed and modified under the terms of the GNU
rem General Public License version 2 (GPL) as published by the Free Software
rem Foundation and appearing in the file GPL.TXT included in the packaging of
rem this file. Please note that GPL Section 2[b] requires that all works based
rem on this software must also be made publicly available under the terms of
rem the GPL ("Copyleft").
rem
rem Alternatively, this software may be distributed and modified under the
rem terms of Quantum Leaps commercial licenses, which expressly supersede
rem the GPL and are specifically designed for licensees interested in
rem retaining the proprietary status of their code.
rem
rem Contact information:
rem Quantum Leaps Web site:  http://www.quantum-leaps.com
rem e-mail:                  info@quantum-leaps.com
rem ==========================================================================
setlocal

rem adjust the following path to the location where you've installed
rem the Renesas NC308 toolset...
rem
set RENESAS_NC308_DIR=C:\tools\Renesas\Hew\Tools\Renesas\nc308wa\v541r01

rem
rem Adjust the Target MCU to your project
rem 
set TARGET_MCU=M82

set PATH=%RENESAS_NC308_DIR%\bin;%PATH%
set LIB308=%RENESAS_NC308_DIR%\lib308
set CC=nc308.exe
set ASM=as308.exe
set LIB=lb308.exe

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCFLAGS=-c -g -v -%TARGET_MCU% -Wall
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCFLAGS=-c -O3 -v -%TARGET_MCU% -Wall -DNDEBUG
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCFLAGS=-c -g -v -%TARGET_MCU% -Wall -DQ_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=-C

rem QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qhsm_in.c

%LIB% %LIBFLAGS% %LIBDIR%\qep_%TARGET_MCU%.lib %BINDIR%\qep.r30 %BINDIR%\qfsm_ini.r30 %BINDIR%\qfsm_dis.r30 %BINDIR%\qhsm_ini.r30 %BINDIR%\qhsm_dis.r30 %BINDIR%\qhsm_top.r30 %BINDIR%\qhsm_in.r30
@echo off

rem QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qte_rarm.c

%LIB% %LIBFLAGS% %LIBDIR%\qf_%TARGET_MCU%.lib %BINDIR%\qa_defer.r30 %BINDIR%\qa_fifo.r30 %BINDIR%\qa_lifo.r30 %BINDIR%\qa_get_.r30 %BINDIR%\qa_sub.r30 %BINDIR%\qa_usub.r30 %BINDIR%\qa_usuba.r30 %BINDIR%\qeq_fifo.r30 %BINDIR%\qeq_get.r30 %BINDIR%\qeq_init.r30 %BINDIR%\qeq_lifo.r30 %BINDIR%\qf_act.r30 %BINDIR%\qf_gc.r30 %BINDIR%\qf_log2.r30 %BINDIR%\qf_new.r30 %BINDIR%\qf_pool.r30 %BINDIR%\qf_psini.r30 %BINDIR%\qf_pspub.r30 %BINDIR%\qf_pwr2.r30 %BINDIR%\qf_tick.r30 %BINDIR%\qmp_get.r30 %BINDIR%\qmp_init.r30 %BINDIR%\qmp_put.r30 %BINDIR%\qte_ctor.r30 %BINDIR%\qte_arm.r30 %BINDIR%\qte_darm.r30 %BINDIR%\qte_rarm.r30

rem QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qk_mutex.c

%LIB% %LIBFLAGS% %LIBDIR%\qk_%TARGET_MCU%.lib %BINDIR%\qk.r30 %BINDIR%\qk_sched.r30 %BINDIR%\qk_mutex.r30

rem QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -dir%BINDIR% %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\qs_%TARGET_MCU%.lib %BINDIR%\qs.r30 %BINDIR%\qs_.r30 %BINDIR%\qs_blk.r30 %BINDIR%\qs_byte.r30 %BINDIR%\qs_f32.r30 %BINDIR%\qs_f64.r30 %BINDIR%\qs_mem.r30 %BINDIR%\qs_str.r30

rem --------------------------------------------------------------------------
:clean

erase %BINDIR%\*.r30


endlocal