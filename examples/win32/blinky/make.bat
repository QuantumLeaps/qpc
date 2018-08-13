set QPC=C:\qp\qpc
set QTOOLS=C:\qp\qtools
%QTOOLS%\bin\gcc blinky.c -oblinky.exe -I%QPC%\include -I%QPC%\ports\win32 -L%QPC%\ports\win32\dbg -lqp