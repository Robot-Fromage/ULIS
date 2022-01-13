# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindBlend2D.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# For freetype skip install
set( BLEND2D_STATIC     ON )
set( BLEND2D_NO_INSTALL ON )

# Add project
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/blend2d ${PROJECT_BINARY_DIR}/3rdparty/build_blend2d )

# Warnings
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    set_target_properties( blend2d PROPERTIES COMPILE_FLAGS "/wd4267 /wd4244 /wd4018 /wd4312 /wd4100 /wd4457 /wd4189" )
endif()

# Folder
set_target_properties( blend2d PROPERTIES FOLDER ULIS/3rdparty )

