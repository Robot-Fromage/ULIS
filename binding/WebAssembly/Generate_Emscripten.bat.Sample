@echo off
setlocal
pushd %~dp0

:: Make Generated dir for generated project if not exist
IF NOT EXIST Generated_Emscripten ( MKDIR Generated_Emscripten )

:: Step in Generated dir
cd Generated_Emscripten

:: Clean cmake garbage if there
IF EXIST CMakeFiles ( rmdir /S /Q CMakeFiles )
IF EXIST cmake_install.cmake ( del cmake_install.cmake )
IF EXIST CMakeCache.txt ( del CMakeCache.txt )

:: Rebuild Project
emcmake cmake ../../ULIS3/bindings/wasm