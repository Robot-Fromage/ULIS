@echo off
setlocal
pushd %~dp0

call ULIS/Tools/Scripts/Windows/SetupHooks.bat
if not exist "VisualStudio_Solution_MSVC_Config.cmake" copy "ULIS\Tools\DefaultConfig.cmake" "VisualStudio_Solution_MSVC_Config.cmake"
if not exist "MinGW_Makefile_GCC_Config.cmake" copy "ULIS\Tools\DefaultConfig.cmake" "MinGW_Makefile_GCC_Config.cmake"
if not exist "VisualStudio_Solution_CLANG_LLVM_Config.cmake" copy "ULIS\Tools\DefaultConfig.cmake" "VisualStudio_Solution_CLANG_LLVM_Config.cmake"
