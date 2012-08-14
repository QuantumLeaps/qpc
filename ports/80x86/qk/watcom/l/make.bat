@echo off
:: ===========================================================================
:: Product: QP/C buld script for 80x86, QK port, Open Watcom compiler
:: Last Updated for Version: 4.2.00
:: Date of the Last Update:  Jul 08, 2011
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
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
set LIB=wlib.exe

if not "%1"=="rel" goto spy
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-d0 -ot @cc_opt.rsp
    set ASFLAGS=-fpi87
goto compile
:spy
if not "%1"=="spy" goto dbg
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-d2 -dQ_SPY @cc_opt.rsp
    set ASFLAGS=-fpi87
goto compile
:dbg
echo default selected
set BINDIR=dbg
set CCFLAGS=-d2 @cc_opt.rsp
set ASFLAGS=-fpi87

::===========================================================================
:compile
set LIBDIR=%BINDIR%

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\..\qep\source
set CCINC=@inc_qep.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qep.obj      %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qfsm_ini.obj %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qfsm_dis.obj %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_ini.obj %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_dis.obj %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_top.obj %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_in.obj  %SRCDIR%\qhsm_in.c

erase %LIBDIR%\qep.lib
%LIB% -n %LIBDIR%\qep
%LIB% %LIBDIR%\qep +%BINDIR%\qep
%LIB% %LIBDIR%\qep +%BINDIR%\qfsm_ini
%LIB% %LIBDIR%\qep +%BINDIR%\qfsm_dis
%LIB% %LIBDIR%\qep +%BINDIR%\qhsm_ini
%LIB% %LIBDIR%\qep +%BINDIR%\qhsm_dis
%LIB% %LIBDIR%\qep +%BINDIR%\qhsm_top
%LIB% %LIBDIR%\qep +%BINDIR%\qhsm_in
@echo off

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\..\qf\source
set CCINC=@inc_qf.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_defer.obj %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_fifo.obj  %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_lifo.obj  %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_get_.obj  %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_sub.obj   %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_usub.obj  %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_usuba.obj %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qeq_fifo.obj %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qeq_get.obj  %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qeq_init.obj %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qeq_lifo.obj %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_act.obj   %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_gc.obj    %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_log2.obj  %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_new.obj   %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pool.obj  %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_psini.obj %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pspub.obj %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pwr2.obj  %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_tick.obj  %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmp_get.obj  %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmp_init.obj %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmp_put.obj  %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_ctor.obj %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_arm.obj  %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_darm.obj %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_rarm.obj %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_ctr.obj  %SRCDIR%\qte_ctr.c

erase %LIBDIR%\qf.lib
%LIB% -n %LIBDIR%\qf
%LIB% %LIBDIR%\qf +%BINDIR%\qa_defer
%LIB% %LIBDIR%\qf +%BINDIR%\qa_fifo
%LIB% %LIBDIR%\qf +%BINDIR%\qa_lifo
%LIB% %LIBDIR%\qf +%BINDIR%\qa_get_
%LIB% %LIBDIR%\qf +%BINDIR%\qa_sub
%LIB% %LIBDIR%\qf +%BINDIR%\qa_usub
%LIB% %LIBDIR%\qf +%BINDIR%\qa_usuba
%LIB% %LIBDIR%\qf +%BINDIR%\qeq_fifo
%LIB% %LIBDIR%\qf +%BINDIR%\qeq_get
%LIB% %LIBDIR%\qf +%BINDIR%\qeq_init
%LIB% %LIBDIR%\qf +%BINDIR%\qeq_lifo
%LIB% %LIBDIR%\qf +%BINDIR%\qf_act
%LIB% %LIBDIR%\qf +%BINDIR%\qf_gc
%LIB% %LIBDIR%\qf +%BINDIR%\qf_log2
%LIB% %LIBDIR%\qf +%BINDIR%\qf_new
%LIB% %LIBDIR%\qf +%BINDIR%\qf_pool
%LIB% %LIBDIR%\qf +%BINDIR%\qf_psini
%LIB% %LIBDIR%\qf +%BINDIR%\qf_pspub
%LIB% %LIBDIR%\qf +%BINDIR%\qf_pwr2
%LIB% %LIBDIR%\qf +%BINDIR%\qf_tick
%LIB% %LIBDIR%\qf +%BINDIR%\qmp_get
%LIB% %LIBDIR%\qf +%BINDIR%\qmp_init
%LIB% %LIBDIR%\qf +%BINDIR%\qmp_put
%LIB% %LIBDIR%\qf +%BINDIR%\qte_ctor
%LIB% %LIBDIR%\qf +%BINDIR%\qte_arm
%LIB% %LIBDIR%\qf +%BINDIR%\qte_darm
%LIB% %LIBDIR%\qf +%BINDIR%\qte_rarm
%LIB% %LIBDIR%\qf +%BINDIR%\qte_ctr
@echo off

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\..\qk\source
set CCINC=@inc_qk.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qk.obj       %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qk_sched.obj %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qk_mutex.obj %SRCDIR%\qk_mutex.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qk_ext.obj   %SRCDIR%\qk_ext.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qk_port.obj  src\qk_port.c
%AS% %ASFLAGS%         -fo=%BINDIR%\fpu.obj      src\fpu.asm

erase %LIBDIR%\qk.lib
%LIB% -n %LIBDIR%\qk
%LIB% %LIBDIR%\qk +%BINDIR%\qk
%LIB% %LIBDIR%\qk +%BINDIR%\qk_sched
%LIB% %LIBDIR%\qk +%BINDIR%\qk_mutex
%LIB% %LIBDIR%\qk +%BINDIR%\qk_ext
%LIB% %LIBDIR%\qk +%BINDIR%\qk_port
%LIB% %LIBDIR%\qk +%BINDIR%\fpu
@echo off

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\..\qs\source
set CCINC=@inc_qs.rsp

@echo on
%LIB% -n %LIBDIR%\qs
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs.obj      %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_.obj     %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_blk.obj  %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_byte.obj %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_f32.obj  %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_f64.obj  %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_mem.obj  %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_str.obj  %SRCDIR%\qs_str.c

erase %LIBDIR%\qs.lib
%LIB% -n %LIBDIR%\qs
%LIB% %LIBDIR%\qs +%BINDIR%\qs.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_blk.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_byte.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_f32.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_f64.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_mem.obj
%LIB% %LIBDIR%\qs +%BINDIR%\qs_str.obj
@echo off

:clean
@echo off

erase %BINDIR%\*.obj
erase %LIBDIR%\*.bak
