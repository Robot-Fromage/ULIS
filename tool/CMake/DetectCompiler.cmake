# IDDN.FR.001.250001.006.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         DetectCompiler.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set( ULIS_CLANG 1 )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    if( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux" )
        set( ULIS_GCC 1 )
    elseif( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows" ) #mingw
        set( ULIS_MINGW 1 )
    endif()
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" ) #msvc
    set( ULIS_MSVC 1 )
endif()

