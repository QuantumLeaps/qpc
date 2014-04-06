@echo off
:: ===========================================================================
:: Product: QP/C buld script for ARM Cortex-M4F, QK port, TI_ARM comiler
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
:: the TI_ARM toolset...
::
if "%TI_ARM%"=="" set TI_ARM="C:\tools\TI\ccsv5\ccsv5\tools\compiler\arm_5.0.1"

set CC=%TI_ARM%\bin\armcl
set ASM=%TI_ARM%\bin\armcl
set LIB=%TI_ARM%\bin\armar

set QP_INCDIR=..\..\..\..\include

set ARM_CORE=cortex-m4
set ARM_ARCH=v7M4
set ARM_FPU=FPv4SPD16

if "%1"=="" (
    echo default selected
    set BINDIR=dbg
    set CCFLAGS=-c -g -O -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include 
    set ASMFLAGS=-c -g -O -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include 
)
if "%1"=="rel" (
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-c -O3 -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include 
    set ASMFLAGS=-c -O3 -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include 
)
if "%1"=="spy" (
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-c -g -O -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include -DQ_SPY 
    set ASMFLAGS=-c -g -O -m%ARM_ARCH% --float_support=%ARM_FPU% --code_state=16 --gcc --abi=eabi -me --gen_func_subsections=on --ual --preproc_with_compile -I%TI_ARM%\include 
)

mkdir %BINDIR%
set LIBDIR=%BINDIR%
set LIBFLAGS=r
erase %LIBDIR%\qp_%ARM_CORE%f_ti.lib

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I. -I%QP_INCDIR% -I%SRCDIR% -fr%BINDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmsm_ini.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmsm_dis.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmsm_in.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qhsm_in.c

%LIB% %LIBFLAGS% %LIBDIR%\qp_%ARM_CORE%f_ti.lib %BINDIR%\qep.obj %BINDIR%\qmsm_ini.obj %BINDIR%\qmsm_dis.obj %BINDIR%\qmsm_in.obj %BINDIR%\qfsm_ini.obj %BINDIR%\qfsm_dis.obj %BINDIR%\qhsm_ini.obj %BINDIR%\qhsm_dis.obj %BINDIR%\qhsm_top.obj %BINDIR%\qhsm_in.obj
@echo off
erase %BINDIR%\*.obj

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I. -I%QP_INCDIR% -I%SRCDIR% -fr%BINDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_ctor.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_fifo.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_lifo.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_get_.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_fifo.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_get.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_init.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qeq_lifo.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qma_ctor.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_get.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_init.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qmp_put.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qte_ctr.c

%LIB% %LIBFLAGS% %LIBDIR%\qp_%ARM_CORE%f_ti.lib %BINDIR%\qa_ctor.obj %BINDIR%\qa_defer.obj %BINDIR%\qa_fifo.obj %BINDIR%\qa_lifo.obj %BINDIR%\qa_get_.obj %BINDIR%\qa_sub.obj %BINDIR%\qa_usub.obj %BINDIR%\qa_usuba.obj %BINDIR%\qeq_fifo.obj %BINDIR%\qeq_get.obj %BINDIR%\qeq_init.obj %BINDIR%\qeq_lifo.obj %BINDIR%\qf_act.obj %BINDIR%\qf_gc.obj %BINDIR%\qf_log2.obj %BINDIR%\qf_new.obj %BINDIR%\qf_pool.obj %BINDIR%\qf_psini.obj %BINDIR%\qf_pspub.obj %BINDIR%\qf_pwr2.obj %BINDIR%\qf_tick.obj %BINDIR%\qma_ctor.obj %BINDIR%\qmp_get.obj %BINDIR%\qmp_init.obj %BINDIR%\qmp_put.obj %BINDIR%\qte_ctor.obj %BINDIR%\qte_arm.obj %BINDIR%\qte_darm.obj %BINDIR%\qte_rarm.obj %BINDIR%\qte_ctr.obj
@echo off
erase %BINDIR%\*.obj

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I. -I%QP_INCDIR% -I%SRCDIR% -fr%BINDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qk_mutex.c
%ASM% %ASMFLAGS% -fr%BINDIR% qk_port.s 

%LIB% %LIBFLAGS% %LIBDIR%\qp_%ARM_CORE%f_ti.lib %BINDIR%\qk.obj %BINDIR%\qk_sched.obj %BINDIR%\qk_mutex.obj %BINDIR%\qk_port.obj
@echo off
erase %BINDIR%\*.obj

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I. -I%QP_INCDIR% -I%SRCDIR% -fr%BINDIR%

@echo on
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_dict.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\qp_%ARM_CORE%f_ti.lib %BINDIR%\qs.obj %BINDIR%\qs_.obj %BINDIR%\qs_blk.obj %BINDIR%\qs_byte.obj %BINDIR%\qs_dict.obj %BINDIR%\qs_f32.obj %BINDIR%\qs_f64.obj %BINDIR%\qs_mem.obj %BINDIR%\qs_str.obj
@echo off
erase %BINDIR%\*.obj

:: --------------------------------------------------------------------------

:clean

endlocal
