@echo off

:: Ensure local scope
setlocal

:: Ensure current dir
pushd %~dp0

:: CD 4 steps up to root
cd ../../../../

call Generate_MinGW_Makefile_GCC.bat
call Generate_VisualStudio_Solution_CLANG_LLVM.bat
call Generate_VisualStudio_Solution_MSVC.bat

cd ULIS
cd Generated_VisualStudio_Solution_MSVC
cmake --build . --config Release
cd ..

cd Generated_VisualStudio_Solution_CLANG_LLVM
cmake --build . --config Release
cd ..

cd Generated_MinGW_Makefile_GCC
cd Release
cmake --build .
