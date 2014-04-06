@echo off
:: ===========================================================================
:: Product: QP/C buld script for ARM Cortex-M3, Vanilla port, ARM-KEIL
:: Last updated for version 5.3.0
:: Last updated on  2014-03-30
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) Quantum Leaps, www.state-machine.com.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 3 of the License, or
:: (at your option) any later version.
::
:: Alternatively, this program may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GNU General Public License and are specifically designed for
:: licensees interested in retaining the proprietary status of their code.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: Web:   www.state-machine.com
:: Email: info@state-machine.com
:: ===========================================================================
setlocal

:: adjust the following path to the location where you've installed
:: the ARM_KEIL toolset...
::
if "%ARM_KEIL%"=="" set ARM_KEIL="C:\tools\Keil\ARM\ARMCC"

set CC=%ARM_KEIL%\bin\armcc
set ASM=%ARM_KEIL%\bin\armasm
set LIB=%ARM_KEIL%\bin\armar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

set ARM_CORE=Cortex-M3

if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCOPT=-O1
    set CCFLAGS=-g -c --cpu %ARM_CORE% --apcs=interwork --split_sections
    set ASMFLAGS=-g --cpu %ARM_CORE% --apcs=interwork
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCOPT=-O3 -Otime
    set CCFLAGS=-c --cpu %ARM_CORE% --apcs=interwork --split_sections -DNDEBUG
    set ASMFLAGS=--cpu %ARM_CORE% --apcs=interwork
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCOPT=-O1
    set CCOPT_QS=-O3 -Otime
    set CCFLAGS=-g -c --cpu %ARM_CORE% --apcs=interwork --split_sections -DQ_SPY
    set ASMFLAGS=-g --cpu %ARM_CORE% --apcs=interwork
)

mkdir %BINDIR%
set LIBDIR=%BINDIR%
set LIBFLAGS=-r
erase %LIBDIR%\libqp_%ARM_CORE%.a

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qep.c      -o%BINDIR%\qep.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmsm_ini.c -o%BINDIR%\qmsm_ini.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmsm_dis.c -o%BINDIR%\qmsm_dis.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmsm_in.c  -o%BINDIR%\qmsm_in.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qfsm_ini.c -o%BINDIR%\qfsm_ini.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qfsm_dis.c -o%BINDIR%\qfsm_dis.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qhsm_ini.c -o%BINDIR%\qhsm_ini.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qhsm_dis.c -o%BINDIR%\qhsm_dis.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qhsm_top.c -o%BINDIR%\qhsm_top.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qhsm_in.c  -o%BINDIR%\qhsm_in.o

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%.a %BINDIR%\qep.o %BINDIR%\qmsm_ini.o %BINDIR%\qmsm_dis.o %BINDIR%\qmsm_in.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo off
erase %BINDIR%\*.o

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_ctor.c  -o%BINDIR%\qa_ctor.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_defer.c -o%BINDIR%\qa_defer.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_fifo.c  -o%BINDIR%\qa_fifo.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_lifo.c  -o%BINDIR%\qa_lifo.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_get_.c  -o%BINDIR%\qa_get_.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_sub.c   -o%BINDIR%\qa_sub.o  
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_usub.c  -o%BINDIR%\qa_usub.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qa_usuba.c -o%BINDIR%\qa_usuba.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qeq_fifo.c -o%BINDIR%\qeq_fifo.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qeq_get.c  -o%BINDIR%\qeq_get.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qeq_init.c -o%BINDIR%\qeq_init.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qeq_lifo.c -o%BINDIR%\qeq_lifo.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_act.c   -o%BINDIR%\qf_act.o  
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_gc.c    -o%BINDIR%\qf_gc.o      
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_log2.c  -o%BINDIR%\qf_log2.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_new.c   -o%BINDIR%\qf_new.o  
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_pool.c  -o%BINDIR%\qf_pool.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_psini.c -o%BINDIR%\qf_psini.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_pspub.c -o%BINDIR%\qf_pspub.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_pwr2.c  -o%BINDIR%\qf_pwr2.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qf_tick.c  -o%BINDIR%\qf_tick.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qma_ctor.c -o%BINDIR%\qma_ctor.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmp_get.c  -o%BINDIR%\qmp_get.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmp_init.c -o%BINDIR%\qmp_init.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qmp_put.c  -o%BINDIR%\qmp_put.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qte_ctor.c -o%BINDIR%\qte_ctor.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qte_arm.c  -o%BINDIR%\qte_arm.o 
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qte_darm.c -o%BINDIR%\qte_darm.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qte_rarm.c -o%BINDIR%\qte_rarm.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qte_ctr.c  -o%BINDIR%\qte_ctr.o
%CC% %CCFLAGS% %CCOPT% %CCINC% %SRCDIR%\qvanilla.c  -o%BINDIR%\qvanilla.o

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%.a %BINDIR%\qa_ctor.o %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qma_ctor.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o %BINDIR%\qvanilla.o
@echo off
erase %BINDIR%\*.o

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs.c      -o%BINDIR%\qs.o     
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_.c     -o%BINDIR%\qs_.o     
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_blk.c  -o%BINDIR%\qs_blk.o 
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_byte.c -o%BINDIR%\qs_byte.o
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_dict.c -o%BINDIR%\qs_dict.o
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_f32.c  -o%BINDIR%\qs_f32.o 
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_f64.c  -o%BINDIR%\qs_f64.o 
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_mem.c  -o%BINDIR%\qs_mem.o 
%CC% %CCFLAGS% %CCOPT_QS% %CCINC% %SRCDIR%\qs_str.c  -o%BINDIR%\qs_str.o 

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_dict.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo off
erase %BINDIR%\*.o

:: --------------------------------------------------------------------------

:clean

endlocal
