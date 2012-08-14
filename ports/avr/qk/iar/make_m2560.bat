@echo off
:: ==========================================================================
:: Product: QP/C buld script for ARM, QK port, IAR compiler
:: Last Updated for Version: 4.3.00
:: Date of the Last Update:  Dec 12, 2011
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

:: adjust the following path to the location where you've installed
:: the IAR toolset...
::
set IAR_AVR=C:\tools\IAR\AVR_KS_6.10

::
:: Adjust the Target MCU to your project
:: 
set TARGET_MCU=m2560
set EEPROM_SIZE=4096

:: Typically, you don't need to modify this file past this line -------------

set PATH=%IAR_AVR%\avr\bin;%IAR_AVR%\common\bin;%PATH%

set CC=iccavr
set ASM=aavr
set LIB=xar

set QP_INCDIR=..\..\..\..\include
set QP_PRTDIR=.

if "%1"=="" (
    @echo default selected
    set BINDIR=dbg
    set CCFLAGS=--cpu=%TARGET_MCU% --eeprom_size=%EEPROM_SIZE% -ms --initializers_in_flash -Om -e --no_inline --no_cross_call --no_tbaa --debug -I%IAR_AVR%\avr\inc -I%IAR_AVR%\avr\inc\dlib --dlib_config %IAR_AVR%\avr\lib\dlib\dlAVR-3s-ec_mul-sf-n.h --diag_suppress Pa050
)
if "%1"=="rel" (
    @echo rel selected
    set BINDIR=rel
    set CCFLAGS=--cpu=%TARGET_MCU% --eeprom_size=%EEPROM_SIZE% -ms -y --initializers_in_flash -Ohz -e --diag_suppress Pa050 -I%IAR_AVR%\avr\inc -I%IAR_AVR%\avr\inc\dlib --dlib_config %IAR_AVR%\avr\lib\dlib\dlAVR-3s-ec_mul-sf-n.h -DNDEBUG
)
if "%1"=="spy" (
    @echo spy selected
    set BINDIR=spy
    set CCFLAGS=--cpu=%TARGET_MCU% --eeprom_size=%EEPROM_SIZE% -ms --initializers_in_flash -Om -e --no_inline --no_cross_call --no_tbaa --debug --diag_suppress Pa050 -I%IAR_AVR%\avr\inc -I%IAR_AVR%\avr\inc\dlib --dlib_config %IAR_AVR%\avr\lib\dlib\dlAVR-3s-ec_mul-sf-n.h -DQ_SPY
)

set LIBDIR=%BINDIR%
set LIBFLAGS=
erase %LIBDIR%\qep_%TARGET_MCU%.r90

:: QEP ----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qep\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qep.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qfsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_ini.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_dis.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_top.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qhsm_in.c 
@echo OFF

:: QF -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qf\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
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
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_rarm.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qte_ctr.c
@echo OFF

:: QK -----------------------------------------------------------------------
set SRCDIR=..\..\..\..\qk\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk_sched.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qk_mutex.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% qk_port.c
@echo OFF

:: QS -----------------------------------------------------------------------
if not "%1"=="spy" goto lib

set SRCDIR=..\..\..\..\qs\source
set CCINC=-I%QP_PRTDIR% -I%QP_INCDIR% -I%SRCDIR%

@@echo ON
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_blk.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_byte.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f32.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_f64.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_mem.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR% %SRCDIR%\qs_str.c

%LIB% %LIBFLAGS% %LIBDIR%\qp_%TARGET_MCU%.lib %BINDIR%\qep.r90 %BINDIR%\qfsm_ini.r90 %BINDIR%\qfsm_dis.r90 %BINDIR%\qhsm_ini.r90 %BINDIR%\qhsm_dis.r90 %BINDIR%\qhsm_top.r90 %BINDIR%\qhsm_in.r90 %BINDIR%\qa_defer.r90 %BINDIR%\qa_fifo.r90 %BINDIR%\qa_lifo.r90 %BINDIR%\qa_get_.r90 %BINDIR%\qa_sub.r90 %BINDIR%\qa_usub.r90 %BINDIR%\qa_usuba.r90 %BINDIR%\qeq_fifo.r90 %BINDIR%\qeq_get.r90 %BINDIR%\qeq_init.r90 %BINDIR%\qeq_lifo.r90 %BINDIR%\qf_act.r90 %BINDIR%\qf_gc.r90 %BINDIR%\qf_log2.r90 %BINDIR%\qf_new.r90 %BINDIR%\qf_pool.r90 %BINDIR%\qf_psini.r90 %BINDIR%\qf_pspub.r90 %BINDIR%\qf_pwr2.r90 %BINDIR%\qf_tick.r90 %BINDIR%\qmp_get.r90 %BINDIR%\qmp_init.r90 %BINDIR%\qmp_put.r90 %BINDIR%\qte_ctor.r90 %BINDIR%\qte_arm.r90 %BINDIR%\qte_darm.r90 %BINDIR%\qte_rarm.r90 %BINDIR%\qte_ctr.r90 %BINDIR%\qk.r90 %BINDIR%\qk_sched.r90 %BINDIR%\qk_mutex.r90 %BINDIR%\qk_port.r90 %BINDIR%\qs.r90 %BINDIR%\qs_.r90 %BINDIR%\qs_blk.r90 %BINDIR%\qs_byte.r90 %BINDIR%\qs_f32.r90 %BINDIR%\qs_f64.r90 %BINDIR%\qs_mem.r90 %BINDIR%\qs_str.r90
@echo OFF

goto clean

:: --------------------------------------------------------------------------
:lib
%LIB% %LIBFLAGS% %LIBDIR%\qp_%TARGET_MCU%.lib %BINDIR%\qep.r90 %BINDIR%\qfsm_ini.r90 %BINDIR%\qfsm_dis.r90 %BINDIR%\qhsm_ini.r90 %BINDIR%\qhsm_dis.r90 %BINDIR%\qhsm_top.r90 %BINDIR%\qhsm_in.r90 %BINDIR%\qa_defer.r90 %BINDIR%\qa_fifo.r90 %BINDIR%\qa_lifo.r90 %BINDIR%\qa_get_.r90 %BINDIR%\qa_sub.r90 %BINDIR%\qa_usub.r90 %BINDIR%\qa_usuba.r90 %BINDIR%\qeq_fifo.r90 %BINDIR%\qeq_get.r90 %BINDIR%\qeq_init.r90 %BINDIR%\qeq_lifo.r90 %BINDIR%\qf_act.r90 %BINDIR%\qf_gc.r90 %BINDIR%\qf_log2.r90 %BINDIR%\qf_new.r90 %BINDIR%\qf_pool.r90 %BINDIR%\qf_psini.r90 %BINDIR%\qf_pspub.r90 %BINDIR%\qf_pwr2.r90 %BINDIR%\qf_tick.r90 %BINDIR%\qmp_get.r90 %BINDIR%\qmp_init.r90 %BINDIR%\qmp_put.r90 %BINDIR%\qte_ctor.r90 %BINDIR%\qte_arm.r90 %BINDIR%\qte_darm.r90 %BINDIR%\qte_rarm.r90 %BINDIR%\qte_ctr.r90 %BINDIR%\qk.r90 %BINDIR%\qk_sched.r90 %BINDIR%\qk_mutex.r90 %BINDIR%\qk_port.r90


:clean
@echo OFF

erase %BINDIR%\*.r90
rename %BINDIR%\*.lib *.r90

endlocal