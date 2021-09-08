## Build From Source
This page explains how to build ULIS from source.  
ULIS favors the use of [CMake](https://cmake.org/) to manage the build process, so this document shows how to generate project files and compile using it.  
It is possible to setup your own custom build process, but this is not covered here.

# Table of Contents
- [Requirements](@ref requirements)
    + [Softwares](@ref softwares)
    + [Operating Systems](@ref os)
    + [Compilers](@ref compilers)
- [Getting Started](@ref getting-started)
- [Windows](@ref windows)
    + [Visual Studio 15 2017 - MSVC](@ref vs2017)
    + [Visual Studio 16 2019 - MSVC](@ref vs2019)
    + [Visual Studio - Clang/LLVM](@ref vsclang)
    + [MinGW Makefiles - GCC](@ref mingw)
- [macOS](@ref macos)
    + [Xcode - Clang/LLVM](@ref xcode)
- [Linux](@ref linux)
    + [Sublime Text2 - Unix Makefiles - GCC](@ref sublime)
- [Additional Build Options](@ref additional)

# Requirements {#requirements}
ULIS is meant to be used with desktop software applications on modern architectures, for processors supporting x86-64, and with 64-bit operating systems.  
There is no guaranty that ULIS is able to compile and run for other platforms such as Android or embedded systems.

### Softwares {#softwares}
The following softwares are required in order to collect and build ULIS from source:
- [CMake](https://cmake.org/)
- [git](https://git-scm.com/)

### Operating Systems {#os}
The following operating systems are officially supported, both as target platforms and hosts for compiling the library. They are tested on a regular basis:
- Windows
    + Windows 7
    + Windows 8
    + Windows 10
- macOS
- Linux
    + Ubuntu

### Compilers {#compilers}
The following compilers are officially supported and frequently tested:
- MSVC
    + 2017
    + 2019
- Clang/LLVM
- GCC
- MinGW GCC

# Getting Started {#getting-started}
First and foremost, you have to clone the repo and checkout a stable commit, we recommend building the release and not the latest dev state, unless you are willing to contribute or try the possibly unstable latest features.
Then the build process is very similar for all the following platforms.  
The steps involved describe commands typed in the shell or command prompt, assuming both cmake and git are available from the path.  
This is the typical out of source build process with CMake:
```d
    git clone https://github.com/Praxinos/ULIS
    cd ULIS
    git checkout release
    cd ..
    mkdir ULIS_Build
    cd ULIS_Build
    cmake -G "Generator" ../ULIS
```
[Here is a list of CMake generators and documentation](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)

# Windows {#windows}
The following sections describes the step-by-step process for Windows.
### Visual Studio 15 2017 - MSVC {#vs2017}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    cmake -G "Visual Studio 15 2017 Win64" ../ULIS
```
### Visual Studio 16 2019 - MSVC {#vs2019}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    cmake -G "Visual Studio 16 2019" -A x64 ../ULIS
```
### Visual Studio - Clang/LLVM {#vsclang}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    cmake -G "Visual Studio 15 2017 Win64" -T"llvm" ../ULIS
```
### MinGW Makefiles - GCC {#mingw}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    mkdir Release
    cd Release
    cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../../ULIS
    cd ..
    mkdir Debug
    cd Debug
    cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../../ULIS
```
# macOS {#macos}
The following sections describes the step-by-step process for macOS.
### Xcode - Clang/LLVM {#xcode}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    cmake -G "Xcode" ../ULIS
```
# Linux {#linux}
The following sections describes the step-by-step process for Linux.
### Sublime Text2 - Unix Makefiles - GCC {#sublime}
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    mkdir Release
    cd Release
    cmake -G "Sublime Text 2 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../../ULIS
    cd ..
    mkdir Debug
    cd Debug
    cmake -G "Sublime Text 2 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../../ULIS
```

# Additional Build Options {#additional}
On top of the default CMake generation process, you can specify more build options to customize your build or enable or disable components in the ULIS library.
| Option Name                   | Type      | Default   | Meaning                                                                                                   |
|------------------------------ |-----------|-----------|-----------------------------------------------------------------------------------------------------------|
| ULIS_BUILD_SHARED             | BOOL      | ON        | Use to build dynamic library or not.                                                                      |
| ULIS_BUILD_PYTHON_MODULE      | BOOL      | OFF       | Use to generate and build the python binding or not.                                                      |
| ULIS_BUILD_TESTS              | BOOL      | OFF       | Use to generate and build the tests or not.                                                               |
| ULIS_BUILD_SAMPLES            | BOOL      | OFF       | Use to generate and build the samples or not.                                                             |
| ULIS_BINARY_PREFIX            | STRING    | ""        | Indicates a prefix for the output binaries.                                                               |
| ULIS_QT_CMAKE_PATH            | STRING    | ""        | The path to Qt, needed for the samples and some tests.                                                    |
| ULIS_CURSES_CMAKE_PATH        | STRING    | ""        | The path to Curses, needed for the samples and some tests.                                                |


This is an example for Windows, Visual Studio 16 2019 - MSVC, with all options:
```d
    git clone https://github.com/Praxinos/ULIS
    mkdir ULIS_Build
    cd ULIS_Build
    cmake   -G "Visual Studio 15 2017 Win64"                                                                ^
            -DULIS_BUILD_SHARED=ON                                                                          ^
            -DULIS_BUILD_TESTS=ON                                                                           ^
            -DULIS_BUILD_EXAMPLES=ON                                                                        ^
            -DULIS_BINARY_PREFIX:STRING=""                                                                  ^
            -DULIS_QT_CMAKE_PATH:STRING="C:/Qt/5.12.2/msvc2017_64/lib/cmake/"                               ^
            -DULIS_CURSES_CMAKE_PATH:STRING="C:/Users/PRAXINOS/work/PDCurses/"                              ^
            -DULIS_BUILD_PYTHON_MODULE=ON                                                                   ^
            ../../ULIS
```

