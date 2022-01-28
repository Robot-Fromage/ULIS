# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindZLib.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# Add project
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib         ${PROJECT_BINARY_DIR}/3rdparty/build_zlib  )

# Warnings
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    set_target_properties( zlibstatic PROPERTIES COMPILE_FLAGS "/wd4267" )
endif()

# Folders
set_target_properties( zlib         PROPERTIES FOLDER ULIS/3rdparty )
set_target_properties( zlibstatic   PROPERTIES FOLDER ULIS/3rdparty )
#set_target_properties( minigzip     PROPERTIES FOLDER ULIS/3rdparty )
#set_target_properties( example      PROPERTIES FOLDER ULIS/3rdparty )

