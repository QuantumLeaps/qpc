@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TRG=host
@echo Target    : %TRG%
@echo Target    : %TRG% > log_%TRG%.txt
@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=Makefile
@set LOGEXT=log
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: infrastructure test (QUTest self-check) ================================
set TEST=TUN_QP_qutest
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 2 goto err
:: the infrastructure check passes only with exactly the expected failures
findstr /L /C:"Tests     : 21" *.log > nul
if %ERRORLEVEL% neq 0 goto err
findstr /L /C:"FAILED    : 4 [ 1 3 4 5 ]" *.log > nul
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%


:: unit tests ================================================================
set TEST=TUN_QP_qep_hsm
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qep_msm
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_act
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_actq8
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_actq16
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_defer
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_dyn
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_dyn_raii
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_mem
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_ps
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact64
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact32
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact16
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qact8
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qeq8
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qeq16
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_qmact
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_time16
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qf_time32
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qv
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TUN_QP_qk
cd %TESTDIR%\%TEST%\test\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

:: integration tests =========================================================
set TEST=TIN_QP_qk
cd %TESTDIR%\%TEST%\test_sched\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TIN_QP_qv
cd %TESTDIR%\%TEST%\test_sched\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
@del log_%TRG%.txt
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
