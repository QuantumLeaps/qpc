1. generating mocks:

set UNITY_DIR=%QTOOLS%\unity
ruby %QTOOLS%\unity\extras\cmock\lib\cmock.rb ..\src\Led.h

2. generating test runner:

set UNITY_DIR=%QTOOLS%\unity
ruby %QTOOLS%\unity\auto\generate_test_runner.rb TestLedBar.c

