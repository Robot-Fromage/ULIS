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
#
cmake_minimum_required(VERSION 3.11.0)

option( ULIS_BUILD_SHARED           "Build the library shared or statically"            ON  )
option( ULIS_BUILD_PYTHON_MODULE    "Build the library python module"                   OFF )
option( ULIS_BUILD_TESTS            "Build the library test modules"                    OFF )
option( ULIS_BUILD_SAMPLES          "Build the library sample modules"                  OFF )
option( ULIS_FORCE_ASSERT           "Build the library with forced assert in release"   OFF )
option( ULIS_FORCE_STATISTICS       "Build the library with forced stats in release"    OFF )
SET( ULIS_BINARY_PREFIX             "" CACHE STRING "Indicates a prefix for the output binaries"            )
SET( ULIS_QT_CMAKE_PATH             "" CACHE STRING "Indicates the path to Qt cmake package"                )
SET( ULIS_PYTHON_REL_LIB            "" CACHE STRING "Indicates the path to python release static library"   )
SET( ULIS_PYTHON_DEB_LIB            "" CACHE STRING "Indicates the path to python debug static library"     )
SET( ULIS_PYTHON_INCLUDE_PATH1      "" CACHE STRING "Indicates the path to python include path"             )
SET( ULIS_PYTHON_INCLUDE_PATH2      "" CACHE STRING "Indicates extra path to python include path"           )

# Setup generation options
set( CMAKE_OSX_DEPLOYMENT_TARGET "10.13" )
set( CMAKE_CONFIGURATION_TYPES RelWithDebInfo Debug Release CACHE STRING INTERNAL FORCE )
set( CMAKE_SUPPRESS_REGENERATION true )
set( CMAKE_DEBUG_POSTFIX d )
set( CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true )
# For freetype skip install
set( SKIP_INSTALL_ALL true )

# Set C++ standard options
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Create solution
project( ULIS )
set( ULIS_VERSION_MAJOR 4 )
set( ULIS_VERSION_MINOR 0 )
set( ULIS_VERSION_PATCH 0 )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

message( "Generating: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID}" )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG" )
    set( CMAKE_CXX_FLAGS "-std=c++14" )
endif()

# Flags for Compiler / Platforms
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    # CLANG
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx2 -mfma" )

elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    # GNU
    if( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux" )
        # LINUX GNU GCC
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W -pthread" )

    elseif( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows" )
        # WINDOWS MINGW GCC
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W" )

    endif()

elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    # MSVC
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc -D__SSE4_2__ -D__AVX2__" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Z7 -MP -Od -MDd" )
    # EHsc: Exception stuff
    # Z7:   Produced object files contain full symbolic debugging information.
    # MP:   Parallelize compilation of source files.
    # Od:   Turns off all optimizations in the program and speeds compilation.

endif()

set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D ULIS_FORCE_ASSERT -D ULIS_FORCE_STATISTICS -D ULIS_RELWITHDEBINFO" )

# Add Dependencies
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/clip         ${PROJECT_BINARY_DIR}/3rdparty/build_clip       )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS   ${PROJECT_BINARY_DIR}/3rdparty/build_lcms2      )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2    ${PROJECT_BINARY_DIR}/3rdparty/build_freetype2  )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppfs        ${PROJECT_BINARY_DIR}/3rdparty/build_cppfs      EXCLUDE_FROM_ALL )


set_target_properties( cppfs PROPERTIES COMPILE_FLAGS "/wd4146 /wd4267" )
set_target_properties( clip PROPERTIES COMPILE_FLAGS "/wd4267" )
set_target_properties( freetype PROPERTIES COMPILE_FLAGS "/wd4267 /wd4244 /wd4018 /wd4312" )

set_target_properties( clip                 PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( lcms2                PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( freetype             PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( cppfs                PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( component_install    PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( check-all            PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( check-cppfs          PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( check-template       PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( pack                 PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( pack-cppfs           PROPERTIES FOLDER ULIS/3rdparty )

#if( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows" )
#    set( OpenCL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OCL-SDK/include/"                 )
#    set( OpenCL_LIBRARY     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OCL-SDK/lib/x86_64/OpenCL.lib"    )
#endif()
#find_package(OpenCL)

####################################
# Add Main Project
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*"
    "source/*"
)

if( ${ULIS_BUILD_SHARED} )
   add_library( ULIS SHARED ${source_list} )
   target_compile_definitions( ULIS PRIVATE ULIS_BUILD_SHARED )
else()
   add_library( ULIS STATIC ${source_list} )
   target_compile_definitions( ULIS PRIVATE ULIS_STATIC_LIBRARY )
endif()

if( ${ULIS_FORCE_ASSERT} )
    target_compile_definitions( ULIS PUBLIC ULIS_FORCE_ASSERT )
endif()

if( ${ULIS_FORCE_STATISTICS} )
    target_compile_definitions( ULIS PUBLIC ULIS_FORCE_STATISTICS )
endif()

target_compile_definitions( ULIS PUBLIC UNICODE )
target_compile_definitions( ULIS PRIVATE ULIS_DEF_VERSION_MAJOR=${ULIS_VERSION_MAJOR} ULIS_DEF_VERSION_MINOR=${ULIS_VERSION_MINOR} ULIS_DEF_VERSION_PATCH=${ULIS_VERSION_PATCH} )
set_target_properties( ULIS PROPERTIES OUTPUT_NAME "ULIS${ULIS_VERSION_MAJOR}" )
set_target_properties( ULIS PROPERTIES PREFIX "${ULIS_BINARY_PREFIX}" )
set_target_properties( ULIS PROPERTIES FOLDER ULIS )
target_link_libraries( ULIS PRIVATE clip )
target_link_libraries( ULIS PRIVATE lcms2 )
target_link_libraries( ULIS PRIVATE freetype )
target_link_libraries( ULIS PRIVATE cppfs )
#target_link_libraries( ULIS PRIVATE ${OpenCL_LIBRARY} )

target_include_directories(
    ULIS
    PUBLIC
    "include/"
    PRIVATE
    #${OpenCL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/VCL/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/static_math/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/clip/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppfs/source/cppfs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eigen/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/FeatureDetector/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm/
    "source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

####################################
# Load Qt
# Set simple boolean for testing qt availability
set( ULIS_USE_QT OFF )
if( NOT "${ULIS_QT_CMAKE_PATH}" STREQUAL "" )
    set( ULIS_USE_QT ON )
    set( CMAKE_PREFIX_PATH ${ULIS_QT_CMAKE_PATH} )
    find_package(
        Qt5
        REQUIRED COMPONENTS
        Core
        Widgets
        Gui
    )
    set( CMAKE_AUTOMOC ON )
endif()

####################################
# Add Python Project
include(${CMAKE_CURRENT_SOURCE_DIR}/binding/Python/CMakeLists.txt)

####################################
# Add Test Projects
if( ${ULIS_BUILD_TESTS} )
include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()

if( ${ULIS_BUILD_SAMPLES} )
include(${CMAKE_CURRENT_SOURCE_DIR}/sample/CMakeLists.txt)
endif()

####################################
# Install
string( TIMESTAMP ULIS_TODAY_DATE "%d%m%Y" )
SET( ULIS_INSTALL_DIR "${ULIS_TODAY_DATE}_ULIS${ULIS_VERSION_MAJOR}.${ULIS_VERSION_MINOR}.${ULIS_VERSION_PATCH}_${CMAKE_HOST_SYSTEM_NAME}_${CMAKE_CXX_COMPILER_ID}_x64" )
set( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/../${ULIS_INSTALL_DIR}" CACHE PATH "Install Path" FORCE )

install( TARGETS ULIS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/include
    COMPONENT headers
)

