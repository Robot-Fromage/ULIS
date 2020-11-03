@echo off

:: Ensure local scope
setlocal

:: Ensure current dir
pushd %~dp0

:: CD 3 steps up to root/ULIS
cd ../../../

:: Step in Source dir
cd Source

doxygen %~dp0Doxyfile_private
PAUSE
