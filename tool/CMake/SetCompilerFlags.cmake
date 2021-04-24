# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         DetectCompiler.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# Clang
if( ${ULIS_CLANG} )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -msse4.2 -mavx2 -mfma -Wno-comment -Wno-unused-function -Wno-missing-braces -Wno-switch -MP" )
    # Disable C++ exceptions.
    string( REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
    # Disable RTTI.
    #string( REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti" )
endif()

# GCC
if( ${ULIS_GCC} )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -msse -msse2 -msse3 -mssse3 -msse4.1 -mxop -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W -pthread" )
    # Disable C++ exceptions.
    string( REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
    # Disable RTTI.
    #string( REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti" )
endif()

# MinGW
if( ${ULIS_MINGW} )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -Wno-sign-compare -msse -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W" )
    # Disable C++ exceptions.
    string( REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
    # Disable RTTI.
    #string( REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti" )
endif()

# MSVC
if( ${ULIS_MSVC} )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc -D__SSE4_2__ -D__AVX2__" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /Od /MDd" )
    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7" )
    # EHsc: Exception stuff
    # Z7:   Produced object files contain full symbolic debugging information.
    # MP:   Parallelize compilation of source files.
    # Od:   Turns off all optimizations in the program and speeds compilation.

    # Disable C++ exceptions.
    string( REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-" )
    add_definitions( -D_HAS_EXCEPTIONS=0 )
    # Disable RTTI.
    #string( REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-" )
endif()

