@echo off
:: ==========================================================================
:: Product: QP/C buld script for AVR32, QK port, GNU compiler
:: Last Updated for Version: 4.5.02
:: Date of the Last Update:  Sep 24, 2012
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
:: ==========================================================================
setlocal

:: adjust the following path to the location where you've installed
:: the GNU AVR32 toolset...
::
if "%GNU_AVR%"=="" set GNU_AVR=C:\tools\Atmel\Studio_6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain

set PATH=%GNU_AVR%\bin;%PATH%

set CC=avr32-gcc.exe
set ASM=avr32-gcc.exe
set LIB=avr-ar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

::
:: Adjust the Target MCU to your project
:: 
set TARGET_MCU=uc3a3256

if "%1"=="" (
    @echo default selected
    set BINDIR=dbg
    set CCFLAGS=-c -g3 -O1 -mpart=%TARGET_MCU% -fdata-sections -ffunction-sections -Wall -c -std=gnu99 -Wall
)
if "%1"=="rel" (
    @echo rel selected
    set BINDIR=rel
    set CCFLAGS=-c -DNDEBUG -c -Os -mpart=%TARGET_MCU% -fdata-sections -ffunction-sections -Wall -c -std=gnu99 -Wall
)
if "%1"=="spy" (
    @echo spy selected
    set BINDIR=spy
    set CCFLAGS=-c -g3 -DQ_SPY -O1 -mpart=%TARGET_MCU% -fdata-sections -ffunction-sections -Wall -c -std=gnu99 -Wall
)
set ASMFLAGS=-x assembler-with-cpp -c -mrelax -Wa,-g -mpart=%TARGET_MCU%

mkdir %BINDIR%
set LIBDIR=%BINDIR%
set LIBFLAGS=rs
erase %LIBDIR%\libqp_%TARGET_MCU%.a

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

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%TARGET_MCU%.a %BINDIR%\qep.o %BINDIR%\qfsm_ini.o %BINDIR%\qfsm_dis.o %BINDIR%\qhsm_ini.o %BINDIR%\qhsm_dis.o %BINDIR%\qhsm_top.o %BINDIR%\qhsm_in.o
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
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qte_ctr.o  %SRCDIR%\qte_ctr.c

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%TARGET_MCU%.a %BINDIR%\qa_defer.o %BINDIR%\qa_fifo.o %BINDIR%\qa_lifo.o %BINDIR%\qa_get_.o %BINDIR%\qa_sub.o %BINDIR%\qa_usub.o %BINDIR%\qa_usuba.o %BINDIR%\qeq_fifo.o %BINDIR%\qeq_get.o %BINDIR%\qeq_init.o %BINDIR%\qeq_lifo.o %BINDIR%\qf_act.o %BINDIR%\qf_gc.o %BINDIR%\qf_log2.o %BINDIR%\qf_new.o %BINDIR%\qf_pool.o %BINDIR%\qf_psini.o %BINDIR%\qf_pspub.o %BINDIR%\qf_pwr2.o %BINDIR%\qf_tick.o %BINDIR%\qmp_get.o %BINDIR%\qmp_init.o %BINDIR%\qmp_put.o %BINDIR%\qte_ctor.o %BINDIR%\qte_arm.o %BINDIR%\qte_darm.o %BINDIR%\qte_rarm.o %BINDIR%\qte_ctr.o
@echo OFF

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk.o       %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk_sched.o %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\qk_mutex.o %SRCDIR%\qk_mutex.c
%ASM% %ASMFLAGS% -o%BINDIR%\qk_port.o  qk_port.asm

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%TARGET_MCU%.a %BINDIR%\qk.o %BINDIR%\qk_sched.o %BINDIR%\qk_mutex.o %BINDIR%\qk_port.o
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

%LIB% %LIBFLAGS% %LIBDIR%\libqp_%TARGET_MCU%.a %BINDIR%\qs.o %BINDIR%\qs_.o %BINDIR%\qs_blk.o %BINDIR%\qs_byte.o %BINDIR%\qs_f32.o %BINDIR%\qs_f64.o %BINDIR%\qs_mem.o %BINDIR%\qs_str.o
@echo OFF
:: --------------------------------------------------------------------------

:clean
@@echo off

erase %BINDIR%\*.o

endlocal