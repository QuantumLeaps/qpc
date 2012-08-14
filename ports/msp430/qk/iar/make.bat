@echo off
:: ===========================================================================
:: Product: QP/C buld script for MSP430, QK port, IAR compiler
:: Last Updated for Version: 4.3.00
:: Date of the Last Update:  Nov 28, 2011
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
setlocal

:: define the IAR_430 environment variable to point to the location 
:: where you've installed the IAR toolset or adjust the following 
:: set instruction 
if "%IAR_430%"=="" set IAR_430="C:\tools\IAR\430_KS_5.30"

:: Typically, you don't need to modify this file past this line --------------

set PATH=%IAR_430%\430\bin;%IAR_430%\common\bin;%PATH%

set CC=icc430
set ASM=a430
set LIB=xar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-I%IAR_430%\430\inc -I%IAR_430%\430\inc\dlib -Ohz --debug -e --double=32 --reduce_stack_usage --dlib_config %IAR_430%\430\lib\dlib\dl430fn.h --diag_suppress Pa050
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-I%IAR_430%\430\inc -I%IAR_430%\430\inc\dlib -Ohz -e --double=32 --reduce_stack_usage --dlib_config %IAR_430%\430\lib\dlib\dl430fn.h --diag_suppress Pa050 -DNDEBUG
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-I%IAR_430%\430\inc -I%IAR_430%\430\inc\dlib -Ohz --debug -e --double=32 --reduce_stack_usage --dlib_config %IAR_430%\430\lib\dlib\dl430fn.h --diag_suppress Pa050 -DQ_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_in.c 

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_ctr.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_rarm.c

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk_mutex.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% qk_port.c

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto no_spy

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\libqp.lib %BINDIR%\qep.r43 %BINDIR%\qfsm_ini.r43 %BINDIR%\qfsm_dis.r43 %BINDIR%\qhsm_ini.r43 %BINDIR%\qhsm_dis.r43 %BINDIR%\qhsm_top.r43 %BINDIR%\qhsm_in.r43 %BINDIR%\qa_defer.r43 %BINDIR%\qa_fifo.r43 %BINDIR%\qa_lifo.r43 %BINDIR%\qa_get_.r43 %BINDIR%\qa_sub.r43 %BINDIR%\qa_usub.r43 %BINDIR%\qa_usuba.r43 %BINDIR%\qeq_fifo.r43 %BINDIR%\qeq_get.r43 %BINDIR%\qeq_init.r43 %BINDIR%\qeq_lifo.r43 %BINDIR%\qf_act.r43 %BINDIR%\qf_gc.r43 %BINDIR%\qf_log2.r43 %BINDIR%\qf_new.r43 %BINDIR%\qf_pool.r43 %BINDIR%\qf_psini.r43 %BINDIR%\qf_pspub.r43 %BINDIR%\qf_pwr2.r43 %BINDIR%\qf_tick.r43 %BINDIR%\qmp_get.r43 %BINDIR%\qmp_init.r43 %BINDIR%\qmp_put.r43 %BINDIR%\qte_ctor.r43 %BINDIR%\qte_arm.r43 %BINDIR%\qte_darm.r43 %BINDIR%\qte_ctr.r43 %BINDIR%\qte_rarm.r43 %BINDIR%\qk.r43 %BINDIR%\qk_sched.r43 %BINDIR%\qk_mutex.r43 %BINDIR%\qk_port.r43 %BINDIR%\qs.r43 %BINDIR%\qs_.r43 %BINDIR%\qs_blk.r43 %BINDIR%\qs_byte.r43 %BINDIR%\qs_f32.r43 %BINDIR%\qs_f64.r43 %BINDIR%\qs_mem.r43 %BINDIR%\qs_str.r43
:: --------------------------------------------------------------------------

:no_spy
erase %LIBDIR%\libqp.lib
%LIB% %LIBFLAGS% %LIBDIR%\libqp.lib %BINDIR%\qep.r43 %BINDIR%\qfsm_ini.r43 %BINDIR%\qfsm_dis.r43 %BINDIR%\qhsm_ini.r43 %BINDIR%\qhsm_dis.r43 %BINDIR%\qhsm_top.r43 %BINDIR%\qhsm_in.r43 %BINDIR%\qa_defer.r43 %BINDIR%\qa_fifo.r43 %BINDIR%\qa_lifo.r43 %BINDIR%\qa_get_.r43 %BINDIR%\qa_sub.r43 %BINDIR%\qa_usub.r43 %BINDIR%\qa_usuba.r43 %BINDIR%\qeq_fifo.r43 %BINDIR%\qeq_get.r43 %BINDIR%\qeq_init.r43 %BINDIR%\qeq_lifo.r43 %BINDIR%\qf_act.r43 %BINDIR%\qf_gc.r43 %BINDIR%\qf_log2.r43 %BINDIR%\qf_new.r43 %BINDIR%\qf_pool.r43 %BINDIR%\qf_psini.r43 %BINDIR%\qf_pspub.r43 %BINDIR%\qf_pwr2.r43 %BINDIR%\qf_tick.r43 %BINDIR%\qmp_get.r43 %BINDIR%\qmp_init.r43 %BINDIR%\qmp_put.r43 %BINDIR%\qte_ctor.r43 %BINDIR%\qte_arm.r43 %BINDIR%\qte_darm.r43 %BINDIR%\qte_ctr.r43 %BINDIR%\qte_rarm.r43 %BINDIR%\qk.r43 %BINDIR%\qk_sched.r43 %BINDIR%\qk_mutex.r43 %BINDIR%\qk_port.r43

:clean
@echo off

erase %BINDIR%\*.r43
rename %BINDIR%\*.lib *.r43

endlocal