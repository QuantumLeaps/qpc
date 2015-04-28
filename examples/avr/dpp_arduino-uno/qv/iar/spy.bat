set COM_PORT=COM5
if [%1] NEQ [] set COM_PORT=%1

qspy -c%COM_PORT% -b115200 -O2 -F2 -E1 -P1 -B1