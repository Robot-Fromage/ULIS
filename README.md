# ULIS
Unlimited Library for Imaging Systems

## Description
        - Cross-Platform C++14 Library
        - Generic digital image processing
        - 2D Software rasterizer
        - Unlimited image formats ( u8, u16, u32, float, double )
        - Custom bit-ordered memory layouts
        - All Photoshop pixel blending modes
        - Color models ( RGB, HSL, HSV, CMYK, ... )
        - Color-managed pipelines, Color Space support ( icm profiles and device-independant )
        - Optimized algorithms with multithreading
        - Optimized algorithms with SIMD Extensions ( SSE2 )
        - Big Images ( 300000 px * 300000 px)
        - Image pools and caches for optimisations
        - Memory storage of animated image sequences

## Software Requirements Specification ( SRS )
        - 64bit OS - Linux, Windows or MacOS
        - CMake ( 3.11.4 or greater )
        - Git ( 2.17.0 or greater )
        - C++ compiler with C++14 support
        - Python ( 3.7 or greater ) available from PATH as "python3" ( optional )

## Supported Compilers
        - MinGW 64 7.xx
        - Clang LLVM xx
        - MSVC
        - GNU GCC
        
## Enforcing a x64 toolchain
        - #Windows:
        - For Windows MSVC, ensure these global environment variables are set when launching an MSVC build environment:
        - _IsNativeEnvironment true
        - PreferredToolArchitecture x64

## Library Dependencies
        - coal ( https://github.com/Robot-Fromage/coal )
        - boost_preprocessor ( https://github.com/boostorg/preprocessor )
        - glm ( https://github.com/g-truc/glm )
        - Qt ( https://www.qt.io/, Optional )
        - Little-CMS ( https://github.com/Robot-Fromage/Little-CMS, my fork provides a CMakeLists )


## Officially Supported IDEs
        - Windows: VisualStudio 2017
        - macOS: XCode
        - Linux: SublimeText

## Getting Started
        - git clone https://github.com/Robot-Fromage/ULIS
        - cd ULIS
        - #Windows: call Setup.bat
        - #macOS / Linux : sh Setup.sh

You should see the following files appear in the repository root:

        - #Windows:
        - MinGW_Makefile_GCC_Config.cmake
        - VisualStudio_Solution_CLANG_LLVM_Config.cmake
        - VisualStudio_Solution_MSVC_Config.cmake
        - #macOS / Linux:
        - SublimeText_Project_GNU_GCC_Config.cmake
        - Xcode_Config.cmake

The contents of these files should look like this:

        SET( ULIS_BOOST_INCLUDE_PATH        "default" )
        SET( ULIS_COAL_INCLUDE_PATH         "default" )
        SET( ULIS_BUILD_SHARED              OFF )
        SET( ULIS_BUILD_TESTS               ON )
        SET( ULIS_BUILD_GUIS                OFF )
        SET( ULIS_QT_CMAKE_PATH             "default" )
        SET( ULIS_GLM_INCLUDE_PATH          "default" )
        SET( ULIS_LCMS2_INCLUDE_PATH        "default" )
        SET( ULIS_LCMS2_DEBUG_LIB_PATH      "default" )
        SET( ULIS_LCMS2_RELEASE_LIB_PATH    "default" )

Make sure you cloned the dependencies and built them if needed. You can then copy the path for the requested components. All paths should use forward slashes on all OS, even Windows macOS and Linux. All paths should have a trailing forward slash too. `ULIS_QT_CMAKE_PATH` is optional and can remain to `"default"`, in that case `ULIS_BUILD_GUIS` should remain `OFF`too. Make sure you fill the path for the config file of your compiler / IDE of choice, and that components you use were built with the same compiler. The building process of the dependencies will not be detailled here, it is left for the user to figure out, although most dependencies are header-only and i provide a CMakeLists in order to build Little-CMS easily, which can also be found in the Scripts folder of ULIS.

There is an example of a fully configured config.cmake file:

        SET( ULIS_BOOST_INCLUDE_PATH        "C:/Users/WorkStation/work/boost_preprocessor/include/" )
        SET( ULIS_COAL_INCLUDE_PATH         "C:/Users/WorkStation/work/coal/coal/Source/coal/Include/" )
        SET( ULIS_BUILD_SHARED              OFF )
        SET( ULIS_BUILD_TESTS               ON )
        SET( ULIS_BUILD_GUIS                ON )
        SET( ULIS_QT_CMAKE_PATH             "C:/Qt/5.12.2/msvc2017_64/lib/cmake/" )
        SET( ULIS_GLM_INCLUDE_PATH          "C:/Users/WorkStation/work/glm_distribution/glm/" )
        SET( ULIS_LCMS2_INCLUDE_PATH        "C:/Users/WorkStation/work/Little-CMS_MSVC_distribution/include/" )
        SET( ULIS_LCMS2_DEBUG_LIB_PATH      "C:/Users/WorkStation/work/Little-CMS_MSVC_distribution/lcms2d.lib" )
        SET( ULIS_LCMS2_RELEASE_LIB_PATH    "C:/Users/WorkStation/work/Little-CMS_MSVC_distribution/lcms2.lib" )

When you're done you should launch the appropriate script according to your configuration:

        Generate_MinGW_Makefile_GCC.bat
        Generate_VisualStudio_Solution_CLANG_LLVM.bat
        Generate_VisualStudio_Solution_MSVC.bat
        Generate_SublimeText_Project_GNU_GCC.sh
        Generate_Xcode.sh

The generated project files will be found under `ULIS/Generated_*/` where you fill find solution files according to your config.
Project files are git-ignored and are never commited.

## Optional Components
On Linux, the provided scripts generate the CMake makefiles for SublimeText solutions, you can install the free demo or paid version of SublimeText in order to enjoy it.

The CMake makefiles will work with wathever generator you chose however.
On macOS the provided scripts generate solutions for Xcode only, however you should be able to use other generators too.

On Windows, the provided scripts generate solutions for VisualStudio using either MSVC or Clang LLVM. There is also a script for MinGW GCC which will produce MinGW makefiles.

If you wish to use `Qt` you should download it ( OpenSource Qt versions 5.10+ ) from the official website, either pre-built for Windows MSVC or MinGW or Linux GCC or for MacOS Clang, or build it from source.

On Windows, using VisualStudio you should download the VS plugin `Qt VisualStudio Tools`.

If you wish to use Clang LLVM VisualStudio solution generator, you should download the VS plugin `LLVM Compiler Toolchain`.

If you wish to use the `MinGW GCC` generator with Qt and test GUIS enabled, you should make sure that the MinGW GCC version available from PATH is the one provided with the Qt version you downloaded.
