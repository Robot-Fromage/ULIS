# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         CMakeLists.txt
# @author       Clement Berthaud
# @brief        Project generation for javascript emscripten webassembly binding.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required(VERSION 3.11.0)

# wULIS module
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "*"
)

add_library( wULIS STATIC ${source_list} )

target_link_libraries(
    wULIS
    PUBLIC
    ULIS
)

set_target_properties( wULIS PROPERTIES OUTPUT_NAME "wULIS${ULIS_VERSION_MAJOR}" )
set_target_properties( wULIS PROPERTIES PREFIX "${ULIS_BINARY_PREFIX}" )
