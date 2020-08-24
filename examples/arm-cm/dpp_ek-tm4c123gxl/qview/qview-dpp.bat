@setlocal

if "%QTOOLS%"=="" (
    set QTOOLS=C:\qp\qtools
)
python %QTOOLS%\qview\qview.py dpp.py

@endlocal
