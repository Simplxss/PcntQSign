@echo off
setlocal enabledelayedexpansion

for /f "tokens=2*" %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\QQ" /v "UninstallString"') do (
    set "RetString=%%b"
    goto :boot
)
echo QQ installation not found
pause
exit

:boot
for %%a in ("!RetString!") do (
    set "pathWithoutUninstall=%%~dpa"
)

if not exist "%pathWithoutUninstall%dbghelp.dll" (
    if not exist %~dp0PcntQSign.dll (
        echo PcntQSign.dll not found
        pause
        exit
    )
    goto copydll
) else (
    set /p Choice="dbghelp.dll already exists, do you want to replace it?(Y/N):"
    if /i "!Choice!"=="Y" goto copydll
    if /i "!Choice!"=="y" goto copydll
    goto launch
)

:copydll
copy /y "%~dp0PcntQSign.dll" "%pathWithoutUninstall%dbghelp.dll"
if errorlevel 1 (
    set /p Choice="Copy error, do you want to attempt to running as administrator?(Y/N):"
    if /i "!Choice!"=="Y" goto restart
    if /i "!Choice!"=="y" goto restart
    exit
)
goto launch

:restart
powershell Start-Process -FilePath cmd.exe -ArgumentList """/c pushd %~dp0 && %~s0 %*""" -Verb RunAs
exit

:launch
set "QQPath=!pathWithoutUninstall!QQ.exe"

echo Launching QQ
"!QQPath!"