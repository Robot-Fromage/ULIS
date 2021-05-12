# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         CMakeLists.txt
# @author       Clement Berthaud
# @brief        Project generation for python binding.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required(VERSION 3.11.0)

if( ${ULIS_BUILD_PYTHON_MODULE} )
    # pyULIS module
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "binding/Python/module/src/*"
    )

    add_library( pyULIS SHARED ${source_list} )
    set_target_properties( pyULIS PROPERTIES SUFFIX ".pyd" )
    set_target_properties( pyULIS PROPERTIES FOLDER ULIS )
    target_compile_definitions( pyULIS PRIVATE pyULIS_BUILD_SHARED )

    # Disable python to enable Pypy
    #set( PYBIND11_NOPYTHON ON CACHE BOOL "Disable search for Python" )
    add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/pybind11-2.6.2 )
    target_link_libraries(
        pyULIS
        PUBLIC
        ULIS
        pybind11::module
    )

    target_include_directories(
        pyULIS
        PUBLIC
        "Python/module/src/"
    )

    set_target_properties(
        pyULIS
        PROPERTIES
        MAP_IMPORTED_CONFIG_RELEASE Release
        MAP_IMPORTED_CONFIG_DEBUG Release
        MAP_IMPORTED_CONFIG_RELWITHDEBUGINFO Release
    )

    set_target_properties( pyULIS PROPERTIES OUTPUT_NAME "pyULIS${ULIS_VERSION_MAJOR}" )
    set_target_properties( pyULIS PROPERTIES PREFIX "${ULIS_BINARY_PREFIX}" )

    # For MSVC parse pretty filter names
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string( REPLACE "/" "\\" source_path_msvc "${source_path}" )
        string( REPLACE "binding/Python/module/src" "" source_path_msvc "${source_path}" )
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()





    # pyULIS interactive
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "binding/Python/interactive/src/*"
    )

    add_executable( pyULIS_Interactive ${source_list} )
    set_target_properties( pyULIS_Interactive PROPERTIES FOLDER ULIS )

    target_link_libraries(
        pyULIS_Interactive
        PUBLIC
        pyULIS
        pybind11::embed
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::WinExtras
        ${OPENGL_gl_LIBRARY}
        libglew_static
    )

    target_include_directories(
        pyULIS_Interactive
        PUBLIC
        "python/interactive/src/"
    )

    # For MSVC parse pretty filter names
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string( REPLACE "/" "\\" source_path_msvc "${source_path}" )
        string( REPLACE "binding/Python/interactive/src" "" source_path_msvc "${source_path}" )
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()

endif()

