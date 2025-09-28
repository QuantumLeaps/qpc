@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=Makefile
@set LOGEXT=host
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: unit tests ================================================================
set TEST=TUN_QP_qutest
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 2 goto err
copy /b/y *.log %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qep_hsm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qep_msm
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_act
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_actq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_defer
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_dyn
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_mem
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_ps
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qact32
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qact64
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qeq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_qmact
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

set TEST=TUN_QP_qf_time
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\%TEST%.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build_host) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
