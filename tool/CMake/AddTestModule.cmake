# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         AddTestModule.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

function( AddTestModule iModuleName )
    # ADD PROJECT
    add_executable( ${iModuleName} )
    set_target_properties( ${iModuleName} PROPERTIES FOLDER ULIS/test )

    # ADD MODULE SOURCE
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "test/${iModuleName}/*"
    )
    target_sources( ${iModuleName} PRIVATE ${source_list} )

    # PARSE PRETTIER FOLDER GROUPS
    foreach( source IN LISTS source_list )
        get_filename_component( source_path "${source}" PATH )
        string( REPLACE "/" "\\" source_path_msvc "${source_path}" )
        string( REPLACE "test/${iModuleName}" "" source_path_msvc "${source_path}" )
        source_group( "${source_path_msvc}" FILES "${source}" )
    endforeach()
    source_group( "" FILES "${CMAKE_CURRENT_BINARY_DIR}/${iModuleName}_autogen/mocs_compilation.cpp" )

    # UNICODE
    target_compile_definitions(
        ${iModuleName}
        PUBLIC
        UNICODE
    )

    target_link_libraries(
        ${iModuleName}
        ULIS
    )

endfunction()

