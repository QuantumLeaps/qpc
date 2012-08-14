@echo off
rem ==========================================================================
rem Product: QP/C buld script for 80251, Vanilla port, Keil compiler
rem Last Updated for Version: 4.0.01
rem Date of the Last Update:  Aug 06, 2008
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
rem the Keil toolset...
rem
set KEIL_C251_DIR=C:\tools\Keil\C251

set PATH=%KEIL_C251_DIR%\BIN;%PATH%
set C251INC=%KEIL_C251_DIR%\INC
set C251LIB=%KEIL_C251_DIR%\INC

set CC=C251.EXE
set ASM=A251.EXE
set LIB=LIB251.EXE

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.


if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCFLAGS="XTINY FUNCTIONS(REENTRANT) OPTIMIZE(6,SPEED) FIXDRK WARNING(disable=138) DEBUG CODE SYMBOLS NOPRINT"
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCFLAGS="XTINY FUNCTIONS(REENTRANT) FIXDRK WARNING(disable=138) CODE SYMBOLS NOPRINT DEFINE(NODEBUG)"
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCFLAGS="XTINY FUNCTIONS(REENTRANT) OPTIMIZE(6,SPEED) FIXDRK WARNING(disable=138) DEBUG CODE SYMBOLS NOPRINT DEFINE(Q_SPY)"
)

set LIBDIR=%BINDIR%

rem QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=INCDIR(%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%)

%CC% %SRCDIR%\qep.c      %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qep.obj)
%CC% %SRCDIR%\qfsm_ini.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qfsm_ini.obj)
%CC% %SRCDIR%\qfsm_dis.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qfsm_dis.obj)
%CC% %SRCDIR%\qhsm_ini.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qhsm_ini.obj)
%CC% %SRCDIR%\qhsm_dis.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qhsm_dis.obj)
%CC% %SRCDIR%\qhsm_top.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qhsm_top.obj)
%CC% %SRCDIR%\qhsm_in.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qhsm_in.obj)

erase %LIBDIR%\qep.lib
%LIB% CREATE %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qep.obj      TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qfsm_ini.obj TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qfsm_dis.obj TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qhsm_ini.obj TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qhsm_dis.obj TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qhsm_top.obj TO %LIBDIR%\qep.lib
%LIB% ADD %BINDIR%\qhsm_in.obj  TO %LIBDIR%\qep.lib

rem QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=INCDIR(%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%)

%CC% %SRCDIR%\qa_defer.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_defer.obj)
%CC% %SRCDIR%\qa_fifo.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_fifo.obj)
%CC% %SRCDIR%\qa_lifo.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_lifo.obj)
%CC% %SRCDIR%\qa_get_.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_get_.obj)
%CC% %SRCDIR%\qa_sub.c   %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_sub.obj)
%CC% %SRCDIR%\qa_usub.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_usub.obj)
%CC% %SRCDIR%\qa_usuba.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qa_usuba.obj)
%CC% %SRCDIR%\qeq_fifo.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qeq_fifo.obj)
%CC% %SRCDIR%\qeq_get.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qeq_get.obj)
%CC% %SRCDIR%\qeq_init.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qeq_init.obj)
%CC% %SRCDIR%\qeq_lifo.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qeq_lifo.obj)
%CC% %SRCDIR%\qf_act.c   %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_act.obj)
%CC% %SRCDIR%\qf_gc.c    %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_gc.obj)
%CC% %SRCDIR%\qf_log2.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_log2.obj)
%CC% %SRCDIR%\qf_new.c   %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_new.obj)
%CC% %SRCDIR%\qf_pool.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_pool.obj)
%CC% %SRCDIR%\qf_psini.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_psini.obj)
%CC% %SRCDIR%\qf_pspub.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_pspub.obj)
%CC% %SRCDIR%\qf_pwr2.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_pwr2.obj)
%CC% %SRCDIR%\qf_tick.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qf_tick.obj)
%CC% %SRCDIR%\qmp_get.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qmp_get.obj)
%CC% %SRCDIR%\qmp_init.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qmp_init.obj)
%CC% %SRCDIR%\qmp_put.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qmp_put.obj)
%CC% %SRCDIR%\qte_ctor.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qte_ctor.obj)
%CC% %SRCDIR%\qte_arm.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qte_arm.obj)
%CC% %SRCDIR%\qte_darm.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qte_darm.obj)
%CC% %SRCDIR%\qte_rarm.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qte_rarm.obj)
%CC% %SRCDIR%\qvanilla.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qvanilla.obj)

erase %LIBDIR%\qf.lib
%LIB% CREATE %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_defer.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_fifo.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_lifo.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_get_.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_sub.obj   TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_usub.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qa_usuba.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qeq_fifo.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qeq_get.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qeq_init.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qeq_lifo.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_act.obj   TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_gc.obj    TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_log2.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_new.obj   TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_pool.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_psini.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_pspub.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_pwr2.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qf_tick.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qmp_get.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qmp_init.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qmp_put.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qte_ctor.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qte_arm.obj  TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qte_darm.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qte_rarm.obj TO %LIBDIR%\qf.lib
%LIB% ADD %BINDIR%\qvanilla.obj TO %LIBDIR%\qf.lib

if not "%1"=="spy" goto clean

rem QS -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qs\source
set CCINC=INCDIR(%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%)

%CC% %SRCDIR%\qs.c      %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs.obj)
%CC% %SRCDIR%\qs_.c     %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_.obj)
%CC% %SRCDIR%\qs_blk.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_blk.obj)
%CC% %SRCDIR%\qs_byte.c %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_byte.obj)
%CC% %SRCDIR%\qs_f32.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_f32.obj)
%CC% %SRCDIR%\qs_f64.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_f64.obj)
%CC% %SRCDIR%\qs_mem.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_mem.obj)
%CC% %SRCDIR%\qs_str.c  %CCFLAGS% %CCINC% OBJECT(%BINDIR%\qs_str.obj)

erase %LIBDIR%\qs.lib
%LIB% CREATE %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs.obj      TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_.obj     TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_blk.obj  TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_byte.obj TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_f32.obj  TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_f64.obj  TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_mem.obj  TO %LIBDIR%\qs.lib
%LIB% ADD %BINDIR%\qs_str.obj  TO %LIBDIR%\qs.lib

:clean
@echo off

erase %BINDIR%\*.obj

endlocal