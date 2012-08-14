@echo off
:: ==========================================================================
:: Product: QP/C buld script for PIC24FJ128GA010, QK port, MPLAB C30
:: Last Updated for Version: 4.4.00
:: Date of the Last Update:  Feb 03, 2012
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
:: ==========================================================================
setlocal

:: adjust the following path to the location where you've installed
:: the MPLAB C30 toolset...
::
set MPLAB_C30="C:\tools\Microchip\mplab-c30"
set MPLAB_ASM="C:\tools\Microchip\mplab-c30"

:: adjust the following symbol to the PIC24/dsPIC device you're using
::
set PIC_DEVICE=24FJ128GA010

:: Typically, you don't need to modify this file past this line -------------

set CC=%MPLAB_C30%\bin\pic30-gcc.exe
set ASM=%MPLAB_C30%\bin\pic30-as.exe
set LIB=%MPLAB_C30%\bin\pic30-ar.exe

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-mcpu=%PIC_DEVICE% -x c -g -Wall -O1
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-mcpu=%PIC_DEVICE% -x c -DNDEBUG -Wall -Os
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-mcpu=%PIC_DEVICE% -x c -g -Wall -O1 -DQ_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=-q

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qep.o      -c %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qfsm_ini.o -c %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qfsm_dis.o -c %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_ini.o -c %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_dis.o -c %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_top.o -c %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_in.o  -c %SRCDIR%\qhsm_in.c 

%LIB% %LIBFLAGS% %LIBDIR%\libqep_%PIC_DEVICE%.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo off
erase %BINDIR%\*.o

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_defer.o -c %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_fifo.o  -c %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_lifo.o  -c %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_get_.o  -c %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_sub.o   -c %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_usub.o  -c %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_usuba.o -c %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_fifo.o -c %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_get.o  -c %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_init.o -c %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_lifo.o -c %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_act.o   -c %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_gc.o    -c %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_log2.o  -c %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_new.o   -c %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pool.o  -c %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_psini.o -c %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pspub.o -c %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pwr2.o  -c %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_tick.o  -c %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_get.o  -c %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_init.o -c %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_put.o  -c %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_ctor.o -c %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_arm.o  -c %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_darm.o -c %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_rarm.o -c %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_ctr.o  -c %SRCDIR%\qte_ctr.c

%LIB% %LIBFLAGS% %LIBDIR%\libqf_%PIC_DEVICE%.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o
@echo off
erase %BINDIR%\*.o

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk.o       -c %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk_sched.o -c %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk_mutex.o -c %SRCDIR%\qk_mutex.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk_port.o  -c src\qk_port.c

%LIB% %LIBFLAGS% %LIBDIR%\libqk_%PIC_DEVICE%.a %BINDIR%\qk.o %BINDIR%\qk_sched.o %BINDIR%\qk_mutex.o %BINDIR%\qk_port.o
@echo off
erase %BINDIR%\*.o

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs.o      -c %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_.o     -c %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_blk.o  -c %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_byte.o -c %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_f32.o  -c %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_f64.o  -c %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_mem.o  -c %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_str.o  -c %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\libqs_%PIC_DEVICE%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo off
erase %BINDIR%\*.o

:: --------------------------------------------------------------------------

:clean
@echo off

endlocal
