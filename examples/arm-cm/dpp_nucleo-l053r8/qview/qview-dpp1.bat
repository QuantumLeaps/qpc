@setlocal

if "%QTOOLS%"=="" (
    set QTOOLS=C:\qp\qtools
)
python3 %QTOOLS%\qview\qview.py dpp1.py

@endlocal
