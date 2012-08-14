@echo off
:: ==========================================================================
:: Product: QP/C buld script for ARM, Vanilla port, IAR compiler
:: Last Updated for Version: 4.5.00
:: Date of the Last Update:  Jun 10, 2012
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

:: define the IAR_ARM environment variable to point to the location 
:: where you've installed the IAR toolset or adjust the following 
:: set instruction 
::if "%IAR_ARM%"=="" set IAR_ARM="C:\Program Files\IAR Systems\Embedded Workbench 6.30"
if "%IAR_ARM%"=="" set IAR_ARM="C:\tools\IAR\ARM_KS_6.40"

set PATH=%IAR_ARM%\arm\bin;%IAR_ARM%\common\bin;%PATH%

set CC=iccarm
set ASM=iasmarm
set LIB=iarchive

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

set ARM_CORE=ARM7TDMI

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-I%IAR_ARM%\arm\inc -D DEBUG --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling --endian little --cpu %ARM_CORE% -e --fpu None --dlib_config %IAR_ARM%\ARM\INC\c\DLib_Config_Normal.h --diag_suppress=Pa050 --interwork -On
    set ASMFLAGS=-s+ -w+ -r --cpu %ARM_CORE% --fpu None -I%IAR_ARM%\arm\inc
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-I%IAR_ARM%\arm\inc -D NDEBUG --endian little --cpu %ARM_CORE% -e --fpu None --dlib_config %IAR_ARM%\ARM\INC\c\DLib_Config_Normal.h --diag_suppress=Pa050 --interwork -Ohz
    set ASMFLAGS=-s+ -w+ -r --cpu %ARM_CORE% --fpu None -I%IAR_ARM%\arm\inc
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-I%IAR_ARM%\arm\inc -D Q_SPY --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling --endian little --cpu %ARM_CORE% -e --fpu None --dlib_config %IAR_ARM%\ARM\INC\c\DLib_Config_Normal.h --diag_suppress=Pa050 --interwork -On
    set ASMFLAGS=-s+ -w+ -r --cpu %ARM_CORE% --fpu None -I%IAR_ARM%\arm\inc
)

set LIBDIR=%BINDIR%
set LIBFLAGS=

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qep.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_ini.c
%CC% --cpu_mode thumb --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_dis.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_ini.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_dis.c
%CC% --cpu_mode thumb --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_top.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_in.c 

erase %LIBDIR%\qep_%ARM_CORE%.a
%LIB% %LIBFLAGS% -o%LIBDIR%\qep_%ARM_CORE%.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
@echo off

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_defer.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_fifo.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_lifo.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_get_.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_sub.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_usub.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qa_usuba.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_fifo.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_get.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_init.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qeq_lifo.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_act.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_gc.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_log2.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_new.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pool.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_psini.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pspub.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_pwr2.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qf_tick.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_get.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_init.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qmp_put.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_ctor.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_arm.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_darm.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_rarm.c
%CC% --cpu_mode thumb %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_ctr.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qvanilla.c
%ASM% src\qf_port.s -o %BINDIR%\qf_port.o %ASMFLAGS%

erase %LIBDIR%\qf_%ARM_CORE%.a
%LIB% %LIBFLAGS% -o%LIBDIR%\qf_%ARM_CORE%.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o %BINDIR%\qvanilla.o %BINDIR%\qf_port.o
@echo off

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo on
%CC% --cpu_mode thumb --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_blk.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_byte.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f32.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f64.c
%CC% --cpu_mode arm   %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_mem.c
%CC% --cpu_mode arm   --section .text=.textrw %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_str.c

erase %LIBDIR%\qs_%ARM_CORE%.a
%LIB% %LIBFLAGS% -o%LIBDIR%\qs_%ARM_CORE%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo off

:: --------------------------------------------------------------------------

:clean
@echo off

erase %BINDIR%\*.o

endlocal