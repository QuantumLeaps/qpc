@echo off
:: ==========================================================================
:: Product: QP/C buld script for ARM, Vanilla port, GNU toolchain
:: Last Updated for Version: 4.3.00
:: Date of the Last Update:  Nov 06, 2011
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
:: ==========================================================================
setlocal

:: define the GNU_ARM environment variable to point to the location 
:: where you've installed the GNU ARM toolset or adjust the following 
:: set instruction 
if "%GNU_ARM%"=="" set GNU_ARM=C:/tools/devkitPro/devkitARM

set PATH=%GNU_ARM%\bin;%PATH%

set CC=arm-eabi-gcc
set ASM=arm-eabi-as
set LIB=arm-eabi-ar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

set ARM_CORE=arm7tdmi

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-g -c -mcpu=%ARM_CORE% -mthumb-interwork -mlong-calls -ffunction-sections -Wall -O
    set ASMFLAGS=-g -mcpu=%ARM_CORE% -mthumb-interwork
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-c -mcpu=%ARM_CORE% -mthumb-interwork -mlong-calls -ffunction-sections -Wall -Os -DNDEBUG
    set ASMFLAGS=-mcpu=%ARM_CORE% -mthumb-interwork
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-g -c -mcpu=%ARM_CORE% -mthumb-interwork -mlong-calls -ffunction-sections -Wall -O -DQ_SPY
    set ASMFLAGS=-g -mcpu=%ARM_CORE% -mthumb-interwork
)

set LIBDIR=%BINDIR%
set LIBFLAGS=rs

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qep.c      -o%BINDIR%\qep.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qfsm_ini.c -o%BINDIR%\qfsm_ini.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qfsm_dis.c -o%BINDIR%\qfsm_dis.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qhsm_ini.c -o%BINDIR%\qhsm_ini.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qhsm_dis.c -o%BINDIR%\qhsm_dis.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qhsm_top.c -o%BINDIR%\qhsm_top.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qhsm_in.c  -o%BINDIR%\qhsm_in.o

%LIB% %LIBFLAGS% %LIBDIR%\libqep_%ARM_CORE%.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo off
erase %BINDIR%\*.o

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qa_defer.c -o%BINDIR%\qa_defer.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qa_fifo.c  -o%BINDIR%\qa_fifo.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qa_lifo.c  -o%BINDIR%\qa_lifo.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qa_get_.c  -o%BINDIR%\qa_get_.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qa_sub.c   -o%BINDIR%\qa_sub.o  
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qa_usub.c  -o%BINDIR%\qa_usub.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qa_usuba.c -o%BINDIR%\qa_usuba.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qeq_fifo.c -o%BINDIR%\qeq_fifo.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qeq_get.c  -o%BINDIR%\qeq_get.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qeq_init.c -o%BINDIR%\qeq_init.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qeq_lifo.c -o%BINDIR%\qeq_lifo.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qf_act.c   -o%BINDIR%\qf_act.o  
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_gc.c    -o%BINDIR%\qf_gc.o      
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_log2.c  -o%BINDIR%\qf_log2.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_new.c   -o%BINDIR%\qf_new.o  
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qf_pool.c  -o%BINDIR%\qf_pool.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qf_psini.c -o%BINDIR%\qf_psini.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_pspub.c -o%BINDIR%\qf_pspub.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_pwr2.c  -o%BINDIR%\qf_pwr2.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qf_tick.c  -o%BINDIR%\qf_tick.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qmp_get.c  -o%BINDIR%\qmp_get.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qmp_init.c -o%BINDIR%\qmp_init.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qmp_put.c  -o%BINDIR%\qmp_put.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qte_ctor.c -o%BINDIR%\qte_ctor.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qte_arm.c  -o%BINDIR%\qte_arm.o 
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qte_darm.c -o%BINDIR%\qte_darm.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qte_rarm.c -o%BINDIR%\qte_rarm.o
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qte_ctr.c  -o%BINDIR%\qte_ctr.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qvanilla.c -o%BINDIR%\qvanilla.o  
%ASM% src\qf_port.s -o %BINDIR%\qf_port.o %ASMFLAGS%

%LIB% %LIBFLAGS% %LIBDIR%\libqf_%ARM_CORE%.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o %BINDIR%\qvanilla.o %BINDIR%\qf_port.o
@echo off
erase %BINDIR%\*.o

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% -mthumb %CCFLAGS% %CCINC% %SRCDIR%\qs.c      -o%BINDIR%\qs.o     
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_.c     -o%BINDIR%\qs_.o     
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_blk.c  -o%BINDIR%\qs_blk.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_byte.c -o%BINDIR%\qs_byte.o
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_f32.c  -o%BINDIR%\qs_f32.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_f64.c  -o%BINDIR%\qs_f64.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_mem.c  -o%BINDIR%\qs_mem.o 
%CC% -marm   %CCFLAGS% %CCINC% %SRCDIR%\qs_str.c  -o%BINDIR%\qs_str.o 

%LIB% %LIBFLAGS% %LIBDIR%\libqs_%ARM_CORE%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo off
erase %BINDIR%\*.o

:: --------------------------------------------------------------------------

:clean

endlocal