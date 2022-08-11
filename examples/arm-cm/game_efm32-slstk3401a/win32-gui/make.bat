:: Batch file for building the specified PROJ (solution) prepared for
:: Microsoft Visual Studio
::
:: usage:
:: make
::
@setlocal

:: specifiy project and build configuration (Debug|Release|Spy)
@set PROJ=game-gui
@set CONF=Debug

:: adjust the MSBuild location for your system
@set MSBuild="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"

:: execute the build...
%MSBuild% %PROJ%.sln -t:Build -p:Configuration=%CONF% -p:Platform=Win32

:: cleanup after the build...
::rmdir /S /Q %CONF%\%PROJ%.tlog
::del %CONF%\%PROJ%.vcxproj* %CONF%\%PROJ%.log %CONF%\*.obj %CONF%\*.pdb

@endlocal

