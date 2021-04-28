# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         CMakeLists.txt
# @author       Clement Berthaud
# @brief        This file provides an abstract way to describe the project
#               structure to CMake in order to generate and build on many
#               platforms.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required(VERSION 3.11.0)

# Message
message( "Generating: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID}" )

# Input
option( ULIS_BUILD_SHARED           "Build the library shared or statically"            ON  )
option( ULIS_BUILD_PYTHON_MODULE    "Build the library python module"                   OFF )
option( ULIS_BUILD_TESTS            "Build the library test modules"                    OFF )
option( ULIS_BUILD_EXAMPLES         "Build the library example modules"                 OFF )
SET( ULIS_BINARY_PREFIX             "" CACHE STRING "Indicates a prefix for the output binaries"            )
SET( ULIS_QT_CMAKE_PATH             "" CACHE STRING "Indicates the path to Qt cmake package"                )
SET( ULIS_PYTHON_REL_LIB            "" CACHE STRING "Indicates the path to python release static library"   )
SET( ULIS_PYTHON_DEB_LIB            "" CACHE STRING "Indicates the path to python debug static library"     )
SET( ULIS_PYTHON_INCLUDE_PATH1      "" CACHE STRING "Indicates the path to python include path"             )
SET( ULIS_PYTHON_INCLUDE_PATH2      "" CACHE STRING "Indicates extra path to python include path"           )

# Setup
message( "Starting environnement setup before generation." )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/ConfigureGeneration.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/DetectVersion.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/DetectTimestamp.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/DetectGit.cmake )

# Functions
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/AddExampleModule.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/AddTestModule.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/SubdirList.cmake )

# Project
message( "Starting project generation." )
project( ULIS${ULIS_VERSION_MAJOR}_Project )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/DetectCompiler.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/SetCompilerFlags.cmake )

# Library Dependencies
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/FindZLib.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/FindFreetype2.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/FindLCMS2.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/FindOpenGL.cmake )

# Library
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/Main.cmake )

# Python
include(${CMAKE_CURRENT_SOURCE_DIR}/binding/Python/CMakeLists.txt)

# Test & Example
if( ${ULIS_BUILD_TESTS} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()

include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/FindQt.cmake )

if( ${ULIS_BUILD_EXAMPLES} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/example/CMakeLists.txt)
endif()

# Install
include( ${CMAKE_CURRENT_SOURCE_DIR}/tool/CMake/Install.cmake )

