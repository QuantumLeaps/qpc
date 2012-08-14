@echo off
:: ===========================================================================
:: Product: QP/C buld script for ARM Cortex-M0, Vanilla port, Code Sourcery
:: Last Updated for Version: 4.5.02
:: Date of the Last Update:  Jul 25, 2012
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 2 of the License, or
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
:: Quantum Leaps Web sites: http://www.quantum-leaps.com
::                          http://www.state-machine.com
:: e-mail:                  info@quantum-leaps.com
:: ===========================================================================
setlocal

:: adjust the following path to the location where you've installed
:: the GNU_ARM toolset...
::
if "%GNU_ARM%"=="" set GNU_ARM="C:\tools\CodeSourcery"

set CC=%GNU_ARM%\bin\arm-none-eabi-gcc
set ASM=%GNU_ARM%\bin\arm-none-eabi-as
set LIB=%GNU_ARM%\bin\arm-none-eabi-ar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

set ARM_CORE=cortex-m0

if "%1"=="" (
    echo default selected
    set BINDIR=%QP_PRTDIR%\dbg
    set CCFLAGS=-g -c -mcpu=%ARM_CORE% -mthumb -Wall -O
    set ASMFLAGS=-g -mcpu=%ARM_CORE%
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=%QP_PRTDIR%\rel
    set CCFLAGS=-c -mcpu=%ARM_CORE% -mthumb -Wall -Os -DNDEBUG
    set ASMFLAGS=-mcpu=%ARM_CORE%
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=%QP_PRTDIR%\spy
    set CCFLAGS=-g -c -mcpu=%ARM_CORE% -mthumb -Wall -O -DQ_SPY
    set ASMFLAGS=-g -mcpu=%ARM_CORE%
)

mkdir %BINDIR%
set LIBDIR=%BINDIR%
set LIBFLAGS=rs
erase %LIBDIR%\libqp_%ARM_CORE%_cs.a

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qep.c      -o%BINDIR%\qep.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qfsm_ini.c -o%BINDIR%\qfsm_ini.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qfsm_dis.c -o%BINDIR%\qfsm_dis.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_ini.c -o%BINDIR%\qhsm_ini.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_dis.c -o%BINDIR%\qhsm_dis.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_top.c -o%BINDIR%\qhsm_top.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_in.c  -o%BINDIR%\qhsm_in.o

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%_cs.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo off
erase %BINDIR%\*.o

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_defer.c -o%BINDIR%\qa_defer.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_fifo.c  -o%BINDIR%\qa_fifo.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_lifo.c  -o%BINDIR%\qa_lifo.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_get_.c  -o%BINDIR%\qa_get_.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_sub.c   -o%BINDIR%\qa_sub.o  
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_usub.c  -o%BINDIR%\qa_usub.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_usuba.c -o%BINDIR%\qa_usuba.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_fifo.c -o%BINDIR%\qeq_fifo.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_get.c  -o%BINDIR%\qeq_get.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_init.c -o%BINDIR%\qeq_init.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_lifo.c -o%BINDIR%\qeq_lifo.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_act.c   -o%BINDIR%\qf_act.o  
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_gc.c    -o%BINDIR%\qf_gc.o      
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_log2.c  -o%BINDIR%\qf_log2.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_new.c   -o%BINDIR%\qf_new.o  
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pool.c  -o%BINDIR%\qf_pool.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_psini.c -o%BINDIR%\qf_psini.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pspub.c -o%BINDIR%\qf_pspub.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pwr2.c  -o%BINDIR%\qf_pwr2.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_tick.c  -o%BINDIR%\qf_tick.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_get.c  -o%BINDIR%\qmp_get.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_init.c -o%BINDIR%\qmp_init.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_put.c  -o%BINDIR%\qmp_put.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_ctor.c -o%BINDIR%\qte_ctor.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_arm.c  -o%BINDIR%\qte_arm.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_darm.c -o%BINDIR%\qte_darm.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_rarm.c -o%BINDIR%\qte_rarm.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_ctr.c  -o%BINDIR%\qte_ctr.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qvanilla.c -o%BINDIR%\qvanilla.o 

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%_cs.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o %BINDIR%\qvanilla.o
@echo off
erase %BINDIR%\*.o

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs.c      -o%BINDIR%\qs.o     
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_.c     -o%BINDIR%\qs_.o     
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_blk.c  -o%BINDIR%\qs_blk.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_byte.c -o%BINDIR%\qs_byte.o
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_f32.c  -o%BINDIR%\qs_f32.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_f64.c  -o%BINDIR%\qs_f64.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_mem.c  -o%BINDIR%\qs_mem.o 
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_str.c  -o%BINDIR%\qs_str.o 

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%ARM_CORE%_cs.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo off
erase %BINDIR%\*.o

:: --------------------------------------------------------------------------

:clean

endlocal