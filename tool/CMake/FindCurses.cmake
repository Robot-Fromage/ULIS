# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindCurses.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

set( ULIS_USE_CURSES OFF )
if( NOT "${ULIS_CURSES_CMAKE_PATH}" STREQUAL "" )
    set( ULIS_USE_CURSES ON )
    # Add project
    add_subdirectory( ${ULIS_CURSES_CMAKE_PATH} ${PROJECT_BINARY_DIR}/3rdparty/build_curses )
    set_target_properties( PDCurses PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( firework PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( ozdemo PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( ptest PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( rain PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( testcurs PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( tuidemo PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( worm PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
    set_target_properties( xmas PROPERTIES FOLDER ULIS/3rdparty/PDCurses )
endif()

