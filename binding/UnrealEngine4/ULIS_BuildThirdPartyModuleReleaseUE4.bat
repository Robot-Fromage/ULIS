@echo off
:: Put this file next to ULIS3 directory and launch to trigger full build and Redist
setlocal
pushd %~dp0

:: Check if we're next to ULIS3 directory
IF NOT EXIST ULIS3 EXIT /B 1

:: Make dir for build if not exist
IF NOT EXIST ULIS3_FullBuildUE4_WinMSVCx64_Redist ( MKDIR ULIS3_FullBuildUE4_WinMSVCx64_Redist )

:: Step in build dir
cd ULIS3_FullBuildUE4_WinMSVCx64_Redist

:: Make Generated dir for generated project if not exist
IF NOT EXIST Generated_VisualStudio_Solution_MSVC ( MKDIR Generated_VisualStudio_Solution_MSVC )

:: Step in Generated dir
cd Generated_VisualStudio_Solution_MSVC

:: Clean cmake garbage if there
IF EXIST CMakeFiles ( rmdir /S /Q CMakeFiles )
IF EXIST cmake_install.cmake ( del cmake_install.cmake )
IF EXIST CMakeCache.txt ( del CMakeCache.txt )

:: Rebuild Project
:: Replace with "Visual Studio 15 2017 Win64" if needed
cmake   -G "Visual Studio 16 2019"                          ^
        -A x64                                              ^
        -DULIS3_BUILD_SHARED=ON                             ^
        -DULIS3_FORCE_ASSERTS=ON                            ^
        ../../ULIS3

:: Replace RelWithDebInfo with Release if needed
cmake --build . --config RelWithDebInfo

:: go up to build dir
cd ..

:: Make Redist dir for generated project if not exist
IF EXIST redist ( RMDIR /S /Q redist )
MKDIR redist
cd redist
mkdir include
cd include
mkdir ULIS3
cd ..
mkdir bin
mkdir lib

:: CD to root and copy
cd ../../
XCOPY /E /C /I /Q /H /K /Y /B ".\ULIS3\include"                                                                 "ULIS3_FullBuildUE4_WinMSVCx64_Redist\redist\include\ULIS3"
COPY /Y "ULIS3_FullBuildUE4_WinMSVCx64_Redist\Generated_VisualStudio_Solution_MSVC\RelWithDebInfo\ULIS3.lib"    "ULIS3_FullBuildUE4_WinMSVCx64_Redist\redist\lib\ULIS3.lib"
COPY /Y "ULIS3_FullBuildUE4_WinMSVCx64_Redist\Generated_VisualStudio_Solution_MSVC\RelWithDebInfo\ULIS3.dll"    "ULIS3_FullBuildUE4_WinMSVCx64_Redist\redist\bin\ULIS3.dll"
COPY /Y "ULIS3_FullBuildUE4_WinMSVCx64_Redist\Generated_VisualStudio_Solution_MSVC\RelWithDebInfo\ULIS3.pdb"    "ULIS3_FullBuildUE4_WinMSVCx64_Redist\redist\bin\ULIS3.pdb"
COPY /Y "ULIS3\scripts\UnrealEngine4\ULIS.Build.cs"                                                             "ULIS3_FullBuildUE4_WinMSVCx64_Redist\ULIS.Build.cs"
