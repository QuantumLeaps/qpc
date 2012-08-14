@echo off
rem ==========================================================================
rem Product: QP/C library buld script, H8S/2000A, Vanilla, Renesas H8 compiler
rem Last Updated for Version: 4.0.02
rem Date of the Last Update:  Dec 02, 2008
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
rem the Renesas H8 toolset...
rem
set RENESAS_H8_DIR=C:\tools\Renesas\Hew\Tools\Renesas\H8\6_2_1

rem
rem Adjust the Target MCU to your project
rem 
rem
set TARGET_MCU=2000A
set TARGET_MCU_VARIANT=":24"

set PATH=%RENESAS_H8_DIR%\bin;%PATH%
set CC=ch38.exe
set ASM=asm38.exe
set LIB=optlnk.exe
set H8_INCDIR=%RENESAS_H8_DIR%\include

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCFLAGS=-debug -nolist -chgincpath -cpu=%TARGET_MCU%%TARGET_MCU_VARIANT%
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCFLAGS=-nolist -chgincpath -cpu=%TARGET_MCU%%TARGET_MCU_VARIANT% -define=NDEBUG -speed=register,switch,shift,struct,expression,loop=2,inline
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCFLAGS=-debug -nolist -chgincpath -cpu=%TARGET_MCU%%TARGET_MCU_VARIANT% -define=Q_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=-form=library -noprelink

rem QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-include=%H8_INCDIR%,%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qep.obj      %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qfsm_ini.obj %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qfsm_dis.obj %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qhsm_ini.obj %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qhsm_dis.obj %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qhsm_top.obj %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qhsm_in.obj  %SRCDIR%\qhsm_in.c

%LIB% %LIBFLAGS% -output=%LIBDIR%\qep_%TARGET_MCU%.lib %BINDIR%\qep.obj %BINDIR%\qfsm_ini.obj %BINDIR%\qfsm_dis.obj %BINDIR%\qhsm_ini.obj %BINDIR%\qhsm_dis.obj %BINDIR%\qhsm_top.obj %BINDIR%\qhsm_in.obj
@echo off

rem QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-include=%H8_INCDIR%,%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_defer.obj %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_fifo.obj  %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_lifo.obj  %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_get_.obj  %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_sub.obj   %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_usub.obj  %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qa_usuba.obj %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qeq_fifo.obj %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qeq_get.obj  %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qeq_init.obj %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qeq_lifo.obj %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_act.obj   %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_gc.obj    %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_log2.obj  %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_new.obj   %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_pool.obj  %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_psini.obj %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_pspub.obj %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_pwr2.obj  %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qf_tick.obj  %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qmp_get.obj  %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qmp_init.obj %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qmp_put.obj  %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qte_ctor.obj %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qte_arm.obj  %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qte_darm.obj %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qte_rarm.obj %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qvanilla.obj %SRCDIR%\qvanilla.c

%LIB% %LIBFLAGS% -output=%LIBDIR%\qf_%TARGET_MCU%.lib %BINDIR%\qa_defer.obj %BINDIR%\qa_fifo.obj %BINDIR%\qa_lifo.obj %BINDIR%\qa_get_.obj %BINDIR%\qa_sub.obj %BINDIR%\qa_usub.obj %BINDIR%\qa_usuba.obj %BINDIR%\qeq_fifo.obj %BINDIR%\qeq_get.obj %BINDIR%\qeq_init.obj %BINDIR%\qeq_lifo.obj %BINDIR%\qf_act.obj %BINDIR%\qf_gc.obj %BINDIR%\qf_log2.obj %BINDIR%\qf_new.obj %BINDIR%\qf_pool.obj %BINDIR%\qf_psini.obj %BINDIR%\qf_pspub.obj %BINDIR%\qf_pwr2.obj %BINDIR%\qf_tick.obj %BINDIR%\qmp_get.obj %BINDIR%\qmp_init.obj %BINDIR%\qmp_put.obj %BINDIR%\qte_ctor.obj %BINDIR%\qte_arm.obj %BINDIR%\qte_darm.obj %BINDIR%\qte_rarm.obj %BINDIR%\qvanilla.obj
@echo off

rem QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-include=%H8_INCDIR%,%QP_PRTDIR%,%QP_INCDIR%,%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs.obj      %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_.obj     %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_blk.obj  %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_byte.obj %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_f32.obj  %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_f64.obj  %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_mem.obj  %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -object=%BINDIR%\qs_str.obj  %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% -output=%LIBDIR%\qs_%TARGET_MCU%.lib %BINDIR%\qs.obj %BINDIR%\qs_.obj %BINDIR%\qs_blk.obj %BINDIR%\qs_byte.obj %BINDIR%\qs_f32.obj %BINDIR%\qs_f64.obj %BINDIR%\qs_mem.obj %BINDIR%\qs_str.obj
@echo off

rem --------------------------------------------------------------------------
:clean

erase %BINDIR%\*.obj


endlocal