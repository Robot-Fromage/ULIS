@echo off
setlocal
pushd %~dp0
Powershell.exe -executionpolicy remotesigned -File  .\code_statistics.ps1
PAUSE
