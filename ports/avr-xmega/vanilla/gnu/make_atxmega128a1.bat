@echo off
:: ==========================================================================
:: Product: QP/C buld script for AVR-xmega, Vanilla port, GNU compiler
:: Last Updated for Version: 4.1.01
:: Date of the Last Update:  Dec 07, 2009
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
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
:: the WinAVR toolset...
::
if "%GNU_AVR%"=="" set GNU_AVR=C:\tools\GNU\WinAVR

set PATH=%GNU_AVR%\bin;%PATH%

set CC=avr-gcc
set LIB=avr-ar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

::
:: Adjust the Target MCU to your project
:: 
set TARGET_MCU=atxmega128a1


if "%1"=="" (
    @echo default selected
    set BINDIR=dbg
    set CCFLAGS=-c -gdwarf-2 -std=gnu99 -Os -fsigned-char -fshort-enums -mmcu=%TARGET_MCU% -Wall
)
if "%1"=="rel" (
    @echo rel selected
    set BINDIR=rel
    set CCFLAGS=-c -DNDEBUG -std=gnu99 -Os -fsigned-char -fshort-enums -mmcu=%TARGET_MCU% -Wall
)
if "%1"=="spy" (
    @echo spy selected
    set BINDIR=spy
    set CCFLAGS=-c -gdwarf-2 -DQ_SPY -std=gnu99 -Os -fsigned-char -fshort-enums -mmcu=%TARGET_MCU% -Wall
)

set LIBDIR=%BINDIR%
set LIBFLAGS=rs

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qep.o      %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qfsm_ini.o %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qfsm_dis.o %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_ini.o %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_dis.o %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_top.o %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qhsm_in.o  %SRCDIR%\qhsm_in.c 

%LIB% %LIBFLAGS% %LIBDIR%\libqep_%TARGET_MCU%.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo OFF

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_defer.o %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_fifo.o  %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_lifo.o  %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_get_.o  %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_sub.o   %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_usub.o  %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qa_usuba.o %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_fifo.o %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_get.o  %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_init.o %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qeq_lifo.o %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_act.o   %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_gc.o    %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_log2.o  %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_new.o   %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pool.o  %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_psini.o %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pspub.o %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_pwr2.o  %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qf_tick.o  %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_get.o  %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_init.o %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qmp_put.o  %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_ctor.o %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_arm.o  %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_darm.o %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_rarm.o %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qvanilla.o %SRCDIR%\qvanilla.c

%LIB% %LIBFLAGS% %LIBDIR%\libqf_%TARGET_MCU%.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qvanilla.o
@echo OFF

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs.o      %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_.o     %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_blk.o  %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_byte.o %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_f32.o  %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_f64.o  %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_mem.o  %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qs_str.o  %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\libqs_%TARGET_MCU%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo OFF
:: --------------------------------------------------------------------------

:clean
@@echo off

erase %BINDIR%\*.o

endlocal