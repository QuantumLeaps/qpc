@echo off
rem ==========================================================================
rem Product: QP/C buld script for ColdFire, Vanilla port, IAR EWCF 1.20
rem Last Updated for Version: 4.0.00
rem Date of the Last Update:  Apr 23, 2008
rem
rem                    Q u a n t u m     L e a P s
rem                    ---------------------------
rem                    innovating embedded systems
rem
rem Copyright (C) 2002-2007 Quantum Leaps, LLC. All rights reserved.
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
rem the IAR toolset...
rem
set IAR_CF=c:\tools\IAR\ColdFire_KS_1.20

set PATH=%IAR_CF%\cf\bin;%IAR_CF%\common\bin;%PATH%

set CC=icccf
set ASM=acf
set LIB=xar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCFLAGS=-I %IAR_CF%\cf\INC\ -D DEBUG --debug --core v2 --isa isa_a+ --code_model far --dlib_config %IAR_CF%\cf\LIB\dlcfaffn.h --diag_suppress Pa050 -Ol
    set ASMFLAGS=-r --isa isa_a+ --mac mac -I%IAR_CF%\cf\INC\ 

)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCFLAGS=-I %IAR_CF%\cf\INC\ -D NDEBUG --core v2 --isa isa_a+ --code_model far --dlib_config %IAR_CF%\cf\LIB\dlcfaffn.h --diag_suppress Pa050 -Oh
    set ASMFLAGS=-r --isa isa_a+ --mac mac -I%IAR_CF%\cf\INC\ 
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCFLAGS=-I %IAR_CF%\cf\INC\ -D Q_SPY --debug --core v2 --isa isa_a+ --code_model far --dlib_config %IAR_CF%\cf\LIB\dlcfaffn.h --diag_suppress Pa050 -Ol
    set ASMFLAGS=-r --isa isa_a+ --mac mac -I%IAR_CF%\cf\INC\ 
)

set LIBDIR=%BINDIR%
set LIBFLAGS=

rem QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qhsm_in.c

%LIB% %LIBFLAGS% %LIBDIR%\libqep.lib %BINDIR%\qep.r68 %BINDIR%\qfsm_ini.r68 %BINDIR%\qfsm_dis.r68 %BINDIR%\qhsm_ini.r68 %BINDIR%\qhsm_dis.r68 %BINDIR%\qhsm_top.r68 %BINDIR%\qhsm_in.r68
@echo off

rem QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qvanilla.c

%LIB% %LIBFLAGS% %LIBDIR%\libqf.lib %BINDIR%\qa_defer.r68 %BINDIR%\qa_fifo.r68 %BINDIR%\qa_lifo.r68 %BINDIR%\qa_get_.r68 %BINDIR%\qa_sub.r68 %BINDIR%\qa_usub.r68 %BINDIR%\qa_usuba.r68 %BINDIR%\qeq_fifo.r68 %BINDIR%\qeq_get.r68 %BINDIR%\qeq_init.r68 %BINDIR%\qeq_lifo.r68 %BINDIR%\qf_act.r68 %BINDIR%\qf_gc.r68 %BINDIR%\qf_log2.r68 %BINDIR%\qf_new.r68 %BINDIR%\qf_pool.r68 %BINDIR%\qf_psini.r68 %BINDIR%\qf_pspub.r68 %BINDIR%\qf_pwr2.r68 %BINDIR%\qf_tick.r68 %BINDIR%\qmp_get.r68 %BINDIR%\qmp_init.r68 %BINDIR%\qmp_put.r68 %BINDIR%\qte_ctor.r68 %BINDIR%\qte_arm.r68 %BINDIR%\qte_darm.r68 %BINDIR%\qte_rarm.r68 %BINDIR%\qvanilla.r68 
@echo off

if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\ %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\libqs.lib %BINDIR%\qs.r68 %BINDIR%\qs_.r68 %BINDIR%\qs_blk.r68 %BINDIR%\qs_byte.r68 %BINDIR%\qs_f32.r68 %BINDIR%\qs_f64.r68 %BINDIR%\qs_mem.r68 %BINDIR%\qs_str.r68
@echo off

:clean
@echo off
erase %BINDIR%\*.r68
rename %BINDIR%\*.lib *.r68

endlocal