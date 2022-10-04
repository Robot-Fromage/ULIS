# IDDN.FR.001.250001.006.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindFreetype2.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# For freetype skip install
set( SKIP_INSTALL_ALL true )
set( CMAKE_DISABLE_FIND_PACKAGE_ZLIB        ON )
set( CMAKE_DISABLE_FIND_PACKAGE_PNG         ON )
set( CMAKE_DISABLE_FIND_PACKAGE_BZip2       ON )
set( CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz    ON )
set( CMAKE_DISABLE_FIND_PACKAGE_BrotliDec   ON )

# Add project
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2    ${PROJECT_BINARY_DIR}/3rdparty/build_freetype2  )

# Warnings
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    set_target_properties( freetype PROPERTIES COMPILE_FLAGS "/wd4267 /wd4244 /wd4018 /wd4312" )
endif()

# Folder
set_target_properties( freetype PROPERTIES FOLDER ULIS/3rdparty )

