@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TRG=nucleo-u545re
@echo Target    : %TRG%
@echo Target    : %TRG% > log_%TRG%.txt

@if "%1"=="" (
@echo usage  : run_%TRG%.bat USB-NUCLEO-BOARD
@echo example: run_%TRG%.bat f:
exit /b
)

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=%TRG%.mak
@set LOGEXT=log
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: unit tests ================================================================
set TEST=TUN_QP_qutest
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 2 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qep_hsm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qep_msm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_act
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_actq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_defer
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_dyn
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_mem
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_ps
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact32
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact64
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qeq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qmact
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_time
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

:: integration tests =========================================================
set TEST=TIN_QP_mem
cd %TESTDIR%\%TEST%\test_mpu
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TIN_QP_qk
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qv
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_mutex
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_mutex-%TRG%.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build_%TRG%) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
@del log_%TRG%.txt
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
