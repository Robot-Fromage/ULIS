# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         AddExampleModule.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

function( AddExampleModule iModuleName )
    # Only with Qt !
    if( NOT ${ULIS_USE_QT} )
        return()
    endif()

    #if( NOT ${ULIS_USE_CURSES} )
    #    return()
    #endif()

    # ADD PROJECT
    add_executable( ${iModuleName} )
    set_target_properties( ${iModuleName} PROPERTIES FOLDER ULIS/example )

    # ADD MODULE SOURCE
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "example/${iModuleName}/*"
    )
    target_sources( ${iModuleName} PRIVATE ${source_list} )

    # PARSE PRETTIER FOLDER GROUPS
    foreach( source IN LISTS source_list )
        get_filename_component( source_path "${source}" PATH )
        string( REPLACE "/" "\\" source_path_msvc "${source_path}" )
        string( REPLACE "example/${iModuleName}" "" source_path_msvc "${source_path}" )
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
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        #PDCurses
    )

endfunction()

