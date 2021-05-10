# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         CMakeLists.txt
# @author       Clement Berthaud
# @brief        Project generation targeting Emscripten / WebAssembly.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required(VERSION 3.11.0)

# Message
message( "Generating: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID}" )

# Setup
message( "Starting environnement setup before generation." )
set( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
set( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s USE_SDL=2 -std=c++17" )
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../tool/CMake/DetectTimestamp.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/../../tool/CMake/DetectGit.cmake )

# Version
file( STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../../VERSION" ULIS_FULL_VERSION )
string( REPLACE "." ";" ULIS_VERSION_LIST ${ULIS_FULL_VERSION} )
list( GET ULIS_VERSION_LIST 0 ULIS_VERSION_MAJOR )
list( GET ULIS_VERSION_LIST 1 ULIS_VERSION_MINOR )
list( GET ULIS_VERSION_LIST 2 ULIS_VERSION_PATCH )

# Project
project( wULIS${ULIS_VERSION_MAJOR}_Project )

# Library Dependencies
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/zlib       ${PROJECT_BINARY_DIR}/3rdparty/build_zlib       )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/Little-CMS ${PROJECT_BINARY_DIR}/3rdparty/build_lcms2      )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype2  ${PROJECT_BINARY_DIR}/3rdparty/build_freetype2  )

# Gather sources
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "../../include/*"
    "../../source/*"
)

# Create Library
add_library( libwULIS STATIC ${source_list} )

# Compile Definitions
target_compile_definitions(
    libwULIS
    PUBLIC
    UNICODE
    PRIVATE
    ULIS_DEF_VERSION_MAJOR=${ULIS_VERSION_MAJOR}
    ULIS_DEF_VERSION_MINOR=${ULIS_VERSION_MINOR}
    ULIS_DEF_VERSION_PATCH=${ULIS_VERSION_PATCH}
    ULIS_DEF_GIT_COMMIT_HASH=${ULIS_GIT_COMMIT_HASH}
    ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH=${ULIS_GIT_COMMIT_ABBREVIATED_HASH}
    ULIS_DEF_GIT_BRANCH_NAME=${ULIS_GIT_BRANCH_NAME}
    ULIS_GENERATION_TIMESTAMP=${ULIS_GENERATION_TIMESTAMP}
)

# Include
target_include_directories(
    libwULIS
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/VCL/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/stb/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/Little-CMS/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype2/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/cppfs/source/cppfs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/zlib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/FeatureDetector/
    ${PROJECT_BINARY_DIR}/3rdparty/build_zlib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../source/
)

# Link
target_link_libraries( libwULIS PRIVATE lcms2 )
target_link_libraries( libwULIS PRIVATE freetype )
target_link_libraries( libwULIS PRIVATE zlibstatic )

# Configure
set_target_properties( libwULIS PROPERTIES OUTPUT_NAME "libwULIS${ULIS_VERSION_MAJOR}" )
set_target_properties( libwULIS PROPERTIES PREFIX "" )
set( CMAKE_EXECUTABLE_SUFFIX ".html" )

# Test & Example
include( ${CMAKE_CURRENT_SOURCE_DIR}/wFormats/CMakeLists.txt )
include( ${CMAKE_CURRENT_SOURCE_DIR}/wInfo/CMakeLists.txt )
include( ${CMAKE_CURRENT_SOURCE_DIR}/wimgui_interop/CMakeLists.txt )
include( ${CMAKE_CURRENT_SOURCE_DIR}/wBlendDemo/CMakeLists.txt )
include( ${CMAKE_CURRENT_SOURCE_DIR}/wULIS/CMakeLists.txt )

