@echo off

if not exist "%~dp0ULIS\Tools\Scripts\Windows\FullBuildUE4_WinMSVCx64.bat" goto Error_NotFound
call "%~dp0ULIS\Tools\Scripts\Windows\FullBuildUE4_WinMSVCx64.bat" %*
exit /B %ERRORLEVEL%

:Error_NotFound
echo FullBuildUE4_WinMSVCx64 ERROR: The batch file does not appear to be located where it should.
pause
exit /B 1
