@setlocal
@set KEILBUILD_ARM=C:\tools\Keil_v5\UV4\UV4.exe

%KEILBUILD_ARM% -j0 -b dpp-qxk.uvprojx -z

@endlocal
