@echo off
:: ===========================================================================
:: Product: QP/C buld script for uC/OS-II port, Open Watcom compiler
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

:: If you have defined the WATCOM environment variable, the following line has
:: no effect and your definition is used. However, if the varible WATCOM is
:: not defined, the following default is assuemed: 
if "%WATCOM%"=="" set WATCOM=c:\tools\WATCOM

path %WATCOM%\binw
set INCLUDE=%WATCOM%\H
set CC=wcc.exe
set AS=wasm.exe
set LIB=wlib.exe

:: NOTE: you don't need to have the uC/OS-II source code to build the
:: QP libraries for uC/OS-II. In this case you will use the default
:: uC/OS-II configuration, as provided in the ucos2.86 directory.
:: However, if you do have uC/OS-II source code, you can recompile it
:: to use your own configuration. The following symbols define the
:: locations of uC/OS-II source and port directories
:: 
set UCOS_SRCDIR=\software\uCOS-II\Source
set UCOS_INCDIR=ucos2.86
set UCOS_PRTDIR=ucos2.86

if not "%1"=="rel" goto spy
    echo rel selected
    set BINDIR=rel
    set CCFLAGS=-d0 -ot @cc_opt.rsp
    set ASFLAGS=-fpi87
goto compile
:spy
if not "%1"=="spy" goto dbg
    echo spy selected
    set BINDIR=spy
    set CCFLAGS=-d2 -dQ_SPY @cc_opt.rsp
    set ASFLAGS=-fpi87
goto compile
:dbg
echo default selected
set BINDIR=dbg
set CCFLAGS=-d2 @cc_opt.rsp
set ASFLAGS=-fpi87

::===========================================================================
:compile
set LIBDIR=%BINDIR%
erase %LIBDIR%\qp.lib
%LIB% -n %LIBDIR%\qp

:: uC/OS-II -----------------------------------------------------------------
:: NOTE: uncomment the following block to re-build uC/OS-II from sources

::@echo on
::%AS% %ASFLAGS% -fo=%UCOS_PRTDIR%\os_cpu_a.obj %UCOS_PRTDIR%\os_cpu_a.asm
::%CC% %CCFLAGS% @inc_ucos.rsp -fo=%UCOS_PRTDIR%\os_cpu_c.obj %UCOS_PRTDIR%\os_cpu_c.c
::%CC% %CCFLAGS% @inc_ucos.rsp -fo=%UCOS_PRTDIR%\ucos_ii.obj  %UCOS_SRCDIR%\ucos_ii.c
::@echo off


:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\qep\source
set CCINC=@inc_qep.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qep.obj      %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmsm_ini.obj %SRCDIR%\qmsm_ini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmsm_dis.obj %SRCDIR%\qmsm_dis.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qmsm_in.obj  %SRCDIR%\qmsm_in.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qfsm_ini.obj %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qfsm_dis.obj %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_ini.obj %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_dis.obj %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_top.obj %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qhsm_in.obj  %SRCDIR%\qhsm_in.c

%LIB% %LIBDIR%\qp +%BINDIR%\qep
%LIB% %LIBDIR%\qp +%BINDIR%\qmsm_ini
%LIB% %LIBDIR%\qp +%BINDIR%\qmsm_dis
%LIB% %LIBDIR%\qp +%BINDIR%\qmsm_in
%LIB% %LIBDIR%\qp +%BINDIR%\qfsm_ini
%LIB% %LIBDIR%\qp +%BINDIR%\qfsm_dis
%LIB% %LIBDIR%\qp +%BINDIR%\qhsm_ini
%LIB% %LIBDIR%\qp +%BINDIR%\qhsm_dis
%LIB% %LIBDIR%\qp +%BINDIR%\qhsm_top
%LIB% %LIBDIR%\qp +%BINDIR%\qhsm_in
@echo off

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\qf\source
set CCINC=@inc_qf.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_ctor.obj  %SRCDIR%\qa_ctor.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_defer.obj %SRCDIR%\qa_defer.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_sub.obj   %SRCDIR%\qa_sub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_usub.obj  %SRCDIR%\qa_usub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qa_usuba.obj %SRCDIR%\qa_usuba.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_act.obj   %SRCDIR%\qf_act.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_gc.obj    %SRCDIR%\qf_gc.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_log2.obj  %SRCDIR%\qf_log2.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_new.obj   %SRCDIR%\qf_new.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pool.obj  %SRCDIR%\qf_pool.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_psini.obj %SRCDIR%\qf_psini.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pspub.obj %SRCDIR%\qf_pspub.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_pwr2.obj  %SRCDIR%\qf_pwr2.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_tick.obj  %SRCDIR%\qf_tick.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qma_ctor.obj %SRCDIR%\qma_ctor.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_ctor.obj %SRCDIR%\qte_ctor.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_arm.obj  %SRCDIR%\qte_arm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_darm.obj %SRCDIR%\qte_darm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_rarm.obj %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qte_ctr.obj  %SRCDIR%\qte_ctr.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qf_port.obj  qf_port.c

%LIB% %LIBDIR%\qp +%BINDIR%\qa_ctor
%LIB% %LIBDIR%\qp +%BINDIR%\qa_defer
%LIB% %LIBDIR%\qp +%BINDIR%\qa_sub
%LIB% %LIBDIR%\qp +%BINDIR%\qa_usub
%LIB% %LIBDIR%\qp +%BINDIR%\qa_usuba
%LIB% %LIBDIR%\qp +%BINDIR%\qf_act
%LIB% %LIBDIR%\qp +%BINDIR%\qf_gc
%LIB% %LIBDIR%\qp +%BINDIR%\qf_log2
%LIB% %LIBDIR%\qp +%BINDIR%\qf_new
%LIB% %LIBDIR%\qp +%BINDIR%\qf_pool
%LIB% %LIBDIR%\qp +%BINDIR%\qf_psini
%LIB% %LIBDIR%\qp +%BINDIR%\qf_pspub
%LIB% %LIBDIR%\qp +%BINDIR%\qf_pwr2
%LIB% %LIBDIR%\qp +%BINDIR%\qf_tick
%LIB% %LIBDIR%\qp +%BINDIR%\qma_ctor
%LIB% %LIBDIR%\qp +%BINDIR%\qte_ctor
%LIB% %LIBDIR%\qp +%BINDIR%\qte_arm
%LIB% %LIBDIR%\qp +%BINDIR%\qte_darm
%LIB% %LIBDIR%\qp +%BINDIR%\qte_rarm
%LIB% %LIBDIR%\qp +%BINDIR%\qte_ctr
%LIB% %LIBDIR%\qp +%BINDIR%\qf_port
@echo off

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto clean

set SRCDIR=..\..\..\qs\source
set CCINC=@inc_qs.rsp

@echo on
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs.obj      %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_.obj     %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_blk.obj  %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_byte.obj %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_dict.obj %SRCDIR%\qs_dict.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_f32.obj  %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_mem.obj  %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -fo=%BINDIR%\qs_str.obj  %SRCDIR%\qs_str.c

%LIB% %LIBDIR%\qp +%BINDIR%\qs
%LIB% %LIBDIR%\qp +%BINDIR%\qs_
%LIB% %LIBDIR%\qp +%BINDIR%\qs_blk
%LIB% %LIBDIR%\qp +%BINDIR%\qs_byte
%LIB% %LIBDIR%\qp +%BINDIR%\qs_dict
%LIB% %LIBDIR%\qp +%BINDIR%\qs_f32
%LIB% %LIBDIR%\qp +%BINDIR%\qs_mem
%LIB% %LIBDIR%\qp +%BINDIR%\qs_str
@echo off

:clean
@echo off

erase %BINDIR%\*.obj
erase %LIBDIR%\*.bak
