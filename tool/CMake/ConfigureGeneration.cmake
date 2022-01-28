# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         ConfigureGeneration.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# macOS
set( CMAKE_OSX_DEPLOYMENT_TARGET "10.13" )

# Disable CMake extra
set( CMAKE_SUPPRESS_REGENERATION true )
set( CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true )

# Postfix
set( CMAKE_DEBUG_POSTFIX d )

# Configuration
set( CMAKE_CONFIGURATION_TYPES RelWithDebInfo Debug Release CACHE STRING INTERNAL FORCE )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D ULIS_FORCE_ASSERT -D ULIS_RELWITHDEBINFO" )

# C++ standard options
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG" )
    set( CMAKE_CXX_FLAGS "-std=c++17" )
endif()

# Folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

