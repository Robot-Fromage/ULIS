@echo off
setlocal
pushd %~dp0

cd ../../include
powershell "dir -Recurse *.h | Get-Content | Measure-Object -Line"
powershell "dir -Recurse *.cpp | Get-Content | Measure-Object -Line"
cd ../source
powershell "dir -Recurse *.h | Get-Content | Measure-Object -Line"
powershell "dir -Recurse *.cpp | Get-Content | Measure-Object -Line"

PAUSE
