@echo off
:: ===========================================================================
:: Product: QP/C buld script for MSP430, QK port, TI CCS MSP430 compiler
:: Last Updated for Version: 4.4.00
:: Date of the Last Update:  Feb 16, 2012
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
setlocal

:: define the CCS_DIR environment variable to point to the location 
:: where you've installed the CCS toolset or adjust the following 
:: set instruction 
if "%CCS_DIR%"=="" set CCS_DIR=C:\tools\TI\ccsv5

:: Typically, you don't need to modify this file past this line -------------

set PATH="%CCS_DIR%\tools\compiler\msp430\bin";%PATH%

set CC=cl430
set ASM=acp430
set LIB=ar430

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-vmsp --abi=eabi -g --include_path="%CCS_DIR%\tools\compiler\msp430\include" --include_path="%CCS_DIR%\ccs_base\msp430"
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-vmsp --abi=eabi -O0 --include_path="%CCS_DIR%\tools\compiler\msp430\include" --include_path="%CCS_DIR%\ccs_base\msp430" --define=NDEBUG
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-vmsp --abi=eabi -g --include_path="%CCS_DIR%\tools\compiler\msp430\include" --include_path="%CCS_DIR%\ccs_base\msp430" --define=Q_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=a

erase %BINDIR%\qp.lib

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=--include_path=%QP_PRTDIR% --include_path=%QP_INCDIR% --include_path=%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qhsm_in.c 

%LIB% %LIBFLAGS% %LIBDIR%\qp.lib %BINDIR%\qep.obj %BINDIR%\qfsm_ini.obj %BINDIR%\qfsm_dis.obj %BINDIR%\qhsm_ini.obj %BINDIR%\qhsm_dis.obj %BINDIR%\qhsm_top.obj %BINDIR%\qhsm_in.obj
@echo off

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=--include_path=%QP_PRTDIR% --include_path=%QP_INCDIR% --include_path=%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qte_ctr.c

%LIB% %LIBFLAGS% %LIBDIR%\qp.lib %BINDIR%\qep.obj %BINDIR%\qfsm_ini.obj %BINDIR%\qfsm_dis.obj %BINDIR%\qhsm_ini.obj %BINDIR%\qhsm_dis.obj %BINDIR%\qhsm_top.obj %BINDIR%\qhsm_in.obj %BINDIR%\qa_defer.obj %BINDIR%\qa_fifo.obj %BINDIR%\qa_lifo.obj %BINDIR%\qa_get_.obj %BINDIR%\qa_sub.obj %BINDIR%\qa_usub.obj %BINDIR%\qa_usuba.obj %BINDIR%\qeq_fifo.obj %BINDIR%\qeq_get.obj %BINDIR%\qeq_init.obj %BINDIR%\qeq_lifo.obj %BINDIR%\qf_act.obj %BINDIR%\qf_gc.obj %BINDIR%\qf_log2.obj %BINDIR%\qf_new.obj %BINDIR%\qf_pool.obj %BINDIR%\qf_psini.obj %BINDIR%\qf_pspub.obj %BINDIR%\qf_pwr2.obj %BINDIR%\qf_tick.obj %BINDIR%\qmp_get.obj %BINDIR%\qmp_init.obj %BINDIR%\qmp_put.obj %BINDIR%\qte_ctor.obj %BINDIR%\qte_arm.obj %BINDIR%\qte_darm.obj %BINDIR%\qte_rarm.obj %BINDIR%\qte_ctr.obj
@echo off

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=--include_path=%QP_PRTDIR% --include_path=%QP_INCDIR% --include_path=%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qk_mutex.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" qk_port.c

%LIB% %LIBFLAGS% %LIBDIR%\qp.lib %BINDIR%\qk.obj %BINDIR%\qk_sched.obj %BINDIR%\qk_mutex.obj %BINDIR%\qk_port.obj
@echo off

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=--include_path=%QP_PRTDIR% --include_path=%QP_INCDIR% --include_path=%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% --obj_directory="%QP_PRTDIR%\%BINDIR%" %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\qp.lib %BINDIR%\qs.obj %BINDIR%\qs_.obj %BINDIR%\qs_blk.obj %BINDIR%\qs_byte.obj %BINDIR%\qs_f32.obj %BINDIR%\qs_f64.obj %BINDIR%\qs_mem.obj %BINDIR%\qs_str.obj
@echo off

:: --------------------------------------------------------------------------

:clean
@echo off

erase %BINDIR%\*.obj

endlocal
