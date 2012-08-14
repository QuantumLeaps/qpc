setlocal

@echo off
if "%1"=="rel" (
   set CONF=rel
) else if "%1"=="spy" (
   set CONF=spy
) else (
   set CONF=dbg
)
@echo on
arm-eabi-insight -x jlink.gdb %CONF%\dpp.elf

@echo off
endlocal
