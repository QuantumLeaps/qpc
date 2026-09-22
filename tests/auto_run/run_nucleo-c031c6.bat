@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TRG=nucleo-c031c6
@echo Target    : %TRG%
@echo Target    : %TRG% > log_%TRG%.txt

@if "%1"=="" (
@echo usage  : run_%TRG%.bat USB-NUCLEO-BOARD
@echo example: run_%TRG%.bat f:/
exit /b
)

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=Makefile
@set LOGEXT=log
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: integration tests =========================================================
set TEST=TIN_QP_mem
cd %TESTDIR%\%TEST%\test_mem\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y %LOGDIR%\log_%TRG%.txt + *.log %LOGDIR%\%TEST%-%TRG%.%LOGEXT%

set TEST=TIN_QP_qk
cd %TESTDIR%\%TEST%\test_sched\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qv
cd %TESTDIR%\%TEST%\test_sched\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_sched\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_sched-%TRG%.%LOGEXT%

set TEST=TIN_QP_qxk
cd %TESTDIR%\%TEST%\test_mutex\%TRG%
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=cx USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\%TEST%_mutex-%TRG%.%LOGEXT%

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
