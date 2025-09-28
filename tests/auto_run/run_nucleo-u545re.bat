@setlocal
@set HOMEDIR=%CD%

::@echo off

@if "%1"=="" (
@echo usage  : run_nucleo-u545re.bat USB-NUCLEO-BOARD
@echo example: run_nucleo-u545re.bat f:
exit /b
)

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=nucleo-u545re.mak
@set LOGEXT=trg
@set LOGSEP=%HOMEDIR%\log_sect_sep.txt

:: unit tests ================================================================
set TEST=TUN_QP_qutest
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 2 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qep_hsm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qep_msm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_act
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_actq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_defer
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_dyn
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_mem
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_ps
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qact32
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qact64
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qeq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qmact
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_time
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

:: integration tests =========================================================
set TEST=TIN_QP_mem
cd %TESTDIR%\%TEST%\test_mpu
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TIN_QP_qk
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched.%LOGEXT%

set TEST=TIN_QP_qv
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_sched
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_mutex
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_mutex.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build_nucleo-u545re) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
